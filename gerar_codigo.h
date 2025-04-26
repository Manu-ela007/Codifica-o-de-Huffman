#ifndef GERAR_CODIGO
#define GERAR_CODIGO

#include "codigo.h"
#include "tabela_de_frequencias.h"

boolean gerar_codigo(
    Ptr_de_no_de_arvore_binaria raiz,
    Codigo tabela_de_codigos[256],
    Codigo *atual);

#endif
