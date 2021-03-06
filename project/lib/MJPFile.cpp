#include "MJPFile.hpp"

#include "FileUtilities.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <fstream>
#include <regex>

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::format;
using std::cerr;
using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

MJPFile::MJPFile(const path& file, const MJPEntry::FactoryFunction& convert) :
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

MJPFile::MJPFile(istream& input, const MJPEntry::FactoryFunction& convert)
{
	init(input, convert);
}

size_t MJPFile::getNrEntries() const
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

void MJPFile::removeEntry(const MJPEntryKey& key)
{
	entries.erase(key);
}

void MJPFile::init(istream& input, const MJPEntry::FactoryFunction& convert)
{
	auto lineNumber = 0U;
	plons::common::FileUtilities::processLines(input, [&](const string& line)
	{
		try
		{
			if (lineNumber++ > 0
					&& !line.empty()
					&& !boost::algorithm::starts_with(line, ";;;")
					&& !std::regex_match(line, std::regex("[^;]+;*"))) // Ignore comment in first column
			{
				MJPEntry entry = convert(line);
				if (entry.getKey().actie == "GBR-VBJ") return;

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
