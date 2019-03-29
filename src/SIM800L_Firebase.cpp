#include "SIM800L_Firebase.h"

SIM800L_Firebase::SIM800L_Firebase(uint8_t RX,uint8_t TX, uint8_t RST, uint8_t DTR):SoftwareSerial(RX,TX,false,2048)
{
	RX_PIN=RX;
	TX_PIN=TX;
	RST_PIN=RST;
	DTR_PIN=DTR;
	//_debug = Serial;
}

// void SIM800L_Firebase::setDebug(HardwareSerial& debug){
// 	_debug =debug;
// 	debug_flag = true;
// }

void SIM800L_Firebase::waitResponse(){
	uint32_t start = millis();
	while(!(this->available()>0)){
		yield();
		/*wait for data 5 seconds*/
		if(millis()-start > 5000){ 
			break;
		}
	}
}

String SIM800L_Firebase::waitUntil(String str){
	String answ="";
	waitResponse();
	while((this->available()>0)){
		answ+=this->readString();
	}

	if(answ.indexOf(str)!=-1){
		return str;
	}else{
		return answ;
	}

	return answ;
}

bool SIM800L_Firebase::testAT(){
	outBuffer = readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		delay(500);
		outBuffer = readSerial();
	}

	return true;
}

bool SIM800L_Firebase::waitForNetwork(){
	uint32_t start = millis();
	this->print(F("AT+CREG?\r\n"));
	waitResponse();
	delay(100);
	outBuffer = readSerial();
	
	if(outBuffer.indexOf(",1")!=-1){
		return true;
	}

	while(!(outBuffer.indexOf(",1")!=-1)){
		this->print(F("AT+CREG?\r\n"));
		waitResponse();
		delay(100);
		outBuffer = readSerial();
		
		if(millis()-start >= CONNECTION_TIMEOUT){
			break;
		}
	}	
	return false;
}

void SIM800L_Firebase::init(){
	pinMode(RST_PIN,OUTPUT);
	pinMode(DTR_PIN,OUTPUT);
	BAUD_RATE=DEFAULT_BAUD_RATE;
	this->begin(BAUD_RATE);
	/*reset every init*/
	digitalWrite(RST_PIN,LOW);
	delay(100);
	digitalWrite(RST_PIN,HIGH);
	digitalWrite(DTR_PIN,HIGH);
	firebase_url = "";
	firebase_root = DEFAULT_ROOT;
	outBuffer.reserve(BUFFER_RESERVE_MEMORY);
	
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
	}
	digitalWrite(DTR_PIN,LOW);
}
void SIM800L_Firebase::init(String _url){
	pinMode(RST_PIN,OUTPUT);
	pinMode(DTR_PIN,OUTPUT);
	BAUD_RATE=DEFAULT_BAUD_RATE;
	this->begin(BAUD_RATE);
	/*reset every init*/
	digitalWrite(RST_PIN,LOW);
	delay(100);
	digitalWrite(RST_PIN,HIGH);
	digitalWrite(DTR_PIN,HIGH);
	
	
	firebase_url = _url;
	firebase_root = DEFAULT_ROOT;
	outBuffer.reserve(BUFFER_RESERVE_MEMORY);
	
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
	}
	digitalWrite(DTR_PIN,LOW);
	
}

void SIM800L_Firebase::init(String _url, String _key){
	pinMode(RST_PIN,OUTPUT);
	pinMode(DTR_PIN,OUTPUT);
	BAUD_RATE=DEFAULT_BAUD_RATE;
	this->begin(BAUD_RATE);
	/*reset every init*/
	digitalWrite(RST_PIN,LOW);
	delay(100);
	digitalWrite(RST_PIN,HIGH);
	digitalWrite(DTR_PIN,HIGH);
	
	firebase_url = _url;
	firebase_key = _key;
	firebase_root = DEFAULT_ROOT;
	outBuffer.reserve(BUFFER_RESERVE_MEMORY);
	
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
	}
	digitalWrite(DTR_PIN,LOW);
	
}

