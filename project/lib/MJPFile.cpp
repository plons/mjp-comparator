#include "MJPFile.hpp"

#include "FileUtilities.hpp"

#include <fstream>

using boost::filesystem::path;
using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using std::shared_ptr;

MJPFile::MJPFile(const path& file, const ConvertLineFunction& convert)
{
	ifstream input(file.string());
	init(input, convert);
}

MJPFile::MJPFile(istream& input, const ConvertLineFunction& convert)
{
	init(input, convert);
}

uint MJPFile::getNrEntries() const
{
	return entries.size();
}

bool MJPFile::containsKey(const MJPEntryKey& key) const
{
	return entries.find(key) != entries.end();
}

void MJPFile::init(istream& input, const ConvertLineFunction& convert)
{
	bool skippedFirst = false;
	plons::common::FileUtilities::processLines(input, [&](const string& line){
		if (!skippedFirst)
		{
			skippedFirst = true;
		}
		else
		{
			shared_ptr<MJPEntry> entry = convert(line);
			if (containsKey(entry->getKey()))
			{
				entries[entry->getKey()]->updateAmounts(entry->getAmounts());
			}
			else
			{
				entries[entry->getKey()] = entry;
			}
		}
	});
}

void MJPFile::printOn(std::ostream& ws) const
{
	for (auto& entry : entries)
	{
		auto amounts = entry.second->getAmounts();
		ws << entry.first << " ";
		std::copy(amounts.begin(), amounts.end(), std::ostream_iterator<double>(ws, " "));
		ws << "\n";
	}
}

ostream& operator<<(ostream& ws, const MJPFile& file)
{
	file.printOn(ws);
	return ws;
}
