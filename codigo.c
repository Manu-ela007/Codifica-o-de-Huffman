#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "codigo.h"

boolean novo_codigo(Codigo *c)
{
    c->byte = (U8 *)malloc(1 * sizeof(U8));
    if (c->byte == NULL)
    {
        return false;
    }
    c->capacidade = 8;
    c->tamanho = 0;
    return true;
}

boolean adiciona_bit(Codigo *c, U8 valor)
{
    if (c->tamanho >= c->capacidade)
    {
        c->capacidade *= 2;
        c->byte = (U8 *)realloc(c->byte, c->capacidade / 8 * sizeof(U8));
        if (c->byte == NULL)
        {
            return false;
        }
    }

    int byte_index = c->tamanho / 8;
    int bit_index = c->tamanho % 8;

    if (valor == 1)
    {
        c->byte[byte_index] |= (1 << (7 - bit_index));
    }
    else
    {
        c->byte[byte_index] &= ~(1 << (7 - bit_index));
    }

    c->tamanho++;
    return true;
}

void imprime_codigo(Codigo *c)
{
    for (int i = 0; i < c->tamanho; i++)
    {
        int byte_index = i / 8;
        int bit_index = i % 8;

        int bit = (c->byte[byte_index] >> (7 - bit_index)) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

boolean joga_fora_bit(Codigo *c)
{
    if (c->tamanho == 0)
    {
        return false;
    }
    c->tamanho--;
    return true;
}

void free_codigo(Codigo *c)
{
    free(c->byte);
    c->byte = NULL;
    c->capacidade = 0;
    c->tamanho = 0;
}
