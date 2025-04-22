// BLYNK Configuration
#define BLYNK_TEMPLATE_ID "TMPL6Yz-qEzTJ"
#define BLYNK_TEMPLATE_NAME "IOT MONITORING"
#define BLYNK_AUTH_TOKEN "KytktLLtnl4-ULNkybetfPvYjUUImHt4"

#define BLYNK_PRINT Serial

// Library ESP8266 dan Blynk
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Library Sensor & LCD
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD (alamat 0x27, ukuran 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin dan Tipe Sensor DHT
#define DHTPIN 14     // GPIO14 = D5 (bisa disesuaikan)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Relay
const int relay = D6; // GPIO12

// WiFi Credentials
char ssid[] = "Karsono";
char pass[] = "Karson123";

// Variabel Global
float h;
float t;

// Timer Blynk
BlynkTimer timer;

void setup() {
  // Setup Awal
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // Matikan relay di awal

  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();

  // Tampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("IOT Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("Mulai...");

  // Koneksi ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Set Timer untuk kirim data tiap 1 detik
  timer.setInterval(1000L, kirimData);

  delay(1000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
}

void kirimData() {
  // Baca suhu dan kelembaban dari sensor DHT
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  // Validasi pembacaan sensor
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  // Debug ke Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C, Kelembaban: ");
  Serial.print(kelembaban);
  Serial.println(" %");

  // Tampilkan ke LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhu, 1); // 1 angka di belakang koma
  lcd.print((char)223); // simbol derajat
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(kelembaban, 1);
  lcd.print("% ");

  // Kontrol Relay otomatis
  if (suhu > 32) {
    digitalWrite(relay, HIGH); // Nyalakan
  } else if (suhu < 30) {
    digitalWrite(relay, LOW); // Matikan
  }

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, t); // V0 = suhu
  Blynk.virtualWrite(V1, h); // V1 = kelembaban

}
