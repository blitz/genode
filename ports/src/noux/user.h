/*
 * \brief  User database entry
 * \author Julian Stecklina
 * \date   2012-02-11
 */

#pragma once

/* Genode includes */
#include <util/list.h>
#include <util/xml_node.h>

/* Noux includes */
#include <noux_session/sysio.h>

namespace Noux {

	class User : public Genode::List<User>::Element
	{
		// XXX We want C++ 2011
		//constexpr char DEFAULT_SHELL[] = "/bin/sh";
		//constexpr char DEFAULT_HOME[]  = "/";

		enum { MAX_USERNAME_LEN = 128 };

		char     _name[MAX_USERNAME_LEN];
		char     _fullname[MAX_USERNAME_LEN];
		unsigned _uid;
		unsigned _gid;
		char     _home[Sysio::MAX_PATH_LEN];
		char     _shell[Sysio::MAX_PATH_LEN];

		// XXX Is there a better way to do this?
		void try_string(Genode::Xml_node config,
				const char *name, const char *dflt,
				char *buf, Genode::size_t size)
		{
			try {
				config.attribute(name).value(buf, size);
			} catch (Genode::Xml_node::Nonexistent_attribute) {
				Genode::strncpy(buf, dflt, size);
			}
		}

		unsigned try_unsigned(Genode::Xml_node config,
				      const char *name, unsigned dflt)
		{
			unsigned res;
			try {
				config.attribute(name).value(&res);
			} catch (Genode::Xml_node::Nonexistent_attribute) {
				res = dflt;
			}
			return res;
		}

	public:
		unsigned uid() const { return _uid; }

		User(Genode::Xml_node config)
		{
			try_string(config, "name",  "john",    _name,  sizeof(_name));
			try_string(config, "fullname",  "John Genode", _fullname,  sizeof(_fullname));
			try_string(config, "home",  "/",       _home,  sizeof(_home));
			try_string(config, "shell", "/bin/sh", _shell, sizeof(_shell));
			_uid = try_unsigned(config, "uid", 0);
			_gid = try_unsigned(config, "gid", 0);
		};

		User(const char *name, const char *fullname,
		     unsigned uid = 0, unsigned gid = 0,
		     const char *home = "/", const char *shell = "/bin/sh")
		{
			Genode::strncpy(_name, name, sizeof(_name));
			Genode::strncpy(_fullname, fullname, sizeof(_fullname));
			_uid = uid;
			_gid = gid;
			Genode::strncpy(_home, home, sizeof(_home));
			Genode::strncpy(_shell, home, sizeof(_shell));
		}
	};
}

// EOF
