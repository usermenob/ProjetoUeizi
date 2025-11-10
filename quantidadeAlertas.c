#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h> 

void somaAlertas(const char *nomeArquivoBrs, FILE *saida) {
    char caminho[256];
    sprintf(caminho, "output/%s", nomeArquivoBrs);
    FILE *arquivo = fopen(caminho, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivoBrs);
        return;
    }

    int alertaA = 0, alertaB = 0, alertaC = 0, alertaD = 0, alertaE = 0, alertaF = 0;
    char linha[1024];

    while (fgets(linha, sizeof(linha), arquivo)) {
        for (int i = 0; linha[i] != '\0'; i++) {
            switch (linha[i]) {
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

    char br[50];
    strcpy(br, nomeArquivoBrs);
    char *ponto = strchr(br, '.');
    if (ponto) *ponto = '\0';

    if (strlen(br) == 0)
        strcpy(br, "desconhecida");

    if (alertaA > 0) fprintf(saida, "%s;A;%d\n", br, alertaA);
    if (alertaB > 0) fprintf(saida, "%s;B;%d\n", br, alertaB);
    if (alertaC > 0) fprintf(saida, "%s;C;%d\n", br, alertaC);
    if (alertaD > 0) fprintf(saida, "%s;D;%d\n", br, alertaD);
    if (alertaE > 0) fprintf(saida, "%s;E;%d\n", br, alertaE);
    if (alertaF > 0) fprintf(saida, "%s;F;%d\n", br, alertaF);
}

int main() {
    DIR *dir;
    struct dirent *ent;
    FILE *saida = fopen("alertas_brs_todas.csv", "w");

    if (saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return 1;
    }

    fprintf(saida, "br;tipoAlerta;quantidade\n");

    dir = opendir("output");

    if (dir == NULL) {
        printf("Erro ao abrir o diretório.\n");
        fclose(saida);
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".csv") && isdigit(ent->d_name[0])) {
        somaAlertas(ent->d_name, saida);
        }

}

    closedir(dir);
    fclose(saida);

    return 0;
}