void SIM800L_Firebase::init(String _url, String _key, String _root){
	pinMode(RST_PIN,OUTPUT);
	pinMode(DTR_PIN,OUTPUT);
	BAUD_RATE=DEFAULT_BAUD_RATE;
	this->begin(BAUD_RATE);
	/*reset every init*/
	digitalWrite(RST_PIN,LOW);
	delay(100);
	digitalWrite(RST_PIN,HIGH);
	digitalWrite(DTR_PIN,HIGH);
	
	firebase_url = _url;
	firebase_key = _key;
	firebase_root = _root;
	outBuffer.reserve(BUFFER_RESERVE_MEMORY);
	
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
	}
	digitalWrite(DTR_PIN,LOW);
	
}

void SIM800L_Firebase::init(String _url, String _key, uint32_t baud){
	pinMode(RST_PIN,OUTPUT);
	pinMode(DTR_PIN,OUTPUT);
	BAUD_RATE=baud;
	this->begin(BAUD_RATE);
	/*reset every init*/
	digitalWrite(RST_PIN,LOW);
	delay(100);
	digitalWrite(RST_PIN,HIGH);
	digitalWrite(DTR_PIN,HIGH);
	
	firebase_url = _url;
	firebase_key = _key;
	outBuffer.reserve(BUFFER_RESERVE_MEMORY);
	
	
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
	}
	digitalWrite(DTR_PIN,LOW);
	
}
void SIM800L_Firebase::setGPRSContex(String apn){
	this->print(F("AT+CIPCSGP= 1,\""));
	this->print(apn);
	this->print(F("\",\"\",\"\"\r\n"));
	waitResponse();
}

void SIM800L_Firebase::setGPRSContex(String apn,String user){
	this->print(F("AT+CIPCSGP= 1,\""));
	this->print(apn);
	this->print(F("\",\""));
	this->print(user);
	this->print(F("\",\"\"\r\n"));
	waitResponse();
}

void SIM800L_Firebase::setGPRSContex(String apn,String user,String pswd){
	this->print(F("AT+CIPCSGP= 1,\""));
	this->print(apn);
	this->print(F("\",\""));
	this->print(user);
	this->print(F("\",\""));
	this->print(pswd);
	this->print(F("\"\r\n"));
	waitResponse();
}


int SIM800L_Firebase::configureModem(String apn){
	setGPRSContex(apn);
	this->write("AT+CSTT?\r\n");
	if(waitUntil(apn)==apn){
		return COMPLETE;
	}else{
		this->write("AT+CSTT=\"");
		this->print(apn);
		this->write("\"\r\n");
		if(waitUntil("OK")=="OK"){
	    	this->write("AT+CIICR\r\n");
	    	if(waitUntil("OK")=="OK"){
	      		this->write("AT+CIFSR\r\n");
	      		if(!(waitUntil("0.0.0.0")=="0.0.0.0")){ /*this means that the module get an ip*/
	        		this->write("AT+CIPSSL=1\r\n");
	        		if(waitUntil("OK")=="OK"){
	        			return COMPLETE;
	        		}else{
	        			return CIPSSL_ERROR;
	      			}
	      		}else{
	      			return CIFSR_ERROR;
	      		}
	      	}else{
	      		return CIICR_ERROR;
	      	}
	    }else{
	    	return CSTT_ERROR;
	    }
	}
}

int SIM800L_Firebase::configureModem(String apn, String user){
	setGPRSContex(apn,user);
	this->write("AT+CSTT?\r\n");
	waitResponse();
	outBuffer=readSerial();
	if((outBuffer.indexOf(apn)!=-1) && (outBuffer.indexOf(user)!=-1)){
		return COMPLETE;
	}else{
		this->write("AT+CSTT=\"");
		this->print(apn);
		this->write("\",\"");
		this->print(user);
		this->write("\"\r\n");
		if(waitUntil("OK")=="OK"){
	    	this->write("AT+CIICR\r\n");
	    	if(waitUntil("OK")=="OK"){
	      		this->write("AT+CIFSR\r\n");
	      		if(!(waitUntil("0.0.0.0")=="0.0.0.0")){ /*this means that the module get an ip*/
	        		this->write("AT+CIPSSL=1\r\n");
	        		if(waitUntil("OK")=="OK"){
	        			return COMPLETE;
	        		}else{
	        			return CIPSSL_ERROR;
	      			}
	      		}else{
	      			return CIFSR_ERROR;
	      		}
	      	}else{
	      		return CIICR_ERROR;
	      	}
	    }else{
	    	return CSTT_ERROR;
	    }
	}
}

