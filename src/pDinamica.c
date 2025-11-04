#include <stdio.h>
#include <stdlib.h>
#include "pDinamica.h"

EstruturaPD* criarEstruturaPD(int linhas, int colunas) {
    EstruturaPD* estruturaPD = malloc(sizeof(EstruturaPD));
    if (estruturaPD == NULL) {
        printf("Erro ao alocar memória para a estrutura PD!\n");
        exit(1);
    }

    estruturaPD->linhas = linhas;
    estruturaPD->colunas = colunas;

    // tempo = 0 → mapa do presente, tempo = 1 → mapa do passado
    estruturaPD->melhorForca = malloc(2 * sizeof(int**));
    if (estruturaPD->melhorForca == NULL) {
        printf("Erro ao alocar memória para os mapas de tempo!\n");
        exit(1);
    }

    for (int tempo = 0; tempo < 2; tempo++) {
        estruturaPD->melhorForca[tempo] = malloc(linhas * sizeof(int*));
        if (estruturaPD->melhorForca[tempo] == NULL) {
            printf("Erro ao alocar memória para as linhas do tempo %d!\n", tempo);
            exit(1);
        }

        for (int linha = 0; linha < linhas; linha++) {
            estruturaPD->melhorForca[tempo][linha] = malloc(colunas * sizeof(int));
            if (estruturaPD->melhorForca[tempo][linha] == NULL) {
                printf("Erro ao alocar memória para as colunas do tempo %d!\n", tempo);
                exit(1);
            }

            for (int coluna = 0; coluna < colunas; coluna++) {
                estruturaPD->melhorForca[tempo][linha][coluna] = -1; // -1 = célula inalcançável
            }
        }
    }

    return estruturaPD;
}

void liberarEstruturaPD(EstruturaPD* estruturaPD) {
    if (estruturaPD == NULL) return;

    for (int tempo = 0; tempo < 2; tempo++) {
        for (int linha = 0; linha < estruturaPD->linhas; linha++) {
            free(estruturaPD->melhorForca[tempo][linha]);
        }
        free(estruturaPD->melhorForca[tempo]);
    }

    free(estruturaPD->melhorForca);
    free(estruturaPD);
}
