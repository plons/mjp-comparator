#include "MJPEntry.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <iomanip>
#include <map>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <vector>

using boost::algorithm::split;
using boost::format;

using std::accumulate;
using std::invalid_argument;
using std::map;
using std::make_tuple;
using std::runtime_error;
using std::string;
using std::tuple;
using std::vector;

static vector<string> splitLine(const string& line, const string& delimiter, uint minimumNrOfFields)
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

static double parseAmount(string amountString, bool usesCommaAsdecimalSeperator = true)
{
	boost::algorithm::replace_all(amountString, "€", "");
	if (usesCommaAsdecimalSeperator)
	{
		boost::algorithm::replace_all(amountString, ".", "");
		boost::algorithm::replace_all(amountString, ",", ".");
	}
	else
	{
		boost::algorithm::replace_all(amountString, ",", "");
	}
	boost::algorithm::replace_all(amountString, " ", "");
	if (amountString.empty() || amountString == "-") return 0;
	if (amountString == "VOLLEDIGNAARCONTRACTUELEN") return 0;
	try
	{
		return boost::lexical_cast<double>(amountString);
	}
	catch (const boost::bad_lexical_cast& e)
	{
		throw std::invalid_argument(
				(format("Could not convert the amount '%s' to a double value. (decimalSeparator=%s)")
						%amountString %(usesCommaAsdecimalSeperator?",":".")).str());
	}
}

MJPEntry::FactoryFunction MJPEntry::factoryFunction(MJPEntry::Source source, MJPEntry::Type type, uint32_t year)
{
	using Key = tuple<MJPEntry::Source, MJPEntry::Type, uint32_t>;
	static map<Key,FactoryFunction> factoryFunctions{
		{make_tuple(MJPEntry::FOXBELEID, MJPEntry::MJP, 2015), &MJPEntry::fromFoxBeleidMJP2015},
		{make_tuple(MJPEntry::FOXBELEID, MJPEntry::BUDGET_CHANGE, 2016), &MJPEntry::fromFoxBeleidBudgetChange2016},
		{make_tuple(MJPEntry::FOXBELEID, MJPEntry::MJP, 2016), &MJPEntry::fromFoxBeleidMJP2016},
		{make_tuple(MJPEntry::FOXBELEID, MJPEntry::MJP, 2017), &MJPEntry::fromFoxBeleidMJP2016},
		{make_tuple(MJPEntry::CUSTOM_FILE, MJPEntry::MJP, 2015), &MJPEntry::fromCustomFileMJP2015},
		{make_tuple(MJPEntry::CUSTOM_FILE, MJPEntry::BUDGET_CHANGE, 2016), &MJPEntry::fromCustomFileBudgetChange2016},
		{make_tuple(MJPEntry::CUSTOM_FILE, MJPEntry::MJP, 2016), &MJPEntry::fromCustomFileMJP2016},
		{make_tuple(MJPEntry::CUSTOM_FILE, MJPEntry::MJP, 2017), &MJPEntry::fromCustomFileMJP2016},
	};

	auto iter = factoryFunctions.find(std::make_tuple(source, type, year));
	if (iter != factoryFunctions.end())
	{
		return iter->second;
	}
	else
	{
		throw runtime_error((format("Could not find factory function for MJPEntry from (Source=%d, type=%d year=%d)")%source %type %year).str());
	}
}

MJPEntry MJPEntry::fromFoxBeleidMJP2015(const string& line)
{

	vector<string> columns = splitLine(line, ";", 12);
	vector<string> combinedKeyParts = splitLine(columns.at(0), "/", 8);


	return MJPEntry(
		MJPEntryKey(columns.at(1), columns.at(2), columns.at(3), columns.at(4), combinedKeyParts.at(7)),
		{
			parseAmount(columns.at(7)),
			parseAmount(columns.at(8)),
			parseAmount(columns.at(9)),
			parseAmount(columns.at(10)),
			parseAmount(columns.at(11))
		}
	);
}

MJPEntry MJPEntry::fromFoxBeleidBudgetChange2016(const string& line)
{

	vector<string> columns = splitLine(line, ";", 7);
	vector<string> combinedKeyParts = splitLine(columns.at(0), "/", 8);


	return MJPEntry(
		MJPEntryKey(columns.at(1), columns.at(2), columns.at(3), columns.at(4), combinedKeyParts.at(7)),
		{
			parseAmount(columns.at(6)),
		}
	);
}

MJPEntry MJPEntry::fromFoxBeleidMJP2016(const string& line)
{

	vector<string> columns = splitLine(line, ";", 12);
	vector<string> combinedKeyParts = splitLine(columns.at(0), "/", 8);


	return MJPEntry(
		MJPEntryKey(columns.at(1), columns.at(2), columns.at(3), columns.at(4), combinedKeyParts.at(7)),
		{
			parseAmount(columns.at(6), false),
			parseAmount(columns.at(7), false),
			parseAmount(columns.at(8), false),
			parseAmount(columns.at(9), false),
			parseAmount(columns.at(10), false),
			parseAmount(columns.at(11), false)
		}
	);
}

MJPEntry MJPEntry::fromCustomFileMJP2015(const string& line)
{
	vector<string> columns = splitLine(line, ";", 15);

	return MJPEntry(
		MJPEntryKey(columns.at(1), columns.at(3), columns.at(5), columns.at(7), columns.at(4)),
		{
			parseAmount(columns.at(10)),
			parseAmount(columns.at(11)),
			parseAmount(columns.at(12)),
			parseAmount(columns.at(13)),
			parseAmount(columns.at(14))
		}
	);
}

MJPEntry MJPEntry::fromCustomFileBudgetChange2016(const string& line)
{
	vector<string> columns = splitLine(line, ";", 17);

	return MJPEntry(
		MJPEntryKey(columns.at(0), columns.at(2), columns.at(5), columns.at(7), columns.at(4)),
		{
			parseAmount(columns.at(16)),
		}
	);
}

MJPEntry MJPEntry::fromCustomFileMJP2016(const string& line)
{
	vector<string> columns = splitLine(line, ";", 16);
	vector<string> combinedKeyParts = splitLine(columns.at(0), "/", 8);

	return MJPEntry(
		MJPEntryKey(columns.at(1), columns.at(3), columns.at(4), columns.at(6), combinedKeyParts.at(7)),
		{
			parseAmount(columns.at(10), false),
			parseAmount(columns.at(11), false),
			parseAmount(columns.at(12), false),
			parseAmount(columns.at(13), false),
			parseAmount(columns.at(14), false),
			parseAmount(columns.at(15), false),
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

std::ostream& operator<<(std::ostream& ws, const MJPEntry& entry)
{
	auto key = entry.getKey();
	auto amounts = entry.getAmounts();
	ws << key << ": ";
	ws << std::fixed << std::setprecision(2);
	std::copy(amounts.begin(), amounts.end(), std::ostream_iterator<double>(ws, " "));
	return ws;
}
