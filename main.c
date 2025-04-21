#include <stdio.h>

int main() {
    FILE *arquivo;          
    int freq[256] = {0};    
    int caractere;         

    arquivo = fopen("frase_huffman.txt", "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    caractere = fgetc(arquivo);
    if (caractere == EOF) {
        printf("O arquivo está vazio.\n");
        fclose(arquivo); 
        return 1; 
    }

    ungetc(caractere, arquivo);

    while ((caractere = fgetc(arquivo)) != EOF) {
        freq[caractere]++; 
    }

    fclose(arquivo);

    printf("Frequência de cada caractere:\n\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (freq[i] == 1) {
                if (i >= 32 && i <= 126) {
                    if (i == 44) {
                        printf("Vírgula apareceu 1 vez\n");
                    } else if (i == 46) {
                        printf("Ponto apareceu 1 vez\n");
                    } else if (i == 33) {
                        printf("Ponto de exclamação apareceu 1 vez\n");
                    } else {
                        printf("'%c' apareceu 1 vez\n", i);
                    }
                } else {
                    if (i == 9) {
                        printf("Tabulação apareceu 1 vez\n");
                    } else if (i == 10) {
                        printf("Quebra de linha (LF) apareceu 1 vez\n");
                    } else if (i == 13) {
                        printf("Retorno de carro (CR) apareceu 1 vez\n");
                    } else if (i == 32) {
                        printf("Espaço apareceu 1 vez\n");
                    } else {
                        printf("Caractere não visível (ASCII %d) apareceu 1 vez\n", i);
                    }
                }
            } else {

                if (i >= 32 && i <= 126) {
                    if (i == 44) {
                        printf("Vírgula apareceu %d vezes\n", freq[i]);
                    } else if (i == 46) {
                        printf("Ponto apareceu %d vezes\n", freq[i]);
                    } else if (i == 33) {
                        printf("Ponto de exclamação apareceu %d vezes\n", freq[i]);
                    } else {
                        printf("'%c' apareceu %d vezes\n", i, freq[i]);
                    }
                } else {
                    if (i == 9) {
                        printf("Tabulação apareceu %d vezes\n", freq[i]);
                    } else if (i == 10) {
                        printf("Quebra de linha apareceu %d vezes\n", freq[i]);
                    } else if (i == 13) {
                        printf("Retorno de carro apareceu %d vezes\n", freq[i]);
                    } else if (i == 32) {
                        printf("Espaço apareceu %d vezes\n", freq[i]);
                    } else {
                        printf("Caractere não visível (ASCII %d) apareceu %d vezes\n", i, freq[i]);
                    }
                }
            }
        }
    }

    return 0;
}
