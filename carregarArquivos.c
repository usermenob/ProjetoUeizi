#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dados{
    int idUsuario;
    int timestamp;
    int br;
    float km;
    char tipoAlerta;
};

int lerArquivo(struct Dados **registro){
    FILE *arquivo = fopen("alertas_1000_1.csv", "r");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return 1;
    }
    struct Dados temp;
    char linha[256];
    int total = 0;
    *registro = NULL;

        while (fgets(linha, sizeof(linha), arquivo)!= NULL){
            if(sscanf(linha,"%d;%d;%d;%f;%c", &temp.idUsuario, &temp.timestamp, &temp.br, &temp.km, &temp.tipoAlerta)==5){
            if (temp.km - (int)temp.km < 0.5){
                temp.km = (int)temp.km;
            }else{
                temp.km = (int)temp.km + 0.5;
            }
                *registro = realloc(*registro, (total + 1)* sizeof(struct Dados));
            (*registro)[total] = temp;
            total++;
            
            char nomeArquivo[64];
            sprintf(nomeArquivo, "%d.csv", temp.br);
            FILE *saida = fopen(nomeArquivo, "a");
            if (saida == NULL){
                printf("erro ao criar o arquivo %s\n", nomeArquivo);
                continue;
            }
            fprintf(saida, "%d;%d;%d;%.2f;%c\n", temp.idUsuario, temp.timestamp, temp.br, temp.km, temp.tipoAlerta);
            fclose(saida);
        }
    }
    fclose(arquivo);
    return total;
}

int main(){
    struct Dados *registro = NULL;
    int total = lerArquivo(&registro);
}