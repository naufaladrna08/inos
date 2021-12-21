# Peripheral Component Interconnect
## PCI bus 

PCI bus didefinisikan untuk membangun kinerja tinggi 
berbiaya rendah yang akan bertahan melalui beberapa
generasi dari produk. Dengan mengkombinasikan jalur
upgrade transparan dari 132 MB/s (32-bit pada 33 Mhz)
dan kedua lingkungan pensinyalan 5 dan 3.3 volt, PCI
bus memenuhi kebutuhan sistem desktop kelas bawah dan 
server kelas atas.

## Ruang Konfigurasi

Spesifikasi PCI menyediakan inisialisasi dan konfigurasi 
yang digerakkan oleh perangkat lunak sepenuhnya dari 
setiap perangkat (atau target) pada Bus PCI melalui Ruang
Alamat Konfigurasi yang terpisah. Semua perangkat PCI, 
kecuali jembatan bus host, diharuskan menyediakan 256 
byte register konfigurasi untuk tujuan ini.

Siklus baca/tulis konfigurasi digunakan untuk mengakses 
Ruang Konfigurasi setiap perangkat target. Sebuah target 
dipilih selama akses konfigurasi ketika sinyal IDSEL-nya 
ditetapkan. IDSEL bertindak sebagai sinyal klasik "pilih 
chip". Selama fase alamat dari siklus konfigurasi, prosesor 
dapat menangani salah satu dari 64 register 32-bit dalam 
ruang konfigurasi dengan menempatkan nomor register yang 
diperlukan pada baris alamat 2 hingga 7 (AD[7..2]) dan 
baris pengaktifan byte.

Perangkat PCI pada dasarnya adalah little-endian, yang 
berarti semua bidang beberapa byte memiliki nilai paling 
tidak signifikan di alamat yang lebih rendah. Ini membutuhkan
prosesor big-endian, seperti Power PC, untuk melakukan 
pertukaran byte yang tepat dari data yang dibaca dari atau 
ditulis ke perangkat PCI, termasuk akses apa pun ke Ruang 
Alamat Konfigurasi.

Sistem harus menyediakan mekanisme yang memungkinkan akses 
ke ruang konfigurasi PCI, karena kebanyakan CPU tidak 
memiliki mekanisme seperti itu. Tugas ini biasanya dilakukan 
oleh Host to PCI Bridge (Host Bridge). Dua mekanisme berbeda 
ditentukan untuk memungkinkan perangkat lunak menghasilkan 
akses konfigurasi yang diperlukan. Mekanisme konfigurasi #1 
adalah metode yang disukai, sedangkan mekanisme #2 disediakan 
untuk kompatibilitas ke belakang.

## Mekanisme Akses Ruang Konfigurasi #1

Dua lokasi I/O 32-bit digunakan, lokasi pertama (0xCF8) 
diberi nama CONFIG_ADDRESS, dan yang kedua (0xCFC) disebut 
CONFIG_DATA. CONFIG_ADDRESS menentukan alamat konfigurasi 
yang diperlukan untuk mengakses, sedangkan akses ke 
CONFIG_DATA sebenarnya akan menghasilkan akses konfigurasi 
dan akan mentransfer data ke atau dari register CONFIG_DATA.

CONFIG_ADDRESS adalah register 32-bit dengan format yang 
ditunjukkan pada gambar berikut. Bit 31 adalah tanda 
pengaktifan untuk menentukan kapan akses ke CONFIG_DATA 
harus diterjemahkan ke siklus konfigurasi. Bit 23 hingga 
16 memungkinkan perangkat lunak konfigurasi memilih bus 
PCI tertentu dalam sistem. Bit 15 hingga 11 pilih perangkat 
tertentu pada Bus PCI. Bit 10 hingga 8 memilih fungsi 
tertentu di perangkat (jika perangkat mendukung banyak fungsi).

Byte paling signifikan memilih offset ke dalam ruang 
konfigurasi 256-byte yang tersedia melalui metode ini. 
Karena semua pembacaan dan penulisan harus 32-bit dan 
selaras untuk bekerja pada semua implementasi, dua bit 
terendah CONFIG_ADDRESS harus selalu nol, dengan enam 
bit sisanya memungkinkan Anda memilih masing-masing 
dari 64 kata 32-bit. Jika Anda tidak membutuhkan semua 
32 bit, Anda harus melakukan akses tidak selaras dalam 
perangkat lunak dengan menyelaraskan alamat, diikuti 
dengan menutupi dan menggeser jawabannya.

|===========|==========|===========|=================|==============|================|  
| Bit Nyala | Tersedia | Nomor Bus | Nomor Perangkat | Nomor Fungsi | Letak Register | <= Isi  
|===========|==========|===========|=================|==============|================|  
|     31    |  30-24   |   23-16   |      15-11      |     10-8     |       7-0      | <= Bit  
|===========|==========|===========|=================|==============|================|  

Register Offset harus mengarah ke DWORD yang berurutan, 
yaitu. bit 1: 0 selalu 0b00 (mereka masih bagian dari 
Register Offset).

Ketika akses konfigurasi mencoba untuk memilih perangkat 
yang tidak ada, jembatan host akan menyelesaikan akses 
tanpa kesalahan, menghapus semua data saat menulis dan 
mengembalikan semua data saat dibaca. Segmen kode berikut 
menggambarkan pembacaan perangkat yang tidak ada.

#### Sumber: wiki.osdev.org, Diterjemahkan oleh Naufal Adriansyah
