
#include "WioServer.h"
#include "Util.h"
#include <Seeed_Arduino_FreeRTOS.h>
#include "Display.h"

const char* ssid = "M5CL";
const char* password = "cheatplayer";
bool isAPConnect=false;

TaskHandle_t serverloop;

WiFiServer server(80);
void WioServer::startAP(){
    if(!isAPConnect){
        WiFi.softAPConfig(IPAddress(192,168,4,1),IPAddress(192,168,4,1),IPAddress(255,255,255,0));
        WiFi.softAP(ssid, password);
        server.begin();
        Serial.println("server started.");
        isAPConnect=true;
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        Display::showStatus(312,8,0x07E0);
        xTaskCreate(WioServer::serverLoop,"serverLoop",1024, NULL, tskIDLE_PRIORITY, &serverloop);
    }
    
}

void WioServer::serverLoop(void* pvParameters){
while(1){
    WiFiClient client = server.available();   
    if (client) {                             
        Serial.println("client connected.");           
        String reqLine = "";               
        while (client.connected()) {            
            if (client.available()) {           
                char c = client.read();             
//                Serial.write(c);
                if (c == '\n') {
                    HttpReq req;  
                    std::vector<String> header;          
                    if(reqLine.substring(0,3)=="GET"||reqLine.substring(0,4)=="POST"){
                        Serial.print("req.");
                        Serial.println(reqLine);
                        req=WioServer::parseReq(reqLine);
                        Serial.println(req.path);
                        Serial.println(req.query);
                        Display::showCmd(req.query);
                    }else{
                        Serial.print("head.");
                        Serial.println(reqLine);
                        header.push_back(reqLine);
                    }
                    if (reqLine.length() == 0) {
                        Serial.print("resp.");
                        WioServer::response(client,"ok");
                        break;
                    } else {    
                        reqLine = "";
                    }
                } else if (c != '\r') {  
                    reqLine += c;      
                }
            }
        }
        client.stop();
        Serial.println("client disconnected.");
    }
    delay(10);
}
}


void WioServer::response(WiFiClient client,String body){
    client.println("HTTP/1.1 200 OK\r\nContent-type:text/plain\r\n\r\n"+body+"\r\n\r\n");
}

HttpReq WioServer::parseReq(String req){
    std::vector<String> m_url_p=Util::split(req,' ');
    std::vector<String> p_q=Util::split(m_url_p[1],'?');
    HttpReq r={
        m_url_p[0],
        m_url_p[1],
        m_url_p[2],
        p_q[0],
        p_q[1]
    };
    return r;
}

String WioServer::urlDecode(String input) {
  String s = input;
  s.replace("+", " ");
  s.replace("%20", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%2B", "+");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3B", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "_");
  s.replace("%60", "`");
  s.replace("%7B", "{");
  s.replace("%7C", "|");
  s.replace("%7D", "}");
  s.replace("%0D", "\0xd");//\r
  s.replace("%0A", "");//\n
  // s.replace("%09", "\t");
  
  return s;
}
