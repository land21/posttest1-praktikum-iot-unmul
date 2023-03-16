// Library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// define LED dan Buzzer
#define LED 8
#define BUZZER 4

// SSID dan Password AP/Hotspot
const char* ap_ssid = "Kelompok 2 IOT A";     
const char* ap_password = "egamithaalan"; 


ESP8266WebServer server(80);

// Define LED
const int LED_PIN = D8;
const int BUTTON_RESET = D7;
const int BUTTON_ADD = D5; 
const int BUTTON_MIN = D6;

// Define Buzzer
const int BUZZER_PIN = D4;
const int BUTTONB_RESET = D3;
const int BUTTONB_ADD = D2; 
const int BUTTONB_MIN = D1;

// Define variables
int enable = 0;
int enableBuzzer = 0;
int add;
int minus;
int addBuzzer;
int minusBuzzer;
int kecerahan = 10;
int kecerahan2 = 10;
int freq = 0;
int freq2 = 100;
int maxKecerahan = 250;
int minKecerahan = 0;
int kelipatan = 5;
int maxBuzzer = 2500;
int minBuzzer = 100;

//Tampilan Awal WEB
void handleRoot() {
  String html = "<h1>KELOMPOK 2 IOT A</h1>";
  html += "<p>Kecerahan LED : " + String(kecerahan) + "</p>";
  html += "<p>Frekuensi Buzzer : " + String(freq) + "</p>";
  html += "<form method='post' action='/cerah'>";
  html += "<label for='pos'>Kecerahan (0-250):</label>";
  html += "<input type='number' id='bright' name='bright' min='0' max='250' value='" + String(kecerahan) + "'>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";
  html += "<form method='post' action='/frequen'>";
  html += "<label for='pos'>Frekuensi (100-2500):</label>";
  html += "<input type='number' id='frek' name='frek' min='100' max='2500' value='" + String(freq) + "'>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";
  html += "LED ON OFF : ";
  html += "<form  method='post' action='/LEDON' >";
  html += "<button type='submit' value='Submit'>ON</button>";
  html += "</form>";
  html += "<form method='post' action='/LEDOFF' >";
  html += "<button type='submit' value='Submit'>OFF</button>";
  html += "</form>";
  html += "BUZZER ON OFF : ";
  html += "<form  method='post' action='/BUZZERON' >";
  html += "<button type='submit' value='Submit'>ON</button>";
  html += "</form>";
  html += "<form method='post' action='/BUZZEROFF' >";
  html += "<button type='submit' value='Submit'>OFF</button>";
  html += "</form>";
  server.send(200, "text/html", html);
}

// Fungsi untuk menyalakan LED
void LED_ON(){
  if (server.method() == HTTP_POST){
    enable = 1;
    kecerahan = kecerahan2;
    Serial.println(enable);
    server.send(200, "text/plain", "OK");
    return;
  }
}

//Fungsi Untuk mematikan LED
void LED_OFF(){
  if (server.method() == HTTP_POST){
    enable = 0;
    kecerahan2 = kecerahan;
    kecerahan = 0;
    Serial.println(enable);
    server.send(200, "text/plain", "OK");
    return;
    }
  }


//Fungsi Untuk menyalakan Buzzer
void BUZZER_ON(){
  if (server.method() == HTTP_POST){
    enableBuzzer = 1;
    freq = freq2;
    Serial.println(enableBuzzer);
    server.send(200, "text/plain", "OK");
    return;
    }
  }

//Fungsi Untuk mematikan Buzzer
void BUZZER_OFF(){
  if (server.method() == HTTP_POST){
    enableBuzzer = 0;
    freq2 = freq;
    freq = 0;
    Serial.println(enableBuzzer);
    server.send(200, "text/plain", "OK");
    return;
    }
  }

