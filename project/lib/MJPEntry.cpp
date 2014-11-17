#include "MJPEntry.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

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
	boost::algorithm::replace_all(amountString, ",", "");
	boost::algorithm::replace_all(amountString, " ", "");
	if (amountString.empty()) return 0;
	return boost::lexical_cast<double>(amountString);
}

MJPEntry::MJPEntry() {}
MJPEntry::MJPEntry(const MJPEntry& other) :
		key(new MJPEntryKey(*other.key)),
		amounts(other.amounts)
{
}

const MJPEntryKey& MJPEntry::getKey() const
{
	assert(key);
	return *key;
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

FoxBeleidMJPEntry::FoxBeleidMJPEntry(const std::string& line)
{
	vector<string> columns = splitLine(line, ";", 13);
	vector<string> combinedKeyParts = splitLine(columns[0], "/", 9);
	key.reset(new MJPEntryKey(columns[1], columns[2], columns[3], columns[5], combinedKeyParts[8]));
	amounts.push_back(parseAmount(columns[8]));
	amounts.push_back(parseAmount(columns[9]));
	amounts.push_back(parseAmount(columns[10]));
	amounts.push_back(parseAmount(columns[11]));
	amounts.push_back(parseAmount(columns[12]));
}

CustomMJPEntry::CustomMJPEntry(const string& line)
{
	vector<string> columns = splitLine(line, ";", 15);
	key.reset(new MJPEntryKey(columns[1], columns[3], columns[5], columns[6], columns[8]));
	amounts.push_back(parseAmount(columns[10]));
	amounts.push_back(parseAmount(columns[11]));
	amounts.push_back(parseAmount(columns[12]));
	amounts.push_back(parseAmount(columns[13]));
	amounts.push_back(parseAmount(columns[14]));
}
