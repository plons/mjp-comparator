#ifndef MJPEntry_h
#define MJPEntry_h

#include "MJPEntryKey.hpp"

#include <functional>
#include <memory>
#include <vector>

class MJPEntry {
public:
	using FactoryFunction = std::function<MJPEntry (const std::string&)>;
	enum Source{FOXBELEID, CUSTOM_FILE};
	enum Type {BUDGET_CHANGE, MJP};

	static FactoryFunction factoryFunction(Source source, Type type, uint32_t year);

	// MJP 2015
	static MJPEntry fromFoxBeleidMJP2015(const std::string& line);
	static MJPEntry fromCustomFileMJP2015(const std::string& line);

	// Budget change 2106
	static MJPEntry fromFoxBeleidBudgetChange2016(const std::string& line);
	static MJPEntry fromCustomFileBudgetChange2016(const std::string& line);

	// MJP 2016
	static MJPEntry fromFoxBeleidMJP2016(const std::string& line);
	static MJPEntry fromCustomFileMJP2016(const std::string& line);


	MJPEntry(const MJPEntry& other);
	virtual ~MJPEntry() {}


	virtual const MJPEntryKey& getKey() const final;
	virtual const std::vector<double>& getAmounts() const final;
	virtual void updateAmounts(const std::vector<double>& amountsToAdd) final;
	virtual double totalAmount() const final;

private:
	MJPEntry(MJPEntryKey&& key, const std::initializer_list<double>& amounts);
	MJPEntryKey key;
	std::vector<double> amounts;
};

std::ostream& operator<<(std::ostream& ws, const MJPEntry& entry);

#endif
