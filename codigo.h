#ifndef CODIGO
#define CODIGO
#include "meustiposonlinegdb.h"

typedef struct
{
    U8 *byte;
    U8 capacidade;
    U8 tamanho;
} Codigo;

boolean novo_codigo(Codigo *c /* por referência */);
void free_codigo(Codigo *c /* por referência */);

boolean adiciona_bit(Codigo *c /* por referência */,
                     U8 valor /* 0 ou 1 */);
boolean joga_fora_bit(Codigo *c /* por referência */);

boolean clone(Codigo original, /* por valor */
              Codigo *copia /* por referencia */);

void imprime_codigo(Codigo *c);

boolean joga_fora_bit(Codigo *c);

#endif