#include "rle.h"
#include <cstring>  // Para strlen
#include <cstdlib>  // Para malloc, free

char* descomprimirRLE(const char* entrada) {
    int len = strlen(entrada);

    // arreglo dinámico de chars
    // Reserva inicial (Se asume que el texto expandido no será más de 9 veces más grande que el comprimido)
    char* salida = (char*) malloc(len * 9);
    if (!salida) return nullptr; // Verificación de memoria

    int posSalida = 0;

    for (int i = 0; i < len; ) {
        // Paso 1: obtener el número (cantidad de repeticiones)
        int count = 0;
        while (entrada[i] >= '0' && entrada[i] <= '9') {
            count = count * 10 + (entrada[i] - '0');
            i++;
        }

        // Paso 2: obtener el carácter
        char c = entrada[i];
        i++;

        // Paso 3: repetir el carácter "count" veces
        for (int j = 0; j < count; j++) {
            salida[posSalida++] = c;
        }
    }

    salida[posSalida] = '\0';
    return salida; // Se debera liberar memoria con free()
}
