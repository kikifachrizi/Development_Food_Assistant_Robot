T.I.F.A Codebase - Tel-U Interactive Food Assistant 🚚🍱

Gambar di atas adalah placeholder, ganti dengan gambar terkait proyek.

🛠️ Tentang Proyek
Ini adalah repository kode untuk T.I.F.A (Tel-U Interactive Food Assistant), robot autonomous pengantar makanan dengan berbagai fitur canggih seperti:

Pintu otomatis dan tray otomatis
Pergerakan diagonal dengan roda omnidirectional
Kapasitas beban hingga 10 kg
Navigasi menggunakan RPLIDAR A2 dan ROS Noetic
Antarmuka pengguna (UI) yang menarik
Sistem smart charge dengan docking charge otomatis
Kode ini mencakup semua fungsi inti, mulai dari pengontrol perangkat keras (Teensy 4.1), pemrosesan data utama (Raspberry Pi), hingga sistem navigasi berbasis ROS.

📂 Struktur Folder
plaintext
Copy code
.
├── code/                # Semua kode utama
│   ├── teensy/          # Kode untuk Teensy 4.1 (motor control, tray control, dll.)
│   ├── raspberry_pi/    # Kode untuk Raspberry Pi (UI, komunikasi, dan pengendali utama)
│   ├── ros_noetic/      # Konfigurasi dan skrip ROS
│   ├── sensors/         # Driver dan skrip untuk sensor (RPLIDAR, dll.)
├── assets/              # Gambar, diagram, dan dokumentasi visual
├── README.md            # File ini
└── LICENSE              # Lisensi proyek
🚀 Cara Menggunakan
1. Persiapan Perangkat Keras
Pastikan robot T.I.F.A dirakit sesuai desain, termasuk pemasangan Raspberry Pi, Teensy 4.1, dan RPLIDAR A2.
Sambungkan semua komponen sesuai pinout yang telah didefinisikan di dokumentasi hardware.
2. Persiapan Perangkat Lunak
Raspberry Pi:

Instal ROS Noetic di Raspberry Pi dengan mengikuti panduan resmi.
Pastikan semua dependensi untuk navigasi dan sensor sudah terpasang:
bash
Copy code
sudo apt install ros-noetic-navigation ros-noetic-rplidar-ros
Teensy 4.1:

Gunakan Arduino IDE atau PlatformIO untuk mengunggah kode ke Teensy 4.1.
Library yang diperlukan untuk Teensy:
Encoder
Servo
Teensyduino Core
Kode Sensor:

Pastikan driver RPLIDAR A2 sudah terpasang di Raspberry Pi.
Gunakan perintah berikut untuk menguji RPLIDAR:
bash
Copy code
roslaunch rplidar_ros view_rplidar.launch
3. Menjalankan Robot
Jalankan ROS Master Node di Raspberry Pi:
bash
Copy code
roscore
Jalankan node navigasi dan sensor:
bash
Copy code
roslaunch tifa_navigation bringup.launch
Nyalakan Teensy 4.1 untuk mengontrol motor dan tray.
Gunakan UI di layar interaktif untuk mengatur tujuan pengantaran.
📋 Konfigurasi
1. Pinout Teensy 4.1
Fungsi	Pin
Motor Omnidirectional	GPIO 2, 3, 4, 5
Tray Control	GPIO 6
Pintu Otomatis	GPIO 7
2. Parameter Navigasi
Parameter navigasi dapat ditemukan di ros_noetic/config/navigation.yaml.

🛠️ Fitur yang Didukung dalam Kode
Kontrol Motor dan Tray: Skrip Teensy untuk mengatur pergerakan roda omnidirectional dan tray otomatis.
Navigasi Autonomous: Konfigurasi ROS untuk menggunakan RPLIDAR A2 dan algoritma navigasi berbasis SLAM.
Antarmuka Pengguna (UI): Aplikasi Python dengan PyQt5 untuk menampilkan status robot dan menerima input tujuan.
Smart Charge: Skrip Python di Raspberry Pi untuk mendeteksi level baterai dan memicu docking otomatis.
📧 Kontak
Jika Anda memiliki pertanyaan atau masalah dengan kode ini, silakan hubungi:
📬 Email: kikifachrizi235@gmail.com
