#ifndef ENCRIPTACION_H
#define ENCRIPTACION_H


// Función para desencriptar un buffer aplicando
// primero XOR con la clave y luego rotación a la derecha.
// Parámetros:
//   data     -> puntero al arreglo encriptado
//   len      -> longitud del arreglo
//   n_rotate -> cantidad de bits a rotar (1–7)
//   key      -> clave XOR (1 byte)
//
// Retorna un nuevo puntero a un arreglo desencriptado
// (el llamador debe liberar con delete[]).
unsigned char* tryDecrypt(const unsigned char* data, int len, int n_rotate, unsigned char key);


#endif // ENCRIPTACION_H
