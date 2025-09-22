#include "rle.h"
#include <cstring>  // Para strlen

char* descomprimirRLE(const char* entrada) {
    int len = strlen(entrada);

    // Reserva dinámica (margen de expansión)
    char* salida = new char[len * 9];
    if (!salida) return nullptr;

    int posSalida = 0;

    for (int i = 0; i < len;) {
        int count = 0;

        // Leer número (puede ser de varios dígitos)
        while (entrada[i] >= '0' && entrada[i] <= '9') {
            count = count * 10 + (entrada[i] - '0');
            i++;
        }

        // Leer carácter
        char c = entrada[i];
        i++;

        // Escribir carácter repetido
        for (int j = 0; j < count; j++) {
            salida[posSalida++] = c;
        }
    }

    salida[posSalida] = '\0';
    return salida;            // Se debe liberar con delete[]
}
