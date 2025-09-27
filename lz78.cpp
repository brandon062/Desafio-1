#include "lz78.h"
#include <cstring>


// construimos diccionario como arreglo de punteros char*
// Liberamos toda la memoria antes de retornar, excepto el buffer de salida que retornamos
char* descomprimirLZ78(const EntradaLZ78* entradas, int numEntradas) {
    if (!entradas || numEntradas <= 0) {
        char* vacio = new char[1];
        vacio[0] = '\0';
        return vacio;
    }


    // Diccionario: arreglo de punteros a cadenas
    char** dic = new char*[numEntradas];
    if (!dic) return 0;
    for (int i = 0; i < numEntradas; ++i) dic[i] = 0;


    // Salida acumulada (reserva amplia)
    // Máximo: cada entrada añade al menos 1 char; como cota grosera, 4x numEntradas puede ser poco,
    // preferimos 8x para evitar realocaciones. Ajustar según tamaño de entrada real.
    int cap = numEntradas * 8 + 1024;
    char* salida = new char[cap];
    if (!salida) { delete[] dic; return 0; }
    int pos = 0;


    for (int i = 0; i < numEntradas; ++i) {
        const int idx = entradas[i].indice;
        const char c = entradas[i].caracter;


        if (idx == 0) {
            // nueva frase = "" + c
            char* s = new char[2];
            s[0] = c; s[1] = '\0';
            dic[i] = s;
            // añadir a salida
            if (pos + 1 >= cap) {
                // crecimiento simple: duplicar
                int nuevoCap = cap * 2;
                char* tmp = new char[nuevoCap];
                if (!tmp) break;
                std::memcpy(tmp, salida, pos);
                delete[] salida;
                salida = tmp; cap = nuevoCap;
            }
            salida[pos++] = c;
        } else {
            // frase = dic[idx-1] + c
            const char* base = (idx > 0 && idx <= i) ? dic[idx - 1] : "";
            int lenBase = (int)std::strlen(base);
            char* s = new char[lenBase + 2];
            for (int k = 0; k < lenBase; ++k) s[k] = base[k];
            s[lenBase] = c; s[lenBase + 1] = '\0';
            dic[i] = s;


            // añadir a salida
            if (pos + lenBase + 1 >= cap) {
                int nuevoCap = cap;
                while (pos + lenBase + 1 >= nuevoCap) nuevoCap *= 2;
                char* tmp = new char[nuevoCap];
                if (!tmp) break;
                std::memcpy(tmp, salida, pos);
                delete[] salida;
                salida = tmp; cap = nuevoCap;
            }
            for (int k = 0; k < lenBase; ++k) salida[pos++] = base[k];
            salida[pos++] = c;
        }
    }


    // Cerrar cadena
    if (pos >= cap) {
        // crecer 1
        char* tmp = new char[cap + 1];
        if (tmp) { std::memcpy(tmp, salida, pos); delete[] salida; salida = tmp; cap = cap + 1; }
    }
    salida[pos] = '\0';

    // Liberar diccionario
    for (int i = 0; i < numEntradas; ++i) delete[] dic[i];
    delete[] dic;


    return salida;
}
