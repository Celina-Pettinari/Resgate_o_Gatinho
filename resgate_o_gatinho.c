#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

#ifdef _WIN32
    #include <windows.h>
    #define tempo(msegundos) Sleep(msegundos)
    #define strcasecmp _stricmp
#else
    #include <unistd.h>
    #define tempo(msegundos) usleep(msegundos * 1000)

#endif

/*-----------------------------------------------------------------*/

int verifica_entrada(char *entrada);  /* verifica se digitou sim, não ou outra coisa e retorna 1, 2, ou 0 */
void titulo(char *texto, int pausa);  /* limpa tela, add o titulo, coloca texto e caso afirmativo, pausa por 3.5 segundos */
void printa(int x, int y, char tabuleiro[y][x]); /* printa o tabuleiro */
int perdeu(); /* mensagem de perdeu */
int vitoria(); /* mensagem de vitoria */
int procura_x(int x, int y, char tabuleiro[y][x]); /* verifica posição das jogadas*/
int movimento (int x, int y, char tabuleiro[y][x],int gato_x, int gato_y); /* verifica a movimentação possivel*/

/*---------------------------------------------------------------*/

int main(){
    bool continua_geral = true;  /* verifica loop do programa inteiro */
    bool continua_etapa = true; /* continua_geral mais especifico */
    int x, y, temp, temp_temp;  /* plano cartesiano e duas variaveis reutilizaveis (temporarias) */
    char entrada[6], gato;  /* vai ser usado para qualquer entrada do usuari, vai marcar a posição do gato */

    /* Cria o menu iniciar
    volta_1 == goto para erro do primeiro menu
    volta_2 == goto para erro do segundo menu*/

    do {
        srand(time(NULL));

        volta_1:
            memset(entrada, 0, sizeof(entrada));
            
            titulo("\n  Você deseja um tabuleiro quadrado? \n  ", 0);
            scanf("%s", &entrada);
            temp = verifica_entrada(entrada);
            

        volta_2:
            switch(temp) {
                case 1:
                    titulo("\n  Digite um numero impar de colunas para o tamanho: \n  ", 0);
                    scanf("%d", &x);
                    y = x;
                    break;
                
                case 2:
                    titulo("\n  Digite um numero impar de colunas para o tamanho: \n  ", 0);
                    scanf("%d", &x);
                    printf("  Digite um numero impar de linhas para o tamanho: \n  ");
                    scanf("%d", &y);
                    break;
                
                default:
                    titulo("Resposta invalida: digite apenas sim ou não \n", 1);
                    goto volta_1;
            }
            if((x % 2 == 0) || (y % 2 == 0)) {
                    titulo("Numero invalido: precisa ser impar \n  ", 1);
                    goto volta_2;
            }

        /* printa o tabuleiro, verifica vitória ou perda e faz a jogada
        volta_3 == goto para coluna e linha errada */
        
        char tabuleiro[y][x];
        memset(tabuleiro, 0, sizeof(tabuleiro));
        tabuleiro[y / 2] [x / 2] = '*';
        
        do {
            int temp_y = 0, temp_x = 0;
            int gato_x = 0, gato_y = 0;

            titulo("\n", 0);
            printa(x, y, tabuleiro);

            for(temp = 0; temp < y; temp++) for(temp_temp = 0; temp_temp < x; temp_temp++){
                if(tabuleiro[temp][temp_temp] == '*') {
                    gato_x = temp_temp;
                    gato_y = temp;
                }
            }

            if(gato_y == 0 || gato_y == (y - 1) || gato_x == 0 || gato_x == (x - 1)) {
                temp = perdeu();
                if(temp == 2) continua_geral = false;
                break;
            }

            temp = procura_x(x, y, tabuleiro);
            temp_temp = (x * 2) + (y * 2);
            if(temp == temp_temp){
                temp = vitoria();
                if(temp == 2) continua_geral = false;
                break;
            }

            volta_3:
                temp_x = 0;
                temp_y = 0;

                printf("\n\n  Digite a coluna que deseja bloquear: \n  ");
                scanf("%d", &temp_x);
                printf("\n  Digite a linha que deseja bloquear: \n  ");
                scanf("%d", &temp_y);

                if((temp_x > x) || (temp_x <= 0) || (temp_y > y) || (temp_y <= 0)) {
                    printf("\n\n  Resposta invalida: digite um campo existente");
                    goto volta_3;
                }

                if((tabuleiro[(temp_y - 1)][(temp_x - 1)] == '*') || (tabuleiro[(temp_y - 1)][(temp_x - 1)] == 'X')) {
                    printf("\n  Resposta invalida: o campo já está ocupado");
                    goto volta_3;
                }

            tabuleiro[(temp_y - 1)] [(temp_x - 1)] = 'X';

            temp = movimento(x, y, tabuleiro, gato_x, gato_y);
            tabuleiro[gato_y] [gato_x] = ' ';

            if(temp == 1) tabuleiro[gato_y][(gato_x - 1)] = '*';
            else if(temp == 2) tabuleiro[(gato_y - 1)][gato_x] = '*';
            else if(temp == 3) tabuleiro[gato_y][(gato_x + 1)] = '*';
            else if(temp == 4) tabuleiro[(gato_y + 1)][gato_x] = '*';
            else {
                temp = vitoria();
                if(temp == 2) continua_geral = false;
                break;
            }
        }
        while(continua_etapa);
    }
    while(continua_geral);
}



/*---------------------------------------------------------------*/

