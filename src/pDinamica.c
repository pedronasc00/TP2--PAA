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

//calcula a força depois mover para uma célula
int calcularNovaForca(int forcaAnterior, int valorCelula) {
    if (forcaAnterior <= 0) {
        return -1; 
    }
    
    if (valorCelula == ANCORA) {
        return forcaAnterior; 
    } else if (valorCelula == 0) {
        return forcaAnterior + tropa->forcaDescanso; 
    } else if (valorCelula > 0) {
        return forcaAnterior - valorCelula; 
    }
    return -1;
}

//pegar o valor da célula no mapa global
int getValorCelula(int tempo, int linha, int coluna) {
    if (tempo == 0) {
        return mapa->presenteGrid[linha][coluna];
    } else {
        return mapa->passadoGrid[linha][coluna];
    }
}

int getForcaOrigem(EstruturaPD* pd, int tempo, int linha, int coluna) {
    if (linha < 0 || linha >= pd->linhas) {
        return -1; // Fora dos limites
    }

    int forca = pd->melhorForca[tempo][linha][coluna];
    return (forca > 0) ? forca : -1;
}

int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

void resolverPD(EstruturaPD* pd) {
    int h = pd->linhas;
    int w = pd->colunas;

    // 1. Caso Base
    for (int i = 0; i < h; i++) {
        for (int t = 0; t < 2; t++) {
            int valor = getValorCelula(t, i, 0);
            if (valor != INTRANSPONIVEL) {
                pd->melhorForca[t][i][0] = calcularNovaForca(tropa->forcaTripulacao, valor);
            }
        }
    }

    // 2. Passo Recursivo
    for (int j = 1; j < w; j++) {
        for (int i = 0; i < h; i++) {
            
            int valorPresente = getValorCelula(0, i, j);
            int valorPassado = getValorCelula(1, i, j);

            if (valorPresente == ANCORA) {
                int maxAnterior = -1;
                for (int k = i - 1; k <= i + 1; k++) {
                    int forcaVindaPresente = getForcaOrigem(pd, 0, k, j - 1);
                    int forcaVindaPassado = getForcaOrigem(pd, 1, k, j - 1);
                    
                    if (forcaVindaPresente > maxAnterior) maxAnterior = forcaVindaPresente;
                    if (forcaVindaPassado > maxAnterior) maxAnterior = forcaVindaPassado;
                }

                if (maxAnterior > 0) {
                    int forcaFinal = calcularNovaForca(maxAnterior, ANCORA);
                    pd->melhorForca[0][i][j] = forcaFinal;
                    pd->melhorForca[1][i][j] = forcaFinal;
                }
            } else {
                //calculando para o presente
                if (valorPresente != INTRANSPONIVEL) {
                    int maxAnteriorPresente = max3(
                        getForcaOrigem(pd, 0, i - 1, j - 1),
                        getForcaOrigem(pd, 0, i, j - 1),
                        getForcaOrigem(pd, 0, i + 1, j - 1)
                    );
                    pd->melhorForca[0][i][j] = calcularNovaForca(maxAnteriorPresente, valorPresente);
                }

                //calculando para o Passado
                if (valorPassado != INTRANSPONIVEL) {
                    int maxAnteriorPassado = max3(
                        getForcaOrigem(pd, 1, i - 1, j - 1),
                        getForcaOrigem(pd, 1, i, j - 1),
                        getForcaOrigem(pd, 1, i + 1, j - 1)
                    );
                    pd->melhorForca[1][i][j] = calcularNovaForca(maxAnteriorPassado, valorPassado);
                }
            }
        }
    }
}

//estrutura para guardar uma coordenada do caminho
typedef struct {
    int i, j;
} Ponto;

void imprimirCaminho(EstruturaPD* pd) {
    int h = pd->linhas;
    int w = pd->colunas;
    
    int forcaFinal = -1;
    int linhaFinal = -1;
    int tempoFinal = -1;

    for (int i = 0; i < h; i++) {
        if (pd->melhorForca[0][i][w - 1] > forcaFinal) {
            forcaFinal = pd->melhorForca[0][i][w - 1];
            linhaFinal = i;
            tempoFinal = 0;
        }
        if (pd->melhorForca[1][i][w - 1] > forcaFinal) {
            forcaFinal = pd->melhorForca[1][i][w - 1];
            linhaFinal = i;
            tempoFinal = 1;
        }
    }

    if (forcaFinal <= 0) {
        printf("A calamidade de Nikador é inevitável\n");
        return;
    }

    // 3. Reconstruir o caminho
    Ponto* caminho = malloc(w * sizeof(Ponto)); 
    if (caminho == NULL) exit(1);

    int linhaAtual = linhaFinal;
    int tempoAtual = tempoFinal;
    
    for (int j = w - 1; j >= 0; j--) {
        caminho[j].i = linhaAtual;
        caminho[j].j = j;
        
        if (j == 0) break;

        int valorAtual = getValorCelula(tempoAtual, linhaAtual, j);
        int melhorForcaAnterior = -1;
        int proximaLinha = -1;
        int proximoTempo = -1;

        if (valorAtual == ANCORA) {
            proximoTempo = 1 - tempoAtual; //entrou na ancora e trocou de tempo

            for (int k = linhaAtual - 1; k <= linhaAtual + 1; k++) {
                if (k < 0 || k >= h) continue; //fora do mapa

                int f_origem = getForcaOrigem(pd, proximoTempo, k, j - 1);
                
                if (f_origem > melhorForcaAnterior) {
                    melhorForcaAnterior = f_origem;
                    proximaLinha = k;
                }
            }
        } else {

            proximoTempo = tempoAtual;//continuou no mesmo tempo

            for (int k = linhaAtual - 1; k <= linhaAtual + 1; k++) {
                if (k < 0 || k >= h) continue;

                int f_origem = getForcaOrigem(pd, proximoTempo, k, j - 1);

                if (f_origem > melhorForcaAnterior) {
                    melhorForcaAnterior = f_origem;
                    proximaLinha = k;
                }
            }
        }
        linhaAtual = proximaLinha;
        tempoAtual = proximoTempo;
    }
    for (int j = 0; j < w; j++) {
        // A saída no PDF de exemplo (20, 11).
        // Assumindo que signifique "linha 2, coluna 0", "linha 1, coluna 1", etc.
        printf("%d %d\n", caminho[j].i, caminho[j].j);
    }

    if (forcaFinal >= forcaNikador) {
        printf("A ruina de Nikador e iminente\n");
    } else {
        printf("Sera necessario mais planejamento para parar a calamidade\n");
    }

    free(caminho);
}