/*
Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programacao de Computadores -
1/2019
Aluno(a): Luiza Nogueira Costa
Matricula: 190017074
Turma: A
Versao do compilador: GCC C99
Descricao: Trabalho final de APC - Parte 02 - Jogo 2D similar a Puzzle Bobble.
*/

//Bibliotecas:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#ifndef _WIN32
#include <termios.h>
#include <string.h>

int kbhit(){
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO,&oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF){
        ungetc(ch,stdin);
        return 1;
    }
return 0;
}
    int getch(void) {
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO,&oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#else
    #include <conio.h>
#endif

//Definicoes:
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define PINK "\033[38;5;206m"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

//Variaveis globais:
int altura=9, largura=16;
int velocidade=60; //Nota: por motivos esteticos, algumas das velocidades no usleep() foram alteradas.
double coefmin = -5.9;
double coefmax = 5.9;
char tabuleiro[9][16] = {
    "################",
    "#              #",
    "#              #",
    "#              #",
    "#              #",
    "#              #",
    "#              #",
    "#              #",
    "################"};
int conect[9][16];
char tabuleirocopy[9][16];
char p; //char para a peca a ser disparada
char filedereplay[20];
char filederanking[20];
FILE* freplay;
FILE* frank;
FILE* config;

typedef struct{
    int pontuacao;
    int velocidade;
} Cof; //Struct das configuracoes;

Cof configuracoes;

typedef struct {
char nick[11];
int score;

} Player; //Struct do jogador;

//Declaracao das funcoes:
int welcome();
int menuMain();
int line (double coef, int x);
char criachar();
int pop(char p, int x, int y, int cont, int pecas);
void jogar();
int instrucoes();
int settings();
int ranking();
int sair();
void gameover();
void tabuleiroreset();
void printtabuleiro(int pontos);
void drop(int pontos);
double max (double coef);
double min (double coef);
void move(double prevcoef, double coef);
void resetline();
int movedown();
void clean(int pontos);
void name(int rankeado);
int replay();
void registerMatch(Player jogador);

//Funcoes:
void tabuleiroreset(){
    //Reseta o tabuleiro para sua forma inicial;
    int i, j;//Variaveis auxiliares;

    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++){
            tabuleiro[i][j] = ' ';
        }
    }

    for (j = 0; j < largura; j++){
        tabuleiro[0][j] = '#';
        tabuleiro[altura - 1][j] = '#';
    }

    for (i = 0; i < altura; i++){
        tabuleiro[i][0] = '#';
        tabuleiro[i][largura - 1] = '#';
    }

}

void gameover(){
    //Apresenta a tela de fim de jogo e retorna jogador ao menu principal;
    //Variaveis auxiliares;
    int enter;
    int i, j;
    if (freplay != NULL){
        fclose(freplay);
    }
    if (config != NULL){
        fclose(config);
    }


    system(CLEAR);
    tabuleiroreset();
    printf(PINK "\n###############\n");
    printf("##GAME  OVER!##\n");
    printf("###############\n");
    printf("\n");
    printf("Aperte ENTER para encerrar o jogo \n" RESET);
    getchar();
    while(1){

        if (getchar() == '\n'){
            menuMain();//Retorna ao menu principal;
        }

        else {
            printf(PINK "Aperte ENTER para encerrar o jogo \n" RESET);
        }
    }

}

int welcome(){
    //Pagina de abertura do jogo;
    int enter;//Variavel auxiliar;
    system(CLEAR);
    printf(PINK "\n###############\n");
    printf("#Puzzle Bobble#\n");
    printf("###############\n");
    printf("\n");
    printf("Aperte ENTER para iniciar o jogo \n" RESET);

    while(1){
        enter = getchar();
        if (enter == '\n'){
            menuMain(0); //Abre o menu principal;
            return 0;
        }
        else {
            printf(PINK "Aperte ENTER para iniciar o jogo \n" RESET);
        }
    }
    return 0;
}

