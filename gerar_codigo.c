#include <stdlib.h>
#include "gerar_codigo.h"
#include <stdbool.h>

boolean gerar_codigo(
    Ptr_de_no_de_arvore_binaria raiz,
    Codigo tabela_de_codigos[256],
    Codigo *atual)
{
    if (raiz == NULL)
        return true;

    if (raiz->esquerda == NULL && raiz->direita == NULL)
        return clone(*atual, &tabela_de_codigos[raiz->informacao.byte]);

    adiciona_bit(atual, 0);
    if (!gerar_codigo(raiz->esquerda, tabela_de_codigos, atual))
        return false;
    joga_fora_bit(atual);

    adiciona_bit(atual, 1);
    if (!gerar_codigo(raiz->direita, tabela_de_codigos, atual))
        return false;
    joga_fora_bit(atual);

    return true;
}
