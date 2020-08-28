/*
 2020/5/18 by cp
*/
#include "Menu.h"
#include "Display.h"
#include "Util.h"

const int SCRIPTLEN=2;
ScriptString scripts[SCRIPTLEN]={
    {"stty","stty rows 14;stty columns 50;"},
    {"halt","sudo halt"}
};

int script_index=-1;

const int MENULEN= 3;
MenuFunc menus[MENULEN]={
    {"screen off",Display::turnOff},
    {"escape ANSI",Menu::escapeANSI},
    {"history",Menu::history},
};

int menu_index=-1;
extern bool isLight;

int cmd_index=0;

char keyboard_1[40]={
    (char)0x1b,';','|','<','>','_','-','&',(char)0x7f,'\b',
    'q','w','e','r','t','y','u','i','o','p',
    'a','s','d','f','g','h','j','k','l','\n',
    'z','x','c','v','b','n','m',' ','\t',(char)0x03
};

char keyboard_2[40]={
    '~','1','2','3','$','{','}','(',')','?',
    '`','4','5','6','+','*','[',']','\\','|',
    '@','7','8','9','/','-','_','"','\'','\n',
    '#','%','0','.','=','!',',',':',';','^'
};

char keyboard_3[40]={
    '1','2','3','4','5','6','7','8','9','0',
    'Q','W','E','R','T','Y','U','I','O','P',
    'A','S','D','F','G','H','J','K','L','\n',
    'Z','X','C','V','B','N','M',' ','_','\b'
};

int keyboard_index=0;
int key_index=26;
char *now_keyboard;

extern String cmd_str;
extern std::vector<String> history_cmd;

extern bool is_history;
int history_index=0;
extern bool is_sd_card;
extern std::vector<String> history_record;
extern File history_file;
extern uint8_t history_file_mode;
extern String log_name;

void Menu::history(){
    is_history=!is_history;
    if(is_history){
        Display::showA("history on");
        if(is_sd_card){
            if(log_name!=""){
                history_file.close();
            }
            Util::lsAndLog();
        }
    }else{
        Display::showA("history off");
        history_file.close();
        log_name="";
    }
}

extern bool is_escape_ansi;

void Menu::escapeANSI(){
    is_escape_ansi=!is_escape_ansi;
    if(is_escape_ansi){
        Display::showA("escape ANSI on");
    }else{
        Display::showA("escape ANSI off");
    }
}

void Menu::menuScripts(){
    menu_index=-1;
    Display::clearMenus(MENULEN);
    keyboard_index=0;
    Display::clearKeyboard();
    script_index++;
    if(script_index>=SCRIPTLEN){
        script_index=-1;
        Display::showB("script");
        Display::clearScripts(SCRIPTLEN);
    }else{
        Menu::showScripts();
    }
}

void Menu::showScripts(){
    Display::clearScripts(SCRIPTLEN);
    for(int i=0;i<SCRIPTLEN;i++){
        if(script_index==i){
            Display::showScripts(i,scripts[i].name,0x03EF);
        }else{
            Display::showScripts(i,scripts[i].name,0x07FF);
        }
    }
}


void Menu::menuMenus(){
    script_index=-1;
    Display::clearScripts(SCRIPTLEN);
    keyboard_index=0;
    Display::clearKeyboard();
    menu_index++;
    if(menu_index>=MENULEN){
        menu_index=-1;
        Display::showC("menu");
        Display::clearMenus(MENULEN);
    }else{
        Menu::showMenus();
    }
    if(!isLight){
        Display::turnOn();
    }
}

void Menu::showMenus(){
    Display::clearMenus(MENULEN);
    for(int i=0;i<MENULEN;i++){
        if(menu_index==i){
            Display::showMenu(i,menus[i].name,0x03EF);
        }else{
            Display::showMenu(i,menus[i].name,0x07FF);
        }
    }
}



void Menu::menuKeyboard(){
    script_index=-1;
    Display::clearScripts(SCRIPTLEN);
    menu_index=-1;
    Display::clearMenus(MENULEN);
    key_index=26;
    if(keyboard_index==0){
        keyboard_index=1;
        now_keyboard=keyboard_1;
        Menu::showKeyboard();
    }else if(keyboard_index==1){
        keyboard_index=2;
        now_keyboard=keyboard_2;
        Menu::showKeyboard();
    }else if(keyboard_index==2){
        keyboard_index=3;
        now_keyboard=keyboard_3;
        Menu::showKeyboard();
    }else{
        keyboard_index=0;
        Display::clearKeyboard();
    }
}