int menuMain(int rankeado){
    //Menu principal;
    int select = 0;//Variavel de selecao;

    system(CLEAR);

    tabuleiroreset();//Reseta o tabuleiro;

    while (select != 5){
        printf(PINK "1 - Jogar\n");
        printf("2 - Instrucoes\n");
        printf("3 - Configuracoes\n");
        printf("4 - Ranking\n");
        printf("5 - Sair\n");
        printf("Digite o numero correspondente a opcao desejada: " RESET);

        scanf("%d", &select);//Scan da opcao escolhida;

        switch (select) {//Abre uma certa pagina dependendo da selecao do jogador;
            case 1:
                if (rankeado == 1){
                    name(1);
                }
                else {
                    replay();
                }
            break;
            case 3: settings(); break;
            case 2: instrucoes(); break;
            case 4: ranking(); break;
            case 5: sair(); break;
            default:
            exit(EXIT_FAILURE);
        }

        getch();
    }

    return 0;
}
void name(int rankeado){
    //Quando em modo rankeado, le o nome do jogador;
    Player jogador;
    system(CLEAR);
    printf(PINK"Digite seu nickname:\n");
    getchar();
    scanf("%s", jogador.nick);

    while (strlen(jogador.nick) < 1 || strlen(jogador.nick) > 10){
        printf(PINK"Digite seu nickname:\n");
        scanf("%s", jogador.nick);//Scanf do nick na struct;
    }

    jogar(0, rankeado, jogador);

}
int replay(){
    //Abre menu para criacao de replay
    int enter;
    int replay = 0;//Flag de replay ativado(1)
    system(CLEAR);
    printf(PINK "1 - Criar arquivo de replay\n");
    printf("2 - Usar arquivo de Replay\n");
    printf("3 - Jogar sem replay\n");
    printf("Aperte ENTER para retornar ao menu do jogo\n" RESET);
    getchar();
    while(1){
        enter = getchar();
        if (enter == '\n'){
            menuMain(0); //Abre o menu principal;
            return 0;
        }
        else if (enter == '1'){
            //Cria um novo arquivo de replay;
            printf(PINK "Digite um nome para seu arquivo de replay (inclua o .txt)\n" RESET);
            scanf("%s", filedereplay);
            freplay = fopen(filedereplay, "w");//Cria o arquivo de replay;
            replay = 1;//Flag do tipo de replay;
            jogar(replay, 0);//Executa o jogo;
            return 0;
        }
        else if (enter == '2'){
            //Abre um arquivo de replay;
            printf(PINK "Digite o nome para do arquivo de replay que deseja abrir (inclua o .txt)\n" RESET);
            scanf("%s", filedereplay);

            freplay = fopen(filedereplay, "r");//Abre arquivo para leitura.
            replay = 2;//Flag do tipo de replay;

            if (freplay == NULL){
                printf(PINK "ERRO - Arquivo nao encontrado. Sera criado um novo arquivo\n" RESET);
                freplay = fopen(filedereplay, "w");//Se abrir o arquivo gera erro, cria um arquivo para leitura de mesmo nome;
                jogar(replay, 0);
            }
            else {
                rewind(freplay);//Retorna o arquivo de replay ao seu comeco;
                jogar(replay, 0);//Executa o jogo;
            }
            return 0;
        }
        else if (enter == '3'){
            jogar(0,0); //Abre o jogo sem ativar o replay;
            return 0;
        }
        else {
            printf(PINK "Escolha uma das opcoes acima\n" RESET);
        }
    }


}

int line (double coef, int x){
    //Calcula a reta que compoe a mira;
    int y;
    y = coef*x + 8; //Equacao da reta;

    if(y<14 && y>1){
        return y;
    }
    else if(y >= 14){
        return 14;
    }
    else {
        return 1;
    }
}

char criachar(){
    //Cria aleatoriamente uma letra;
    char p;
    p = 'A' + rand() % 5;
    return p;
}

