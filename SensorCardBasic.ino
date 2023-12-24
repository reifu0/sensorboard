
/*
 Basic ESP8266 MQTT example


*/

/*
Update+
try to add the digital output for LED 

*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "credentials.h"
#include "timing.h"
#include "mqtt_setting.h"

// SHT31 
#include "Adafruit_SHT31.h"


// Update these with values suitable for your network. 
// login credentials for WiFi moved to ...  headerfile>  credentials.h
// MQTT credentials for WiFi moved to ...  headerfile>  mqtt_setting.h

// SHT31  
Adafruit_SHT31 sht31 = Adafruit_SHT31();

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  Serial.print("start  ");
  Serial.print("version 0.00.03  ");
  Serial.print("version nonSleep 2021-05-05  ");
 
 // reset ping>
 pinMode(16, OUTPUT);
digitalWrite(16, HIGH);

// pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  digitalWrite(12, HIGH);
digitalWrite(13, HIGH);
digitalWrite(14, HIGH);

 digitalWrite(12, LOW);
digitalWrite(13, LOW);
digitalWrite(14, LOW);

  setup_wifi();
 // client.setServer(mqtt_server, 1883);    // without password
 client.setServer(mqtt_server, mqtt_server_port );    // with password
 client.setCallback(callback);

  //  setup the sensor 
  measSeupt(); 

  // demo test
  measurePrint(); 
}      // end of setup 

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);   // only wifi client 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
  //  digitalWrite(14, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
  //  digitalWrite(14, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
                           // without pw 
    if (client.connect(ESPCLIENTNAME, MQTT_NAME, MQTT_PASSWORD)) {     // with pw
      //  boolean connect (clientID, username, password)  
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outLn", "hello world");
     // client.publish("home/room21/temp", "46.3");
      // ... and resubscribe
      client.subscribe("inTopic");
      delay(100); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//===============================================================
//                     utility functions
//===============================================================

float measureHumid()
{
  // sht31 code below > 
  //   float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
    return h;
  } else { 
    Serial.println("Failed to read humidity");
    return -1;
  }
} // end of  measureHumid 

float measureTemp()
{
  // sht31 code below > 
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); 
    Serial.println(t);
    return t;
  } else { 
    Serial.println("Failed to read temperature");
     return 0;
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Temp. % = "); Serial.println(h);
    
  } else { 
    Serial.println("Failed to read temperature");
     
  }
} // void measureTemp 

void measSeupt()
{
   Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void measurePrint()
{
  // sht31 code below > 
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
     
  } else { 
    Serial.println("Failed to read humidity");
    
  }
} // void fkn 



//===============================================================
//                     LOOP
//===============================================================
void loop() {
 float measureIs;
 float measureIsdec;
 char measureTbuf[20];
 char measurebuf[20];
 int test; 
 int i;
  digitalWrite(13, HIGH); 
    // read from sensor
    measureIs = measureHumid();
    dtostrf(measureIs, 6, 1, measurebuf);
    measureIs = measureTemp();
    dtostrf(measureIs, 6, 1, measureTbuf);
    
    if (client.connect(ESPCLIENTNAME, MQTT_NAME, MQTT_PASSWORD)) {     // login to mqtt broker with pw
        Serial.print("H-measurebuf is>"); Serial.println(measurebuf); 
        client.publish(MQTTPATHHumid , measurebuf); 
        delay(300);
        Serial.print("T-measurebufT is>"); Serial.println(measureTbuf); 
        client.publish(MQTTPATHTemp  , measureTbuf); 
    }
    delay(2000);
    if (client.connect(ESPCLIENTNAME, MQTT_NAME, MQTT_PASSWORD)) {     // login to mqtt broker with pw
        Serial.print("T-measurebufT is>"); Serial.println(measureTbuf); 
        client.publish(MQTTPATHTemp  , measureTbuf); 
    }
     delay(200);
    digitalWrite(13, LOW);  
    client.loop();
    delay(15000);
} // end loop  gensai


void loop1() {
 //  hensuu variables
 float measureIs;
 float measureIsdec;
 char measurebuf[20];
 int test; 
 int i;

 
 // the code ?? for what?? 
  if (!client.connected()) {
    reconnect();
    delay(100);
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    // read from sensor
    measureIs = measureHumid();
   dtostrf(measureIs, 6, 2, measurebuf);
    // 
 
 for(i=0;i++;i<15)
    {msg[i] = 0x20; }

    // humidity
  test =(int)(measureIs);  
  snprintf (msg, 15, "%d ", test);      
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("home/room1/humidity", msg);  
      
digitalWrite(13, HIGH);
    // temperature
    measureIs = measureTemp();
     
   dtostrf(measureIs, 8, 2, measurebuf);   //  
    test =(int)(measureIs);   
  measureIsdec = (measureIs*10 - test*10); 
  snprintf (msg, 15, "%d", test);     // msg buffer,  
 
   String strSend = "A"; 
   String gurka;
   gurka.concat(measureIs);
   Serial.println("gurka  ");  
    Serial.println(gurka);  
   strSend.concat(msg);  
   strSend.concat(".");  
   strSend.concat(measureIsdec); 
   strSend.concat("w"); 
   Serial.print("strSend  "); 
    Serial.print(strSend);
    Serial.print("  strSendEnd\n");  
    gurka.toCharArray(msg,7); 
    Serial.print("Publish message: ");
    Serial.println(msg);
    // measure buffer
    client.publish("home/yoko1/temperature", msg);
 
    client.publish("home/yoko1/temperature", measurebuf);
    digitalWrite(13, LOW);  
  }
  
  delay(5000);
}