void titulo(char *texto, int pausa) {
    char tracinho[] = "_-_-_-_-_-_-_-";
    printf("\033[H\033[J");

    switch(pausa) {
        case 1:
            printf("  %s Resgate o Gatinho %s \n\n  %s", tracinho, tracinho, texto);
            fflush(stdout);
            tempo(3000);
            break;

        default:
            printf("  %s Resgate o Gatinho %s \n  %s", tracinho, tracinho, texto);
    }
}



int verifica_entrada(char *entrada) {
    if(strcasecmp(entrada, "s") == 0 || strcasecmp(entrada, "sim") == 0) return 1;
    else if(strcasecmp(entrada, "n") == 0 || strcasecmp(entrada, "nao") == 0 || strcasecmp(entrada, "Não") == 0) return 2;
    else return 0;
}



void printa(int x, int y, char tabuleiro[y][x]) {
        int temp_x = 0, temp_y = 0;
        int controle = 0, temp_controle = 1;
        char temp_char;

    do {
        do {
            if(temp_y == 0) for(; temp_controle <= (x + 1); temp_controle++) {
                if(temp_controle == 1) printf("      %3d ", temp_controle);
                else if(temp_controle <= x) printf("%3d ", temp_controle);
                else printf("\n");                
            }

            if((temp_x == 0)) printf("%3d ", (temp_y + 1));

            if(controle == 0) printf("%3s", " |");
            else if(controle < x) printf("%1s", "|");
            else {
                printf("%3s", "|  \n");
                controle = -1;
            }
            
                if(temp_x < x){
                    temp_char = tabuleiro[temp_y][temp_x];
                    if(temp_char == '*') printf("%1s", " * ");
                    else if(temp_char == 'X') printf("%1s", " X ");
                    else printf("%3s", "  ");
                }

            controle++;
            temp_x++;
        }
        while(temp_x < (x + 1));
        temp_y++;
        temp_x = 0;
    } 
    while(temp_y < y);
}



int perdeu() {
    char entrada[6];
    int temp;

    volta_perdeu:
        memset(entrada, 0, sizeof(entrada));
        titulo("\n  Você perdeu: o gato escapou  \n\n  Deseja jogar novamente? \n  ", 0);
        scanf("%s", &entrada);
        temp = verifica_entrada(entrada);

            switch(temp) {
                case 1:
                    return 1;
                    break;
                
                case 2:
                    return 2;
                    break;
                
                default:
                    titulo("Resposta invalida: digite apenas sim ou não \n", 1);
                    goto volta_perdeu;
            }
}



int vitoria() {
    char entrada[6];
    int temp;

    volta:
        memset(entrada, 0, sizeof(entrada));
        titulo("\n  Você resgatou o gato  \n\n  Deseja jogar novamente? \n  ", 0);
        scanf("%s", entrada);
        temp = verifica_entrada(entrada);

            switch(temp) {
                case 1:
                    return 1;
                    break;
                
                case 2:
                    return 2;
                    break;
                
                default:
                    titulo("Resposta invalida: digite apenas sim ou não \n", 1);
                    goto volta;
            }
}



int procura_x(int x, int y, char tabuleiro[y][x]) {
    int  verifica = 0;
    char procura;
    
    for(int temp_y = 0; temp_y < y; temp_y++) {
        if(tabuleiro[temp_y][0] == 'X') verifica++;
        if(tabuleiro[temp_y][x - 1] == 'X') verifica++;
    }

    for(int temp_x = 0; temp_x < x; temp_x++) {
        if(tabuleiro[0][temp_x] == 'X') verifica++;
        if(tabuleiro[(y - 1)][temp_x] == 'X') verifica++;
    }

    return verifica;
}



int movimento (int x, int y, char tabuleiro[y][x],int gato_x, int gato_y) {
    int esquerda = 0, direita = 0, cima = 0, baixo = 0;
    int temp_x = 0, temp_y = 0, temp = 0;
    char bloqueio;

    for(; temp_x < gato_x; temp_x++) if(tabuleiro[gato_y][temp_x] != 'X') esquerda++;
    for(; temp_y < gato_y; temp_y++) if(tabuleiro[temp_y][gato_x] != 'X') cima++;
    
    temp_x += 1;
    temp_y += 1;
    for(; ((temp_x > gato_x) && (temp_x < x)); temp_x++) if(tabuleiro[gato_y][temp_x] != 'X') direita++;
    for(; ((temp_y > gato_y) && (temp_y < y)); temp_y++) if(tabuleiro[temp_y][gato_x] != 'X') baixo++;

    if((esquerda == 0) && (cima == 0) && (direita == 0) && (baixo == 0)) return 0;

    if((esquerda == cima) && (cima == direita) && (direita == baixo)) {
        temp = rand() % 100;
        if(temp <= 25) esquerda++;
        else if(temp <= 50) cima++;
        else if(temp <= 75) direita++;
        else baixo++;
    }

    temp = 0;

    if(temp <= esquerda) temp = esquerda;
    if(temp <= cima) temp = cima;
    if(temp <= direita) temp = direita;
    if(temp <= baixo) temp = baixo;

    if(temp == esquerda) esquerda++;
    else if(temp == cima) cima++;
    else if(temp == direita) direita++;
    else if(temp == baixo) baixo++;

    if(esquerda > cima && esquerda > direita && esquerda > baixo && tabuleiro[gato_y][(gato_x - 1)] != 'X') return 1;
    else if(cima > esquerda && cima > direita && cima > baixo && tabuleiro[(gato_y - 1)][gato_x] != 'X') return 2;
    else if(direita > cima && direita > esquerda && direita > baixo && tabuleiro[gato_y][(gato_x + 1)] != 'X') return 3;
    else if(baixo > cima && baixo > direita && baixo > esquerda && tabuleiro[(gato_y + 1)][gato_x] != 'X') return 4;
}