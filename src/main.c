/*
    HC128 Implemented by Minh Kien 20203471
*/

#include <stdio.h>
#include <stdint.h>

typedef uint32_t uint32;

// HC-128 state structure
typedef struct {
    uint32 P[512];
    uint32 Q[512];
} HC128_State;

// Initialization HC128 function, include f1 f2 g1 g2 h1 h2
uint32 ROTR32(uint32 x, int n) {
    return ((x) >> (n)) ^ ((x) << (32 - (n)));
}

uint32 ROTL32(uint32 x, int n) {
    return ((x) << (n)) ^ ((x) >> (32 - (n)));
}

uint32 F1(uint32 x) {
    return ((ROTR32(x, 7) ^ ROTR32(x, 18)) ^ ROTR32(x, 3));
}

uint32 F2(uint32 x) {
    return ((ROTR32(x, 17) ^ ROTR32(x, 19)) ^ ROTR32(x, 10));

}

uint32 G1(uint32 x, uint32 y, uint32 z) {
    uint32 tmp = (ROTR32(x, 10) ^ ROTR32(z, 23));
    tmp = (tmp + ROTR32(y, 8)) % (1ULL << 32);
    return tmp;
}

uint32 G2(uint32 x, uint32 y, uint32 z) {
    uint32 tmp = (ROTL32(x, 10) ^ ROTL32(z, 23));
    tmp = (tmp + ROTL32(y, 8)) % (1ULL << 32);
    return tmp;
}


uint32 H1(uint32 x, HC128_State *state) {
    uint32 x0 = x & (0xff);
    uint32 x2 = (x & (0xff0000)) >> 16;
    return (state->Q[x0] + state->Q[x2 + 256]) % (1ULL << 32);
}

uint32 H2(uint32 x, HC128_State *state) {
    uint32 x0 = x & (0xff);
    uint32 x2 = (x & (0xff0000)) >> 16;
    return (state->P[x0] + state->P[x2 + 256]) % (1ULL << 32);
}


// Initialization function
void HC128_Init(HC128_State *state, const uint32 *key, const uint32 *iv) {
    // expanded key and IV into array Wi
    int i = 0;
    uint32 W[1280];
    for (i = 0; i <= 7; ++i){
        W[i] = key[i];
    }
    for (i = 8; i <= 15; ++i) {
        W[i] = iv[i -8];
    }
    for (i = 16; i <= 1279; ++i) {
        uint32 tmp = (F2(W[i-2]) + W[i - 7]) % (1ULL << 32);
        tmp = (tmp + F1(W[i - 15])) % (1ULL << 32);
        tmp = (tmp + W[i - 16]) % (1ULL << 32);
        tmp = (tmp + i) % (1ULL << 32);
        W[i] = tmp;
    }

    // Update P and Q 
    for (i = 0; i <= 511; ++i) {
        state->P[i] = W[i + 256];
        state->Q[i] = W[i + 768];
    }

    // Run cipher 1024 steps 
    for (i = 0; i <= 511; ++i) {
        uint32 tmp = (state->P[i] + G1(state->P[(i - 3) % 512], state->P[(i - 10) % 512], state->P[(i - 511) % 512])) % (1ULL << 32);
        tmp = tmp ^ H1(state->P[(i-12) % 512], state);
        state->P[i] = tmp;
    }

    for (i = 0; i <= 511; ++i) {
        uint32 tmp = (state->Q[i] + G2(state->Q[(i - 3) % 512], state->Q[(i - 10) % 512], state->Q[(i - 511) % 512])) % (1ULL << 32);
        tmp = tmp ^ H2(state->Q[(i-12) % 512], state);
        state->Q[i] = tmp;
    }

}

// Generate N word keysteam
void HC128_Generate_Keystream(HC128_State *state, uint32 key_length, uint32* S) {
    uint32 i = 0;
    for (i = 0; i < key_length; ++i) {
        uint32 j = i % 512;
        if ( (i % 1024) < 512 ) {
            state->P[j] = (state->P[j] + G1(state->P[(j - 3) % 512], state->P[(j - 10) % 512], state->P[(j - 511) % 512])) % (1ULL << 32);
            S[i] = H1(state->P[(j - 12) % 512], state) ^ state->P[j];
        }
        else {
            state->Q[j] = (state->Q[j] + G2(state->Q[(j - 3) % 512], state->Q[(j - 10) % 512], state->Q[(j - 511) % 512])) % (1ULL << 32);
            S[i] = H2(state->Q[(j - 12) % 512], state) ^ state->Q[j];
        }
    }
}

int main() {
    // Example key and IV
    uint32 key[8] = {0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210, 0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210};
    uint32 iv[8] = {0x0F0E0D0C, 0x04030201, 0x08070605, 0x0C0B0A09, 0x0F0E0D0C, 0x04030201, 0x08070605, 0x0C0B0A09};

    // Initialize HC-128 state
    HC128_State state;
    HC128_Init(&state, key, iv);

    // Generate key stream
    size_t numWords = 16; // Number of 32-bit words to generate
    uint32 keystream[numWords];
    HC128_Generate_Keystream(&state, numWords, keystream);

    // Print the generated key stream
    for (size_t i = 0; i < numWords; i++) {
        printf("%08X ", keystream[i]);
    }
    printf("\n");

    return 0;
}