int SIM800L_Firebase::configureModem(String apn, String user, String pswd){
	setGPRSContex(apn,user,pswd);
	this->write("AT+CSTT?\r\n");
	waitResponse();
	outBuffer=readSerial();
	if((outBuffer.indexOf(apn)!=-1) && (outBuffer.indexOf(user)!=-1)&& (outBuffer.indexOf(pswd)!=-1)){
		this->write("AT+CIFSR\r\n");
		waitResponse();
		outBuffer = readSerial();
		if(outBuffer.indexOf("0.0.0.0")==-1){
			return COMPLETE;
		}else{
			goto configuration;
		}
	}else{
		
		configuration: this->write("AT+CSTT=\"");
		this->print(apn);
		this->write("\",\"");
		this->print(user);
		this->write("\",\"");
		this->print(pswd);
		this->write("\"\r\n");
		if(waitUntil("OK")=="OK"){
	    	this->write("AT+CIICR\r\n");
	    	if(waitUntil("OK")=="OK"){
	      		this->write("AT+CIFSR\r\n");
	      		if(!(waitUntil("0.0.0.0")=="0.0.0.0")){ /*this means that the module get an ip*/
	        		this->write("AT+CIPSSL=1\r\n");
	        		if(waitUntil("OK")=="OK"){
	        			return COMPLETE;
	        		}else{
	        			#ifdef DEBUG
	        				Serial.println("CIPSSL ERROR");
	        			#endif
	        			return CIPSSL_ERROR;
	      			}
	      		}else{
	      			#ifdef DEBUG
	        			Serial.println("CIFSR ERROR");
	        		#endif
	      			return CIFSR_ERROR;
	      		}
	      	}else{
	      		#ifdef DEBUG
	        		Serial.println("CIICR ERROR");
	        	#endif
	      		return CIICR_ERROR;
	      	}
	    }else{
	    	#ifdef DEBUG
	        	Serial.println("CSTT ERROR");
	        #endif
	    	return CSTT_ERROR;
	    }
	}
}

void SIM800L_Firebase::turnOffGPRS(){

}
void SIM800L_Firebase::initDeepSleep(){
	#ifdef DEBUG
		Serial.println("Init DeepSleep SIM800L");
	#endif
	
	delay(50);
	outBuffer="";
	while(outBuffer.indexOf("OK")==-1){
		this->print(F("AT+CSCLK=1\r\n"));
		delay(100);
		outBuffer=readSerial();
		#ifdef DEBUG
			Serial.println(outBuffer);
		#endif
	}
	digitalWrite(DTR_PIN,HIGH);
	#ifdef DEBUG
		_deepInit = millis();
	#endif
}

void SIM800L_Firebase::endDeepSleep(){
	#ifdef DEBUG
		Serial.println("End DeepSleep SIM800L");
	#endif
	digitalWrite(DTR_PIN,LOW);
	delay(100); //this must end deep sleep
	uint8_t count = 0;
	outBuffer=readSerial();
	uint32_t start = millis();
	while(outBuffer.indexOf("OK")==-1){
		this->print(F("AT\r\n"));
		delay(10);
		this->print(F("AT\r\n"));
		delay(10);
		this->print(F("AT+CSCLK=0\r\n"));	
		delay(10);
		outBuffer=readSerial();
		#ifdef DEBUG
			Serial.println(outBuffer);
		#endif
		if(millis()-start< 50){
			continue;
		}
		//digitalWrite(DTR_PIN,LOW);
		delay(100);
		uint32_t start = millis();
		if(count>10){
			this->resetModem();
			break;
		}
		count++;
	}
	
	#ifdef DEBUG
		Serial.print("Time elapsed: ");
		Serial.println(millis()-_deepInit);
	#endif
}

