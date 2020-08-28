// 2020/5/13 by cp

#include "Util.h"
#include "Display.h"
#include <vector>

bool is_sd_card=false;
bool is_history=false;

std::vector<String> history_record;
File history_file;
std::vector<String> history_cmd;
String log_name;
uint8_t history_file_mode=0;

bool is_escape_ansi=false;
String cmd_str="";
std::vector<String> record_vec;


void Util::record(String s){
    Serial.printf("###r %d n %d len %d",s.indexOf("\r"),s.indexOf("\n"),s.length());
    if(s.length()==1&&s!="\012"&&s!="\n"&&s!="\r"){
        cmd_str+=s;
        Display::showCmd(cmd_str);
        return; 
    }

    if(s[0]=='\r'&&s[1]=='\n'&&cmd_str.length()>0){//screen keyboard
        record_vec.push_back(String(cmd_str));
        history_cmd.push_back(String(cmd_str));
        // Serial.println("### cmd "+cmd_str);
        cmd_str="";
        Display::showCmd("");
        s.remove(0,2);
    }

    if(s[0]=='\r'&&s.length()==1){//wire keyboard
        record_vec.push_back(String(cmd_str));
        history_cmd.push_back(String(cmd_str));
        // Serial.println("### cmd "+cmd_str);
        cmd_str="";
        Display::showCmd("");
        s="";
    }
    if(s[0]=='\n'&&s.length()==1){
        s="";
    }

    if(is_escape_ansi){
        s=Util::escapeANSI(s);
    }

    if(is_history){
        history_file.print(s);
    }
    
    if(s.indexOf("\r")>0){
        std::vector<String> ssv=Util::split(s,'\r');
        // Serial.printf("### split r %d",ssv.size());
        for(int ii=0;ii<ssv.size();ii++){
            if(ssv[ii].length()>52){
                record_vec.push_back(ssv[ii].substring(0,52));
                record_vec.push_back(ssv[ii].substring(52));
                // if(is_history){
                //     history_record.push_back(ssv[ii].substring(0,52));
                //     history_record.push_back(ssv[ii].substring(52));
                // }
            }else{
                record_vec.push_back(String(ssv[ii]));
                // if(is_history){
                //     history_record.push_back(String(ssv[ii]));
                // }
            }
        } 
    }

    else if(s.length()>0){
        record_vec.push_back(String(s));
        // if(is_history){
        //     history_record.push_back(String(s));
        // }
    }

    if(record_vec.size()>14){
        record_vec.erase(record_vec.begin(),record_vec.begin()+(record_vec.size()-15));
    }
    
    Util::printTerminal();
}


String Util::escapeANSI(String s){
    s.replace("\033[H","");
    s.replace("\033[J","");
    s.replace("\033[m","");
    s.replace("\033(B","");
    s.replace("\033[K","");
    s.replace("\033[1m","");
    s.replace("\033[p","");
    return s;
}

void Util::printTerminal(){
    Display::clear();
    for(int i=0;i<record_vec.size();i++){
        Display::print(record_vec[i],2,20+i*15);
    }
}

std::vector<String> Util::split(String str,char sep){
    std::vector<String> result;
    int pos2 = str.indexOf(sep);
    int pos1 = 0;
    while (pos2>=0){
        result.push_back(str.substring(pos1, pos2));
        pos1 = pos2 + 1;
        pos2 = str.indexOf(sep, pos1);
    }
    result.push_back(str.substring(pos1));
    return result;
}

void Util::initSDCard(){
    if (SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        Serial.println("sd card ok");
        is_sd_card=true;
        is_history=true;
        Util::lsAndLog();
    }else{
        Serial.println("sd card fail");
        is_history=false;
    }
}

void Util::lsAndLog(){
    File root=SD.open("/log");
    File file=root.openNextFile();
    int count=0;
    while(file){
        Serial.println(file.name());
        file=root.openNextFile();
        count++;
    }
    log_name="/log/"+String(count)+".log";
    history_file=SD.open(log_name,FILE_APPEND);
    history_file_mode=FILE_APPEND;
    Serial.println(log_name);
    Display::showA(log_name);
}

bool Util::readHistoryRecord(){
    if(is_sd_card&&log_name&&history_record.size()==0){
        if(history_file_mode==FILE_APPEND){
            history_file.close();
        }
        File f=SD.open(log_name,FILE_READ);
        history_file_mode=FILE_READ;
        if(f){
            uint32_t now = micros();
            String s="";
            while(f.available()){
                char c=(char)f.read();
                s+=c;
                if(c=='\r'){
                    history_record.push_back(s);
                    s="";
                }else if(s.length()>52){
                    history_record.push_back(s);
                    s="";
                }
            }
            Serial.println(log_name);
            Serial.print(micros()-now);
            Serial.print(" ms cause and ");
            Serial.print(history_record.size());
            Serial.println(" lines read from history record");
            f.close();
            history_file_mode=0;
        }  
    }
    return false;
}