#ifndef RLE_H
#define RLE_H

// Función para descomprimir texto con RLE
// Entrada: cadena comprimida (ej: "4A3B2C1D2A")
// Salida: cadena descomprimida (ej: "AAAABBBCCDAA")
// IMPORTANTE: la memoria devuelta debe liberarse con free() después de usarla.
char* descomprimirRLE(const char* entrada);

#endif // RLE_H

