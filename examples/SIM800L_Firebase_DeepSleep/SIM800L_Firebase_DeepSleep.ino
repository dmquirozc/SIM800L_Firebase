#include <SIM800L_Firebase.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*This library must be compile using Nodemcu 1.0 board*/
/* Undef debug sentence to final program*/
//#undef DEBUG
#define RX_SIM800  D5 //must check gpio map
#define TX_SIM800  D6
#define RST_SIM800 D2
#define DTR_SIM800 D1

#define APNENTEL "bam.entelpcs.cl"
#define USERENTEL "entelpcs"
#define PSWENTEL "entelpcs"

String apn = APNENTEL;
String user = USERENTEL;
String pass = PSWENTEL;


#define FIREBASE_URL "your_database.firebaseio.com"
#define FIREBASE_KEY "your_database_key"

String firebase_url=FIREBASE_URL;
String firebase_key=FIREBASE_KEY;

SIM800L_Firebase modem(RX_SIM800,TX_SIM800,RST_SIM800,DTR_SIM800);
void setup() {
  Serial.begin(230400);
  Serial.println("init");
  modem.init();
  if(modem.testAT()){
    Serial.println("AT checked");
  }
  Serial.println("wait for network");
  while(!modem.waitForNetwork()){
    Serial.println(".");
    delay(100);
  } 
  int conf = modem.configureModem(apn,user,pass);  
  if(conf!=0){
      Serial.println("Error on configuration");
      Serial.print("Error Code: ");
      Serial.println(conf);
  }
}

void loop() {
  modem.initDeepSleep();
  delay(20000);
  modem.endDeepSleep();
  Serial.println("waiting for network"); 
  while(!modem.waitForNetwork());
  
  int conf = modem.configureModem(apn,user,pass);  
  if(conf!=0){
      Serial.println("Error on configuration");
      Serial.print("Error Code: ");
      Serial.println(conf);
  }

}