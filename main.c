#include <stdio.h>
#include <stdlib.h>
#include "src/lerMapa.h"
#include "src/pDinamica.h"

Mapa *mapa;
Tripulacao *tropa;
int forcaNikador;

int main(int argc, char *argv[])
{
    criarMapa();
    leituraArquivo(argc, argv);
    imprimirMapas();

    EstruturaPD *estruturaPD = criarEstruturaPD(mapa->altura, mapa->largura);

    resolverPD(estruturaPD);
    imprimirCaminho(estruturaPD);

    liberarEstruturaPD(estruturaPD);
    destruirMapa();
    free(mapa);
    free(tropa);

    return 0;
}