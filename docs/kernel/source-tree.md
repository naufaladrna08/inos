# InOS Source Tree

Kernel InOS mempunyai 3 folder source: 
1. boot/ - Folder ini berisi bagaimana inos booting. InOS menggunakan bootloader
           GRUB. 
2. filesystem/ - Folder ini adalah hirarki sistem berkas di InOS. InOS menggunakan
                 sistem berkas InOS FS (mungkin sedikit mirip fat).

3. kernel/ - Folder ini berisi kode sumber kernel, yaitu drivers, init, kernel header