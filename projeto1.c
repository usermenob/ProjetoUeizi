#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

struct Dados {
    int idUsuario;
    int timestamp;
    int br;
    float km;
    char tipoAlerta;
};

struct Alerta {
        int idUsuario;
        int timestamp;
        int br;
        float km;
        char tipoAlerta;
        int upvotes;
        struct Alerta *prox;
    };

void criarUpvotes(char *nomeArquivoBrs, struct Dados dado) {
    struct Alerta *inicio = NULL;
    struct Alerta *atual = NULL;
    struct Alerta *ultimo = NULL;

    FILE *arquivo = fopen(nomeArquivoBrs, "r");

    if (arquivo == NULL) {
        FILE *novo = fopen(nomeArquivoBrs, "w");
        if (novo == NULL) {
            printf("erro ao criar o arquivo %s\n", nomeArquivoBrs);
            return;
        }
        fprintf(novo, "%d;%d;%d;%.2f;%c;%d\n", dado.idUsuario, dado.timestamp, dado.br, dado.km, dado.tipoAlerta, 1);
        fclose(novo);
        return;
    }

    char linha[256];
    int idUsuario, timestamp, br, upvotes;
    float km;
    char tipoAlerta;

    while (fgets(linha, sizeof(linha), arquivo) != NULL){
        struct Alerta *novo = malloc(sizeof(struct Alerta));
        if (novo == NULL){
            printf("erro de memoria\n");
            fclose(arquivo);
            return;
        }

        if (sscanf(linha, "%d;%d;%d;%f;%c;%d", &idUsuario, &timestamp, &br, &km, &tipoAlerta, &upvotes) == 6){
            novo -> idUsuario = idUsuario;
            novo -> timestamp = timestamp;
            novo -> br = br;
            novo -> km = km;
            novo -> tipoAlerta = tipoAlerta;
            novo -> upvotes = upvotes;
        } else if (sscanf(linha, "%d;%d;%d;%f;%c", &idUsuario, &timestamp, &br, &km, &tipoAlerta)== 5){
            novo -> idUsuario = idUsuario;
            novo -> timestamp = timestamp;
            novo -> br = br;
            novo -> km = km;
            novo -> tipoAlerta = tipoAlerta;
            novo -> upvotes = 1;
        } else {
            free(novo);
            continue;
        }
        novo->prox = NULL;
        if (inicio == NULL)
            inicio = novo;
        else
            ultimo->prox = novo;
        ultimo = novo;
    }
    fclose(arquivo);

    int encontrado = 0;
    atual = inicio;
    while (atual != NULL) {
        if (atual->km == dado.km && atual->tipoAlerta == dado.tipoAlerta) {
            atual->upvotes++;
            encontrado = 1;
            break;
        }
        atual = atual->prox;
    }

    if (!encontrado) {
        struct Alerta *novo = malloc(sizeof(struct Alerta));
        if (novo == NULL) {
            printf("erro de memória\n");
            return;
        }
        novo->idUsuario = dado.idUsuario;
        novo->timestamp = dado.timestamp;
        novo->br = dado.br;
        novo->km = dado.km;
        novo->tipoAlerta = dado.tipoAlerta;
        novo->upvotes = 1;
        novo->prox = NULL;

        if (inicio == NULL)
            inicio = novo;
        else
            ultimo->prox = novo;
    }

    FILE *saida = fopen(nomeArquivoBrs, "w");
    if (saida == NULL) {
        printf("erro ao reabrir o arquivo %s\n", nomeArquivoBrs);
        return;
    }

    atual = inicio;
    while (atual != NULL) {
        fprintf(saida, "%d;%d;%d;%.2f;%c;%d\n", atual->idUsuario, atual->timestamp, atual->br, atual->km, atual->tipoAlerta, atual->upvotes);
        atual = atual->prox;
    }

    fclose(saida);

    while (inicio != NULL) {
        struct Alerta *tmp = inicio;
        inicio = inicio->prox;
        free(tmp);
    }
}

int lerArquivo() {
    char nomeDoArquivo[100];

    printf("digite o nome do arquivo:\n");
    scanf("%99s", nomeDoArquivo);

    FILE *arquivo = fopen(nomeDoArquivo, "r");
    if (arquivo == NULL) {
        printf("erro ao abrir o arquivo %s\n", nomeDoArquivo);
        return 1;
    }

    struct Dados temp;
    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%d;%d;%d;%f;%c", &temp.idUsuario, &temp.timestamp, &temp.br, &temp.km, &temp.tipoAlerta) == 5) {

            if (temp.km - (int)temp.km < 0.5)
                temp.km = (int)temp.km;
            else
                temp.km = (int)temp.km + 0.5;

            char nomeArquivoBrs[64];
            sprintf(nomeArquivoBrs, "%d.csv", temp.br);

            criarUpvotes(nomeArquivoBrs, temp);
        }
    }

    fclose(arquivo);
    printf("sucesso\n");
    return 0;
}

