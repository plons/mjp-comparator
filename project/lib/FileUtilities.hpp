#ifndef plons_common_FileUtilities_h
#define plons_common_FileUtilities_h

#include <boost/filesystem/path.hpp>

#include <functional>
#include <string>

namespace plons {
namespace common {

class FileUtilities {
public:
	static void processFile(const boost::filesystem::path& inputFile, const std::function<void (const std::string&)>& processLine);
	static void processLines(std::istream& content, const std::function<void (const std::string&)>& process);
};


} // namespace common
} // namespace plons

#endif
