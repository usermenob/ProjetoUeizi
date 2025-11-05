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
    char nomeDoArquivo[100];

    printf("Digite o nome do arquivo que vai ser enviado:\n");
    scanf("%99s", nomeDoArquivo);

    FILE *arquivo = fopen(nomeDoArquivo, "r");
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
            
            char nomeArquivoBrs[64];
            sprintf(nomeArquivoBrs, "%d.csv", temp.br);
            FILE *saida = fopen(nomeArquivoBrs, "a");
            if (saida == NULL){
                printf("erro ao criar o arquivo %s\n", nomeArquivoBrs);
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
    free(registro);
    return 0;
}