// main.cpp - Versi Integrasi Blynk


#define BLYNK_PRINT Serial    // Mengaktifkan Serial Monitor untuk debugging Blynk
#define BLYNK_TEMPLATE_ID   "TMPL6_H-PO4-H" 
#define BLYNK_TEMPLATE_NAME "TASisnam"
#define BLYNK_AUTH_TOKEN "Lsf9EVl_2yN8q6WOg4g2wHY8fTHUJyA_"
#include <WiFi.h>             // Library untuk fungsionalitas Wi-Fi
#include <WiFiClient.h>       // Diperlukan oleh Blynk untuk koneksi Wi-Fi
#include <BlynkSimpleEsp32.h> // Library Blynk untuk ESP32
#include <DHT.h>              // Library untuk sensor DHTxx

// --- Konfigurasi Blynk ---
// Ganti dengan Auth Token yang Anda dapat dari email/aplikasi Blynk

char id[] = "TMPL6_H-PO4-H" ;
char name[] = "TASisnam";
char auth[] = "Lsf9EVl_2yN8q6WOg4g2wHY8fTHUJyA_"; 

// Konfigurasi Wi-Fi Anda. Gunakan Wokwi-GUEST untuk simulasi di Wokwi.
// Untuk ESP32 fisik, ganti dengan kredensial Wi-Fi rumah Anda.
char ssid[] = "Wokwi-GUEST";     // Ganti dengan SSID Wi-Fi Anda
char pass[] = "";                // Ganti dengan password Wi-Fi Anda

// --- Konfigurasi Sensor DHT11 ---
#define DHTPIN 4           // Pin GPIO tempat sensor DHT11 terhubung (GPIO4)
#define DHTTYPE DHT22      // Tipe sensor DHT (DHT11 atau DHT22)
DHT dht(DHTPIN, DHTTYPE);  // Objek DHT

// --- Timer untuk mengirim data secara berkala ---
BlynkTimer timer;
const long PUBLISH_INTERVAL_MS = 5000; // Interval pengiriman data (5 detik)

// Fungsi untuk membaca sensor dan mengirim ke Blynk Virtual Pins
// Pastikan Anda telah membuat widget di aplikasi Blynk dan mengaitkannya ke V5 (suhu) dan V6 (kelembaban)
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 

  if (isnan(h) || isnan(t)) {
    Serial.println(DHTPIN);
    return;
  }

  // Kirim data suhu ke Virtual Pin V5 di Blynk
  Blynk.virtualWrite(V5, t); 
  Serial.print("Suhu dikirim ke Blynk (V5): ");
  Serial.println(t);

  // Kirim data kelembaban ke Virtual Pin V6 di Blynk
  Blynk.virtualWrite(V6, h);
  Serial.print("Kelembaban dikirim ke Blynk (V6): ");
  Serial.println(h);
}

// Fungsi ini akan dipanggil ketika ada perintah dari aplikasi Blynk ke Virtual Pin V1
// Contoh: Anda bisa membuat tombol di aplikasi Blynk yang terhubung ke V1 untuk mengontrol LED
BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // Membaca nilai dari Virtual Pin V1
  Serial.print("Perintah diterima dari Blynk (V1): ");
  Serial.println(pinValue);
  // Tambahkan logika di sini untuk kontrol aktuator
  // Contoh: if (pinValue == 1) { digitalWrite(LED_BUILTIN, HIGH); }
  // else { digitalWrite(LED_BUILTIN, LOW); }
}

void setup() {
  Serial.begin(115200); // Baud rate untuk debugging
  dht.begin();         // Memulai sensor DHT

  // Menghubungkan ke server Blynk menggunakan Auth Token dan kredensial Wi-Fi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Mengatur timer untuk memanggil fungsi sendSensorData setiap 5 detik
  timer.setInterval(PUBLISH_INTERVAL_MS, sendSensorData); 
}

void loop() {
  Blynk.run();   // Ini sangat penting! Menangani semua komunikasi Blynk
  timer.run();   // Menjalankan timer untuk fungsi periodik (sendSensorData)
}