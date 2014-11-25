#ifndef MJPEntry_h
#define MJPEntry_h

#include "MJPEntryKey.hpp"

#include <memory>
#include <vector>

class MJPEntry
{
public:
	static MJPEntry fromFoxBeleidFile(const std::string& line);
	static MJPEntry fromCustomFile(const std::string& line);

	MJPEntry(const MJPEntry& other);
	virtual ~MJPEntry() {}


	virtual const MJPEntryKey& getKey() const final;
	virtual const std::vector<double>& getAmounts() const final;
	virtual void updateAmounts(const std::vector<double>& amountsToAdd) final;

private:
	MJPEntry(MJPEntryKey&& key, const std::initializer_list<double>& amounts);
	MJPEntryKey key;
	std::vector<double> amounts;
};


#endif
