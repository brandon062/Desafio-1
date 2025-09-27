#include "utilidades.h"


// Rotación a la izquierda n posiciones (cíclica)
unsigned char rotLeft(unsigned char b, int n) {
    n &= 7; // asegurar 0..7
    return (unsigned char)((b << n) | (b >> (8 - n)));
}


// Rotación a la derecha n posiciones (cíclica)
unsigned char rotRight(unsigned char b, int n) {
    n &= 7; // asegurar 0..7
    return (unsigned char)((b >> n) | (b << (8 - n)));
}
