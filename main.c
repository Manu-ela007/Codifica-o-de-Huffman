#include <stdio.h>
#include <stdlib.h>
#include "codigo.h"
#include "tabela_de_frequencias.h"

boolean novo_no_de_arvore_binaria_ext(Ptr_de_no_de_arvore_binaria *novo,
                                    Ptr_de_no_de_arvore_binaria esq,
                                    Elemento inf,
                                    Ptr_de_no_de_arvore_binaria dir)
{
    *novo = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
    if (*novo == NULL)
        return false;
    (*novo)->esquerda = esq;
    (*novo)->informacao = inf;
    (*novo)->direita = dir;
    return true;
}

void print_huffman_tree(Ptr_de_no_de_arvore_binaria nodo, int nivel)
{
    if (nodo == NULL)
    {
        printf("Árvore de Huffman vazia ou não formada corretamente.\n");
        return;
    }
    // Imprime primeiro a subárvore direita com indentação maior
    print_huffman_tree(nodo->direita, nivel + 4);

    // Imprime espaços em branco conforme o nível
    for (int i = 0; i < nivel; i++)
        putchar(' ');

    // Se for folha, mostra o caractere; senão, marcador '*'
    if (nodo->esquerda == NULL && nodo->direita == NULL)
    {
        // Folha: caractere + frequência
        U8 b = nodo->informacao.byte;
        if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z'))
            printf("'%c':%u\n", b, nodo->informacao.frequencia);
        else
            printf("0x%02X:%u\n", b, nodo->informacao.frequencia);
    }
    else
    {
        // Nodo interno: apenas frequência
        printf("*:%u\n", nodo->informacao.frequencia);
    }

    // Imprime a subárvore esquerda
    print_huffman_tree(nodo->esquerda, nivel + 4);
}

int main()
{
    FILE *arquivo;
    Tabela_de_frequencias tabela;

    // Inicializa a tabela de frequências
    nova_tabela_de_frequencias(&tabela);

    // Abre o arquivo para leitura
    arquivo = fopen("frase_huffman.txt", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    U8 byte;
    // Lê cada byte do arquivo e conta a frequência
    while (fread(&byte, sizeof(U8), 1, arquivo) == 1)
    {
        inclua_byte(byte, &tabela);
    }

    fclose(arquivo); // Fecha o arquivo

    // Imprime a frequência de cada byte
    printf("Frequência de letras encontradas:\n");
    for (U16 i = 0; i < 256; i++)
    {
        if (tabela.vetor[i] != NULL)
        {
            U8 b = tabela.vetor[i]->informacao.byte;

            // Checa se o byte representa uma letra
            if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z'))
            {
                printf("Letra: '%c', Frequência: %u\n", b, tabela.vetor[i]->informacao.frequencia);
            }
        }
    }

    // Construir árvore de Huffman usando busca dos dois menores
    while (tabela.quantidade_de_posicoes_preenchidas > 1)
    {
        int min1 = -1, min2 = -1;
        // Encontra os dois índices com menor frequência
        for (int i = 0; i < 256; i++)
        {
            if (tabela.vetor[i] == NULL)
                continue;
            if (min1 < 0 || tabela.vetor[i]->informacao.frequencia < tabela.vetor[min1]->informacao.frequencia)
            {
                min2 = min1;
                min1 = i;
            }
            else if (min2 < 0 || tabela.vetor[i]->informacao.frequencia < tabela.vetor[min2]->informacao.frequencia)
            {
                min2 = i;
            }
        }

        // Desenfileira os dois menores
        Ptr_de_no_de_arvore_binaria esquerda = tabela.vetor[min1];
        Ptr_de_no_de_arvore_binaria direita = tabela.vetor[min2];

        // Cria elemento com frequência somada
        Elemento elem;
        elem.byte = '*'; // marcador genérico para nodos internos
        elem.frequencia = esquerda->informacao.frequencia + direita->informacao.frequencia;

        // Cria o novo nodo com os dois como filhos
        Ptr_de_no_de_arvore_binaria novo;
        if (!novo_no_de_arvore_binaria_ext(&novo, esquerda, elem, direita))
        {
            printf("Erro: falha ao alocar novo nodo da árvore de Huffman\n");
            break;
        }

        // Insere o novo nodo no lugar de min1 e libera min2
        tabela.vetor[min1] = novo;
        tabela.vetor[min2] = NULL;
        tabela.quantidade_de_posicoes_preenchidas--;
    }

    if (tabela.vetor[0])
    {
        printf("\nÁrvore de Huffman:");
        print_huffman_tree(tabela.vetor[0], 0);
    }

    return 0;
}
