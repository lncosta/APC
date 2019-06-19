/*
Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programação de Computadores –
1/2019
Aluno(a): Luiza Nogueira Costa
Matricula: 190017074
Turma: A
Versão do compilador: C99
Descricao: jogo 2D similar a Puzzle Bobble
*/


//Bibliotecas:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#include <conio.h>
#include <fcntl.h>



#ifndef _WIN32
#include <termios.h>
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

char tabuleiro2[11][18] = {
    "\n#################",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#       -       #",
    "\n#               #",
    "\n#################"};

 char tabuleiro[11][17] = {
    "#################",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#       -       #",
    "#################"};


int conect[11][17];





int altura=9, largura=16;
int velocidade=60;
double coef = 0;
char p; //char para a peca a ser disparada
int welcome();
int menuMain();
int line (double coef, int x);
char criachar();
int pop(char p, int x, int y, int cont);
void jogar();
int instrucoes();
int configuracoes();
int ranking();
int sair();

int welcome(){
    int enter;


    printf("\n###############\n");
    printf("#Puzzle Bobble#\n");
    printf("###############\n");
    printf("\n");

    printf("Aperte ENTER para iniciar o jogo \n");

    while(1){
        enter = getchar();

        if (enter == '\n'){
            menuMain();
            return 0;
        }

        else {
            printf("Aperte ENTER para iniciar o jogo \n");
        }
    }
    return 0;
}