int pop(char p, int x, int y, int cont, int pecas){
    //Determina se existem letras iguais adjacentes a letra que foi lancada;
    system(CLEAR);

    conect[x][y] = 1;//Marca que a letra lancada eh igual a si mesma;
    int i, j;//Variaveis auxiliares;

    /*
    Cada condicional if verifica se existe uma peca igual em cada uma das 8 direcoes possiveis, adajcentes a peca que foi lancada.
    Se sim, essa peca eh marcada com 1 no tabuleiro auxiliar conect, o contador cont recebe +1, e a funcao pop eh chamada
    novamente, dessa vez com as coordenadas x e y da peca adjacente. Se a peca adjacente ja foi marcada com 1 no tabuleiro conect,
    ela nao eh verificada novamente (previne um loop infinito).
    Quando nao houverem pecas adjacentes e iguais, a funcao retorna o valor de cont para a funcao jogar, e o tabuleiro auxiliar conect mapeia as pecas a serem estouradas.
    */

    if(tabuleiro[x - 1][y] == p && (conect[x - 1][y] == 0) && tabuleiro[x + 1][y] == p && (conect[x+1][y] == 0)){
        conect[x+1][y] = 1;
        conect[x - 1][y] = 1;
        x = x+1;
        cont++;
        pop(p, x, y, cont, pecas);
        x = x-2;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x + 1][y] == p && (conect[x+1][y] == 0)){
        conect[x+1][y] = 1;
        x = x+1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x - 1][y] == p && (conect[x - 1][y] == 0)){
        conect[x - 1][y] = 1;
        x = x - 1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    if(tabuleiro[x][y - 1] == p && (conect[x][y - 1] == 0) && tabuleiro[x][y + 1] == p && (conect[x][y + 1] == 0)){
        conect[x][y + 1] = 1;
        conect[x][y - 1] = 1;
        y = y+1;
        cont++;
        pop(p, x, y, cont, pecas);
        y = y - 2;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x][y + 1] == p && (conect[x][y + 1] == 0)){
        conect[x][y + 1] = 1;
        y = y+1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x][y - 1] == p && (conect[x][y - 1] == 0)){
        conect[x][y - 1] = 1;
        y = y-1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    if(tabuleiro[x + 1][y + 1] == p && (conect[x + 1][y + 1] == 0) && tabuleiro[x + 1][y - 1] == p && (conect[x + 1][y - 1] == 0)){
        conect[x + 1][y + 1] = 1;
        conect[x + 1][y - 1] = 1;
        x = x+1;
        y = y+1;
        cont++;
        pop(p, x, y, cont, pecas);
        y = y - 2;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x + 1][y + 1] == p && (conect[x + 1][y + 1] == 0)){
        conect[x + 1][y + 1] = 1;
        x = x+1;
        y = y+1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x + 1][y - 1] == p && (conect[x + 1][y - 1] == 0)){
        conect[x + 1][y - 1] = 1;
        x = x+1;
        y = y-1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    if (tabuleiro[x - 1][y + 1] == p && (conect[x - 1][y + 1] == 0) && tabuleiro[x - 1][y - 1] == p && (conect[x - 1][y - 1] == 0)){
        conect[x - 1][y + 1] = 1;
        conect[x - 1][y - 1] = 1;
        x = x - 1;
        y = y + 1;
        cont++;
        pop(p, x, y, cont, pecas);
        y = y - 2;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x - 1][y + 1] == p && (conect[x - 1][y + 1] == 0)){
        conect[x - 1][y + 1] = 1;
        x = x - 1;
        y = y + 1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    else if(tabuleiro[x - 1][y - 1] == p && (conect[x - 1][y - 1] == 0)){
        conect[x - 1][y - 1] = 1;
        x = x-1;
        y = y-1;
        cont++;
        pop(p, x, y, cont, pecas);
    }

    if (cont > pecas - 2){//Substitui as pecas que foram marcadas com 1 no tabuleiro conect por *
        for (i = 0; i < altura; i++){
            for (j = 0; j <largura; j++){
                if(conect[i][j] == 1){
                    tabuleiro[i][j] = '*';
                }
            }
        }
        return cont;
    }

    return 0;

}