//Fungsi untuk mengontrol Nilai dari LED dan Buzzer
void kontrol() {
  if (server.method() == HTTP_POST) {
    String kecerahan3 = server.arg("bright");
    String freq3 = server.arg("frek");
    if (kecerahan3 != "") {
      int bright = kecerahan3.toInt();
      if (bright >= 0 && bright <= 250) {
        kecerahan = bright;
        analogWrite(LED_PIN, kecerahan);
        server.send(200, "text/plain", "OK");
        return;
      }
    }else if (freq3 != "") {
      int frek = freq3.toInt();
      if (frek >= 100 && frek <= 2500) {
        freq = frek;
        analogWrite(BUZZER_PIN, freq);
        server.send(200, "text/plain", "OK");
        return;
      }
    }
    server.send(400, "text/plain", "Bad Request");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}


void setup() {
  Serial.begin(9600);
  
   // Start access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access point started");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start web server
  server.on("/", handleRoot);
  server.on("/cerah", kontrol);
  server.on("/frequen", kontrol);
  server.on("/LEDON", LED_ON);
  server.on("/LEDOFF", LED_OFF);
  server.on("/BUZZERON", BUZZER_ON);
  server.on("/BUZZEROFF", BUZZER_OFF);
  server.begin();
  
  // Inisialisasi pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_RESET, INPUT_PULLUP);
  pinMode(BUTTON_ADD, INPUT_PULLUP);
  pinMode(BUTTON_MIN, INPUT_PULLUP);
  pinMode(BUTTONB_RESET, INPUT_PULLUP);
  pinMode(BUTTONB_ADD, INPUT_PULLUP);
  pinMode(BUTTONB_MIN, INPUT_PULLUP);
  
  server.begin();
  Serial.println("Server dijalankan");
}

void loop() {
  server.handleClient();
  
  // Check tombol
  add = digitalRead(BUTTON_ADD);
  minus = digitalRead(BUTTON_MIN);
  addBuzzer = digitalRead(BUTTONB_ADD);
  minusBuzzer = digitalRead(BUTTONB_MIN);

  //LED RESET
  if ( digitalRead(BUTTON_RESET) == LOW && enable == 0 ) {
    enable = 1;
    kecerahan = kecerahan2;
    Serial.println(enable);
  } else if ( digitalRead(BUTTON_RESET) == LOW && enable == 1 ) {
    enable = 0;
    kecerahan2 = kecerahan;
    kecerahan = 0;
    Serial.println(enable);
  }

  //Buzzer RESET
  if ( digitalRead(BUTTONB_RESET) == LOW && enableBuzzer == 0 ) {
    enableBuzzer = 1;
    freq = freq2;
    Serial.println(enableBuzzer);
  } else if ( digitalRead(BUTTONB_RESET) == LOW && enableBuzzer == 1 ) {
    enableBuzzer = 0;
    freq2 = freq;
    freq = 0;
    Serial.println(enableBuzzer);
  }
  
  // LED Kontrol
  if ( add == LOW && kecerahan < maxKecerahan && enable == 1 ) {
    kecerahan += kelipatan;
    Serial.println(kecerahan);
  }else if ( add == LOW && kecerahan >= maxKecerahan && enable == 1 ) {
    kecerahan = 250;
    Serial.println(kecerahan);
  }else if ( minus == LOW && kecerahan > minKecerahan && enable == 1 ) {
    kecerahan -= kelipatan;
    Serial.println(kecerahan);
  }else if ( minus == LOW && kecerahan <= minKecerahan && enable == 1 ) {
    kecerahan = 0;
    Serial.println(kecerahan);
  }

  //BUZZER Kontrol
  if ( addBuzzer == LOW && freq < maxBuzzer && enableBuzzer == 1 ) {
    freq += kelipatan;
    Serial.println(freq);
  }else if ( addBuzzer == LOW && freq >= maxBuzzer && enableBuzzer == 1 ) {
    freq = 2500;
    Serial.println(freq);
  }else if ( minusBuzzer == LOW && freq > minBuzzer && enableBuzzer == 1 ) {
    freq -= kelipatan;
    Serial.println(freq);
  }else if ( minusBuzzer == LOW && freq <= minBuzzer && enableBuzzer == 1 ) {
    freq = 100;
    Serial.println(freq);
  }

  delay(100);
  analogWrite(LED_PIN, kecerahan);
  analogWrite(BUZZER_PIN, freq);
}
