#ifndef LZ78_H
#define LZ78_H


// Par (indice, caracter)
struct EntradaLZ78 {
    int indice;
    char caracter;
};


// Descomprime LZ78. 'entradas' tiene numEntradas elementos.
// Retorna cadena nueva terminada en '\0' (delete[] por el llamador).
char* descomprimirLZ78(const EntradaLZ78* entradas, int numEntradas);


#endif // LZ78_H
