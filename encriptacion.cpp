#include "encryption.h"
#include "utils.h"   // contiene rotRight()

// Se asume que la encriptación original fue:
//   rotLeft(n) -> XOR(key)
// Entonces para desencriptar se hace en orden inverso:
//   XOR(key) -> rotRight(n)

unsigned char* tryDecrypt(const unsigned char* data, int len, int n_rotate, unsigned char key) {
    unsigned char* out = new unsigned char[len];
    for (int i = 0; i < len; ++i) {
        unsigned char b = data[i];
        // Paso 1: revertir XOR
        b = (unsigned char)(b ^ key);
        // Paso 2: revertir rotación (hacia la derecha)
        b = rotRight(b, n_rotate);
        out[i] = b;
    }
    return out;
}