void Menu::showKeyboard(){
    Display::clearKeyboard();
    for(int i=0;i<40;i++){
        int x=i%10*20+100;
        int y=floor(i/10)*20+15;
        if(i==key_index){
            Display::showKey(x,y,String(*(now_keyboard+i)),0x03EF);
        }else if(*(now_keyboard+i)=='\n'){
            Display::showKey(x,y,"CR",0x07E0);
        }else if(*(now_keyboard+i)=='\b'){
            Display::showKey(x,y,"BS",0xFDA0);
        }else if(*(now_keyboard+i)=='\t'){
            Display::showKey(x,y,"TB", 0xFFE0);
        }else if(*(now_keyboard+i)==(char)0x7f){
            Display::showKey(x,y,"DL",0xFDA0);
        }else if(*(now_keyboard+i)==(char)0x03){
            Display::showKey(x,y,"^C",0xF800);
        }else if(*(now_keyboard+i)==(char)0x1b){
            Display::showKey(x,y,"ES",0xF800);
        }else{
            Display::showKey(x,y,String(*(now_keyboard+i)),0x07FF);
        }
    }
}

void Menu::doConfirm(){
    if(keyboard_index!=0){
        char input=*(now_keyboard+key_index);
        Serial1.write(input);
        if(input=='\n'){
            keyboard_index=0;
            Display::clearKeyboard();
        }
    }else if(menu_index!=-1){
        menus[menu_index].func();
        Display::showC(menus[menu_index].name);
        menu_index=-1;
        Display::clearMenus(MENULEN);
    }else if(script_index!=-1){
        Serial1.write(scripts[script_index].script.c_str());
        Serial1.write("\n");
        Display::showB(scripts[script_index].name);
        script_index=-1;
        Display::clearScripts(SCRIPTLEN);
    }else{
        String tmp=cmd_str;
        cmd_str="";
        Serial1.write(tmp.c_str());
        Serial1.write("\n");
        Display::showCmd("");
    }
}


void Menu::doUp(){
    if(keyboard_index!=0&&key_index>9){
        key_index=key_index-10;
        Menu::showKeyboard();
    }else if(menu_index>0){
        menu_index--;
        Menu::showMenus();
    }else if(script_index>0){
        script_index--;
        Menu::showScripts();
    }else{
        if(cmd_index>0){
            cmd_index--;
            cmd_str=history_cmd[cmd_index];
            Display::showCmd(cmd_str);
        }  
    }
}

void Menu::doRight(){
    if(keyboard_index!=0&&key_index<39){
        key_index++;
        Menu::showKeyboard();
    }else if(is_history){
        history_index=history_index-10;
        int head=history_record.size()-history_index-15;
        if(head>=0&&head+14<history_record.size()){
            Display::clear();
            for(int i=0;i<15;i++){
                Display::print(history_record[head+i],2,20+i*15);
            }
        }else{
            history_index=history_index+10;
        }
    }
}

void Menu::doDown(){
    if(keyboard_index!=0&&key_index<30){
        key_index=key_index+10;
        Menu::showKeyboard();
    }else if(menu_index<MENULEN-1&&menu_index!=-1){
        menu_index++;
        Menu::showMenus();
    }else if(script_index<SCRIPTLEN-1 && script_index!=-1){
        script_index++;
        Menu::showScripts();
    }else{
        history_index=0;
        cmd_index=history_cmd.size();
        history_record.clear();
        if(is_history&&history_file_mode==0&&log_name!=""){
            history_file=SD.open(log_name,FILE_APPEND);
            history_file_mode=FILE_APPEND;
            Serial.println("reopen history_file");
        }
        Util::printTerminal();
        Display::showCmd("");
    }
}

void Menu::doLeft(){
    if(keyboard_index!=0 && key_index>0){
        key_index--;
        Menu::showKeyboard();
    }else if(is_history){
        Util::readHistoryRecord();
        history_index=history_index+10;
        int head=history_record.size()-history_index-15;
        if(head>=0&&head+14<history_record.size()){
            Display::clear();
            for(int i=0;i<15;i++){
                Display::print(history_record[head+i],2,20+i*15);
            }
        }else{
            history_index=history_index-10;
        }
    }
}