void SIM800L_Firebase::resetModem(){
	#ifdef DEBUG
		Serial.println("Factory Reset SIM800L");
	#endif
	digitalWrite(DTR_PIN,HIGH);
	delay(50);
	digitalWrite(RST_PIN,LOW);
	delay(1000);
	digitalWrite(RST_PIN,HIGH);
	delay(1000);
	this->print(F("AT&F\r\n"));
	delay(100);
	this->print(F("ATZ\r\n"));
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		delay(10);
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
		#ifdef DEBUG
			Serial.println(outBuffer);
		#endif
	}
	digitalWrite(DTR_PIN,LOW);
}

void SIM800L_Firebase::resetModem(unsigned int _time){
	#ifdef DEBUG
		Serial.println("Factory Reset SIM800L");
	#endif
	digitalWrite(DTR_PIN,HIGH);
	delay(50);
	digitalWrite(RST_PIN,LOW);
	delay(_time);
	digitalWrite(RST_PIN,HIGH);
	delay(1000);
	this->print(F("AT&F\r\n"));
	delay(100);
	this->print(F("ATZ\r\n"));
	outBuffer=readSerial();
	while (outBuffer.indexOf("OK") == -1) {
		this->print(F("AT\r\n"));
		delay(10);
		this->print(F("AT+CSCLK=0\r\n"));
		outBuffer=readSerial();
		delay(10);
		#ifdef DEBUG
			Serial.println(outBuffer);
		#endif
	}
	digitalWrite(DTR_PIN,LOW);
}


void SIM800L_Firebase::setData(String& key, String value){
	if(root.containsKey(key)){
		root.remove(key);
		root[key]=value;
	}else{
		root[key]=value;
	}
}

void SIM800L_Firebase::setData(String& key, float value){
	if(root.containsKey(key)){
		root.remove(key);
		root[key]=value;
	}else{
		root[key]=value;
	}
}

void SIM800L_Firebase::setData(String& key, int value){
	if(root.containsKey(key)){
		root.remove(key);
		root[key]=value;
	}else{
		root[key]=value;
	}
}

void SIM800L_Firebase::setData(String& key, long int value){
	if(root.containsKey(key)){
		root.remove(key);
		root[key]=value;
	}else{
		root[key]=value;
	}
}
void SIM800L_Firebase::setData(String& key, long unsigned int value){
	if(root.containsKey(key)){
		root.remove(key);
		root[key]=value;
	}else{
		root[key]=value;
	}
}

bool SIM800L_Firebase::sendData(){
	#ifdef DEBUG
		Serial.println("Sending data..");
	#endif
	
	if(this->dataLength()==0){
		return false;
	}else{
		this->write("AT+CIFSR\r\n");
	    if(!(waitUntil("0.0.0.0")=="0.0.0.0")){ /*this means that the module get an ip*/
			#ifdef DEBUG
				Serial.println("Device with IP address..");
			#endif
			this->write("AT+CIPSSL=1\r\n");
			if(!(waitUntil("OK")=="OK")){
				#ifdef DEBUG
					Serial.println("SSL Context inactive..");
				#endif
			}
			#ifdef DEBUG
				Serial.println("SSL Context active..");
			#endif
			this->print(F("AT+CIPSTART=\"TCP\",\"https://"));
			this->print(firebase_url);
			this->write("\",443\r\n");
			#ifdef DEBUG
				Serial.print("Sending data to: ");
				Serial.println(firebase_url);
			#endif
			delay(5000); //wait for connection
			if(waitUntil("CONNECT OK")=="CONNECT OK"){
				#ifdef DEBUG
					Serial.println("Connected ok..");
				#endif
				this->print(F("AT+CIPSEND\r\n"));
				delay(10);
				/* HTTP request */
				this->print(F("POST "));
				this->print(firebase_root);
				this->print(F(".json?auth="));
				this->print(firebase_key);
				this->print(F(" HTTP/1.1\r\n"));
				delay(10);
				/* HTTP request header */
				this->print(F("Host: "));
				this->print(firebase_url);
				this->print(F("\r\n"));
				this->print(F("Content-Type: applicacion/json\r\nContent-Length: "));
				String data = "";
				root.printTo(data);
				#ifdef DEBUG
					Serial.print("Data length: ");
					Serial.println(data.length());
				#endif
				this->print(data.length());
				this->print(F("\r\n"));
				this->print(F("X-HTTP-Method-Override: PATCH\r\n\r\n"));
				
				
				this->print(data);
				this->print(F("\r\n"));
				this->write((char)26); //CTRL+Z to end data entry
				delay(15000);
				outBuffer = readSerial();
				#ifdef DEBUG
					Serial.println(outBuffer);
				#endif
				if(outBuffer.indexOf("200 OK")!=-1){
					this->print(F("AT+CIPSHUT\r\n"));
					delay(100);
					waitUntil("SHUT OK");
					return true;
				}else{
					return false;
				}
				
				
			}
			return false;
		}else{
			return false;
		}
	}
}

