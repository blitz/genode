NOUX_CONFIGURE_ARGS  =
NOUX_ENV            += CHGRP=true CHOWN=true CHMOD=true
LIBS += ncurses

include $(REP_DIR)/mk/noux.mk

#
# Make the ncurses linking test succeed
#
Makefile: dummy_libs

NOUX_LDFLAGS += -L$(PWD)

dummy_libs: libncurses.a

libncurses.a:
	$(VERBOSE)$(AR) -rc $@

