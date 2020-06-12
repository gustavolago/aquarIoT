#include <Arduino.h>
#include <analogWrite.h>

int lum = 0;
int red = 0;
int gre = 0;
int blu = 0;

const int pin_red = 32;
const int pin_gre = 33;
const int pin_blu = 25;
const int pin_whi = 14;

const int pin_rel1 = 5;
const int pin_rel2 = 17;
const int pin_rel3 = 16;
const int pin_rel4 = 18;

int botaoTomada1 = 0;
int botaoTomada2 = 0;
int botaoTomada3 = 0;
int botaoTomada4 = 0;

int botaoAlimentar = 0;
int tempoAlimentar = 0;
int inicioAlimentar = 0;
unsigned long previousAlimentar = 0;

// ESP WROOM-32 - https://circuits4you.com/wp-content/uploads/2018/12/ESP32-Pinout.jpg
//
// Display OLED I2C 128x64
// SCK - D22
// SCA - D21
//
// DS18B20
// Data - D4
//
// Luminária LED RGB
// R - 33
// G - 32
// B - 25
// Luminária LED Branco - 14
//
// Relê
// 1 -
// 2 -
// 3 -
// 4 -
//
//
//
//
// Wifi ////////////////////////////////////////////////////////////////////////////
#include <WiFi.h>
//#include <WebServer.h>
const char* ssid = "ssid wifi";
const char* pass = "senha";

// WIFI *****************************************************************************

// NTPClient ////////////////////////////////////////////////////////////////////////
#include <NTPClient.h>
const char* servidorNTP = "a.st1.ntp.br";     // Servidor NTP para pesquisar a hora
const int fusoHorario = -10800;               // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000;        // Taxa de atualização do servidor NTP em milisegundos
WiFiUDP ntpUDP;                               // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);
// NTP *****************************************************************************

// Blynk ///////////////////////////////////////////////////////////////////////////
#include <BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial
char auth[] = "GbjOWeO6K9gFxsdrAGaRzbbq48UU7k7e";
// BLYNK *****************************************************************************

// Sensor de temperatura DS18B20 ///////////////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 4;                     // GPIO where the DS18B20 is connected to
OneWire oneWire(oneWireBus);                  // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);          // Pass our oneWire reference to Dallas Temperature sensor
// DS18B20 *****************************************************************************

// Display OLED 128x64 I2C /////////////////////////////////////////////////////////
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/TomThumb.h>
#include <Adafruit_SSD1306.h>
#include "lcd_var.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Display OLED *************************************************************************

BLYNK_WRITE(V3) // Botão Luminária
{
  lum = param.asInt();
  Serial.print("Luminária: ");
  Serial.println(lum);
  Luminaria();
}

BLYNK_WRITE(V4) // Botão alimentar
{
  botaoAlimentar = param.asInt(); // assigning incoming value from pin V1 to a variable
  Blynk.syncVirtual(V9);
  Serial.print("V4: ");
  Serial.println(botaoAlimentar);
  previousAlimentar = millis();
}

BLYNK_WRITE(V9) // Slider tempo Alimentar
{
  tempoAlimentar = param.asInt() * 1000 * 10; //*60 tempo de alimentção em milisegundos
}

BLYNK_WRITE(V11) //RGB zeRGBa
{
  red = param[0].asInt();
  gre = param[1].asInt();
  blu = param[2].asInt();
  Blynk.syncVirtual(V3); // Luminária e RGB
  Luminaria();
}

BLYNK_WRITE(V12) // Botão Bomba
{
  botaoTomada1 = param.asInt();
  if (botaoTomada1) {
    display.drawBitmap(16, 45, tomada_ligada, 13, 11, 1);
  }
  else   {
    display.drawBitmap(16, 45, tomada_desligada, 13, 11, 1);
  }
}

BLYNK_WRITE(V13) // Botão Bomba
{
  botaoTomada2 = param.asInt();
  if (botaoTomada2) {
    display.drawBitmap(16, 45, tomada_ligada, 13, 11, 1);
  }
  else   {
    display.drawBitmap(16, 45, tomada_desligada, 13, 11, 1);
  }
}

BLYNK_WRITE(V14) // Botão Bomba
{
  botaoTomada3 = param.asInt();
  if (botaoTomada3) {
    display.drawBitmap(30, 45, tomada_ligada, 13, 11, 1);
  }
  else {
    display.drawBitmap(30, 45, tomada_desligada, 13, 11, 1);
  }
}

BLYNK_WRITE(V15) // Botão Bomba
{
  botaoTomada4 = param.asInt();
  if (botaoTomada4) {
    display.drawBitmap(44, 45, tomada_ligada, 13, 11, 1);
  }
  else
  {
    display.drawBitmap(44, 45, tomada_desligada, 13, 11, 1);
  }
}

