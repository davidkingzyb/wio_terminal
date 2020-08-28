/*
 2020/5/13 by cp
*/
#include <vector>
#include "Arduino.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"

#ifndef UTIL_H
#define UTIL_H

namespace Util{
    std::vector<String> split(String str,char sep);
    void record(String s);
    void printTerminal();
    String escapeANSI(String s);

    void initSDCard();
    void lsAndLog();
    bool readHistoryRecord();
}

#endif
