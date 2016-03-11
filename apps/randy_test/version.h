#ifndef VERSION_H
#define VERSION_H

#include <boost\format.hpp>

namespace Version
{
	struct Version
	{
		static const int MAJOR;
		static const int MINOR;
		static const int REVISION;
		static const int BUILD;
	};
	const int Version::MAJOR = 0;
	const int Version::MINOR = 8;
	const int Version::REVISION = 0;
	const int Version::BUILD = 20160217;

	std::string get_version(std::string app_name)
	{
		return boost::str(boost::format("%s Version:%d.%d.%d build:%s") % app_name % Version::MAJOR % Version::MINOR % Version::REVISION % Version::BUILD);
	}
}

#endif // VERSION_H