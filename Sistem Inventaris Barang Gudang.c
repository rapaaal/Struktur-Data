#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAKS_BARANG  100
#define MAKS_RIWAYAT 20        


typedef struct {
    char kode[20];
    char nama[50];
    int  stok;
} Barang;
typedef struct {
    int idx;                    
    int jumlah;                  
    int stokSebelum;
    int stokSesudah;
} Transaksi;
Barang    gudang[MAKS_BARANG];
int       jumlahBarang = 0;
Transaksi stack[MAKS_RIWAYAT];
int       top = -1;


static void bacaTeks(const char *prompt, char *out, size_t n) {
    char line[256];
    printf("%s", prompt);
    if (!fgets(line, sizeof line, stdin)) { printf("\n"); exit(0); }
    line[strcspn(line, "\r\n")] = '\0';
    snprintf(out, n, "%s", line);
}
static int bacaAngka(const char *prompt) {
    char buf[32], *akhir;
    for (;;) {
        bacaTeks(prompt, buf, sizeof buf);
        long v = strtol(buf, &akhir, 10);
        if (akhir != buf && *akhir == '\0') return (int) v;
        printf("[!] Input harus berupa angka.\n");
    }
}
int  stackKosong(void) { return top == -1; }
int  stackPenuh(void)  { return top == MAKS_RIWAYAT - 1; }
void push(Transaksi t) { stack[++top] = t; }
Transaksi pop(void)    { return stack[top--]; }


int cariIndeks(const char *kode) {
    for (int i = 0; i < jumlahBarang; i++)
        if (strcmp(gudang[i].kode, kode) == 0) return i;
    return -1;
}
void tambahBarang(void) {
    if (jumlahBarang >= MAKS_BARANG) {
        printf("[!] Gudang penuh (maksimal %d barang).\n", MAKS_BARANG);
        return;
    }
    Barang b;
    bacaTeks("Kode Barang  : ", b.kode, sizeof b.kode);
    if (b.kode[0] == '\0') { printf("[!] Kode tidak boleh kosong.\n"); return; }
    if (cariIndeks(b.kode) != -1) { printf("[!] Kode %s sudah terdaftar.\n", b.kode); return; }
    bacaTeks("Nama Barang  : ", b.nama, sizeof b.nama);
    if (b.nama[0] == '\0') { printf("[!] Nama tidak boleh kosong.\n"); return; }
    b.stok = bacaAngka("Jumlah Stok  : ");
    if (b.stok < 0) { printf("[!] Stok tidak boleh negatif.\n"); return; }
    gudang[jumlahBarang++] = b;
    printf("[OK] Barang %s berhasil ditambahkan.\n", b.kode);
}
void tampilkanBarang(void) {
    if (jumlahBarang == 0) { printf("[!] Gudang masih kosong.\n"); return; }
    printf("\n%-4s %-12s %-28s %6s\n", "No", "Kode", "Nama Barang", "Stok");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < jumlahBarang; i++)
        printf("%-4d %-12s %-28s %6d\n", i + 1, gudang[i].kode, gudang[i].nama, gudang[i].stok);
}
void cariBarang(void) {
    char kode[20];
    bacaTeks("Kode Barang yang dicari : ", kode, sizeof kode);
    int i = cariIndeks(kode);
    if (i == -1) { printf("[!] Barang dengan kode %s tidak ditemukan.\n", kode); return; }
    printf("Ditemukan -> Kode: %s | Nama: %s | Stok: %d\n", gudang[i].kode, gudang[i].nama, gudang[i].stok);
}
void tambahStok(void) {
    char kode[20];
    bacaTeks("Kode Barang : ", kode, sizeof kode);
    int i = cariIndeks(kode);
    if (i == -1) { printf("[!] Barang tidak ditemukan.\n"); return; }
    if (stackPenuh()) {
        printf("[!] riwayat transaksi penuh (%d). Batalkan transaksi terakhir dulu.\n", MAKS_RIWAYAT);
        return;
    }
    int jml = bacaAngka("Jumlah stok yang ditambahkan : ");
    if (jml <= 0) { printf("[!] Jumlah harus lebih dari 0.\n"); return; }


    Transaksi t = { i, jml, gudang[i].stok, gudang[i].stok + jml };
    gudang[i].stok = t.stokSesudah;
    push(t);
    printf("[OK] Stok %s: %d -> %d (transaksi dicatat ke stack).\n", gudang[i].kode, t.stokSebelum, t.stokSesudah);
}
void batalkanTransaksi(void) {
    if (stackKosong()) {
        printf("[!] STACK UNDERFLOW: belum ada transaksi yang dapat dibatalkan.\n");
        return;
    }
    Transaksi t = pop();
    gudang[t.idx].stok = t.stokSebelum;
    printf("[OK] Transaksi dibatalkan. Stok %s dikembalikan: %d -> %d.\n",
           gudang[t.idx].kode, t.stokSesudah, t.stokSebelum);
}
void tampilkanRiwayat(void) {
    if (stackKosong()) { printf("[!] Riwayat transaksi kosong.\n"); return; }
    printf("\nRiwayat transaksi (paling atas = terakhir):\n");
    printf("%-4s %-12s %8s %8s %8s\n", "No", "Kode", "Tambah", "Sebelum", "Sesudah");
    printf("---------------------------------------------\n");
    for (int i = top, no = 1; i >= 0; i--, no++)
        printf("%-4d %-12s %8d %8d %8d\n", no, gudang[stack[i].idx].kode,
               stack[i].jumlah, stack[i].stokSebelum, stack[i].stokSesudah);
}
int main(void) {
    int pilih;
    do {
        printf("\nINVENTARIS GUDANG\n");
        printf(" 1. Tambah barang\n 2. Tampilkan seluruh barang\n 3. Cari barang berdasarkan kode\n 4. Tambah stok\n 5. Batalkan transaksi terakhir\n 6. Tampilkan riwayat transaksi\n 0. Keluar\n");
        pilih = bacaAngka("Pilih menu : ");
        switch (pilih) {
            case 1: tambahBarang();       break;
            case 2: tampilkanBarang();    break;
            case 3: cariBarang();         break;
            case 4: tambahStok();         break;
            case 5: batalkanTransaksi();  break;
            case 6: tampilkanRiwayat();   break;
            case 0: printf("Program selesai.\n"); break;
            default: printf("[!] Menu tidak valid.\n");
        }
    } while (pilih != 0);
    return 0;
}
