#include <stdio.h>
#include <stdlib.h>

#define JUMLAH_TELLER 7

typedef struct Nasabah {
    int noAntrian;
    struct Nasabah *next;
} Nasabah;

typedef struct {
    Nasabah *front;
    Nasabah *rear;
    int counter;
} AntrianBRI;

void initAntrian(AntrianBRI *q) {
    q->front = NULL;
    q->rear  = NULL;
    q->counter = 1;
}

int isEmpty(AntrianBRI *q) {
    return q->front == NULL;
}

/* ---------- NASABAH: ambil kartu antrian (enqueue) ---------- */
void ambilKartuAntrian(AntrianBRI *q) {
    Nasabah *baru = (Nasabah *) malloc(sizeof(Nasabah));
    if (baru == NULL) {
        printf("\n[ERROR] Memori tidak cukup!\n");
        return;
    }
    baru->noAntrian = q->counter;
    baru->next = NULL;

    if (isEmpty(q)) {
        q->front = baru;
        q->rear  = baru;
    } else {
        q->rear->next = baru;
        q->rear = baru;
    }

    printf("\n[NASABAH] Kartu antrian Anda: Nomor %d\n", baru->noAntrian);
    q->counter++;
}

void panggilNasabah(AntrianBRI *q, int idTeller) {
    if (isEmpty(q)) {
        printf("\n[TELLER %d] Antrian kosong, belum ada nasabah yang menunggu.\n", idTeller);
        return;
    }

    Nasabah *depan = q->front;
    printf("\n[TELLER %d] Memanggil nasabah nomor %d. Silakan menuju Teller %d.\n", 
           idTeller, depan->noAntrian, idTeller);

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(depan);
}

void tampilkanAntrian(AntrianBRI *q) {
    if (isEmpty(q)) {
        printf("\nAntrian saat ini kosong.\n");
        return;
    }

    printf("\nNomor antrian yang masih menunggu: ");
    Nasabah *cur = q->front;
    while (cur != NULL) {
        printf("%d ", cur->noAntrian);
        cur = cur->next;
    }
    printf("\n");
}

void bersihkanBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    AntrianBRI antrian;
    initAntrian(&antrian);

    int pilihanUtama, pilihanSub, idTeller;

    do {
        printf("\n===== SISTEM ANTRIAN TELLER BRI CIK DITIRO =====\n");
        printf("1. Menu Nasabah\n");
        printf("2. Menu Teller\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");

        // Mencegah error jika user mengetik huruf
        if (scanf("%d", &pilihanUtama) != 1) {
            printf("\n[!] Input tidak valid! Harap masukkan angka.\n");
            bersihkanBuffer();
            pilihanUtama = -1;
            continue;
        }

        switch (pilihanUtama) {
            case 1:
                do {
                    printf("\n--- MENU NASABAH ---\n");
                    printf("1. Ambil Kartu Antrian\n");
                    printf("2. Lihat Antrian Saat Ini\n");
                    printf("0. Kembali ke Menu Utama\n");
                    printf("Pilih: ");

                    if (scanf("%d", &pilihanSub) != 1) {
                        printf("\n[!] Input tidak valid!\n");
                        bersihkanBuffer();
                        pilihanSub = -1;
                        continue;
                    }

                    if (pilihanSub == 1) {
                        ambilKartuAntrian(&antrian);
                    } else if (pilihanSub == 2) {
                        tampilkanAntrian(&antrian);
                    } else if (pilihanSub != 0) {
                        printf("\n[!] Pilihan tidak valid!\n");
                    }

                } while (pilihanSub != 0);
                break;

            case 2:
                do {
                    printf("\n--- MENU TELLER (%d Teller Aktif) ---\n", JUMLAH_TELLER);
                    printf("1. Panggil Nasabah Berikutnya\n");
                    printf("2. Lihat Antrian Saat Ini\n");
                    printf("0. Kembali ke Menu Utama\n");
                    printf("Pilih: ");

                    if (scanf("%d", &pilihanSub) != 1) {
                        printf("\n[!] Input tidak valid!\n");
                        bersihkanBuffer();
                        pilihanSub = -1;
                        continue;
                    }

                    if (pilihanSub == 1) {
                        printf("Masukkan nomor teller (1-%d): ", JUMLAH_TELLER);
                        if (scanf("%d", &idTeller) != 1) {
                            printf("\n[!] Input nomor teller harus angka!\n");
                            bersihkanBuffer();
                            continue;
                        }

                        if (idTeller < 1 || idTeller > JUMLAH_TELLER) {
                            printf("[!] Nomor teller tidak valid!\n");
                        } else {
                            panggilNasabah(&antrian, idTeller);
                        }
                    } else if (pilihanSub == 2) {
                        tampilkanAntrian(&antrian);
                    } else if (pilihanSub != 0) {
                        printf("\n[!] Pilihan tidak valid!\n");
                    }

                } while (pilihanSub != 0);
                break;

            case 0:
                printf("\nProgram selesai. Terima kasih.\n");
                break;

            default:
                printf("\n[!] Pilihan tidak valid!\n");
        }
    } while (pilihanUtama != 0);

    // Dealokasi memori tersisa sebelum keluar
    while (!isEmpty(&antrian)) {
        Nasabah *temp = antrian.front;
        antrian.front = antrian.front->next;
        free(temp);
    }

    return 0;
}