#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Alerta{
    int br;
    double km;
    char tipo;
    int upvotes;
    struct Alerta *proximo;
}Alerta;

double trecho500(double km){
    int inteiro = (int)km;
    double trecho = km - inteiro;
    if (trecho < 0.5){
        return (double)inteiro;
    }else{
        return (double)inteiro + 0.5;
    }
}

Alerta* find_node(Alerta *inicio, int br, double km, char tipo){
    for (Alerta *p = inicio; p != NULL; p = p-> proximo){
        if (p->br == br && p-> km == km && p->tipo == tipo){
            return p;
        }
    }
    return NULL;
}

static Alerta* push_front(Alerta *inicio, int br, double km, char tipo){
    Alerta *n = malloc(sizeof *n);
    if (!n){
        return inicio;
    }
    n-> br = br; n-> km = km; n-> tipo = tipo; n->upvotes = 1;
    n->proximo = inicio;
    return n;
}

static Alerta* push_front_u(Alerta *inicio, int br, double km, char tipo, int up){
    Alerta *n = (Alerta*)malloc(sizeof *n);
    if(!n) return inicio;
    n->br = br; n->km = km; n->tipo = tipo; n->upvotes = up;
    n->proximo = inicio;
    return n;
}

int carregarConsolidado(char *caminho, Alerta **inicio){
    FILE *f = fopen(caminho,"r");
    if (!f) return 0;

    char linha[256];
    int linhas = 0;

    while(fgets(linha, sizeof linha, f)){
        if (linhas == 0 && strncmp(linha,"br;", 3) == 0){
            linhas++;
            continue;
        }
        int br, up;
        double km;
        char tipo;

        if (sscanf(linha, " %d ; %lf ; %c ; %d ", &br, &km, &tipo, &up) != 4)
            continue; 
        
        Alerta *n = find_node(*inicio, br, km, tipo);
        if (n) n->upvotes += up;
        else *inicio = push_front_u(*inicio, br, km, tipo, up);
        linhas++;
    }
    fclose(f);
    return linhas;
}

int salvarConsolidado(char *caminho, Alerta *inicio){
    FILE *f = fopen(caminho, "w");
    if (!f){
        perror("fopen");
        return 0;
    }

    fprintf(f, "br;km;tipo;upvotes\n");
    int linhas = 0;
    for (Alerta *p = inicio; p; p = p->proximo){
        fprintf(f,"%d;%.1f;%c;%d\n", p->br, p->km, p->tipo, p->upvotes);
        linhas++;
    }
    fclose(f);
    return linhas;
}

int carregarArquivo(Alerta **inicio){
    char caminho[260];
    printf("Caminho do CSV: ");
    if (scanf(" %259[^\n]", caminho) != 1){
        printf("Entrada inválida");
        return 0;
    }

    FILE *f = fopen(caminho, "r");
    if (!f){
        perror("fopen");
        return 0;
    }

    int linhas = 0;
    int id, ts, br;
    double km;
    char tipo;

    int n;
    while((n = fscanf(f, " %d ; %d ; %d ; %lf ; %c ", &id, &ts, &br, &km, &tipo)) != EOF){
        if (n == 5){
            double trecho = trecho500(km);
            Alerta *novo = find_node(*inicio, br, trecho, tipo);
            if (novo){
                novo-> upvotes += 1;
            }else{
                *inicio = push_front(*inicio, br, trecho, tipo);
            }
            linhas++;
        }else{
            fscanf(f, "%*[^\n]");
            int c = fgetc(f); (void)c;
        }
    }
    fclose(f);
    return linhas;
}

void liberarLista(Alerta **inicio){
    while(*inicio){
        Alerta *t = (*inicio)->proximo;
        free(*inicio);
        *inicio = t;
    }
}

int main(void){
    Alerta *inicio = NULL;
    int rec = carregarConsolidado("consolidado.csv", &inicio);
    if (rec > 0){
        printf("Carregados %d registros do consolidado.csv\n", rec);
    }

    int N = 0;
    printf("Quantos CSVs brutos deseja importar agora? ");
    if (scanf(" %d", &N) != 1) N = 0;

    for (int i = 0; i < N; i++){
        int lidas = carregarArquivo(&inicio);
        printf(" -> CSV %d: %d linhas validas\n", i+1, lidas);
    }

    int gravar = salvarConsolidado("consolidado.csv", inicio);
    printf("Gravados %d registros em consolidado.csv\n", gravar);

    liberarLista(&inicio);
    return 0;
}



