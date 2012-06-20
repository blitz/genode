/*
 * \brief  Signal context for completions and events
 * \author Sebastian Sumpf <sebastian.sumpf@genode-labs.com>
 * \date   2012-05-23
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <signal.h>
#include <lx_emul.h>


static Signal_helper *_signal = 0;

/**
 * Context for events
 */
class Event_context : public Driver_context
{
	private:

		Genode::Signal_context_capability _ctx_cap;
		
		Event_context()
		: _ctx_cap(_signal->receiver()->manage(this)) {
			_signal->sender()->context(_ctx_cap); }

	public:

		static Event_context *e()
		{
			static Event_context _e;
			return &_e;
		}

		void submit() {
			_signal->sender()->submit(); }

		void handle() {
			Routine::schedule_all(); }

		char const *debug() { return "Event_context"; }
};


void Event::init(Genode::Signal_receiver *recv) {
	_signal = new (Genode::env()->heap()) Signal_helper(recv); }


/************************
 ** linux/completion.h **
 ************************/

void __wake_up() { Routine::schedule_all(); }


void __wait_event() {
	Service_handler::s()->process(); }


void init_completion(struct completion *work)
{
	dde_kit_log(DEBUG_COMPLETION, "New completion %p", work);
	work->done = 0;
}


void complete(struct completion *work)
{
	dde_kit_log(DEBUG_COMPLETION, "%p", work);
	work->done = 1;

	/* send signal */
	Event_context::e()->submit();
}


void complete_and_exit(struct completion *work, long code)
{
	dde_kit_log(DEBUG_COMPLETION, "%p", work);
	complete(work);
	Routine::remove();
}



static void __wait_completion(struct completion *work)
{
	while (!work->done)
		__wait_event();

	work->done = 0;
}


static unsigned long
__wait_completion_timeout(struct completion *work, unsigned long timeout)
{
	unsigned long _j = jiffies + (timeout / HZ);
	while (!work->done) {
		__wait_event();

		if (_j >= jiffies)
			return 0;
	}

	work->done = 0;

	return _j - jiffies;
}


unsigned long wait_for_completion_timeout(struct completion *work,
                                          unsigned long timeout)
{
	dde_kit_log(DEBUG_COMPLETION, "%p state: %u", work, work->done);
	__wait_completion(work);
	return 1;
}


int wait_for_completion_interruptible(struct completion *work)
{
	dde_kit_log(DEBUG_COMPLETION, "%p state: %u", work, work->done);

	__wait_completion(work);
	return 0;
}


long wait_for_completion_interruptible_timeout(struct completion *work,
                                               unsigned long timeout)
{
	dde_kit_log(DEBUG_COMPLETION, "%p state: %u", work, work->done);
	__wait_completion(work);
	return 1;
}


void wait_for_completion(struct completion *work)
{
	dde_kit_log(DEBUG_COMPLETION, "%p state: %u", work, work->done);
	__wait_completion(work);
}


/*******************
 ** linux/timer.h **
 *******************/

signed long schedule_timeout_uninterruptible(signed long timeout)
{
	dde_kit_log(DEBUG_COMPLETION, "%ld\n", timeout);
	__wait_event();
	return 0;
}


int wake_up_process(struct task_struct *tsk)
{
	Routine::schedule_all();
	return 0;
}

