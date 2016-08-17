#include "MJPEntry.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <vector>
#include <stdexcept>

using boost::algorithm::split;
using boost::format;

using std::invalid_argument;
using std::string;
using std::vector;

static vector<string> splitLine(const std::string& line, const std::string& delimiter, uint minimumNrOfFields)
{
	vector<string> results;
	boost::algorithm::split(results, line, boost::is_any_of(delimiter));
	if (results.size() < minimumNrOfFields)
	{
		throw std::invalid_argument((format("Error while splitting line '%s' using delimiter '%s': \nExpected at least %d entries, instead found %d entries.")
				%line %delimiter %minimumNrOfFields %results.size()).str());
	}
	return results;
}

static double parseAmount(string amountString)
{
	boost::algorithm::replace_all(amountString, "€", "");
	boost::algorithm::replace_all(amountString, ".", "");
	boost::algorithm::replace_all(amountString, ",", ".");
	boost::algorithm::replace_all(amountString, " ", "");
	if (amountString.empty() || amountString == "-") return 0;
	if (amountString == "VOLLEDIGNAARCONTRACTUELEN") return 0;
	try
	{
		return boost::lexical_cast<double>(amountString);
	}
	catch (const boost::bad_lexical_cast& e)
	{
		throw std::invalid_argument((format("Could not convert the amount '%s' to a double value.")%amountString).str());
	}
}

MJPEntry MJPEntry::fromFoxBeleidFile(const std::string& line)
{

	vector<string> columns = splitLine(line, ";", 12);
	vector<string> combinedKeyParts = splitLine(columns[0], "/", 8);


	return MJPEntry(
		MJPEntryKey(columns[1], columns[2], columns[3], columns[4], combinedKeyParts[7]),
		{
		parseAmount(columns[7]),
		parseAmount(columns[8]),
		parseAmount(columns[9]),
		parseAmount(columns[10]),
		parseAmount(columns[11])
		}
	);
}

MJPEntry MJPEntry::fromFoxBeleidFile2016(const std::string& line)
{

	vector<string> columns = splitLine(line, ";", 7);
	vector<string> combinedKeyParts = splitLine(columns[0], "/", 8);


	return MJPEntry(
		MJPEntryKey(columns[1], columns[2], columns[3], columns[4], combinedKeyParts[7]),
		{
		parseAmount(columns[6]),
		}
	);
}

MJPEntry MJPEntry::fromCustomFile(const std::string& line)
{
	vector<string> columns = splitLine(line, ";", 15);

	return MJPEntry(
		MJPEntryKey(columns[1], columns[3], columns[5], columns[7], columns[4]),
		{
				parseAmount(columns[10]),
				parseAmount(columns[11]),
				parseAmount(columns[12]),
				parseAmount(columns[13]),
				parseAmount(columns[14])
		}
	);
}

MJPEntry MJPEntry::fromCustomFile2016(const std::string& line)
{
	vector<string> columns = splitLine(line, ";", 18);

	return MJPEntry(
		MJPEntryKey(columns[0], columns[2], columns[5], columns[7], columns[4]),
		{
				parseAmount(columns[16]),
		}
	);
}


MJPEntry::MJPEntry(MJPEntryKey&& key, const std::initializer_list<double>& amounts) :
		key(key),
		amounts(amounts)
{
}

MJPEntry::MJPEntry(const MJPEntry& other) :
		key(other.key),
		amounts(other.amounts)
{
}

const MJPEntryKey& MJPEntry::getKey() const
{
	return key;
}

const vector<double>& MJPEntry::getAmounts() const
{
	return amounts;
}

void MJPEntry::updateAmounts(const std::vector<double>& amountsToAdd)
{
	assert(amounts.size() == amountsToAdd.size());
	for (uint index = 0; index < amounts.size(); ++index)
	{
		amounts[index] += amountsToAdd[index];
	}
}

double MJPEntry::totalAmount() const
{
	return accumulate(amounts.begin(), amounts.end(), 0);
}
