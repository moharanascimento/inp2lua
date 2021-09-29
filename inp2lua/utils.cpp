#include "utils.h"
#include <sstream>

// function to convert string to int
int utils::string2int(std::string s)
{
 std::stringstream intValue(s);
 int number = 0;
 intValue >> number;
 return number;
}

// function to convert int to string
std::string utils::int2string(int i)
{
 std::string str;
 std::stringstream sstringValue;
 sstringValue << i;
 sstringValue >> str;
 return str;
}