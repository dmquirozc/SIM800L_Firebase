/* SIM800L_Firebase library. This library was made by: 
 *  Damian Quiroz
 *  Email: damian.quiroz.13@sansano.usm.cl
 *  
 *  The purpose of this library is work with SIM800L GSM module
 *  and Firebase using simple HTTP requests (public database only) 
 *  on NodeMCU ESP8266 using Arduino.
 *
 */


#ifndef SIM800L_FIREBASE_h

#define SIM800L_FIREBASE_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define DEBUG

#define SLEEP_MODE "AT+CSCLK=1"
/*time for wake after sleepmode on dtr pin*/
#define DTR_WAKE_TIME         50
#define CONNECTION_TIMEOUT    30000

#define CIPSEND_MAX_DATA      1460
#define DEFAULT_BAUD_RATE     115200
#define BUFFER_RESERVE_MEMORY 2048
#define DEFAULT_ROOT          "/"
/*
 * For wake up after sleep mode, it may receive an
 * external interrupt or pull down DTR pin
 */
enum conf_resutl{
  COMPLETE =0,
  CIPSSL_ERROR=1,
  CIFSR_ERROR = 2,
  CIICR_ERROR = 3,
  CSTT_ERROR = 4
};
enum result {
  SUCCESS = 0,
  ERROR_INITIALIZATION = 1,
  ERROR_BEARER_PROFILE_GPRS = 2,
  ERROR_BEARER_PROFILE_APN = 3,
  ERROR_OPEN_GPRS_CONTEXT = 4,
  ERROR_QUERY_GPRS_CONTEXT = 5,
  ERROR_CLOSE_GPRS_CONTEXT = 6,
  ERROR_HTTP_INIT = 7,
  ERROR_HTTP_CID = 8,
  ERROR_HTTP_PARA = 9,
  ERROR_HTTP_GET = 10,
  ERROR_HTTP_READ = 11,
  ERROR_HTTP_CLOSE = 12,
  ERROR_HTTP_POST = 13,
  ERROR_HTTP_DATA = 14,
  ERROR_HTTP_CONTENT = 15,
  ERROR_NORMAL_MODE = 16,
  ERROR_LOW_CONSUMPTION_MODE = 17,
  ERROR_HTTPS_ENABLE = 18,
  ERROR_HTTPS_DISABLE = 19
};

class SIM800L_Firebase: public SoftwareSerial
{
  public:
    SIM800L_Firebase(uint8_t RX,uint8_t TX, uint8_t RST, uint8_t DTR);
    void setGPRSContex(String apn);
    void setGPRSContex(String apn,String user);
    void setGPRSContex(String apn,String user,String pswd);
    /* all extra method */
    bool testAT();
    bool testAT(HardwareSerial& debug);
    void waitResponse();
    String waitUntil(String str);
    String readSerial();
    void init();
    void init(String _url);
    void init(String _url, String _key);
    void init(String _url, String _key, uint32_t _baud);
    void init(String _url, String _key, String _root);
    int configureModem(String apn);
    int configureModem(String apn,String user);
    int  configureModem(String apn,String user,String pswd);
    bool waitForNetwork();
    void turnOffGPRS();
    void initDeepSleep();
    void endDeepSleep();
    void resetModem();
    void resetModem(unsigned int _time);
    void setData(String& key, String value);
    void setData(String& key, float value);
    void setData(String& key, int value);
    void setData(String& key, long int value);
    void setData(String& key, long unsigned int value);
    bool sendData();
    bool sendData(String _root_data);
    bool sendData(String _root_data, String _root);
    void printData();
    void printData(HardwareSerial& serial);
    uint32_t dataLength();
    void end();
  private:
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    uint8_t RX_PIN;
    uint8_t TX_PIN;
    uint8_t RST_PIN;
    uint8_t DTR_PIN;
    uint32_t BAUD_RATE;
    String outBuffer; 
    String firebase_key;
    String firebase_url;
    String firebase_root;
    #ifdef DEBUG
      unsigned long int _deepInit=0;
    #endif
    //HardwareSerial& _debug;
    //bool debug_flag=false;

};

#endif