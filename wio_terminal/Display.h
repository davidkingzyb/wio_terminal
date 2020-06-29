//2020/5/13 by cp

//320 x 240
#include "Arduino.h"

#ifndef DISPLAY_H
#define DISPLAY_H

namespace Display{
    void init();
    void print(String s,int x,int y);
    void turnOn();
    void turnOff();
    void showA(String s);
    void showB(String s);
    void showC(String s);
    void showCmd(String s);
    void showStatus(int x,int y,int16_t color);
    void clear();
    void clearKeyboard();
    void showKey(int x,int y,String s,int16_t color);
    void clearMenus(int len);
    void showMenu(int index,String name,int16_t color);
    void clearScripts(int len);
    void showScripts(int index,String name,int16_t color);
}

#endif