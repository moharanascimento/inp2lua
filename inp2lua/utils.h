#ifndef UTILS_INP_2_LUA
#define UTILS_INP_2_LUA
#include <string>
#include <vector>
#include "objects.h"

class utils
{
public:
 static int string2int(std::string s);
 static std::string int2string(int i);
 struct loadHash {
  std::size_t operator() (const load& l) const
  {
   auto h1 = std::hash<std::string>{}(l.loadName);
   return h1 ^ (h1 << 1 );
  }
 };
 struct equalHash {
  bool operator() (const load& l, const load& r) const
  {
   return l == r;
  }
 };

};


#endif