#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "codigo.h"
#include "gerar_codigo.h"
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

void print_huffman_tree(Ptr_de_no_de_arvore_binaria nodo, int nivel, int is_right)
{
    if (nodo == NULL)
    {
        return;
    }

    // Imprime a subárvore direita primeiro
    print_huffman_tree(nodo->direita, nivel + 1, 1);

    // Imprime a indentação
    for (int i = 0; i < nivel - 1; i++)
    {
        printf("    ");
    }

    if (nivel > 0)
    {
        if (is_right)
        {
            printf(" /");
        }
        else
        {
            printf(" \\");
        }
    }

    // Imprime o nó atual
    if (nodo->esquerda == NULL && nodo->direita == NULL)
    {
        U8 b = nodo->informacao.byte;
        if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z'))
        {
            printf("-- '%c':%u\n", b, nodo->informacao.frequencia);
        }
        else
        {
            printf("-- 0x%02X:%u\n", b, nodo->informacao.frequencia);
        }
    }
    else
    {
        printf("-- *:%u\n", nodo->informacao.frequencia);
    }

    // Imprime a subárvore esquerda
    print_huffman_tree(nodo->esquerda, nivel + 1, 0);
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

    /*Manter nodos ativos no início da tabela, faz uma fila de prioridade, mantendo nodos
    Com menor frequencia nas posições iniciais*/
    junte_nodos_no_inicio_do_vetor(&tabela);

    // Construção da árvore
    while (tabela.quantidade_de_posicoes_preenchidas > 1)
    {
        // Organiza os nodos (função existente)
        junte_nodos_no_inicio_do_vetor(&tabela);

        // Pega os 2 primeiros (menores frequências)
        Ptr_de_no_de_arvore_binaria esq = tabela.vetor[0];
        Ptr_de_no_de_arvore_binaria dir = tabela.vetor[1];

        // Cria novo elemento
        Elemento elem;
        elem.byte = 0; // Valor irrelevante para nós internos
        elem.frequencia = esq->informacao.frequencia + dir->informacao.frequencia;

        // Cria novo nó
        Ptr_de_no_de_arvore_binaria novo;
        if (!novo_no_de_arvore_binaria_ext(&novo, esq, elem, dir))
        {
            printf("Falha na criação do nó.\n");
            return 1;
        }

        // Atualiza tabela
        tabela.vetor[0] = novo;
        tabela.vetor[1] = NULL;
        tabela.quantidade_de_posicoes_preenchidas--;
    }

    Codigo tabela_de_codigos[256] = {0};
    Codigo codigoAtual;
    novo_codigo(&codigoAtual);

    gerar_codigo(tabela.vetor[0], tabela_de_codigos, &codigoAtual);

    arquivo = fopen("frase_huffman.txt", "rb");
    FILE *compactado = fopen("compactado.bin", "wb");
    
    if (!arquivo || !compactado) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    unsigned char buffer = 0;
    int contador_bits = 0;

    while (fread(&byte, sizeof(U8), 1, arquivo) == 1) {
        Codigo *codigo = &tabela_de_codigos[byte];
        
        for (int j = 0; j < codigo->tamanho; j++) {
            int byte_index = j / 8;
            int bit_index = 7 - (j % 8);
            int bit = (codigo->byte[byte_index] >> bit_index) & 1;
            
            // Adiciona bit ao buffer
            buffer = (buffer << 1) | bit;
            contador_bits++;
            
            // Buffer cheio? Escreve no arquivo
            if (contador_bits == 8) {
                fwrite(&buffer, 1, 1, compactado);
                buffer = 0;
                contador_bits = 0;
            }
        }
    }
    
    // Escreve bits restantes
    if (contador_bits > 0) {
        buffer <<= (8 - contador_bits);
        fwrite(&buffer, 1, 1, compactado);
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho_original = ftell(arquivo);
    fseek(compactado, 0, SEEK_END);
    long tamanho_compactado = ftell(compactado);
    
    fclose(arquivo);
    fclose(compactado);

        // (Momentâneo, retirar para apresentar ao Maligno) Compactação
    printf("Compactação concluída!\n");
    printf("Tamanho original: %ld bytes\n", tamanho_original);
    printf("Tamanho compactado: %ld bytes\n", tamanho_compactado);
    printf("Taxa de compressão: %.2f%%\n", (1 - (float)tamanho_compactado/tamanho_original)*100);


    free_codigo(&codigoAtual);
    for (int i = 0; i < 256; i++)
    {
        if (tabela_de_codigos[i].tamanho > 0)
        {
            free_codigo(&tabela_de_codigos[i]);
        }
    }

    // (Momentâneo, retirar para apresentar ao Maligno) Exibição da árvore
    printf("\nArvore de Huffman (visualizacao):\n\n");
    print_huffman_tree(tabela.vetor[0], 0, 0);
    printf("\n");

    return 0;
}