void printtabuleiro(int pontos){

    //Imprime o tabuleiro de jogo;
    int i, j;//Variaveis auxiliares;

    system(CLEAR);
    printf(CYAN "Pontos :" RED " %d\n\n\n" RESET, pontos);

    for (i = altura; i >= 0; i--){
        for (j = 0; j < largura; j++){
            if (tabuleiro[i][j] != '-' && tabuleiro[i][j] != ' ' && tabuleiro[i][j] != '#'){
                if(tabuleiro[i][j] == 'A'){
                    printf(RED "A" RESET);
                }
                else if(tabuleiro[i][j] == 'B'){
                    printf(YELLOW "B" RESET);
                }
                else if(tabuleiro[i][j] == 'C'){
                    printf(GREEN "C" RESET);
                }
                else if(tabuleiro[i][j] == 'D'){
                    printf(MAGENTA "D" RESET);
                }
                else if(tabuleiro[i][j] == 'E'){
                    printf(CYAN "E" RESET);
                }
                else if(tabuleiro[i][j] == '*'){
                    printf(PINK "*" RESET);
                }

            }
            else{
                printf("%c", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }

}
void drop(int pontos){
    //Estoura pecas que estejam flutuando no tabuleiro;
    int i, j; //Variaveis auxiliares;

    for (i = altura; i > 0; i--){
        for (j = 0; j < largura; j++){
            if (tabuleiro[i+1][j-1] == ' '  && tabuleiro[i+1][j] == ' '  && tabuleiro[i+1][j+1] == ' '  && tabuleiro[i][j] != '-' && tabuleiro[i][j] != ' ' && tabuleiro[i][j] != '#'){
                tabuleiro[i][j] = '*';
            }
        }
    }

    printtabuleiro(pontos);

    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++){
            if (tabuleiro[i][j] == '*'){
                tabuleiro[i][j] == ' ';
            }
        }
    }
    printtabuleiro(pontos);
    clean(pontos);

}

void clean(int pontos){
    //Estoura pecas que estejam flutuando no tabuleiro, evitando que alguma peca permaneca flutuante como um '*' apos ser estourada;
    int i, j; //Variaveis auxiliares;

    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++){
            if (tabuleiro[i][j] == '*'){
                tabuleiro[i][j] = ' ';
            }
        }
    }
    printtabuleiro(pontos);

}

double max (double coef){
    //Retorna o maximo de dois coeficientes angulares;
    if (coef > coefmax){
        return coef;
    }
    else{
        return coefmax;
    }
}

double min (double coef){
    //Retorna o minimo de dois coeficientes angulares;
    if (coef < coefmin){
        return coef;
    }
    else{
        return coefmin;
    }
}

