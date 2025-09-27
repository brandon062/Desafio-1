#include "encriptacion.h"
#include "utilidades.h"   // contiene rotRight()

// Se asume que la encriptación original fue:
//   rotLeft(n) -> XOR(key)
// Entonces para desencriptar se hace en orden inverso:
//   XOR(key) -> rotRight(n)

unsigned char* tryDecrypt(const unsigned char* data, int len, int n_rotate, unsigned char key) {
    if (len <= 0) return 0;
    unsigned char* out = new unsigned char[len];
    for (int i = 0; i < len; ++i) {
        unsigned char b = data[i];
        b = (unsigned char)(b ^ key); // revertir XOR
        b = rotRight(b, n_rotate); // revertir rotación
        out[i] = b;
    }
    return out;
}
