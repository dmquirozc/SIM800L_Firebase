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
#define DeepSleepIntervall  600e5 //10 seconds

#define APNENTEL "bam.entelpcs.cl"
#define USERENTEL "entelpcs"
#define PSWENTEL "entelpcs"

#define FIREBASE_URL "your_database.firebaseio.com"
#define FIREBASE_KEY "your_database_key"

String firebase_url=FIREBASE_URL;
String firebase_key=FIREBASE_KEY;
String apn = APNENTEL;
String user = USERENTEL;
String pass = PSWENTEL;
String firebase_root="bloomhomie-test";
String firebase_data_root="/bloomojo001";
String humidity_s="hum";
String pressure_s= "press";
String temperature_s="temp";
String voltage_s="voltage";
String flag_s = "flag";
String altitud_s="altitud";
String carga_s = "carga";

String caca = "caca";
String hola = "hola";

ADC_MODE(ADC_VCC);
float h, t, p, a, pin;
String temperatureString, humidityString, pressureString;
String altitudString, flagString, voltaje, carga;

Adafruit_BME280 bme; 
/* Define de los pines seriales que se usan para
  la comunicacion con el modulo SIM800*/

StaticJsonBuffer<256> jsonBuffer;
String data;

int cont=0;

SIM800L_Firebase modem(RX_SIM800,TX_SIM800,RST_SIM800,DTR_SIM800);
void setup() {
  Serial.begin(230400);
  Serial.println("init");
  modem.init(firebase_url,firebase_key);
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
  
  flagString = String(random(1, 1024));
  voltaje = String(ESP.getVcc());
  carga = String((ESP.getVcc()*1.52-3130)/1050);
  //getWeather();
  
  modem.setData(caca,millis());
  modem.setData(hola,cont);
  modem.setData(humidity_s,humidityString);
  modem.setData(pressure_s,pressureString);
  modem.setData(temperature_s,temperatureString);
  modem.setData(voltage_s,voltaje);
  modem.setData(carga_s,carga);
  modem.setData(flag_s,flagString);
  modem.setData(altitud_s,String(0));
  
  modem.printData(Serial);
  Serial.print("dataLength:");
  Serial.println(modem.dataLength());
  if(!modem.sendData(firebase_root+firebase_data_root)){
      Serial.println("Data not sent");
  }
  modem.initDeepSleep(); //deepSleepSim800L
  delay(20000);
  modem.endDeepSleep();
  cont+=10;
}

void getWeather() {
  //serialSIM800.println("Leyendo Sensor...");
  h = float(millis());//bme.readHumidity(); // "%"
  t = float(millis()-100);//bme.readTemperature(); // "C"
  p = float(millis()+500);//bme.readPressure() / 100; // "hPa"
  //a = bme.readAltitude(SEALEVELPRESSURE_HPA);

  temperatureString = String(t);
  humidityString = String(h);
  pressureString = String(p);

}