bool SIM800L_Firebase::sendData(String _root_data){
	#ifdef DEBUG
		Serial.println("Sending data..");
	#endif
	if(this->dataLength()==0){
		return false;
	}else{
		this->write("AT+CIFSR\r\n");
	    if(!(waitUntil("0.0.0.0")=="0.0.0.0")){ /*this means that the module get an ip*/
			#ifdef DEBUG
				Serial.println("Device with IP address..");
			#endif
			this->write("AT+CIPSSL=1\r\n");
			
			if(waitUntil("OK")==String("OK")){
				#ifdef DEBUG
					Serial.println("SSL Context active..");
				#endif
			}else{
				#ifdef DEBUG
					Serial.println("SSL Context inactive..");
				#endif
			}
			
			this->print(F("AT+CIPSTART=\"TCP\",\"https://"));
			this->print(firebase_url);
			this->write("\",443\r\n");
			delay(5000); //wait for connection
			if(waitUntil("CONNECT OK")=="CONNECT OK"){
				#ifdef DEBUG
					Serial.println("Connected ok..");
				#endif
				this->print(F("AT+CIPSEND\r\n"));
				waitUntil(">");
				delay(10);
				/* HTTP request */
				this->print(F("POST "));
				this->print(firebase_root+_root_data);
				this->print(F(".json?auth="));
				this->print(firebase_key);
				this->print(F(" HTTP/1.1\r\n"));
				delay(100);
				/* HTTP request header */
				this->print(F("Host: "));
				this->print(firebase_url);
				this->print(F("\r\n"));
				this->print(F("Content-Type: applicacion/json\r\nContent-Length: "));
				delay(100);
				String data = "";
				root.printTo(data);
				#ifdef DEBUG
					Serial.print("Data length: ");
					Serial.println(data.length());
				#endif
				this->print(data.length());
				this->print(F("\r\n"));
				this->print(F("X-HTTP-Method-Override: PATCH\r\n\r\n"));
				delay(100);
				this->print(data);
				//this->print(F("\r\n"));
				this->write((char)26); //CTRL+Z to end data entry
				delay(15000);
				outBuffer = readSerial();
				#ifdef DEBUG
					Serial.println(outBuffer);
				#endif
				waitUntil("200 OK");
				return true;
			}
			return false;
		}else{
			return false;
		}
	}
}
void SIM800L_Firebase::printData(){
	String data="";
	root.printTo(data);
	this->print(data);
	this->write("\r\n");
}

void SIM800L_Firebase::printData(HardwareSerial& serial){
	root.printTo(serial);
	serial.write("\r\n");
}

uint32_t SIM800L_Firebase::dataLength(){
	return root.measureLength();
}


String SIM800L_Firebase::readSerial(){
	String str = "";

	while (this->available())
	{
		if (this->available())
		{
			str += (char) this->read();
		}
	}

	return str;
}
