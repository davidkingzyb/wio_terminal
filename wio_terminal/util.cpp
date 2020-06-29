// 2020/5/13 by cp

#include "Util.h"
#include "Display.h"
#include <vector>

bool is_history=false;

std::vector<String> history_record;

std::vector<String> history_cmd;

String cmd_str="";
std::vector<String> record_vec;

void Util::record(String s){ 
    if(s.length()==1&&s!="\n"){
        cmd_str+=s;
        Display::showCmd(cmd_str);
        return; 
    }
    if(s.indexOf("\n")!=-1){
        record_vec.push_back(String(cmd_str));
        history_cmd.push_back(String(cmd_str));
        cmd_str="";
    }
    
    if(s.indexOf("\r")!=-1){
        std::vector<String> ssv=Util::split(s,'\r');
        for(int ii=0;ii<ssv.size();ii++){
            if(String(ssv[ii])=="\n"){
                continue;
            }
            if(ssv[ii].length()>52){
                record_vec.push_back(ssv[ii].substring(0,52));
                record_vec.push_back(ssv[ii].substring(52));
                if(is_history){
                    history_record.push_back(ssv[ii].substring(0,52));
                    history_record.push_back(ssv[ii].substring(52));
                }
            }else{
                record_vec.push_back(String(ssv[ii]));
                if(is_history){
                    history_record.push_back(String(ssv[ii]));
                }
            }
        } 
    }else if(s.length()>52){
        record_vec.push_back(s.substring(0,52));
        record_vec.push_back(s.substring(52));
        if(is_history){
            history_record.push_back(s.substring(0,52));
            history_record.push_back(s.substring(52));
        }
    }else{
        if(String(s)!="\n"){
            record_vec.push_back(String(s));
            if(is_history){
                history_record.push_back(String(s));
            }
        }
    }

    if(record_vec.size()>14){
        record_vec.erase(record_vec.begin(),record_vec.begin()+(record_vec.size()-15));
    }
    
    Util::printTerminal();
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