#ifndef MJPEntry_h
#define MJPEntry_h

#include "MJPEntryKey.hpp"

#include <memory>
#include <vector>

class MJPEntry
{
public:
	MJPEntry(const MJPEntry& other);
	virtual ~MJPEntry() {}

	virtual const MJPEntryKey& getKey() const final;
	virtual const std::vector<double>& getAmounts() const final;
	virtual void updateAmounts(const std::vector<double>& amountsToAdd) final;

protected:
	MJPEntry();
	std::unique_ptr<MJPEntryKey> key;
	std::vector<double> amounts;
};

class FoxBeleidMJPEntry : public MJPEntry
{
public:
	FoxBeleidMJPEntry(const std::string& line);
};

class CustomMJPEntry : public MJPEntry
{
public:
	CustomMJPEntry(const std::string& line);
};


#endif
