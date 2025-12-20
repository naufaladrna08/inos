Cara memanggil assembly (inline assembly di C). Ada beberapa keyword yang perlu  dipahami terlebih dahulu, yaitu: static, inline, asm dan volatile. 

- Static di sini gunanya untuk memberitahu compiler bahwa kode assembly ini hanya akan digunakan pada fungsi ini saja, sehingga tidak perlu dioptimasi atau dipindahkan ke tempat lain. 

NOOB TIPS: Jangan terkecoh. Static dapat memiliki arti yang berbeda tergantung konteksnya. Dalam konteks ini, static berarti "terbatas pada fungsi ini saja". Mungkin di konteks lain, static bisa berarti "tetap ada selama program berjalan" dan biasa digunakan sebagai singleton.

- Inline berarti kode assembly ini akan disisipkan langsung ke dalam kode C pada saat kompilasi, bukan dipanggil sebagai fungsi terpisah. Tanpa inline, maka secara gambaran mungkin akan seperti ini

fungsi.h
```c
int fungsi(int a, int b) {
  return a + b;
}
```

 main.c
```c
#include "fungsi.h"
int main() {
  int hasil = fungsi(2, 3);
  return 0;
}
```

Maka pada saat kompilasi, kode di main.c akan memanggil fungsi di fungsi.h sebagai fungsi terpisah. Namun dengan inline, maka kode di fungsi.h akan disisipkan langsung ke dalam main.c sehingga menjadi seperti ini:

 main.c
```c
int main() {
  int hasil = 2 + 3; // kode dari fungsi.h disisipkan langsung
  return 0;
}


- Volatile digunakan untuk memberitahu compiler bahwa kode assembly ini mungkin memiliki efek samping yang tidak diketahui oleh compiler, sehingga compiler tidak boleh mengoptimasi atau mengubah urutan eksekusi kode ini. Ini penting karena kode assembly bisa berinteraksi langsung dengan hardware atau register CPU, sehingga perubahan urutan eksekusi bisa menyebabkan hasil yang tidak diinginkan.

INLINE assembly


static inline void add(int a, int b, int *result) {
  asm volatile (
    "add %[a], %[b]\n"     // Instruction
    "mov %[b], %[res]"     
    : [res] "=r" (*result) // Output operand
    : [a] "r" (a), [b] "r" (b) // Input operands
    :                     // Clobbered registers
  );
}

Penjelasan Bagian:

- Instruction: Bagian ini berisi kode assembly yang akan dijalankan. Dalam contoh ini, kita menggunakan instruksi "add" untuk menjumlahkan dua nilai dan "mov" untuk memindahkan hasilnya ke variabel result. Instruksi dapat multiline.

- Output operand: Bagian ini mendefinisikan variabel output yang akan dihasilkan oleh kode assembly. Dalam contoh ini, kita menggunakan "=r" untuk menunjukkan bahwa result adalah output yang akan diisi oleh register.

- Input operands: Bagian ini mendefinisikan variabel input yang akan digunakan oleh kode assembly. Dalam contoh ini, kita menggunakan "r" untuk menunjukkan bahwa a dan b adalah input yang akan diambil dari register.

- Clobbered registers: Bagian ini mendefinisikan register yang mungkin diubah oleh kode assembly. Dalam contoh ini, kita tidak memiliki register yang diubah, sehingga bagian ini kosong. Namun, jika kode assembly mengubah register tertentu, kita harus menyebutkannya di sini agar compiler dapat mengelola register dengan benar.