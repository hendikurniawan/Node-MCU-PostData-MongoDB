  // --- library nodeMCU ---
  // --- status = fix
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  #include <DNSServer.h>
  #include <WiFiManager.h>
  #include <ESP8266HTTPClient.h>

  // --- library DHT22 ---
  #include <dht.h>
  #define dataPin D7 // Definisi pin nodeMcu yang di gunakan sensor
  dht DHT;           // Create a DHT Object

  // --- library PZEM004Tv3.0 ---
  #include <PZEM004Tv30.h>
  PZEM004Tv30 pzem(D5, D6);// Software Serial pin 14 (D5=TX) & 12 (D6=RX)

void setup() {
  delay(1000);
  Serial.begin(115200);
}

void loop() {

 if (WiFi.status() != WL_CONNECTED) {
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(120); 
    wifiManager.setTimeout(20);
    wifiManager.autoConnect("AP Config","123");
 
}
else
{
  Serial.println("Connected");
  HTTPClient http; //Declare objeck of class HTTPClient

  String mac, sensor, suhu, kelembaban, postData, volt, amper, watt, kwh, hertz, pff;
   
  int readData = DHT.read22(dataPin); // Reads the data from the sensor
  float tm = DHT.temperature;          // Gets the values of the temperature
  float hm = DHT.humidity;             // Gets the values of the humidity

  // --- PZEM004Tv30 ---
   float v = pzem.voltage();
   float a = pzem.current();
   float w = pzem.power();
   float kw = pzem.energy();
   float h = pzem.frequency();
   float pf = pzem.pf();

   volt = String(v);
   amper = String(a);
   watt = String(w);
   kwh = String(kw);
   hertz = String(h);
   pff = String(pf);

  mac = WiFi.macAddress(); //Type of sensor
  suhu = String(tm); // String to integer conversion
  kelembaban = String(hm);

  // --- Post Data ---
  postData = "mac=" + mac + "&suhu=" + suhu + "&kelembaban=" + kelembaban + "&volt=" + volt + "&amper=" + amper + "&watt=" + watt + "&kwh=" + kwh + "&hertz=" + hertz + "&pff=" + pff;

//  http.begin("http://103.230.48.151:5000/api/insert");//api online server ubl
  http.begin("http://192.168.43.5:5600/api/insert");//api local
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  // --- print serial monitor suhu ---
  Serial.println(postData);
  delay(2000);
//  delay(300000); //Post Data at every 5 min
  http.end(); //close connection
 }
}
