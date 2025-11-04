#ifndef LERMAPA_H
#define LERMAPA_H

typedef struct Tripulacao
{
    int forcaTripulacao; // F
    int forcaDescanso; // D
}Tripulacao;

typedef struct Mapa
{
    int altura; // h
    int largura; // w
    int** passadoGrid;
    int** presenteGrid;
}Mapa;

extern Mapa* mapa;
extern Tripulacao* tropa;
extern int forcaNikador; // N

void criarMapa();
void leituraArquivo(int argc, char* argv[]);
void imprimirMapas();
void destruirMapa();

#endif