int menuMain(){

    int select = 0;

    //clrscr();
    system(CLEAR);

    while (select != 5){
        printf("1 - Jogar\n");
        printf("2 - Instrucoes\n");
        printf("3 - Configuracoes\n");
        printf("4 - Ranking\n");
        printf("5 - Sair\n");
        printf("Digite o numero correspondente a opcao desejada: ");

        scanf("%d", &select);

        switch (select) {
            case 1: jogar(); break;
            case 3: configuracoes(); break;
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

int line (double coef, int x){
    int y;
    y = coef*x + 8;

    if(y<15 && y>1){
        return y;
    }
    else if(y <= 1){
        return 1;
    }
    else {
        return 15;
    }


}

char criachar(){
    char p;
    p = 'A' + rand() % 5;
    return p;
}

int pop(char p, int x, int y, int cont){

    system(CLEAR);

    conect[x][y] = 1;
    int i, j;

       if(tabuleiro[x + 1][y] == p && (conect[x+1][y] == 0)){

            conect[x+1][y] = 1;

             x = x+1;

            cont++;
             pop(p, x, y, cont);




        }
          if(tabuleiro[x - 1][y] == p && (conect[x - 1][y] == 0)){

            conect[x - 1][y] = 1;
            x = x - 1;
            cont++;
            pop(p, x, y, cont);


        }
        if(tabuleiro[x][y + 1] == p && (conect[x][y + 1] == 0)){

            conect[x][y + 1] = 1;
            y = y+1;
            cont++;
            pop(p, x, y, cont);



        }

        if(tabuleiro[x][y - 1] == p && (conect[x][y - 1] == 0)){

            conect[x][y - 1] = 1;
            y = y-1;
            cont++;
           pop(p, x, y, cont);


        }
          if(tabuleiro[x + 1][y + 1] == p && (conect[x + 1][y + 1] == 0)){

            conect[x + 1][y + 1] = 1;
            x = x+1;
            y = y+1;

            cont++;
           pop(p, x, y, cont);

       }
        if(tabuleiro[x + 1][y - 1] == p && (conect[x + 1][y - 1] == 0)){

            conect[x + 1][y - 1] = 1;
            x = x+1;
            y = y-1;
            cont++;
            pop(p, x, y, cont);



        }
        if(tabuleiro[x - 1][y + 1] == p && (conect[x - 1][y + 1] == 0)){

            conect[x - 1][y + 1] = 1;
            x = x - 1;
            y = y + 1;
            cont++;
            pop(p, x, y, cont);



        }
        if(tabuleiro[x - 1][y - 1] == p && (conect[x - 1][y - 1] == 0)){

            conect[x - 1][y - 1] = 1;
            x = x-1;
            y = y-1;
            cont++;
            pop(p, x, y, cont);


        }

          if (cont > 2){


                    for (i = 0; i < 11; i++){
                        for (j = 0; j <17; j++){
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
    int i, j;
     system(CLEAR);
            printf("Pontos : %d\n\n\n", pontos);

                    for (i = altura + 1; i >= 0; i--){
                        for (j = 0; j < largura +1; j++){


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
                                else
                                printf("%c", tabuleiro[i][j]);
                                }

                            printf("\n");
                    }
                      usleep(100);


}





void jogar(){

    int pontos = 0, test = 1, i, j, counter;
    int x, y, xprev = 1, yprev = 8;
    char p = criachar();


    double prevcoef = 0;
    tabuleiro[1][8] = p;
    tabuleiro2[9][9] =  tabuleiro[1][8];



    int tempo1, tempo2;
    system("");

    //clrscr();



    system(CLEAR);
    printf("Pontos : %d\n\n", pontos);
    printf(tabuleiro2);

    for (i = 0; i < 11; i++){
        for (j = 0; j < 17; j++){
            conect[i][j] = 0;
            }

        }



    while (1){
          tempo1 = time(0);





         if(kbhit()){

            switch (getch()){

                case 'a' :
                    //move mira para a esquerda
                coef -= 0.1;


                for (i = altura; i >= 2; i--){


                    if(tabuleiro[i][line(prevcoef, i)] == '-'){
                            if(tabuleiro[i][line(prevcoef, i)] == '-' || tabuleiro[i][line(prevcoef, i)] == ' ' || tabuleiro[i][line(prevcoef, i)] == '#'){
                                tabuleiro[i][line(prevcoef, i)] = ' ';
                            }

                    }
                    if(tabuleiro[i][line(coef, i)] == ' '){
                       if(tabuleiro[i][line(prevcoef, i)] == '-' || tabuleiro[i][line(prevcoef, i)] == ' ' || tabuleiro[i][line(prevcoef, i)] == '#'){
                                tabuleiro[i][line(prevcoef, i)] = ' ';
                            }


                    }

                     y = line(coef, i);
                      if (y == 1 || y == 15){
                        tabuleiro[i][j] = '#';

                    }

                      if (tabuleiro[i][line(coef, i)] == ' '){
                             tabuleiro[i][line(coef, i)] = '-';
                        }




                }

                 prevcoef = coef;


                break;

                case 'd':
                    // move mira para a direita

                coef += 0.1;

                    for (i = altura; i >= 2; i--){



                         if(tabuleiro[i][line(prevcoef, i)] == '-'){
                            if(tabuleiro[i][line(prevcoef, i)] == '-' || tabuleiro[i][line(prevcoef, i)] == ' ' || tabuleiro[i][line(prevcoef, i)] == '#'){
                                tabuleiro[i][line(prevcoef, i)] = ' ';
                            }

                    }
                        if(tabuleiro[i][line(coef, i)] == ' '){
                           if(tabuleiro[i][line(prevcoef, i)] == '-' || tabuleiro[i][line(prevcoef, i)] == ' ' || tabuleiro[i][line(prevcoef, i)] == '#'){
                                    tabuleiro[i][line(prevcoef, i)] = ' ';
                                }


                        }


                         y = line(coef, i);
                          if (y == 1 || y == 15){
                            tabuleiro[i][j] = '#';

                        }

                        if (tabuleiro[i][line(coef, i)] == ' '){
                             tabuleiro[i][line(coef, i)] = '-';
                        }





                    }



                  prevcoef = coef;



                break;

                int k, m;

                case 'x' :
                    //Atira a letra
                i = altura;
                j = 0;
                for (i = 0; i <= altura; i++){

                    for (j = 0; j <= largura; j++){
                        if(tabuleiro[i][j] == '-'){
                            tabuleiro[i][j] = p;
                            tabuleiro[xprev][yprev] = '-';
                            xprev = i;
                            yprev = j;
                        }




                    }
                                   /* if(p == 'A'){
                                        tabuleiro[xprev][yprev] == 'A';
                                      }
                                      else if(p == 'B'){
                                         tabuleiro[xprev][yprev] == 'B';
                                      }
                                      else if(p == 'C'){
                                        tabuleiro[xprev][yprev] == 'C';
                                      }
                                      else if(p == 'D'){
                                      tabuleiro[xprev][yprev] == 'D';
                                      }
                                      else if(p == 'E'){
                                         tabuleiro[xprev][yprev] == 'E';
                                      }*/


                        usleep(60000);
                        system(CLEAR);
                        printf("Pontos : %d\n\n\n", pontos);

                        for (k = 10 ; k >= 0; k--){
                                 for (m = 0; m <17 ; m++){

                               if (tabuleiro[k][m] != '-' && tabuleiro[k][m] != ' ' && tabuleiro[k][m] != '#'){


                                      if(tabuleiro[k][m] == 'A'){
                                        printf(RED "A" RESET);
                                      }
                                      else if(tabuleiro[k][m] == 'B'){
                                        printf(YELLOW "B" RESET);
                                      }
                                      else if(tabuleiro[k][m] == 'C'){
                                        printf(GREEN "C" RESET);
                                      }
                                      else if(tabuleiro[k][m] == 'D'){
                                        printf(MAGENTA "D" RESET);
                                      }
                                      else if(tabuleiro[k][m] == 'E'){
                                         printf(CYAN "E" RESET);
                                      }
                                       else if(tabuleiro[i][j] == '*'){
                                          printf(PINK "*" RESET);
                                      }


                                }
                                else
                                printf("%c", tabuleiro[k][m]);
                                }

                            printf("\n");

                        }
                }

                int cont;

               cont = pop(p, xprev, yprev, 0);

                if (cont > 2){


                    for (i = 0; i < 11; i++){
                        for (j = 0; j <17; j++){
                            if(conect[i][j] == 1){

                                tabuleiro[i][j] = '*';
                            }
                             printtabuleiro(pontos);

                        }


                    }
                    pontos = pontos + cont*10;
                }

                else {
                    pontos = pontos;

                }

                printtabuleiro(pontos);



                for (i = 0; i < 11; i++){
                    for (j = 0; j < 17; j++){

                         if(tabuleiro[i][j] == '*'){

                                tabuleiro[i][j] = ' ';
                                pontos+= 10;

                            }

                        }
                        printtabuleiro(pontos);


                    }





                xprev = 1;
                yprev = 8;

                p = criachar();
                tabuleiro[1][8] = p;

                 for (i = 0; i < 11; i++){
                    for (j = 0; j < 17; j++){
                        conect[i][j] = 0;
                        }

                    }



                break;

                }

            }

            printtabuleiro(pontos);

            /*
             system(CLEAR);
            printf("Pontos : %d\n\n\n", pontos);

                    for (i = altura + 1; i >= 0; i--){
                        for (j = 0; j < largura +1; j++){

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
                                else
                                printf("%c", tabuleiro[i][j]);
                                }

                            printf("\n");
                    }/*


           tempo2 = time(0);

           /*if (tempo1 - tempo2 == 20*60){
            for (i = 10; i > 0; i--){
                for(j = 0; j < largura; j++){
                    tabuleiro[i][j] = tabuleiro[i - 1][j];
                }
            }



           }*/
                    }



}



int instrucoes(){
    //clrscr();
    system(CLEAR);
    printf("instrucoes");

    return 2;
}

int configuracoes(){
     //clrscr();
    system(CLEAR);
     printf("Configuracoes em desenvolvimento");

    return 3;
}

int ranking(){
    //clrscr();
    system(CLEAR);
    printf("Ranking em desenvolvimento");

    return 4;
}

int sair(){
    //clrscr();
    system(CLEAR);
    exit(0);

    return 5;
}

int main (){
    system("");
    srand(time(0));

    welcome();



    return 0;
}
