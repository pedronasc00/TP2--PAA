#ifndef DINAMICA_H
#define DINAMICA_H


typedef struct {
    int linhas;                 
    int colunas;                
    int ***melhorForca;         // melhorForca[tempo][linha][coluna]
                               
} EstruturaPD;

EstruturaPD* criarEstruturaPD(int linhas, int colunas);
void liberarEstruturaPD(EstruturaPD* estruturaPD);

#endif
