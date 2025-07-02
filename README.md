# 4-DoF Gripper Arm

Proyek ini adalah implementasi lengan robotik dengan 4 Degrees of Freedom (DoF) yang dilengkapi dengan gripper (penjepit) dan sensor jarak berbasis VL53L0X. Proyek ini dirancang untuk mengenali objek menggunakan sensor IR dan VL53L0X, kemudian secara otomatis mendekati, mengambil, dan memindahkan objek ke posisi yang telah ditentukan.

## Fitur

- **4-DoF Arm:** Kontrol 4 servo pada base, joint, dan gripper.
- **Gripper Otomatis:** Membuka dan menutup gripper secara otomatis berdasarkan deteksi objek.
- **Sensor Jarak VL53L0X:** Untuk deteksi jarak benda secara presisi.
- **Sensor IR:** Untuk deteksi keberadaan objek.
- **Pengaturan Posisi Awal, Akhir, dan Default:** Posisi-posisi penting dapat diatur pada kode.
- **Pengontrolan Menggunakan PWM Servo Driver (I2C).**

## Hardware yang Dibutuhkan

- 1x Lengan robotik 4-DoF dengan servo motor
- 1x Adafruit PWM Servo Driver (PCA9685)
- 1x Sensor Jarak VL53L0X (I2C)
- 1x Sensor IR (infrared)
- Kabel jumper, breadboard, dan power supply yang sesuai

## Instalasi Library Arduino

Pastikan Anda telah menginstal library berikut di Arduino IDE:

- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
- [Adafruit VL53L0X Library](https://github.com/adafruit/Adafruit_VL53L0X)

## Wiring

- Sambungkan servo ke channel PCA9685 sesuai urutan pada kode (`BASE_SERVO`, `JOINT1_LEFT`, dll).
- Sambungkan VL53L0X ke I2C (SDA, SCL).
- Sambungkan sensor IR ke pin sesuai definisi `IR_PIN` (misal: GPIO 32 pada ESP32).

## Cara Kerja Singkat

1. **Inisialisasi:** Semua servo bergerak ke posisi default.
2. **Deteksi Objek:** Jika sensor IR mendeteksi objek, lengan akan mendekat secara bertahap berdasarkan pembacaan jarak VL53L0X.
3. **Grip:** Saat cukup dekat, gripper menutup untuk mengambil objek.
4. **Pindahkan:** Lengan bergerak ke posisi akhir, lalu melepaskan objek.
5. **Reset:** Kembali ke posisi awal, siap untuk siklus berikutnya.

## Penjelasan Pin Servo (sesuaikan channel PCA9685)

| Nama Servo           | Channel |
|----------------------|---------|
| BASE_SERVO           | 0       |
| JOINT1_LEFT          | 1       |
| JOINT1_RIGHT (mirror)| 2       |
| JOINT2               | 3       |
| GRIPPER_ROTATE       | 4       |
| GRIPPER_OPEN_CLOSE   | 5       |

## Kode Utama

Kode utama dapat ditemukan pada [`src/main.cpp`](src/main.cpp).  
Pastikan untuk menyesuaikan parameter PWM pada array `defaultPosition`, `posisiAwal`, dan `posisiAkhir` jika konstruksi fisik robot berbeda.

## Cara Upload

1. Hubungkan board (misal: ESP32/Arduino) ke komputer.
2. Buka `main.cpp` di Arduino IDE.
3. Pilih board dan port yang sesuai.
4. Upload kode ke board.

## Lisensi

Proyek ini bebas digunakan untuk keperluan edukasi dan pengembangan lebih lanjut.

---

**Kontributor:**  
[JustFTan](https://github.com/JustFTan)
