#ifndef RLE_H
#define RLE_H


// Descomprime RLE sobre texto ASCII en formato: <numero><caracter>... (p.ej. 11W1B12W)
// Retorna una nueva cadena terminada en '\0' (delete[] por el llamador).
char* descomprimirRLE(const char* entrada);


#endif // RLE_H

