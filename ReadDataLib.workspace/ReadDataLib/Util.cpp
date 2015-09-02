#include "Util.h"

#include <boost/locale/util.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/generator.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string u::a2u(const std::string& text)
{
	std::string result;
	if (text.empty())
	{
		return result;
	}

	try
	{
		boost::locale::generator generator;
		generator.locale_cache_enabled(true);

		result = boost::locale::conv::to_utf<u::chr>(text,
			generator(boost::locale::util::get_system_locale()));
	}
	catch (...)
	{
	}
	return result;
}

std::string u::u2a(const std::string& text)
{
	std::string result;
	if (text.empty())
	{
		return result;
	}

	try
	{
		boost::locale::generator generator;
		generator.locale_cache_enabled(true);

		result = boost::locale::conv::from_utf<u::chr>(text,
			generator(boost::locale::util::get_system_locale()));
	}
	catch (...)
	{
	}
	return result;
}

