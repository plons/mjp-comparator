#ifndef MJPFileComparator_h
#define MJPFileComparator_h

#include "MJPFile.hpp"

class MJPFileComparator {
public:
	MJPFileComparator(const boost::filesystem::path& foxBeleidFile, const boost::filesystem::path& customFile);

	std::vector<MJPEntry> getEntriesMissingInFoxBeleid() const;
	std::vector<MJPEntry> getEntriesMissingInCustomFile() const;

	void printMismatchingAmounts() const;

private:
	MJPFile foxBeleidFile;
	MJPFile customFile;
};


#endif
