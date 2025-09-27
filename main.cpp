#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "utilidades.h"
#include "encriptacion.h"
#include "rle.h"
#include "lz78.h"

// Lee archivo binario completo a memoria. Devuelve buffer y escribe len. (delete[] por llamador)
unsigned char* leerArchivo(const char* nombre, int* len) {
    if (!nombre || !len) return 0;
    *len = 0;
    FILE* f = std::fopen(nombre, "rb");
    if (!f) {
        std::printf("Error al abrir archivo: %s\n", nombre);
        return 0;
    }
    std::fseek(f, 0, SEEK_END);
    long L = std::ftell(f);
    if (L < 0) { std::fclose(f); return 0; }
    std::fseek(f, 0, SEEK_SET);
    unsigned char* buf = new unsigned char[L];
    if (!buf) { std::fclose(f); return 0; }
    size_t leidos = std::fread(buf, 1, (size_t)L, f);
    std::fclose(f);
    *len = (int)leidos;
    return buf;
}

// Lee archivo de texto (pista) y retorna char* terminado en '\0'
char* leerTexto(const char* nombre) {
    int len = 0;
    unsigned char* raw = leerArchivo(nombre, &len);
    if (!raw) return 0;
    char* s = new char[len + 1];
    for (int i = 0; i < len; ++i) s[i] = (char)raw[i];
    s[len] = '\0';
    delete[] raw;
    return s;
}

// Busca si needle aparece en haystack. Devuelve 1 si aparece, 0 si no.
int contiene(const char* hay, const char* needle) {
    if (!hay || !needle) return 0;
    int n = (int)std::strlen(needle);
    if (n == 0) return 1;
    int m = (int)std::strlen(hay);
    if (n > m) return 0;
    for (int i = 0; i <= m - n; ++i) {
        int k = 0;
        while (k < n && hay[i + k] == needle[k]) ++k;
        if (k == n) return 1;
    }
    return 0;
}

