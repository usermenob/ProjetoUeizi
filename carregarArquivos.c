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

void criarUpvotes(char *nomeArquivoBrs, struct Dados dado){
    FILE *original = fopen(nomeArquivoBrs, "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if (temporario == NULL){
        printf("Erro");
        if (original) fclose(original);
        return;        
    }

    int encontrado = 0;
    char linha [256];
    int idUsuario, timestamp, br, upvotes;
    float km;
    char tipoAlerta;

    if (original != NULL){
        while (fgets(linha, sizeof(linha), original)!=NULL){
            if (sscanf(linha, "%d;%d;%d;%f;%c;%d", &idUsuario, &timestamp, &br, &km, &tipoAlerta, &upvotes)== 6){
                if (km == dado.km && tipoAlerta == dado.tipoAlerta){
                    upvotes++;
                    encontrado = 1;
                }
                fprintf(temporario, "%d;%d;%d;%.2f;%c;%d\n", idUsuario, timestamp, br, km, tipoAlerta, upvotes);
            } else if (sscanf(linha, "%d;%d;%d;%f;%c", &idUsuario, &timestamp, &br, &km, &tipoAlerta) == 5){
                if (km == dado.km && tipoAlerta == dado.tipoAlerta){
                    upvotes = 2;
                    encontrado = 1;
                } else {
                    upvotes = 1;
                }
                fprintf(temporario, "%d;%d;%d;%.2f;%c;%d\n", idUsuario, timestamp, br, km, tipoAlerta, upvotes);
            }
        }
        fclose(original);
            /* code */
        }
        if (!encontrado){
            fprintf(temporario, "%d;%d;%d;%.2f;%c;%d\n", dado.idUsuario, dado.timestamp, dado.br, dado.km, dado.tipoAlerta, 1);
        }
        fclose(temporario);

        remove(nomeArquivoBrs);
        rename("temporario.csv", nomeArquivoBrs);
}

int lerArquivo(){
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

        while (fgets(linha, sizeof(linha), arquivo)!= NULL){
            if(sscanf(linha,"%d;%d;%d;%f;%c", &temp.idUsuario, &temp.timestamp, &temp.br, &temp.km, &temp.tipoAlerta)==5){
            if (temp.km - (int)temp.km < 0.5){
                temp.km = (int)temp.km;
            }else{
                temp.km = (int)temp.km + 0.5;
            }

            char nomeArquivoBrs[64];
            sprintf(nomeArquivoBrs, "%d.csv", temp.br);
           
            criarUpvotes(nomeArquivoBrs, temp);
        }
    }
    fclose(arquivo);
    return 0;
}

int main(){
    lerArquivo();
    return 0;
}