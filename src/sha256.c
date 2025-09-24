#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/sha256.h"

// this took me 2 days
// :3

static void memfail() {
    fprintf(stderr, "Failed to allocate memory\n");
    return;
}

const uint32_t constants[64] = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

char *to_bin(uint64_t n, uint64_t bits) {
    char *result = malloc(bits+1);
    if (result == NULL) {
        memfail();
        return 0;
    }

    for (int i = bits-1; i >= 0; i--) {
        result[bits-1-i] = (n & (1ULL << i)) ? '1' : '0';
    }
    result[bits] = '\0';
    return result;
}

uint64_t uint64_pow(uint64_t base, uint64_t power) {
    uint64_t result = 1;
    while (power > 0) {
        result *= base;
        power--;
    }
    return result;
}

uint64_t to_dec(char *bin, uint64_t bits) {
    uint64_t result = 0;
    
    for (int i = bits-1; i >= 0; i--) {
        result += (bin[i] == '1' ? uint64_pow(2, bits-1-i) : 0);
    }
    return result;
}

uint64_t ror(uint64_t x, uint64_t n, uint64_t bits) {
    return (x >> n) | (x << (bits - n));
}

// for debug
// char *bin_to_hex(char *bin, uint64_t bits) {
//     char *hex = malloc(bits/4+1);
//     memset(hex, 0, bits/4+1);
//     if (hex == NULL) return 0;
    
//     for (uint64_t i = 0; i < bits/4; i++) {
//         char b[4] = {bin[(i+1)*4-4], bin[(i+1)*4-3], bin[(i+1)*4-2], bin[(i+1)*4-1]};
//         uint64_t n = to_dec(b, 4); 
//         switch (n) {
//             case 10:
//                 hex[i] = 'a';
//                 break;
//             case 11:
//                 hex[i] = 'b';
//                 break;
//             case 12:
//                 hex[i] = 'c';
//                 break;
//             case 13:
//                 hex[i] = 'd';
//                 break;
//             case 14:
//                 hex[i] = 'e';
//                 break;
//             case 15:
//                 hex[i] = 'f';
//                 break;
            
//             default:
//                 hex[i] = n%16 + '0';
//                 break;
//         }
//     }
//     hex[bits] = '\0';
//     return hex;
// }

uint64_t create_hash(char* string) {
    uint32_t modifiers[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};

    uint64_t mem = 512;
    char *data = malloc(mem);
    if (data == NULL) {
        memfail();
        return 0;
    }
    for (uint64_t j = 0; j < mem; j++) {
        data[j] = '0';
    }

    int i = 0;
    while(string[i] != '\0') {
        if (i*sizeof(string) > mem) {
            uint64_t *tmp = malloc(mem);
            memcpy(tmp, data, mem);
            data = realloc(data, mem+512);
            if (data == NULL) {
                memfail();
                return 0;
            }
            memcpy(data, tmp, mem);
            for (int j = mem; j < mem+512/8; j++) {
                data[j] = '0';
            }
            free(tmp);
            mem += 512;
        }
        char *bin = to_bin(string[i], 8);
        for (int j = i*8; j < (i*8)+8; j++) {
            data[j] = bin[j%8];
        }
        free(bin);
        i++;
    }
    data[i*8] = '1';
    
    // put size into last 64 bits (big-endian)
    char *bin_size = to_bin(i*8, 64);
    for (uint64_t j = mem-64; j < mem; j++) {
        data[j] = bin_size[j%64];
    }
    free(bin_size);
    // printf("%s\n", data);

    for (uint64_t chunk = 0; chunk < mem/512; chunk++) {
        // for each chunk of 512
        char **words = malloc(64*sizeof(char*));
        if (words == NULL) {
            memfail();
            return 0;
        }

        uint8_t bytes[64];
        for (uint8_t k = 0; k < 64; k++) {
            char *bin = malloc(9);
            if (bin == NULL) {
                memfail();
                return 0;
            }
            for (uint64_t h = k*8; h < k*8+8; h++) {
                bin[h%8] = data[h];
            }
            bin[8] = '\0';
            bytes[k] = to_dec(bin,8);
            free(bin);
        }

        for (uint8_t j = 0; j < 16; j++) {
            // make first 16 words
            words[j] = to_bin((bytes[j*4] << 24) | (bytes[j*4+1] << 16) |  (bytes[j*4+2] << 8) | (bytes[j*4+3]),32);
            // printf("%s\n", words[j]);
        }

        for (uint8_t j = 16; j < 64; j++) {
            //for each empty word
            uint32_t s0 = ror(to_dec(words[j-15],32), 7,32) ^ ror(to_dec(words[j-15],32), 18,32) ^ (to_dec(words[j-15],32) >> 3);
            uint32_t s1 = ror(to_dec(words[j-2],32), 17,32) ^ ror(to_dec(words[j-2],32), 19,32) ^ (to_dec(words[j-2],32) >> 10);
            words[j] = to_bin((to_dec(words[j-16], 32) + s0 + to_dec(words[j-7], 32) + s1),32);
        }

        uint32_t a,b,c,d,e,f,g,h;
        a = modifiers[0];
        b = modifiers[1];
        c = modifiers[2];
        d = modifiers[3];
        e = modifiers[4];
        f = modifiers[5];
        g = modifiers[6];
        h = modifiers[7];

        for (uint8_t k = 0; k < 64; k++) {
            // compression
            uint32_t S1 = ror(e,6,32) ^ ror(e,11,32) ^ ror(e,25,32);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + constants[k] + to_dec(words[k] != NULL ? words[k] : "00000000000000000000000000000000",32);
            uint32_t S0 = ror(a,2,32) ^ ror(a,13,32) ^ ror(a,22,32);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
            free(words[k]);
        }

        modifiers[0] += a;
        modifiers[1] += b;
        modifiers[2] += c;
        modifiers[3] += d;
        modifiers[4] += e;
        modifiers[5] += f;
        modifiers[6] += g;
        modifiers[7] += h;
    }
    free(data);

    // return last 64bit num (we dont need the whole 256bit number)
    char *bin_val = malloc(64);
    if (bin_val == NULL) {
        memfail();
        return 0;
    }
    strcpy(bin_val, to_bin(modifiers[6],32));
    strcat(bin_val, to_bin(modifiers[7],32));
    // printf("%s\n", bin_to_hex(bin_val, 64));
    uint64_t val = to_dec(bin_val,64);
    free(bin_val);
    return val;
}