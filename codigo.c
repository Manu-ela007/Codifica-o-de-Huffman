#include "codigo.h"
#include <stdlib.h>

boolean novo_codigo (Codigo* c /* por referência */)
{
    c->byte=(U8*)malloc(1*sizeof(U8));
    if (c->byte==NULL) return false;

    c->capacidade=8;
    c->tamanho=0;
    return true;
}

void free_codigo (Codigo* c /* por referência */)
{
    if (c->byte!=NULL) free(c->byte);
}

boolean adiciona_bit (Codigo* c /* por referência */,
                      U8 valor /* 0 ou 1 */)
{
    if (c->tamanho==c->capacidade)
    {
        U8* novo=(U8*)malloc((c->capacidade/8+1)*sizeof(U8));
        if (novo==NULL) return false;

        for(int i=0;i<c->tamanho/8;i++)
            novo[i]=c->byte[i];

        free(c->byte);
        c->byte=novo;

        c->capacidade+=8;
    }

    c->byte[c->capacidade/8-1]<<=1;

    if (valor==1)
        c->byte[c->capacidade/8-1]|=1;

    c->tamanho++;
    return true;
}


boolean joga_fora_bit (Codigo* c /* por referência */)
{
    if (c->tamanho==0) return false;

    c->byte[c->capacidade/8-1]>>=1;
    c->tamanho--;

    if (c->capacidade>8 && c->capacidade-c->tamanho==8)
    {
        U8* novo=(U8*)malloc((c->capacidade/8-1)*sizeof(U8));
        if (novo==NULL) return false;

        for(int i=0;i<c->tamanho/8;i++)
            novo[i]=c->byte[i];

        free(c->byte);
        c->byte=novo;

        c->capacidade-=8;
    }

    return true;
}

boolean clone (Codigo original, /* por valor */
               Codigo* copia    /* por referencia */)
{
    copia->byte=(U8*)malloc(original.capacidade*sizeof(U8));
    if (copia->byte==NULL) return false;


    for(int i=0;i<original.capacidade/8;i++)
        copia->byte[i]=original.byte[i];

    copia->capacidade=original.capacidade;
    copia->tamanho=original.tamanho;
    return true;
}
