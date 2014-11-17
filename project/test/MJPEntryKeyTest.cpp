#include <gmock/gmock.h>

#include "MJPEntryKey.hpp"

TEST(MJPEntryKey, canParseCorrectValues)
{
	MJPEntryKey key1("0.3.0.0", "0030-00", "6479999", "IE3", "U");
	MJPEntryKey key2("GBB-CBS", "0119-00", "6143007", "IE-GEEN", "O");
}
