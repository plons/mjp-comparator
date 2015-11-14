#include "MJPFile.hpp"

#include "FileUtilities.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::format;
using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

MJPFile::MJPFile(const path& file, const ConvertLineFunction& convert) :
		file(file)
{
	if (!exists(file))
	{
		throw std::invalid_argument((format("The given file %s does not exist!")%file).str());
	}
	ifstream input(file.string());
	try
	{
		init(input, convert);
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Error while parsing file " << file << std::endl;
		throw e;
	}
}

MJPFile::MJPFile(istream& input, const ConvertLineFunction& convert)
{
	init(input, convert);
}

uint MJPFile::getNrEntries() const
{
	return entries.size();
}

vector<MJPEntry> MJPFile::getAllEntries() const
{
	vector<MJPEntry> allEntries;
	for (auto& entry : entries)
	{
		allEntries.push_back(entry.second);
	}
	return allEntries;
}

bool MJPFile::containsKey(const MJPEntryKey& key) const
{
	return entries.find(key) != entries.end();
}

const MJPEntry& MJPFile::getEntry(const MJPEntryKey& key) const
{
	auto iter = entries.find(key);
	if (iter == entries.end()) throw std::runtime_error((format("Could not return entry for key %s")%key).str());
	return iter->second;
}

void MJPFile::init(istream& input, const ConvertLineFunction& convert)
{
	uint lineNumber = 0;
	plons::common::FileUtilities::processLines(input, [&](const string& line){
		try
		{
			if (lineNumber++ > 0
					&& !line.empty()
					&& !boost::algorithm::starts_with(line, ";;;"))
			{
				MJPEntry entry = convert(line);
				if (containsKey(entry.getKey()))
				{
					entries.at(entry.getKey()).updateAmounts(entry.getAmounts());
				}
				else
				{
					entries.insert(std::make_pair(entry.getKey(), entry));
				}
			}
		}
		catch (const std::invalid_argument& e)
		{
			std::cerr << "Error while parsing line " << lineNumber << ": " << line << std::endl;
			throw e;
		}
	});
}

void MJPFile::printOn(std::ostream& ws) const
{
	for (auto& entry : entries)
	{
		auto& amounts = entry.second.getAmounts();
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
