#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define USE_ARDUINO_INTERRUPTS  true


#define FIREBASE_HOST   "testesp2-21ed9.firebaseio.com"
#define FIREBASE_AUTH   "oOznNcpk9NGsthyBJf5IKjZ4uOJ7FTVErIrY4Htj"

#define WIFI_SSID       "KDU Wi-Fi"  // your wifi Username
#define WIFI_PASSWORD   "12345678"    //wifi password



FirebaseData firebaseData;


void printJsonObjectContent(FirebaseData &data);
void printFirbasedata(FirebaseData &printdata);
void printError(FirebaseData &printData);


String parentPath = "/SensorRead";
float heartRate = 0.00;
float pulse = 0.00;



void setup(){
  Serial.begin(115200);
  
  Serial.println("**********");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");


}

void loop() {
  Serial.println("***************************");
  Serial.println("Pushing....");
  // for example
  heartRate = millis(); // for example heart rate is current milli-Second
  /* Setting Value To Firebase RealTime Database*/
  //pushing the heartRate..
  if (Firebase.setDouble(firebaseData, parentPath + "/heartRate" ,heartRate)){
    printFirbasedata(firebaseData);
  }else {
    printError(firebaseData);
  }
  // pusing the Pulse value
  if (Firebase.setDouble(firebaseData, parentPath + "/pulse" ,pulse)){
    printFirbasedata(firebaseData);
  }else {
    printError(firebaseData);
  }

  
  Serial.println("Subscribing....");
  /* Getting Value From Firebase RealTime Database*/
  if (Firebase.getInt(firebaseData,parentPath + "/pulse")) {
    printFirbasedata(firebaseData);
  }else {
    printError(firebaseData);
  }
  
  delay(5000);  // Setting or Getting Value for Every 5 seconds.
}

// Methods
void printFirbasedata(FirebaseData &printData){
  Serial.println("PASSED");
  Serial.println("PATH: " + firebaseData.dataPath());
  Serial.println("TYPE: " + firebaseData.dataType());
  Serial.println("ETag: " + firebaseData.ETag());
  Serial.print("VALUE: ");
  String typeCheck = printData.dataType();
      if (typeCheck == "int")
        Serial.println(printData.intData());
      else if (typeCheck == "float")
        Serial.println(printData.floatData(), 5);
      else if (typeCheck == "double")
        printf("%.9lf\n", printData.doubleData());
      else if (typeCheck == "boolean")
        Serial.println(printData.boolData() == 1 ? "true" : "false");
      else if (typeCheck == "string")
        Serial.println(printData.stringData());
      else if (typeCheck == "json")
        printJsonObjectContent(printData);
  Serial.println("------------------------------------");
  Serial.println();
}

void printError(FirebaseData &printData) {
  Serial.println("FAILED");
  Serial.println("REASON: " + printData.errorReason());
  Serial.println("------------------------------------");
  Serial.println();
}
  
void printJsonObjectContent(FirebaseData &data){
  size_t tokenCount = data.jsonObject().parse(false).getJsonObjectIteratorCount();
  String key;
  String value;
  FirebaseJsonObject jsonParseResult;
  Serial.println();
  for (size_t i = 0; i < tokenCount; i++)
  {
    data.jsonObject().jsonObjectiterator(i,key,value);
    jsonParseResult = data.jsonObject().parseResult();
    Serial.print("KEY: ");
    Serial.print(key);
    Serial.print(", ");
    Serial.print("VALUE: ");
    Serial.print(value); 
    Serial.print(", ");
    Serial.print("TYPE: ");
    Serial.println(jsonParseResult.type);        

  }
}