// ================= RLE binario (seguro) =================
// Descomprime RLE binario: tripletas (hi, lo, char) -> "aaaa..."
// Con validaciones para evitar explosión de memoria al probar combinaciones inválidas.
char* descomprimirRLE_bin(const unsigned char* buf, int len) {
    if (!buf || len < 3 || (len % 3) != 0) { char* z = new char[1]; z[0]='\0'; return z; }

    const int MAX_RUN    = 10000;        // límite por tramo para evitar abusos
    const int MAX_OUTPUT = 2 * 1024 * 1024; // 2 MB de salida máx.

    // Chequeo de plausibilidad sobre una muestra
    int parts = len / 3;
    int step = parts / 16; if (step <= 0) step = 1;
    int checked = 0;
    for (int p = 0; p < parts && checked < 16; p += step, ++checked) {
        int i = p * 3;
        int count = ((int)buf[i] << 8) | (int)buf[i+1];
        unsigned char c = buf[i+2];
        if (count <= 0 || count > MAX_RUN) { char* z = new char[1]; z[0]='\0'; return z; }
        // El reto usa A-Z, a-z y 0-9; si el símbolo no es texto, probablemente no es RLE válido
        if (!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9'))) { char* z = new char[1]; z[0]='\0'; return z; }
    }

    int cap = 1024;
    char* out = new char[cap];
    if (!out) { char* z = new char[1]; z[0]='\0'; return z; }
    int pos = 0;

    for (int i = 0; i + 2 < len; i += 3) {
        int count = ((int)buf[i] << 8) | (int)buf[i + 1];
        char c = (char)buf[i + 2];

        if (count <= 0 || count > MAX_RUN) { delete[] out; char* z=new char[1]; z[0]='\0'; return z; }
        if (pos + count > MAX_OUTPUT)       { delete[] out; char* z=new char[1]; z[0]='\0'; return z; }

        if (pos + count >= cap) {
            int nuevo = cap;
            while (pos + count >= nuevo) {
                nuevo *= 2;
                if (nuevo > MAX_OUTPUT) nuevo = MAX_OUTPUT;
                if (nuevo <= pos + count) { delete[] out; char* z=new char[1]; z[0]='\0'; return z; }
            }
            char* tmp = new char[nuevo];
            if (!tmp) { delete[] out; char* z=new char[1]; z[0]='\0'; return z; }
            std::memcpy(tmp, out, pos);
            delete[] out; out = tmp; cap = nuevo;
        }
        for (int k = 0; k < count; ++k) out[pos++] = c;
    }
    if (pos >= cap) { char* tmp=new char[cap+1]; if (tmp){ std::memcpy(tmp,out,pos); delete[] out; out=tmp; cap=cap+1; } }
    out[pos] = '\0';
    return out;
}

// ================= LZ78 binario =================
// Enmarcado 3 bytes: [idx_hi][idx_lo][char]
char* descomprimirLZ78_bin(const unsigned char* buf, int len) {
    if (!buf || len < 3) { char* z = new char[1]; z[0] = '\0'; return z; }
    int num = len / 3;
    if (num <= 0) { char* z = new char[1]; z[0] = '\0'; return z; }
    EntradaLZ78* arr = new EntradaLZ78[num];
    if (!arr) { char* z = new char[1]; z[0] = '\0'; return z; }

    int p = 0;
    for (int i = 0; i < num; ++i) {
        int idx = ((int)buf[p] << 8) | (int)buf[p + 1];
        char c  = (char)buf[p + 2];
        p += 3;
        arr[i].indice = idx;
        arr[i].caracter = c;
    }
    char* texto = descomprimirLZ78(arr, num);
    delete[] arr;
    return texto;
}

// Enmarcado UTF-16LE 4 bytes: [idx_lo][idx_hi][char][0x00]
char* descomprimirLZ78_bin_utf16(const unsigned char* buf, int len) {
    if (!buf || len < 4) { char* z = new char[1]; z[0] = '\0'; return z; }
    int num = len / 4;
    if (num <= 0) { char* z = new char[1]; z[0] = '\0'; return z; }
    EntradaLZ78* arr = new EntradaLZ78[num];
    if (!arr) { char* z = new char[1]; z[0] = '\0'; return z; }

    int p = 0;
    for (int i = 0; i < num; ++i) {
        int idx = (int)buf[p] | ((int)buf[p + 1] << 8); // little-endian
        char c  = (char)buf[p + 2];                     // ASCII
        // buf[p + 3] es padding (usualmente 0x00)
        p += 4;
        arr[i].indice = idx;
        arr[i].caracter = c;
    }
    char* texto = descomprimirLZ78(arr, num);
    delete[] arr;
    return texto;
}

// Detecta metodo, n y K usando la pista.
// Retorna 1 si encuentra una combinacion valida y llena los out-params.
int detectar_parametros(const unsigned char* enc, int lenEnc, const char* pista,
                        int* out_n, unsigned char* out_K, char* outMetodo /*"RLE" o "LZ78"*/,
                        char** out_texto) {
    for (int n = 1; n <= 7; ++n) {
        for (int K = 0; K <= 255; ++K) {
            unsigned char* plano = tryDecrypt(enc, lenEnc, n, (unsigned char)K);
            if (!plano) continue;

            // 1) Probar LZ78 3B
            char* lz3 = descomprimirLZ78_bin(plano, lenEnc);
            int okL3 = contiene(lz3, pista);

            // 2) Probar LZ78 UTF-16LE 4B
            char* lz4 = descomprimirLZ78_bin_utf16(plano, lenEnc);
            int okL4 = contiene(lz4, pista);

            if (okL3 || okL4) {
                *out_n = n; *out_K = (unsigned char)K;
                outMetodo[0]='L'; outMetodo[1]='Z'; outMetodo[2]='7'; outMetodo[3]='8'; outMetodo[4]='\0';
                if (okL3) { *out_texto = lz3; delete[] lz4; }
                else      { *out_texto = lz4; delete[] lz3; }
                delete[] plano;
                return 1;
            }

            // 3) Solo si no hubo match LZ78, probar RLE binario seguro
            char* rle = descomprimirRLE_bin(plano, lenEnc);
            int okR = contiene(rle, pista);
            if (okR) {
                *out_n = n; *out_K = (unsigned char)K;
                outMetodo[0]='R'; outMetodo[1]='L'; outMetodo[2]='E'; outMetodo[3]='\0';
                *out_texto = rle;
                delete[] lz3; delete[] lz4; delete[] plano;
                return 1;
            }

            delete[] rle;
            delete[] lz3;
            delete[] lz4;
            delete[] plano;
        }
    }
    return 0;
}

int main() {
    int nCasos = 0;
    std::printf("Ingrese el numero de archivos a procesar: ");
    if (std::scanf("%d", &nCasos) != 1) return 0;

    for (int caso = 1; caso <= nCasos; ++caso) {
        std::printf("\n=== Caso %d ===\n", caso);

        // Construir nombres de archivos: EncriptadoX.txt y pistaX.txt
        char encNombre[64];
        char pistaNombre[64];
        std::snprintf(encNombre, sizeof(encNombre), "Encriptado%d.txt", caso);
        std::snprintf(pistaNombre, sizeof(pistaNombre), "pista%d.txt", caso);

        int lenEnc = 0;
        unsigned char* enc = leerArchivo(encNombre, &lenEnc);
        if (!enc) { std::printf("No se pudo leer %s\n", encNombre); continue; }
        char* pista = leerTexto(pistaNombre);
        if (!pista) { std::printf("No se pudo leer %s\n", pistaNombre); delete[] enc; continue; }

        int n = 0; unsigned char K = 0; char metodo[8] = {0}; char* texto = 0;
        int ok = detectar_parametros(enc, lenEnc, pista, &n, &K, metodo, &texto);

        if (!ok) {
            std::printf("No se encontraron parametros validos.\n");
        } else {
            std::printf("Metodo: %s\nRotacion: %d\nKey: 0x%02X\n", metodo, n, (unsigned)K);
            std::printf("Texto final: %s\n", texto ? texto : "");
            if (texto) delete[] texto;
        }

        delete[] enc;
        delete[] pista;
    }

    return 0;
}


