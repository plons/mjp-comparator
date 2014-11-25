#include "FileUtilities.hpp"

#include <fstream>

using boost::filesystem::path;
using std::ifstream;
using std::istream;
using std::function;
using std::string;

namespace plons {
namespace common {

void FileUtilities::processFile(const path& inputFile, const function<void (const string& line)>& processLine)
{
	ifstream file(inputFile.string());
	processLines(file, processLine);
}

void FileUtilities::processLines(istream& content, const function<void(const string&)>& process)
{
	string line;
	while (getline(content, line))
	{
		process(line);
	}
}


}  // namespace common
}  // namespace plons