void move(double prevcoef, double coef){
    //Calcula a mira;
    int i, j; //Variaveis auxiliares;
    int y; //Coordenada;

    for (i = altura - 2; i > 1; i--){
        y = line(coef, i);
        if (y == 1 || y == 14){ //Estabelce as bordas do tabuleiro;
            for (j = 0; j < altura; j++){
                tabuleiro[i][15] = '#';
                tabuleiro[i][0] = '#';
            }
        }
        if(tabuleiro[i][line(prevcoef, i)] == '-'){ //Remove a linha de mira anterior;
            if(tabuleiro[i][line(prevcoef, i)] == '-' || tabuleiro[i][line(prevcoef, i)] == ' ' || tabuleiro[i][line(prevcoef, i)] == '#'){
                tabuleiro[i][line(prevcoef, i)] = ' ';
            }

        }
        if (tabuleiro[i][line(coef, i)] == ' '){ //Determina a nova mira;
            tabuleiro[i][line(coef, i)] = '-';
        }
    }

}
void resetline(){

    //Reseta a mira apos o tabuleiro diminuir em tamanho;
    int i, j; //Variaveis auxiliares;

    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++){
            if (tabuleiro[i][j] == '-'){
                tabuleiro[i][j] = ' ';
            }
        }
    }

}
int movedown(){

    //Reduz o tamanho do tabuleiro, movendo as pecas uma casa para baixo;
    int i, j; //Variaveis auxiliares;

    for (i = 0; i < altura; i++){
        for(j = 0; j < largura; j++){
            tabuleirocopy[i][j] = tabuleiro[i][j]; //Copia os valores do tabuleiro para um tabuleiro auxiliar;
        }
    }

    for (i = altura - 2; i > 1; i--){
        for(j = 0; j < largura; j++){
            tabuleiro[i][j] = tabuleirocopy[i +1][j]; //Cada peca recebe o conteudo da peca acima dela no tabuleiro auxiliar;
        }
    }

    return 0;
}
void registerMatch(Player jogador){
    //Registra a partida rankeada no ranking;
    int i, j, aul;
    char name[11];
    Player jogadores[11], aux;//Variaveis auxiliares;
    char file[] = "ranking.bin";
    frank = fopen(file, "rb");//Abre o arquivo de ranking;

    if (frank == NULL){//Se arquivo de ranking nao existe...
        for(i = 1; i < 11; i++){
            strcpy(jogadores[i].nick," \0");//Completa o vetor do ranking com os vazios;
            jogadores[i].score = 0;//Coloca os rankings vazios com pontuacao zero;
        }
        frank = fopen(file, "wb");//Cria arquivo de ranking;
        fwrite(&jogador, sizeof(Player), 1, frank);//Escreve as informacoes da partida jogada em primeira posicao do ranking;
        for(i = 1; i < 10; i++){
            fwrite(&jogadores[i], sizeof(Player), 1, frank); //Grava o resto das posicoes no arquivo de ranking;

        }
        fclose(frank); //Fecha arquivo de ranking;

    }
    else{//Se ranking existe...

        for (i = 0; i < 10; i++){
            fread(&jogadores[i], sizeof(Player), 1, frank); //Grava em um vetor as posicoes atuais do ranking;
        }
        strcpy(jogadores[10].nick, jogador.nick); //Copia os dados do jogador para a ultima posicao do vetor;
        jogadores[10].score = jogador.score;
         for (i = 0; i < 11; i++){ //Ordena o vetor de acordo com os scores, em ordem decrescente;
           for (j = 0; j < 10; j++){
                if(jogadores[j].score < jogadores[j +1].score){
                    aux = jogadores[j + 1];
                    jogadores[j + 1] = jogadores[j];
                    jogadores[j] = aux;
                }
           }
        }
        fclose(frank);  //Fecha o ranking em modo leitura;
        frank = fopen(file, "wb"); //Abre o ranking para escrita;

        for (i = 0; i < 11; i++){ //Grava apenas os 10 melhores scores e suas respectivas structs;
           fwrite(&jogadores[i], sizeof(Player), 1, frank);
        }
        fclose(frank);

    }

    gameover(); //Encerra a partida;

}
void jogar(int replay, int rankeado, Player jogador){

    //Executa o jogo;
    //Variaveis:
    int pontos = 0; //Pontos;
    int x, y, xprev = 1, yprev = 8; //Coordenadas da peca;
    char p;
    int k, m, i, j; //Variaveis auxiliares;
    double prevcoef = 0, coef = 0; //Coeficientes angulares;
    int tempo1, tempo2; //Contadores de tempo;
    int cont; //Contador de pecas conectadas;
    int test = 0; //Flag;
    int pecas = 4;
    int velocidade = 20;


    if (rankeado == 1){ //Estabelece config para o modo rankeado;
        pecas = 5;
        velocidade = 15;
    }
    else { //Le as configuracoes
        FILE* config = fopen("config.txt", "r");
        if (config != NULL){
            rewind(config);
            fscanf(config, "%d %d", &pecas, &velocidade);
        }
        else { //Default das configuracoes;
            pecas = 4;
            velocidade = 20;
        }

    }

    if (replay == 2){ //Le peca inicial do replay, se ativado;
        fscanf(freplay, "%c", &p);
    }
    else {
        p = criachar(); //Letra a ser lancada;
    }
    tabuleiro[1][8] = p; //Define a posicao inicial da peca a ser lancada;

    if (replay == 1){
        fprintf(freplay, "%c", p);//Salva as combinacoes no arquivo de replay, se ativado.
    }

    system("");
    printtabuleiro(pontos);
    for (i = 0; i < altura + 1 ; i++){ //Reseta o tabuleiro auxiliar de conexoes;
        for (j = 0; j < largura +1; j++){
            conect[i][j] = 0;
        }
    }
    tempo1 = time(0); //Inicia o primeiro timer;

    while (1){
        tempo2 = time(0);

         if(kbhit()){

            switch (getch()){

                case 'a' :
                        //move mira para a esquerda
                    coef -= 0.1;

                    if (coef < coefmin){ //Limita o coeficiente angular;
                        coef = coefmin;
                    }

                    move(prevcoef, coef); //Calcula a mira;
                    prevcoef = coef;

                break;

                case 'd':
                    // move mira para a direita
                    coef += 0.1;

                    if (coef > coefmax){ //Limita o coeficiente angular;
                        coef = coefmax;
                    }

                    move(prevcoef, coef); //Calcula a mira;
                    prevcoef = coef;

                break;

                case 0x20 :
                    //Atira a letra

                for (i = 0; i < altura; i++){
                    for (j = 0; j < largura; j++){ //Move a letra para a posicao seguinte na mira;
                        if(tabuleiro[i][j] == '-'){
                            tabuleiro[i][j] = p;
                            tabuleiro[xprev][yprev] = '-';
                            xprev = i;
                            yprev = j;
                        }
                    }
                    usleep(60000); //Pausa a funcao para permitir que a animacao seja executada;
                    printtabuleiro(pontos);
                }

                cont = pop(p, xprev, yprev, 0, pecas);//Verifica se a peca lancada se conecta a outras do mesmo tipo;

                if (cont > pecas - 2){ //Se existem ao menos x pecas conectadas;
                    for (i = 0; i < altura; i++){
                        for (j = 0; j < largura; j++){
                            if(conect[i][j] == 1){ //Se a peca estah marcada como "conectada" (== 1) no tabuleiro auxiliar, ela eh removida;
                                tabuleiro[i][j] = '*';
                            }
                             printtabuleiro(pontos);
                        }
                    }

                }

                printtabuleiro(pontos);
                usleep(10000);

                for (i = 0; i < altura; i++){//Completa a animacao para estourar pecas;
                    for (j = 0; j < largura; j++){
                         if(tabuleiro[i][j] == '*'){
                                tabuleiro[i][j] = ' ';
                                pontos+= 10;//Adiciona 10 pontos para cada peca eliminada;
                        }
                    }

                    printtabuleiro(pontos);
                }
                drop(pontos);//Remove pecas que estejam flutuando no tabuleiro;
                clean(pontos); //Limpa o tabuleiro de asteriscos;
                move(prevcoef, coef);//Recalcula a mira;
                //Iniciam a proxima rodada, gerando a nova letra a ser lancada:
                xprev = 1;
                yprev = 8;
                if (replay == 2){
                //Le proxima peca no replay, se ativado
                    if (fscanf(freplay, "%c", &p) == EOF){
                    //Encerra o jogo se nao houverem pecas disponiveis
                        gameover();
                    }
                    else if (p == '\n'){
                    //Failsafe - encerra o jogo se nao houverem pecas disponiveis
                        gameover();
                    }
                }
                else{
                    p = criachar();
                }
                tabuleiro[1][8] = p;
                if (replay == 1){
                    fprintf(freplay, "%c", p);//Salva as combinacoes no arquivo de replay, se ativado.
                }

                for (i = 0; i < altura; i++){ //Reseta o tabuleiro auxiliar;
                    for (j = 0; j < largura; j++){
                        conect[i][j] = 0;
                    }
                }

                break;

                case 'q':
                    //Encerra a partida.
                    if (freplay != NULL){   //Fecha os arquivos abertos;
                         fclose(freplay);
                    }
                    if (config != NULL){
                         fclose(config);
                    }

                    if (rankeado == 1){ //Se em modo rankeado, grava a partida;
                        jogador.score = pontos;
                        registerMatch(jogador);
                    }
                    else{
                        menuMain(0);
                    }

                break;

            }

        }

        printtabuleiro(pontos);

        if((tempo1 - tempo2)%velocidade == 0 && tempo1 != tempo2 && test == 0){//Reduz o tabuleiro a cada 20 segundos;
            test = movedown();
            resetline();//Reseta a mira;
            tempo1 += velocidade;
            test = 1;//Altera a flag para que a funcao nao seja executada mais de uma vez por ciclo;
            resetline();
            move(prevcoef, coef);//Recalcula a mira;
        }

        if ((tempo1 - tempo2)%velocidade != 0){//Reseta a flag;
            test = 0;
        }

        resetline();
        move(prevcoef, coef);
        printtabuleiro(pontos);

        for (j = 1; j < largura - 1; j++){//Encerra a partida caso o tabuleiro chegue ao seu minimo ou se letras chegarem na penultima linha do tabuleiro (acaba o espaco);
            if (tabuleiro[2][j] == '#'){
                if (freplay != NULL){ //Fecha os arquivos abertos;
                    fclose(freplay);
                }
                if (config != NULL){
                    fclose(config);
                }
                if (rankeado == 1){ //Se em modo rankeado, grava a partida;
                    jogador.score = pontos;
                    registerMatch(jogador);
                }
                else{
                    gameover();
                }
            }
            else if (tabuleiro[2][8] == 'A' ||tabuleiro[2][8] == 'B'  ||tabuleiro[2][8] == 'C'  ||tabuleiro[2][8] == 'D'  ||tabuleiro[2][8] == 'E'){
                usleep(5000);
                if (freplay != NULL){ //Fecha os arquivos abertos;
                    fclose(freplay);
                }
                if (config != NULL){
                    fclose(config);
                }
                if (rankeado == 1){ //Se em modo rankeado, grava a partida;
                    jogador.score = pontos;
                    registerMatch(jogador);
                }
                else{
                    gameover();
                }
            }

            else if (tabuleiro[1][j] != ' ' && tabuleiro[1][j] != '-' && tabuleiro[1][j] != '#' && j != 8){
                usleep(5000);
                if (freplay != NULL){
                    fclose(freplay);
                }
                if (config != NULL){
                    fclose(config);
                }
                if (rankeado == 1){
                    jogador.score = pontos;
                    registerMatch(jogador);
                }
                else{

                    gameover();
                }
            }
        }
    }
    if (freplay != NULL){ //Failsafe - fecha qualquer documento aberto;
        fclose(freplay);
    }
    if (config != NULL){
        fclose(config);
    }


}
int instrucoes(){
    //Apresenta o menu de instrucoes;
    int enter;
    system(CLEAR);

    printf(PINK "Comandos:\n");
    printf("    Inclinar mira para a esquerda - a\n");
    printf("    Inclinar mira para a direita - d\n");
    printf("    Atirar a letra - barra de espaco\n");
    printf("Como funciona o jogo:\n");
    printf("    Cada agrupamento de 4 ou mais letras iguais elimina as pecas conectadas, resultando em um numero de pontos proporcional a quantia de letras estouradas.\n");
    printf("    A cada 20 segundos, o tamanho do tabuleiro eh reduzido e as pecas descem uma linha.\n");
    printf("    O jogo acaba quando o tabuleiro chegar ao seu tamanho minimo ou quando as pecas encostarem na borda inferior.\n");
    printf("    Para encerrar o jogo, aperte Q.\n");

    printf("\nAperte ENTER para retornar ao menu \n" RESET);

    while(1){
        enter = getchar();
        if (enter == '\n'){
            getchar();
            menuMain(0);
        }
        else {
            printf(PINK "Aperte ENTER para encerrar o jogo \n" RESET);
        }
    }

    return 2;
}

