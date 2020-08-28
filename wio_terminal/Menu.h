/*
 2020/5/18 by cp
*/
#include "Arduino.h"

#ifndef MENU_H
#define MENU_H

typedef struct {
    const char* name;
    void (*func)();
} MenuFunc;

typedef struct {
    const char* name;
    String script;
} ScriptString;

namespace Menu{
    void menuScripts();
    void showScripts();
    void menuMenus();
    void showMenus();
    void menuKeyboard();
    void showKeyboard();
    void doUp();
    void doRight();
    void doDown();
    void doLeft();
    void doConfirm();
    void history();
    void escapeANSI();
}

#endif