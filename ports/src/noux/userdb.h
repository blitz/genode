/*
 * \brief  User database
 * \author Julian Stecklina
 * \date   2012-02-11
 */

#pragma once

/* Genode includes */
#include <util/list.h>
#include <dataspace/capability.h>

/* Local includes */
#include "user.h"

namespace Noux {

	class UserDb
	{
		Genode::List<User> _users;

	public:

		void add(User *user)
		{
			_users.insert(user);
		}

		User *find(unsigned uid)
		{
			for (User *user = _users.first(); user; user = user->next())
				if (user->uid() == uid)
					return user;
			return 0;
		}

	};

}

// EOF
