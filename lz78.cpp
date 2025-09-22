#include "lz78.h"
#include <cstring>

// Descompresión LZ78
char* descomprimirLZ78(EntradaLZ78* entradas, int numEntradas) {
    Nodo* diccionario = new Nodo[numEntradas];
    if (!diccionario) return nullptr;

    char* salida = new char[10000];
    if (!salida) return nullptr;

    int posSalida = 0;

    for (int i = 0; i < numEntradas; i++) {
        char* nuevo;

        if (entradas[i].indice == 0) {
            // Caso base: solo un carácter
            nuevo = new char[2];
            nuevo[0] = entradas[i].caracter;
            nuevo[1] = '\0';
        } else {
            // Caso general: diccionario[indice-1] + caracter
            char* base = diccionario[entradas[i].indice - 1].cadena;
            int lenBase = strlen(base);

            nuevo = new char[lenBase + 2];
            strcpy(nuevo, base);
            nuevo[lenBase] = entradas[i].caracter;
            nuevo[lenBase + 1] = '\0';
        }

        // Guardar en diccionario
        diccionario[i].cadena = nuevo;

        // Agregar a salida
        int lenNuevo = strlen(nuevo);
        for (int j = 0; j < lenNuevo; j++) {
            salida[posSalida++] = nuevo[j];
        }
    }

    salida[posSalida] = '\0';

    // Nota: el diccionario se libera con liberarDiccionario()
    return salida;
}

// Liberar memoria del diccionario
void liberarDiccionario(Nodo* diccionario, int numEntradas) {
    for (int i = 0; i < numEntradas; i++) {
        delete[] diccionario[i].cadena;
    }
    delete[] diccionario;
}
