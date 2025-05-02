#include <stdio.h>
#include <stdlib.h>
#include "codigo.h"

bool novo_codigo(Codigo *c) {
    c->byte = (U8 *)malloc(1);
    if (c->byte == NULL) return false;
    c->capacidade = 8;
    c->tamanho = 0;
    return true;
}

bool adiciona_bit(Codigo *c, U8 valor) {
    if (c->tamanho >= c->capacidade) {
        int nova_capacidade = c->capacidade * 2;
        U8 *novo = (U8 *)realloc(c->byte, nova_capacidade / 8);
        if (novo == NULL) return false;
        c->byte = novo;
        c->capacidade = nova_capacidade;
    }

    int byte_index = c->tamanho / 8;
    int bit_index = c->tamanho % 8;

    if (valor == 1)
        c->byte[byte_index] |= (1 << (7 - bit_index));
    else
        c->byte[byte_index] &= ~(1 << (7 - bit_index));

    c->tamanho++;
    return true;
}

bool joga_fora_bit(Codigo *c) {
    if (c->tamanho == 0) return false;
    c->tamanho--;

    if (c->capacidade > 8 && c->capacidade - c->tamanho == 8) {
        int nova_capacidade = c->capacidade - 8;
        U8 *novo = (U8 *)realloc(c->byte, nova_capacidade / 8);
        if (novo == NULL) return false;
        c->byte = novo;
        c->capacidade = nova_capacidade;
    }

    return true;
}

void imprime_codigo(Codigo *c) {
    for (int i = 0; i < c->tamanho; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        int bit = (c->byte[byte_index] >> (7 - bit_index)) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

void free_codigo(Codigo *c) {
    if (c->byte != NULL) {
        free(c->byte);
        c->byte = NULL;
    }
    c->capacidade = 0;
    c->tamanho = 0;
}

bool clone(Codigo original, Codigo *copia) {
    copia->byte = (U8 *)malloc(original.capacidade / 8);
    if (copia->byte == NULL) return false;

    for (int i = 0; i < original.capacidade / 8; i++) {
        copia->byte[i] = original.byte[i];
    }

    copia->capacidade = original.capacidade;
    copia->tamanho = original.tamanho;
    return true;
}