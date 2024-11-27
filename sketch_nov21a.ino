#include <OneWire.h>
#include <DallasTemperature.h>

// Inisialisasi pin data untuk sensor DS18B20
const int oneWireBusPin = 4;  // Sesuaikan dengan pin yang digunakan pada Arduino Anda

// Inisialisasi objek OneWire dan DallasTemperature
OneWire oneWire(oneWireBusPin);
DallasTemperature sensors(&oneWire);

// Inisialisasi pin untuk sensor ultrasonik
#define TRIG_PIN 9
#define ECHO_PIN 10

// Inisialisasi pin untuk relay dan LED RGB
#define RELAY_PIN 2
#define RED_LED_PIN 11
#define GREEN_LED_PIN 12

// Inisialisasi pin untuk LED suhu
#define RED_SUHU_PIN 5    // LED untuk suhu tinggi
#define GREEN_SUHU_PIN 6  // LED untuk suhu rendah

// Inisialisasi batas
const float batas_suhu_rendah = 35.0;  // Batas suhu rendah, misal 30 derajat Celsius
const float batas_ketinggian_maksimal = 90.0;  // Batas ketinggian air maksimal dalam persentase

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(9600);

  // Inisialisasi sensor suhu
  sensors.begin();

  // Inisialisasi pin untuk sensor ultrasonik
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Inisialisasi pin relay dan LED RGB sebagai output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Inisialisasi pin LED suhu
  pinMode(RED_SUHU_PIN, OUTPUT);
  pinMode(GREEN_SUHU_PIN, OUTPUT);

  // Matikan relay dan semua LED pada awalnya
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_SUHU_PIN, LOW);
  digitalWrite(GREEN_SUHU_PIN, LOW);
}

void loop() {
  // Membaca suhu
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Tampilkan suhu
  Serial.print("Suhu Celsius: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Indikator LED untuk suhu
  if (temperature < batas_suhu_rendah) {
    digitalWrite(RED_SUHU_PIN, LOW);    // LED suhu tinggi mati
    digitalWrite(GREEN_SUHU_PIN, HIGH); // LED suhu rendah menyala
  } else {
    digitalWrite(RED_SUHU_PIN, HIGH);  // LED suhu tinggi menyala
    digitalWrite(GREEN_SUHU_PIN, LOW); // LED suhu rendah mati
  }

  // Membaca ketinggian air dengan sensor ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  // Konversi jarak ke persentase ketinggian air
  float maxDistance = 100.0;  // Misalkan jarak maksimum adalah 100 cm
  float waterLevelPercentage = (1 - (distance / maxDistance)) * 100;

  // Tampilkan ketinggian air dalam persen
  Serial.print("Ketinggian Air: ");
  Serial.print(waterLevelPercentage);
  Serial.println("%");

  // Algoritma utama berdasarkan suhu dan ketinggian air
  if (temperature < batas_suhu_rendah && waterLevelPercentage < batas_ketinggian_maksimal) {
    aktifkan_pompa();
  } else {
    matikan_pompa();
  }

  // Tunggu sejenak sebelum membaca lagi
  delay(1000);
}

// Fungsi untuk mengaktifkan pompa
void aktifkan_pompa() {
  digitalWrite(RELAY_PIN, HIGH);  // Nyalakan pompa
  digitalWrite(GREEN_LED_PIN, HIGH);  // LED hijau menyala
  digitalWrite(RED_LED_PIN, LOW);  // LED merah mati
  Serial.println("Pompa ON - LED Hijau");
}

// Fungsi untuk mematikan pompa
void matikan_pompa() {
  digitalWrite(RELAY_PIN, LOW);  // Matikan pompa
  digitalWrite(GREEN_LED_PIN, LOW);  // LED hijau mati
  digitalWrite(RED_LED_PIN, HIGH);  // LED merah menyala
  Serial.println("Pompa OFF - LED Merah");
}
