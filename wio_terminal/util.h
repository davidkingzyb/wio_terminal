/*
 2020/5/13 by cp
*/
#include <vector>
#include "Arduino.h"

#ifndef UTIL_H
#define UTIL_H

namespace Util{
    std::vector<String> split(String str,char sep);
    void record(String s);
    void printTerminal();
}

#endif
