#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

void printMaze(int level){
    char filename[20];
    sprintf(filename,"level%d.txt",level);
    system("cls");
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[256];  
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);  
    }

    fclose(fp);
    printf("\n");
}

bool isSpace(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    
    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == ' ' || ci.Char.AsciiChar == 'o'; 
    } else {
        return false; 
    }
}

bool isCoin(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == 'o'; 
    } else {
        return false; 
    }
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);    
    return csbi.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);    
    return csbi.dwCursorPosition.Y;
}

int enterInt(int LL, int UL){
    int enterInt;
    while(1){
        printf("\nEnter a number between %d and %d: ", LL, UL);
        if(scanf("%d",&enterInt)){
            if(enterInt>=LL&&enterInt<=UL){
                break;
            }else{
                printf("\nInvalid number! ");
            }
        }else{
            printf("\nInvalid input! ");
            while(getchar()!='\n');
        }
    }
    while(getchar()!='\n');
    return enterInt;
}

void info(int saveX1, int saveY1, int level, int coin, int seconds){
    gotoxy(saveX1,saveY1);
    printf("Level: %2d   Coin: %4d  Time: %5d",level,coin,seconds);
}


void maze(){
    FILE * fp;
    char buffer[10];
    int intBuffer;
    int mode=0;
    char move;
    int level=1;
    int coin=0;
    int seconds=0;
    int x1=0;
    int y1=40;
    int saveX1=0;
    int saveY1=0;

    time_t t;
    
    system("cls");
    printf("########################\n");
    printf("#######   Maze   #######\n");
    printf("###### 1 New Game ######\n");
    printf("###### 2 Load Save #####\n");
    printf("###### 3   EXIT   ######\n");
    printf("########################\n");
    mode=enterInt(1,3);
    if(mode==3) return;
    if(mode==1){
        printf("Warning: Will ERASE previous save.\n");
        printf("Are you sure? (y/n): ");
            scanf(" %c",&move);
            while(move!='y'&&move!='n'){
                printf("\nEnter a valid option: ");
                scanf(" %c",&move);
                while(getchar()!='\n');
            }
            if(move=='y') {
                fp = fopen("save.txt", "w");
                if (fp == NULL){
                    printf("Could not open file");
                    return;
                }
                fprintf(fp,"%d %d %d",level,coin,seconds);
                fclose(fp);
                mode=2;
                while(getchar()!='\n');
            }
            else if(move=='n') {
                printf("Load Save or Exit? (l/e): ");
                scanf(" %c",&move);
                while(move!='l'&&move!='e'){
                    printf("\nEnter a valid option: ");
                    scanf(" %c",&move);
                    while(getchar()!='\n');
                }
                if(move=='l') {
                    mode=2;
                }
                else if(move=='e') {
                    return;
                }
                while(getchar()!='\n');
            }
    }
    if(mode==2){
        fp = fopen("save.txt", "r");
        if (fp == NULL){
            printf("Could not open file");
            return;
        }
        fscanf(fp,"%d %d %d",&level,&coin,&seconds);
        fclose(fp);

        
        printMaze(level);
        
     
            x1=wherex()+2;
            y1=wherey()-2;
            saveX1=wherex();
            saveY1=wherey();

            t=time(NULL);
            
            
            gotoxy(x1,y1);
            printf("G");
            
            while(1){
                
                if(kbhit()){
                    

                    move=getch();
                    if(move=='w'&&isSpace(x1,y1-1)){
                        if(isCoin(x1,y1-2)){
                            
                            gotoxy(x1,y1-2);
                            printf(" ");
                            coin++;
                            info(saveX1,saveY1,level,coin,seconds);
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        y1-=2;
                        gotoxy(x1,y1);
                        printf("G");


                    }
                    else if(move=='s'&&isSpace(x1,y1+1)){
                        if(isCoin(x1,y1+2)){
                            
                            gotoxy(x1,y1+2);
                            printf(" ");
                            coin++;
                            info(saveX1,saveY1,level,coin,seconds);
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        y1+=2;
                        gotoxy(x1,y1);
                        printf("G");

                    } 
                    else if(move=='a'&&isSpace(x1-2,y1)){
                        if(isCoin(x1-2,y1)){
                            
                            gotoxy(x1-2,y1);
                            printf(" ");
                            coin++;
                            info(saveX1,saveY1,level,coin,seconds);
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        x1-=2;
                        gotoxy(x1,y1);
                        printf("G");

                    } 
                    else if(move=='d'&&isSpace(x1+2,y1)){
                        if(isCoin(x1+2,y1)){
                            
                            gotoxy(x1+2,y1);
                            printf(" ");
                            coin++;
                            info(saveX1,saveY1,level,coin,seconds);
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        x1+=2;
                        gotoxy(x1,y1);
                        printf("G");

                    } 
                    else if(move=='f'){
                       /* 
                       gotoxy(x1,y1);
                       printf("K");
                       */
                    }
                    else if(move=='q'){
                        gotoxy(saveX1,saveY1);
                        printf("Are you sure you want to quit? (y/n): ");
                        move=getch();
                        while(move!='y'&&move!='n'){
                            move=getch();
                        }
                        if(move=='y') return;
                        else if(move=='n') {
                            gotoxy(saveX1,saveY1);
                            printf("                                                     \n");
                            
                        }
                    }

                
                    


                    if(x1>80&&y1<3){
                        printf("You Win!");

                        level++;

                        fopen("save.txt", "w");
                        if (fp == NULL){
                            printf("Could not open file");
                            return;
                        }
                        fprintf(fp,"%d %d %d",level,coin,seconds);
                        fclose(fp);

                        system("cls");
                        printf("You Win!\n");
                        printf("Next Level or Exit? (n/e): ");
                        move=getch();
                        while(move!='n'&&move!='e'){
                            move=getch();
                        }
                        if(move=='n') {
                            printMaze(level);
                            x1=wherex()+2;
                            y1=wherey()-2;
                            saveX1=wherex();
                            saveY1=wherey();
                            gotoxy(x1,y1);
                            printf("G");
                            
                        }
                        else if(move=='e') {
                            break;
                        }

                        
                    }
                    
                    
                }
            
                if(difftime(time(NULL),t)>=1){
                    seconds++;
                    info(saveX1,saveY1,level,coin,seconds);
                    t=time(NULL);
                }

            }
            
            
        
            
        
    }



    

}




int main(){

    int mode=0;
    hidecursor();
    

    while(mode==0){



        system("cls");
        printf("########################\n");
        printf("#######   Maze   #######\n");
        printf("####### 1  Play  #######\n");
        printf("####### 2 *****  #######\n");
        printf("####### 3  ***   #######\n");
        printf("####### 4 ****** #######\n");
        printf("####### 5  Exit  #######\n");
        printf("########################\n");
        mode=enterInt(1,5);
        if(mode==5) return 0;
        
        if(mode==1)maze();
        mode=0;
        



        

        










    }



    return 0;
}
