//CODE KET NOI ESP6 VƠI FIREBASE NHAN GỬI DU LIEUJ TỪ MODULE ZIGBEE
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "esp-test-fc805.firebaseio.com"
#define FIREBASE_AUTH "YF5yt2Q6ZtMQI4V3ifhBEY5IdP4AHuc9HffcCnsn"
#define WIFI_SSID "Tang 2"
#define WIFI_PASSWORD "vanphong"
/////////////
#include <SoftwareSerial.h>                                                 //thu vien uart
SoftwareSerial mySerial(4,5); // RX, TX
//////////

FirebaseData firebaseData;

FirebaseJson json;

void printResult(FirebaseData &data);
void chuoi();
String path = "/IOT";
int lend2old;
int lend1old;
String chuoi1;
int ki=0;
int save_arr=0,i=0;
float h=0,t=0;
double a=0;
char mang[9]="";
void setup()
{
  pinMode (3,OUTPUT);
  digitalWrite (3,LOW);
  Serial.begin(115200);
  mySerial.begin(38400);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
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
  
   //Firebase.setString(firebaseData, "IOT/STTWIFI", "TRUE");
  
 /////////////////////////////NHAN DU LIEU//////////////////   
  /*  if (Firebase.getString(firebaseData,  "IOT/ledD2"))
    {
      Serial.println("DATA NHAN VE");
      printResult(firebaseData);     
    }
    else
    {
      Serial.println("FAILED");   
    }
////////////DAY DU LIEU LEN////////////

    if (Firebase.setString(firebaseData, "IOT/ledD1", "ON"))
    {
      Serial.println("PUB INT OK");
    
    }
    else
    {
      Serial.println("FAILED");   
    }*/

//////////////////////////////////////

}
void printResult(FirebaseData &data)
{

    if (data.dataType() == "int")
        Serial.println(data.intData());
    else if (data.dataType() == "float")
        Serial.println(data.floatData(), 5);
    else if (data.dataType() == "double")
        printf("%.9lf\n", data.doubleData());
    else if (data.dataType() == "boolean")
        Serial.println(data.boolData() == 1 ? "true" : "false");
   // else if (data.dataType() == "string")
        //Serial.println(data.stringData()); 
       chuoi1=data.stringData();
       
}

void loop()
{  
    digitalWrite (3,HIGH);
    while (mySerial.available()>0) {
    char inchar = mySerial.read();
    Serial.print(inchar);
    if(ki==1) {if (inchar=='O') { ki=0;Firebase.setString(firebaseData, "IOT/ledD1", "ON"); } 
               if (inchar=='F') { ki=0;Firebase.setString(firebaseData, "IOT/ledD1", "OFF"); }else {ki=0;}
              }
    if(ki==2) {if (inchar=='O') {ki=0;Firebase.setString(firebaseData, "IOT/STTledD2", "ON");}
               if (inchar=='F') {ki=0;Firebase.setString(firebaseData, "IOT/STTledD2", "OFF"); }else {ki=0;}
               }
    if(save_arr==1) { if(inchar == '>')
        {a=(mang[0]-48)*1000+(mang[1]-48)*100+(mang[3]-48)*10+(mang[4]-48); h=a/100;  Firebase.setFloat(firebaseData, "IOT/DOAM",h); 
         a=(mang[5]-48)*1000+(mang[6]-48)*100+(mang[8]-48)*10+(mang[9]-48); t=a/100;  Firebase.setFloat(firebaseData, "IOT/NHIETDO",t);
         save_arr=0; i=0; } 
    else {mang[i]=inchar; i++; }}
    
    if(inchar=='1') {ki=1;}
    if(inchar=='2') {ki=2;}
    if(inchar=='<')  {save_arr=1;} 
  }
    
  if (Firebase.getString(firebaseData,  "IOT/ledD2"))
    { 
      printResult(firebaseData); 
      int lenLedd2=chuoi1.length();
      if(lenLedd2!= lend2old) {
      mySerial.print ("P2P 841E ledD2");  
      mySerial.println (lenLedd2);
      lend2old=lenLedd2;
      } 
    }
    if (Firebase.getString(firebaseData,  "IOT/ledD1"))
    { 
      printResult(firebaseData); 
      int lenLedd1=chuoi1.length();
      if(lenLedd1!= lend1old) {
      mySerial.print ("P2P 841E ledD1");  
      mySerial.println (lenLedd1);
      lend1old=lenLedd1;
      } 
    }
    
    
}