int settings(){
    //Apresenta o menu de configuracoes.
    int enter;
    int rankeado = 0;
    int flag;
    system(CLEAR);
    printf(PINK "1. Pecas\n");
    printf("2. Ativar Modo Rankeado\n");
    printf("3. Voltar\n");
    printf("Selecione uma das opcoes acima\n" RESET);
    getchar();
    while(1){
        enter = getchar();

        if (enter == '3'){
            getchar();
            menuMain(rankeado);
        }
        else if (enter == '1'){
        //Pecas
            FILE* config = fopen("config.txt", "w");//Abre arquivo de configuracoes
            configuracoes.pontuacao = 0;
            configuracoes.velocidade = 0;
            while (configuracoes.pontuacao> 10 || configuracoes.pontuacao < 3){
                printf(PINK"Digite o numero de pecas a serem conectadas para pontuar:\n");
                printf("Minimo: 3, Maximo: 10\n");
                scanf("%d", &configuracoes.pontuacao);//Le variavel da pontuacao
            }
            while (configuracoes.velocidade> 35 || configuracoes.velocidade < 10){
                printf("Digite a velocidade do jogo:\n");
                printf("Minimo: 10, Maximo: 35\n");
                scanf("%d", &configuracoes.velocidade);//Le variavel da velocidade
            }

            fprintf(config, "%d %d",configuracoes.pontuacao,configuracoes.velocidade);//Salva configuracoes
            fclose(config);//Fecha arquivo das configuracoes
        }
        else if (enter == '2'){
        //Ativar modo rankeado
            rankeado = 1;
            printf(PINK"ATENCAO! O modo rankeado estara ativado apenas para uma partida\n");
            printf("Deseja jogar agora?\n");
            printf("Sim - digite 1\n" RESET);
            scanf("%d", &flag);
            if (flag == 1){
                name(1);
            }
        }


        else {
            printf(PINK"Selecione uma das opcoes acima\n"RESET);
        }
    }

    return 3;
}

int ranking(){
    //Apresenta o menu de ranking;
    int enter, i;
    Player jogador; //struct do jogador;

    system(CLEAR);
    printf(PINK"Ranking:\n");

    frank = fopen("ranking.bin", "rb"); //Abre arquivo ranking para leitura;

    if (frank != NULL){ //Se arquivo ranking existe...

        for (i = 0; i < 10; i++){
            fread(&jogador, sizeof(Player), 1, frank);
            printf("%d - %s\n", jogador.score, jogador.nick); //Imprime o ranking lido;
        }
        fclose(frank);
    }
    printf("\nAperte ENTER para retornar ao menu \n"RESET);
    while(1){
        enter = getchar();
        if (enter == '\n'){
            getchar();
            menuMain(0);
        }
        else {
            printf("Aperte ENTER para encerrar o jogo \n");
        }
    }

    return 4;
}


int sair(){
    //Fecha o jogo;
    system(CLEAR);
    exit(0);

    return 5;
}

int main (){
    //Funcao base;
    system("");
    srand(time(0)); //Gera um tempo aleatorio;
    welcome(); //Abre pagina inicial;
    return 0;
}
