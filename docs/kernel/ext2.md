# Extend Filesystem v2
#### Kebanyakan bersumber dari [OsDev](https://wiki.osdev.org).

## Sejarah Singkat (Brief History) 
Ext2 adalah file sistem pertama untuk Linux. Ext2 digunakan mulai 
tahun 1990s sampai awal-awal tahun 2000 sebelum digantikan oleh peneru-
snya, yaitu Ext3. Mempunyai dukungan untuk UNIX hak akses, simbolik 
link, hardlink dan properti lain yang umum pada sistem operasi mirip-UNIX.
Ext2 membagi ruang diska menjadi grup.

## Blok
Ext2 membagi ruang diska menjadi block logika dari ruang yang berdekat-
an. Besar dari bloknya tidak perlu sama dengan besar dari sektor dari
disknya. Besarnya dapat ditentukan dengan membaca field pada byte 24 
dalam Super Block. 

## Blok Group
Blok-blok bersama Inode dikelompokan menjadi Blok Grup. Setiap blok grup
menyediakan beberapa blok-nya untuk spesial blok seperti:

1. Bitmap untuk block bebas atau yang sudah dialokasikan dalam grup.
2. Bitmap untuk inode yang sudah dialokasikan dalam grup.
3. Tabel struktur inode milik group.
4. Tergantuk dari revisi ext2 yang digunakan, beberapa atau semua blok
juga memiliki backup copy dari superblock dan Tabel Deskripsi Blok Grup.

## Inode
Inode adalah struktur dalam diska yang merepresentasikan berkas, direkto-
ri simbolik link, dll. Inode bukanlah isi datanya, melainkan link pada 
block data aslinya. Ini menyebabkan inode memilik besar yg terdefinis 
dengan baik yang memungkin dia ditempakan dalam array yang mudah diindeks.

## Super Block
Super Block berisi semua informasi tentang tata letak dari file sistem
dan dapat bersisi informasi lain seperti opsi fitur apa yang digunakan
untuk membuat file sitem.

#### Lokasi Super Blok
Superblok selalu terletak pada byte 1024 dari awal volume dan besarnya
pun 1024 bytes. Sebagai contoh, jika diska menggunakan 512 byte sektor,
superblok akan mulai pada awal di Logical Block Address 2 dan akan me-
nempati seluruh sektor 2 dan 3.

#### Menentukan Jumlah dari Blok Grup
Dari Superblok, ekstrak besar dari setiap blok, total inode, total blok,
total blok per grup dan inode dalam setiap grup. Dari informasi ini kita 
dapat menyimpulkan jumlah dari grup blok yang ada dengan: 

1. Membulatkan jumlah blok dibagi dengan jumlah blok per grup.
2. Membulatkan jumlah inode dibagi dengan jumlah inode per grup.
3. Kedunya dan periksa satu sama lain