int Luminaria() {

  if (lum) {
    if (red == 255 && blu == 255 && gre == 255) {
      analogWrite(pin_red, 0);
      analogWrite(pin_gre, 0);
      analogWrite(pin_blu, 0);
      digitalWrite(pin_whi, HIGH);
    }
    else  {
      analogWrite(pin_red, red);
      analogWrite(pin_gre, gre);
      analogWrite(pin_blu, blu);
      digitalWrite(pin_whi, LOW);
    }
  }
  else {
    analogWrite(pin_red, 0);
    analogWrite(pin_gre, 0);
    analogWrite(pin_blu, 0);
    digitalWrite(pin_whi, LOW);
  }
}

int Tomadas()
{
  if (botaoTomada1) {
    display.drawBitmap(2, 45, tomada_ligada, 13, 11, 1);
  }
  else   {
    display.drawBitmap(2, 45, tomada_desligada, 13, 11, 1);
  }
  if (botaoTomada2) {
    display.drawBitmap(16, 45, tomada_ligada, 13, 11, 1);
  }
  else   {
    display.drawBitmap(16, 45, tomada_desligada, 13, 11, 1);
  }
  if (botaoTomada3) {
    display.drawBitmap(30, 45, tomada_ligada, 13, 11, 1);
  }
  else {
    display.drawBitmap(30, 45, tomada_desligada, 13, 11, 1);
  }
  if (botaoTomada4) {
    display.drawBitmap(44, 45, tomada_ligada, 13, 11, 1);
  }
  else
  {
    display.drawBitmap(44, 45, tomada_desligada, 13, 11, 1);
  }
}
void setup() {
  pinMode(pin_whi, OUTPUT);
  pinMode(pin_rel1, OUTPUT);
  pinMode(pin_rel2, OUTPUT);
  pinMode(pin_rel3, OUTPUT);
  pinMode(pin_rel4, OUTPUT);
  //pinMode(32, OUTPUT);
  //pinMode(33, OUTPUT);

  //pinMode(32, OUTPUT);
  //pinMode(34, OUTPUT);
  //pinMode(35, OUTPUT);
  //digitalWrite(32, LOW);
  //digitalWrite(34, LOW);
  //digitalWrite(35, LOW);
  // Incializando display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  Serial.println("Display incializado");
  display.setFont(&TomThumb);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 5);
  display.println("Inicializando...");
  display.display();
  delay(1000);
  //display.setCursor(0, 25);
  display.println("Display...");
  display.display();
  delay(1000);

  // Inicializando Serial
  display.println("Serial...");
  display.display();
  Serial.begin(115200);
  //display.setCursor(0, 38);
  delay(1000);

  // Inicializando Wifi
  Serial.println("Connecting to ");
  Serial.println(ssid);
  //display.setCursor(0, 51);
  display.println("Wifi...");
  display.display();
  //WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, pass);

  // Verificando conectividade com a rede wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connectado..!");
  Serial.print("IP: ");  Serial.println(WiFi.localIP());
  display.println(WiFi.localIP());
  display.display();
  delay(1000);
  Serial.println("Connected to the WiFi network");

  // Inicializando NTP
  display.println("NTP...");
  display.display();
  delay(2000);

  timeClient.begin();
  delay(2000);
  timeClient.update();

  // Blynk
  //Blynk.begin(auth);
  Blynk.syncVirtual(V1, V12, V13, V14, V15); // Luminária e RGB, bomba

  // Inicializando DS18B20
  sensors.begin();
  display.setFont(&FreeSans9pt7b);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

unsigned long previousMillis = 0;

const long interval = 800;
int alimentando = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) { //loop sem delay - Atualiza dados e display
    previousMillis = currentMillis;
    // sensor de temporatura
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    String horario = timeClient.getFormattedTime();

    // Atualiza dados no Blynk
    Blynk.virtualWrite(V6, temperatureC); // Gauge temperatura
    Blynk.virtualWrite(V8, random(0, 100)); // Volume do Sump

    // Atualiza display
    display.clearDisplay();
    display.drawBitmap(0, 0, image_data_Image, 128, 64, 1);
    display.setCursor(18, 15);
    display.print(horario.substring(0, 5));
    display.setCursor(13, 35);
    display.print(String(temperatureC, 1));
    display.print("oC");

    Tomadas();
    display.display();

  }
  if (botaoAlimentar) {
    if (alimentando) {
      Serial.println(millis() - previousAlimentar);
      if ( millis() - previousAlimentar >= tempoAlimentar) { //loop sem delay - desliga a bomba para alimentação
        Serial.println("fim Alimentando...");
        alimentando = 0;
        botaoAlimentar = 0;
        Blynk.virtualWrite(V4, 0);
        Blynk.virtualWrite(V12, 1);
        Blynk.syncVirtual(V4, V12);
        delay(500);
      }
    }
    else {
      Serial.print("Alimentando...");
      Blynk.virtualWrite(V12, 0);
      Blynk.syncVirtual(V4, V12);
      alimentando = 1;
      previousAlimentar = millis();
    }
  }
  //botaoAlimentar
  Blynk.run();
}
