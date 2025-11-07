#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lerMapa.h"

void criarMapa()
{
    mapa = malloc(sizeof(Mapa));
    tropa = malloc(sizeof(Tripulacao));

    if (mapa == NULL || tropa == NULL)
    {
        printf("Error ao alocar memoria!\n");
        exit(1);
    }
}

void leituraArquivo(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Use a formatação correta para a leitura do arquivo!");
        exit(1);
    }

    FILE *arq;
    char *nomeArq = argv[1];

    arq = fopen(nomeArq, "r");

    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo!");
        exit(1);
    }

    fscanf(arq, "%d %d %d %d %d", &mapa->altura, &mapa->largura,
           &tropa->forcaTripulacao, &tropa->forcaDescanso, &forcaNikador);

    mapa->passadoGrid = (int **)malloc(mapa->altura * sizeof(int *));
    mapa->presenteGrid = (int **)malloc(mapa->altura * sizeof(int *));

    if (mapa->passadoGrid == NULL || mapa->presenteGrid == NULL)
    {
        printf("Error ao alocar memoria para os grids!\n");
        exit(1);
    }

    for (int i = 0; i < mapa->altura; i++)
    {
        mapa->passadoGrid[i] = (int *)malloc((mapa->largura) * sizeof(int));
        mapa->presenteGrid[i] = (int *)malloc((mapa->largura) * sizeof(int));

        if (mapa->passadoGrid[i] == NULL || mapa->presenteGrid[i] == NULL)
        {
            printf("Error ao alocar memoria para as linhas dos grids!\n");
            exit(1);
        }
    }

    char buffer[1024];

    fgets(buffer, sizeof(buffer), arq);

    // Leitura com conversão de variaveis
    for (int i = 0; i < mapa->altura; i++)
    {
        if (fgets(buffer, sizeof(buffer), arq) == NULL)
        {
            break;
        }
        char *token = strtok(buffer, " \n");

        for (int j = 0; j < mapa->largura; j++)
        {
            if (token != NULL)
            {
                if (strcmp(token, "***") == 0)
                {
                    mapa->presenteGrid[i][j] = INTRANSPONIVEL;
                }
                else if (strcmp(token, "AAA") == 0)
                {
                    mapa->presenteGrid[i][j] = ANCORA;
                }
                else
                {
                    mapa->presenteGrid[i][j] = atoi(token);
                }
                token = strtok(NULL, " \n");
            }
        }
    }

    fgets(buffer, sizeof(buffer), arq);

    for (int i = 0; i < mapa->altura; i++)
    {

        if (fgets(buffer, sizeof(buffer), arq) == NULL)
        {
            break;
        }
        char *token = strtok(buffer, " \n");

        for (int j = 0; j < mapa->largura; j++)
        {
            if (token != NULL)
            {
                if (strcmp(token, "***") == 0)
                {
                    mapa->passadoGrid[i][j] = INTRANSPONIVEL;
                }
                else if (strcmp(token, "AAA") == 0)
                {
                    mapa->passadoGrid[i][j] = ANCORA;
                }
                else
                {
                    mapa->passadoGrid[i][j] = atoi(token);
                }
                token = strtok(NULL, " \n");
            }
        }
    }

    fclose(arq);
}

void imprimirMapas()
{
    printf("----Mapas e Variveis----\n");
    printf("h: %d, w: %d, F: %d, D: %d, N: %d\n", mapa->altura,
           mapa->largura, tropa->forcaTripulacao, tropa->forcaDescanso, forcaNikador);

    printf("Presente:\n");
    for (int i = 0; i < mapa->altura; i++)
    {
        for (int j = 0; j < mapa->largura; j++)
        {
            printf("%3d ", mapa->presenteGrid[i][j]);
        }
        printf("\n");
    }

    printf("Passado:\n");
    for (int i = 0; i < mapa->altura; i++)
    {
        for (int j = 0; j < mapa->largura; j++)
        {
            printf("%3d ", mapa->passadoGrid[i][j]);
        }
        printf("\n");
    }
}

void destruirMapa()
{
    if (mapa == NULL)
        return;

    for (int i = 0; i < mapa->altura; i++)
    {
        free(mapa->presenteGrid[i]);
        free(mapa->passadoGrid[i]);
    }

    free(mapa->presenteGrid);
    free(mapa->passadoGrid);
}