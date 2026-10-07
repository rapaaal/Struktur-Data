#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Lagu {
    char judul[60];
    char penyanyi[40];
    int  durasi;               
    struct Lagu *next;
} Lagu;

typedef struct Riwayat {
    char judul[60];
    char penyanyi[40];
    struct Riwayat *next;
} Riwayat;

Lagu    *head = NULL;           
Riwayat *topHistory = NULL;      
int jumlahLagu = 0, jumlahHistory = 0;

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
void tambahLagu(void) {
    Lagu *baru = (Lagu *) malloc(sizeof(Lagu));
    if (!baru) { printf("[!] Memori tidak cukup.\n"); return; }
    bacaTeks("Judul     : ", baru->judul, sizeof baru->judul);
    bacaTeks("Penyanyi  : ", baru->penyanyi, sizeof baru->penyanyi);
    if (baru->judul[0] == '\0' || baru->penyanyi[0] == '\0') {
        printf("[!] Judul dan penyanyi tidak boleh kosong.\n");
        free(baru);
        return;
    }
    int menit = bacaAngka("Durasi (menit)  : ");
    int detik = bacaAngka("Durasi (detik)  : ");
    if (menit < 0 || detik < 0 || detik > 59 || (menit == 0 && detik == 0)) {
        printf("[!] Durasi tidak valid (detik 0-59, total > 0).\n");
        free(baru);
        return;
    }
    baru->durasi = menit * 60 + detik;
    baru->next = NULL;

    if (head == NULL) head = baru;
    else {
        Lagu *cur = head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = baru;
    }
    jumlahLagu++;
    printf("[OK] Lagu \"%s\" ditambahkan ke posisi %d.\n", baru->judul, jumlahLagu);
}
void tampilkanPlaylist(void) {
    if (head == NULL) { printf("[!] Playlist masih kosong.\n"); return; }
    printf("\n%-4s %-28s %-20s %s\n", "No", "Judul", "Penyanyi", "Durasi");
    printf("--------------------------------------------------------------\n");
    int no = 1, total = 0;
    for (Lagu *p = head; p != NULL; p = p->next, no++) {
        printf("%-4d %-28s %-20s %d:%02d\n", no, p->judul, p->penyanyi, p->durasi / 60, p->durasi % 60);
        total += p->durasi;
    }
    printf("Total: %d lagu, durasi %d:%02d\n", jumlahLagu, total / 60, total % 60);
}
void hapusLagu(void) {
    if (head == NULL) { printf("[!] Playlist kosong, tidak ada lagu yang dihapus.\n"); return; }
    tampilkanPlaylist();
    int no = bacaAngka("Nomor urut lagu yang dihapus : ");
    if (no < 1 || no > jumlahLagu) { printf("[!] Nomor urut tidak valid.\n"); return; }

    Lagu *hapus;
    if (no == 1) {
        hapus = head;
        head = head->next;
    } else {
        Lagu *prev = head;
        for (int i = 1; i < no - 1; i++) prev = prev->next;
        hapus = prev->next;
        prev->next = hapus->next;
    }
    printf("[OK] Lagu \"%s\" dihapus dari playlist.\n", hapus->judul);
    free(hapus);
    jumlahLagu--;
}
void pushHistory(const Lagu *l) {
    Riwayat *r = (Riwayat *) malloc(sizeof(Riwayat));
    if (!r) { printf("[!] Memori tidak cukup untuk history.\n"); return; }
    snprintf(r->judul, sizeof r->judul, "%s", l->judul);
    snprintf(r->penyanyi, sizeof r->penyanyi, "%s", l->penyanyi);
    r->next = topHistory;
    topHistory = r;
    jumlahHistory++;
}
void putarLagu(void) {
    if (head == NULL) { printf("[!] Playlist kosong, tidak ada lagu untuk diputar.\n"); return; }
    tampilkanPlaylist();
    int no = bacaAngka("Putar lagu nomor urut : ");
    if (no < 1 || no > jumlahLagu) { printf("[!] Nomor urut tidak valid.\n"); return; }
    Lagu *p = head;
    for (int i = 1; i < no; i++) p = p->next;
    printf("[PLAY] Memutar: %s - %s (%d:%02d)\n", p->judul, p->penyanyi, p->durasi / 60, p->durasi % 60);
    pushHistory(p);
}
void lihatTerakhirDiputar(void) {
    if (topHistory == NULL) { printf("[!] History kosong, belum ada lagu yang diputar.\n"); return; }
    printf("Lagu terakhir diputar : %s - %s (total history: %d)\n",
           topHistory->judul, topHistory->penyanyi, jumlahHistory);
}
void hapusHistoryTerakhir(void) {
    if (topHistory == NULL) { printf("[!] history kosong, tidak ada yang bisa dihapus.\n"); return; }
    Riwayat *r = topHistory;
    topHistory = r->next;
    printf("[OK] History terakhir dihapus: %s - %s\n", r->judul, r->penyanyi);
    free(r);
    jumlahHistory--;
}
void bebaskanMemori(void) {
    while (head)       { Lagu *t = head; head = head->next; free(t); }
    while (topHistory) { Riwayat *t = topHistory; topHistory = topHistory->next; free(t); }
}
int main(void) {
    int pilih;
    do {
        printf("\nPLAYLIST MUSIK\n");
        printf(" 1. Tambah lagu\n 2. Hapus lagu\n 3. Tampilkan playlist\n 4. Putar lagu berdasarkan urutan\n 5. Lihat lagu terakhir diputar\n 6. Hapus history terakhir\n 0. Keluar\n");
        pilih = bacaAngka("Pilih menu : ");
        switch (pilih) {
            case 1: tambahLagu();             break;
            case 2: hapusLagu();              break;
            case 3: tampilkanPlaylist();      break;
            case 4: putarLagu();              break;
            case 5: lihatTerakhirDiputar();   break;
            case 6: hapusHistoryTerakhir();   break;
            case 0: printf("Program selesai.\n"); break;
            default: printf("[!] Menu tidak valid.\n");
        }
    } while (pilih != 0);
    bebaskanMemori();
    return 0;
}