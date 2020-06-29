/*
 2020/5/10 by cp
*/
#include <AtWiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#ifndef WIO_SERVER_H
#define WIO_SERVER_H

typedef struct {
    String method;
    String url;
    String protcol;
    String path;
    String query;
} HttpReq;

namespace WioServer{
    void startAP();
    void serverLoop(void* pvParameters);
    void response(WiFiClient client,String body);
    HttpReq parseReq(String req);
    String urlDecode(String input);
} 


#endif
