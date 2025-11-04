#include <stdio.h>
#include <stdlib.h>
#include "src/lerMapa.h"

Mapa* mapa;
Tripulacao* tropa;
int forcaNikador;

int main(int argc, char* argv[]) {
    criarMapa();
    leituraArquivo(argc, argv);
    imprimirMapas();
    
    destruirMapa();
    free(mapa);
    free(tropa);
    
    return 0;
}