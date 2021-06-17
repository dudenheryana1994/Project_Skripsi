// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include "DHT.h"
#define DHTPIN D6     // what digital pin we're connected to

#include <ESP8266WiFi.h> 

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x38, 16, 2);

String apiKey = "REW0P43R1Z4WCVTI";
const char* ssid = "Ci_Gadung";
const char* password = "1sampai9";
const char* server = "api.thingspeak.com";
int hcon = 0;

//#define FAN1 0
//#define FAN2 13
//#define FAN3 14
//#define Humadifier 2
// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  lcd.begin();
  Serial.println("TEST DHT22!");
  dht.begin();
  pinMode(0, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
}
void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: "); //menampilkan nilai kelembapan pada Serial Monitor
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); //menampilkan nilai temperature ada Serial Monitor
  Serial.print(t);
  Serial.print(" *C ");//Satuan Derajat Suhu
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: "); //menampilkan nilai Heat index pada Serial Monitor
  Serial.print(hic);
  Serial.print(" *C ");//Satuan Derajat Suhu
  Serial.print(hif);
  Serial.println(" *F");

  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("Hum : ");
  lcd.print(h);

  if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

client.stop();
  }
if (h <80 && (hcon %2) ==0){  //humid nyala

digitalWrite(16, LOW);
delay(2000);
digitalWrite(16,HIGH);
//delay(2000);
//digitalWrite(16, LOW);
hcon = hcon + 1;
  
}else if (h >86 && (hcon %2) !=0){//humid mati
  digitalWrite(16, LOW);
  delay(2000);
  digitalWrite(16,HIGH);
  hcon = hcon + 1;
}
else  {
  digitalWrite(16,HIGH);
}
if (t > 32){

digitalWrite(0, LOW);
digitalWrite(13, LOW);
digitalWrite(14, LOW);
digitalWrite(2,LOW);
}
else if (t > 31){
digitalWrite(0, LOW);
digitalWrite(13, HIGH);
digitalWrite(14, LOW);
digitalWrite(2,LOW);


}
else if (t > 30){

digitalWrite(0, LOW);
digitalWrite(13, HIGH);
digitalWrite(14, HIGH);
digitalWrite(2,LOW);
}
else{
 digitalWrite(0, HIGH);
digitalWrite(13, HIGH);
digitalWrite(14, HIGH);
}

}
