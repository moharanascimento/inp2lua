#include "utils.h"
#include <sstream>

int utils::string2int(std::string s)
{
 std::stringstream intValue(s);
 int number = 0;
 intValue >> number;
 return number;
}

std::string utils::int2string(int i)
{
 std::string str;
 std::stringstream sstringValue;
 sstringValue << i;
 sstringValue >> str;
 return str;
}