#ifndef CODIGO_H
#define CODIGO_H

#include <stdbool.h>

typedef unsigned char U8;

typedef struct {
    U8 *byte;
    int tamanho;
    int capacidade;
} Codigo;

bool novo_codigo(Codigo *c);
bool adiciona_bit(Codigo *c, U8 valor);
bool joga_fora_bit(Codigo *c);
bool clone(Codigo original, Codigo *copia);
void imprime_codigo(Codigo *c);
void free_codigo(Codigo *c);

#endif