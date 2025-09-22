#ifndef LZ78_H
#define LZ78_H

// Estructura para representar un par (índice, carácter)
struct EntradaLZ78 {
    int indice;
    char caracter;
};

// Nodo del diccionario dinámico
struct Nodo {
    char* cadena;
};

// Función para descomprimir texto con LZ78
// Entrada: arreglo de pares (índice, caracter) y número de pares
// Salida: cadena descomprimida
// IMPORTANTE: la memoria devuelta debe liberarse con free() después de usarla.
char* descomprimirLZ78(EntradaLZ78* entradas, int numEntradas);

// Liberar memoria usada por el diccionario
void liberarDiccionario(Nodo* diccionario, int numEntradas);


#endif // LZ78_H
