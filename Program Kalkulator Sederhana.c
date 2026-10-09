#include <stdio.h>

int main() { 
    float bil1, bil2, hasil; 
    char operator; 

    printf("--- Program Kalkulator Sederhana ---\n"); 
    printf("Masukkan bilangan 1: "); 
    scanf("%f", &bil1); 

    printf("Masukkan operator (+, -, *, /): "); 
    // Spasi sebelum %c untuk mengabaikan karakter newline
    scanf(" %c", &operator); 

    printf("Masukkan bilangan 2: "); 
    scanf("%f", &bil2); 

    switch (operator) { 
        case '+': 
            hasil = bil1 + bil2; 
            break; 
        case '-': 
            hasil = bil1 - bil2; 
            break; 
        case '*': 
            hasil = bil1 * bil2; 
            break; 
        case '/': 
            if (bil2 != 0) {
                hasil = bil1 / bil2; 
            } else { 
                printf("Error: Pembagian dengan nol!\n"); 
                return 1; 
            } 
            break; 
        default: 
            printf("Operator tidak valid!\n"); 
            return 1; 
    } 

    // Menampilkan hasil sesuai format
    printf("Hasilnya dari %.0f %c %.0f adalah %.0f\n", bil1, operator, bil2, hasil); 

    return 0; 
}