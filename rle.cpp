#include "rle.h"
#include <cstring>


char* descomprimirRLE(const char* entrada) {
    if (!entrada) return 0;
    int len = (int)std::strlen(entrada);


    // Reserva conservadora (cada cuenta podría expandir mucho). Usamos 9x como en borrador original.
    char* salida = new char[len * 9 + 1];
    if (!salida) return 0;


    int posSalida = 0;


    for (int i = 0; i < len;) {
        int count = 0;
        // leer numero (uno o mas digitos)
        while (i < len && entrada[i] >= '0' && entrada[i] <= '9') {
            count = count * 10 + (entrada[i] - '0');
            ++i;
        }
        if (i >= len) break; // formato inválido, salimos
        char c = entrada[i++];
        for (int j = 0; j < count; ++j) {
            salida[posSalida++] = c;
        }
    }


    salida[posSalida] = '\0';
    return salida;
}