void somaAlertas(const char *nomeArquivoBrs, FILE *saida) {
    FILE *arquivo = fopen(nomeArquivoBrs, "r");
    if (arquivo == NULL) {
        printf("erro ao abrir o arquivo %s\n", nomeArquivoBrs);
        return;
    }

    int alertaA = 0, alertaB = 0, alertaC = 0, alertaD = 0, alertaE = 0, alertaF = 0;

    char linha[256];
    int idUsuario, timestamp, br, upvotes;
    float km;
    char tipoAlerta;

    fgets(linha, sizeof(linha), arquivo);
    if (strstr(linha, "idUsuario") == NULL && strstr(linha, "br;") == NULL) {
        fseek(arquivo, 0, SEEK_SET);
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d;%d;%d;%f;%c;%d", &idUsuario, &timestamp, &br, &km, &tipoAlerta, &upvotes) == 6) {
            switch (tipoAlerta) {
                case 'A': alertaA++; break;
                case 'B': alertaB++; break;
                case 'C': alertaC++; break;
                case 'D': alertaD++; break;
                case 'E': alertaE++; break;
                case 'F': alertaF++; break;
            }
        }
    }

    fclose(arquivo);

    char brStr[50];
    strcpy(brStr, nomeArquivoBrs);
    char *ponto = strchr(brStr, '.');
    if (ponto) *ponto = '\0';

    if (alertaA > 0) fprintf(saida, "%s;A;%d\n", brStr, alertaA);
    if (alertaB > 0) fprintf(saida, "%s;B;%d\n", brStr, alertaB);
    if (alertaC > 0) fprintf(saida, "%s;C;%d\n", brStr, alertaC);
    if (alertaD > 0) fprintf(saida, "%s;D;%d\n", brStr, alertaD);
    if (alertaE > 0) fprintf(saida, "%s;E;%d\n", brStr, alertaE);
    if (alertaF > 0) fprintf(saida, "%s;F;%d\n", brStr, alertaF);
}


void gerarRelatorio() {
    DIR *dir;
    struct dirent *ent;
    FILE *saida = fopen("alertas_brs_todas.csv", "w");

    if (saida == NULL) {
        printf("erro ao criar o arquivo\n");
        return;
    }

    fprintf(saida, "br;tipoAlerta;quantidade\n");

    dir = opendir(".");
    if (dir == NULL) {
        printf("Erro\n");
        fclose(saida);
        return;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".csv") && isdigit(ent->d_name[0])) {
            somaAlertas(ent->d_name, saida);
        }
    }

    closedir(dir);
    fclose(saida);

    printf("\n'alertas_brs_todas.csv' gerado\n");
}

void gerarRelatorioTrecho() {
    int br;
    float kmInicio, kmFim;

    printf("digite o numero da BR: ");
    scanf("%d", &br);
    printf("digite o km inicial: ");
    scanf("%f", &kmInicio);
    printf("digite o km final: ");
    scanf("%f", &kmFim);

    char nomeArquivoBrs[64];
    sprintf(nomeArquivoBrs, "%d.csv", br);

    FILE *arquivoBr = fopen(nomeArquivoBrs, "r");
    if (arquivoBr == NULL) {
        printf("erro arquivo da BR não encontrado.\n");
        return;
    }

    FILE *saida = fopen("alertas_por_br.csv", "w");
    if (saida == NULL) {
        printf("erro ao criar o arquivo.\n");
        fclose(arquivoBr);
        return;
    }

    fprintf(saida, "br;km;tipoAlerta;upvotes\n");

    char linha[256];
    int idUsuario, timestamp, brLido, upvotes;
    float km;
    char tipoAlerta;

    while (fgets(linha, sizeof(linha), arquivoBr) != NULL) {
        if (sscanf(linha, "%d;%d;%d;%f;%c;%d", &idUsuario, &timestamp, &brLido, &km, &tipoAlerta, &upvotes) == 6) {
            if (km >= kmInicio && km <= kmFim) {
                fprintf(saida, "%d;%.1f;%c;%d\n", brLido, km, tipoAlerta, upvotes);
            }
        }
    }

    fclose(arquivoBr);
    fclose(saida);

    printf("\n 'alertas_por_br.csv' gerado\n");
}

int main() {
    int opcao;

    do {
        printf("1 - inserir arquivo de alertas\n");
        printf("2 - gerar relatorio de alertas das BRs\n");
        printf("3 - Gerar relatorio de trecho de BR\n");
        printf("0 - sair\n");
        printf("escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                lerArquivo();
                break;
            case 2:
                gerarRelatorio();
                break;
            case 3:
                gerarRelatorioTrecho();
                break;
            case 0:
                break;
            default:
                printf("opcao invalida\n");
        }
    } while (opcao != 0);

    return 0;
}