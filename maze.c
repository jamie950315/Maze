/**
 * @file maze.c
 * @brief Maze game implementation in C.
 * 
 * This file contains the implementation of a maze game where the player navigates through
 * different levels, collects coins and keys, and opens doors to reach the exit. The game
 * supports single-player and dual-player modes.
 * 
 * The game uses the console for input and output, and the maze levels and answers are stored
 * in external text files.
 * 
 * The main functions in this file include:
 * - printMaze: Prints the maze for a given level.
 * - printAns: Prints the answer map for a given level.
 * - isSpace: Checks if a given console coordinate is a space or other navigable character.
 * - isCoin: Checks if a given console coordinate contains a coin.
 * - isKey: Checks if a given console coordinate contains a key.
 * - doorAcces: Checks if a door can be accessed and updates the key count.
 * - hidecursor: Hides the console cursor.
 * - gotoxy: Moves the console cursor to a specified coordinate.
 * - wherex: Returns the current x-coordinate of the console cursor.
 * - wherey: Returns the current y-coordinate of the console cursor.
 * - enterInt: Prompts the user to enter an integer within a specified range.
 * - info: Displays game information such as level, coin count, time, key count, and steps.
 * - infoDual: Displays game information for dual-player mode.
 * - formatInput: Formats the input based on the selected input mode.
 * - maze: Main game loop for the maze game.
 * - guide: Displays the game guide.
 * - selectLevel: Allows the player to select a level to play.
 * - input: Sets the input mode for the game.
 * - credit: Displays the game credits.
 * - main: Main function that displays the main menu and handles user selection.
 * 
 * @note The game uses the Windows API for console manipulation and is intended to be run on Windows.
 * 
 * @author Jamie Chen
 * @date December 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>


void printMaze(int level){
    char filename[20];
    sprintf(filename,"./levelMap/level%d.txt",level);
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

void printAns(int level){
    char filename[20];
    sprintf(filename,"./ansMap/ans%d.txt",level);
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
        return ci.Char.AsciiChar == ' ' || ci.Char.AsciiChar == 'o'|| ci.Char.AsciiChar == '>'|| ci.Char.AsciiChar == '/'|| ci.Char.AsciiChar == '@'|| ci.Char.AsciiChar == '$'; 
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

bool isKey(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == '>'; 
    } else {
        return false; 
    }
}

bool doorAcces(int x, int y, int *key){
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        if(ci.Char.AsciiChar == '/'){
            if(*key>0){
                (*key)--;
                return true;
            }else{
                return false;
            }
        } 
        else return true; 
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

void setFullScreen() {
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
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

void info(int saveX1, int saveY1, int level, int coin, int seconds, int key, int step){
    gotoxy(saveX1,saveY1);
    printf("Level: %2d   Coin: %4d  Time: %5d   Key: %2d    Step: %4d",level,coin,seconds,key,step);
}

void infoDual(int saveX1, int saveY1, int level, int coin, int seconds, int key, int coin2, int key2){
    gotoxy(saveX1,saveY1);
    printf("Level:%2d  Time: %3d  P1Coin: %2d  P1Key: %2d  P2Coin: %2d  P2Key: %2d",level,seconds,coin,key,coin2,key2);
}

char formatInput(int mode){
    char move;
    if(mode==1){
        move=getch();
        if(move==-32){
            getch();
            return '\0';
        }else return move;
        
    }else if(mode==2){
        move=getch();
        if(move==-32){
            move=getch();
            if(move==72) return 'w';
            if(move==80) return 's';
            if(move==75) return 'a';
            if(move==77) return 'd';
        } else if (move == 'w' || move == 'a' || move == 's' || move == 'd') {
            return '\0';
        }
        return move;
    }else if(mode==3){
        move=getch();
        if(move==-32){
            move=getch();
            if(move==72) return 'o';
            if(move==80) return 'l';
            if(move==75) return 'k';
            if(move==77) return ';';
        }
        return move;
    }
    return '\0';
}


void maze(int inputMode){
    FILE * fp;
    
    int mode=0;
    char move;
    int level=1;
    int coin=0;
    int seconds=0;
    int key=0;
    int x1=0;
    int y1=40;
    int saveX1=0;
    int saveY1=0;

    int stepPath[10]={170,176,134,174,174,228,164,324,202,194};
    int stepBest[10]={10000,10000,10000,10000,10000,10000,10000,10000,10000,10000};

    int step=0;

    int prevX=100;
    int prevY=100;

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
                fprintf(fp,"%d %d %d %d %d %d %d",level,coin,seconds,key,prevX,prevY,step);
                fclose(fp);

                fp = fopen("stepBest.txt", "w");
                if (fp == NULL){
                    printf("Could not open file");
                    return;
                }
                for(int i=0;i<10;i++){
                    fprintf(fp,"%d ",stepBest[i]);
                }
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
        fscanf(fp,"%d %d %d %d %d %d %d",&level,&coin,&seconds,&key,&prevX,&prevY,&step);
        fclose(fp);

        fp = fopen("stepBest.txt", "r");
        if (fp == NULL){
            printf("Could not open file");
            return;
        }
        for(int i=0;i<10;i++){
            fscanf(fp,"%d ",&stepBest[i]);
        }
        fclose(fp);

        
        printMaze(level);
        
            if(prevX==100&&prevY==100){
                x1=wherex()+2;
                y1=wherey()-2;
            }else{
                x1=prevX;
                y1=prevY;
            }

            prevX=100;
            prevY=100;
                
            saveX1=wherex();
            saveY1=wherey();

            t=time(NULL);
            
            
            gotoxy(x1,y1);
            printf("@");
            info(saveX1,saveY1,level,coin,seconds,key,step);
            
            while(1){
                
                if(kbhit()){
                    

                    move=formatInput(inputMode);
                    if(move=='w'&&isSpace(x1,y1-1)&&doorAcces(x1,y1-1,&key)){
                        if(isCoin(x1,y1-2)){
                            
                            gotoxy(x1,y1-2);
                            printf(" ");
                            coin++;
                            
                        }else if(isKey(x1,y1-2)){
                            gotoxy(x1,y1-2);
                            printf(" ");
                            key++;
                            
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        y1-=2;
                        gotoxy(x1,y1);
                        printf("@");
                        step++;
                        info(saveX1,saveY1,level,coin,seconds,key,step);


                    }
                    else if(move=='s'&&isSpace(x1,y1+1)&&doorAcces(x1,y1+1,&key)){
                        if(isCoin(x1,y1+2)){
                            
                            gotoxy(x1,y1+2);
                            printf(" ");
                            coin++;
                            
                        }else if(isKey(x1,y1+2)){
                            gotoxy(x1,y1+2);
                            printf(" ");
                            key++;
                            
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        y1+=2;
                        gotoxy(x1,y1);
                        printf("@");
                        step++;
                        info(saveX1,saveY1,level,coin,seconds,key,step);

                    } 
                    else if(move=='a'&&isSpace(x1-2,y1)&&doorAcces(x1-2,y1,&key)){
                        if(isCoin(x1-2,y1)){
                            
                            gotoxy(x1-2,y1);
                            printf(" ");
                            coin++;
                            
                        }else if(isKey(x1-2,y1)){
                            gotoxy(x1-2,y1);
                            printf(" ");
                            key++;
                            
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        x1-=2;
                        gotoxy(x1,y1);
                        printf("@");
                        step++;
                        info(saveX1,saveY1,level,coin,seconds,key,step);

                    } 
                    else if(move=='d'&&isSpace(x1+2,y1)&&doorAcces(x1+2,y1,&key)){
                        if(isCoin(x1+2,y1)){
                            
                            gotoxy(x1+2,y1);
                            printf(" ");
                            coin++;
                            
                        }else if(isKey(x1+2,y1)){
                            gotoxy(x1+2,y1);
                            printf(" ");
                            key++;
                            
                        }
                        gotoxy(x1,y1);
                        printf(" ");
                        x1+=2;
                        gotoxy(x1,y1);
                        printf("@");
                        step++;
                        info(saveX1,saveY1,level,coin,seconds,key,step);

                    } 
                    
                    else if(move=='q'){
                        gotoxy(saveX1,saveY1);
                        printf("Are you sure you want to quit? (y/n):                             ");
                        move=getch();
                        while(move!='y'&&move!='n'){
                            move=getch();
                        }
                        if(move=='y'){
                            printf("\nSave before exit? (y/n): ");
                            move=getch();
                            while(move!='y'&&move!='n'){
                                move=getch();
                            }
                            if(move=='y'){
                                fp = fopen("save.txt", "w");
                                if (fp == NULL){
                                    printf("Could not open file");
                                    return;
                                }
                                fprintf(fp,"%d %d %d %d %d %d %d",level,coin,seconds,key,x1,y1,step);
                                fclose(fp);
                            }
                            return;
                        }
                        
                        gotoxy(saveX1,saveY1);
                        printf("                                                       \n");
                        info(saveX1,saveY1,level,coin,seconds,key,step);

                    }

                
                    if(x1>80&&y1<3){

                        if(level==10){
                            system("cls");
                            printf("You have completed all levels!\n");
                            printf("Record: %d coins and %d keys in %d seconds and %d steps!\n",coin,key,seconds,step);
                            printf("Thanks for playing!\n");
                            printf("\n");
                            level=1;
                            coin=0;
                            seconds=0;
                            key=0;
                            step=0;
                            

                            fp = fopen("save.txt", "w");
                            if (fp == NULL){
                                printf("Could not open file");
                                return;
                            }
                            fprintf(fp,"%d %d %d %d %d %d %d",level,coin,seconds,key,prevX,prevY,step);
                            fclose(fp);


                            printf("Press any key to exit...");
                            Sleep(2000);
                            getch();
                            return;
                        }else{

                            level++;

                            system("cls");
                            printf("You complete Level %d!\n",level-1);
                            printf("Record: %d coins and %d keys in %d seconds and %d steps!\n",coin,key,seconds,step);
                            printf("Shortest path: %d steps\n",stepPath[level-2]);
                            if(step<stepBest[level-2]){
                                stepBest[level-2]=step;
                                fp = fopen("stepBest.txt", "w");
                                if (fp == NULL){
                                    printf("Could not open file");
                                    return;
                                }
                                for(int i=0;i<10;i++){
                                    fprintf(fp,"%d ",stepBest[i]);
                                }
                                fclose(fp);
                                printf("New record! %d steps!\n",stepBest[level-2]);
                            }else{
                                printf("Best record: %d steps\n",stepBest[level-2]);
                            }
                            
                            fopen("save.txt", "w");
                            step=0;
                            
                            if (fp == NULL){
                                printf("Could not open file");
                                return;
                            }
                            fprintf(fp,"%d %d %d %d %d %d %d",level,coin,seconds,key,prevX,prevY,step);
                            fclose(fp);

                            printf("Next Level or Exit? (n/e): ");
                            move=getch();
                            while(move!='n'&&move!='e'){
                                move=getch();
                            }
                            if(move=='n') {
                                
                                step=0;
                                printMaze(level);
                                x1=wherex()+2;
                                y1=wherey()-2;
                                saveX1=wherex();
                                saveY1=wherey();
                                gotoxy(x1,y1);
                                printf("@");
                            }
                            else if(move=='e') {
                                break;
                            }
                        }
                        
                        
                    }
                    
                    
                }
            
                if(difftime(time(NULL),t)>=1){
                    seconds++;
                    info(saveX1,saveY1,level,coin,seconds,key,step);
                    t=time(NULL);
                }

            }
            
            
        
            
        
    }



    

}

void guide(){
    system("cls");
    printf("#########################\n");
    printf("#######   Maze   ########\n");
    printf("#########################\n");
    printf("1. Use W, A, S, D to move\n");
    printf("2. Collect coins and keys\n");
    printf("3.  Open doors with keys \n");
    printf("4. Coin: o Key: > Door: /\n");
    printf("5. Reach top right to win\n");
    printf("6.    Press Q to quit    \n");
    printf("7.    Enjoy the game!    \n");
    printf("\n");
    printf("Press any key to continue...\n");
    getch();
    return;
}

void selectLevel(int inputMode){
    
    FILE *fp;

    int mode;
    char move;
    int level=1;
    int coin=0;
    int seconds=0;
    int key=0;
    int x1=0;
    int y1=40;
    int saveX1=0;
    int saveY1=0;
    time_t t;

    bool dual=false;
    int x2=100;
    int y2=100;
    int coin2=0;
    int key2=0;

    int stepPath[10]={170,176,134,174,174,228,164,324,202,194};
    int stepBest[10]={10000,10000,10000,10000,10000,10000,10000,10000,10000,10000};

    int step=0;

    system("cls");
    printf("########################\n");
    printf("#######   Maze   #######\n");
    printf("##### Select Level #####\n");
    printf("#####    1 Play    #####\n");
    printf("#####    2 Dual    #####\n");
    printf("#####    3 Path    #####\n");
    printf("#####    4 EXIT    #####\n");
    printf("########################\n");
    
    mode=enterInt(1,4);
    if(mode==2)dual=true;

    if (mode==1||mode==2)
    {

        system("cls");
        printf("Select level (1~10)");
        level=enterInt(1,10);

        fp = fopen("stepBest.txt", "r");
        if (fp == NULL){
            printf("Could not open file");
            return;
        }
        for(int i=0;i<10;i++){
            fscanf(fp,"%d ",&stepBest[i]);
        }
        fclose(fp);

        printMaze(level);
        x1 = wherex() + 2;
        y1 = wherey() - 2;
        saveX1 = wherex();
        saveY1 = wherey();
        gotoxy(x1, y1);
        printf("@");
        if(dual){
            x2 = x1;
            y2 = y1;
        }
        if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
        else info(saveX1, saveY1, level, coin, seconds, key, step);

        while (1)
        {

            if (kbhit())
            {

                if(dual)move = formatInput(3);
                else move = formatInput(inputMode);
                
                if (move == 'w' && isSpace(x1, y1 - 1) && doorAcces(x1, y1 - 1, &key))
                {
                    if (isCoin(x1, y1 - 2))
                    {

                        gotoxy(x1, y1 - 2);
                        printf(" ");
                        coin++;
                        
                    }
                    else if (isKey(x1, y1 - 2))
                    {
                        gotoxy(x1, y1 - 2);
                        printf(" ");
                        key++;
                        
                    }
                    gotoxy(x1, y1);
                    printf(" ");
                    if(dual){
                        gotoxy(x2,y2);
                        printf("$");
                    }
                    y1 -= 2;
                    gotoxy(x1, y1);
                    printf("@");
                    step++;
                    if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    else info(saveX1, saveY1, level, coin, seconds, key, step);
                }
                else if (move == 's' && isSpace(x1, y1 + 1) && doorAcces(x1, y1 + 1, &key))
                {
                    if (isCoin(x1, y1 + 2))
                    {

                        gotoxy(x1, y1 + 2);
                        printf(" ");
                        coin++;
                        
                    }
                    else if (isKey(x1, y1 + 2))
                    {
                        gotoxy(x1, y1 + 2);
                        printf(" ");
                        key++;
                        
                    }
                    gotoxy(x1, y1);
                    printf(" ");
                    if(dual){
                        gotoxy(x2,y2);
                        printf("$");
                    }
                    y1 += 2;
                    gotoxy(x1, y1);
                    printf("@");
                    step++;
                    if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    else info(saveX1, saveY1, level, coin, seconds, key, step);
                }
                else if (move == 'a' && isSpace(x1 - 2, y1) && doorAcces(x1 - 2, y1, &key))
                {
                    if (isCoin(x1 - 2, y1))
                    {

                        gotoxy(x1 - 2, y1);
                        printf(" ");
                        coin++;
                        
                    }
                    else if (isKey(x1 - 2, y1))
                    {
                        gotoxy(x1 - 2, y1);
                        printf(" ");
                        key++;
                        
                    }
                    gotoxy(x1, y1);
                    printf(" ");
                    if(dual){
                        gotoxy(x2,y2);
                        printf("$");
                    }
                    x1 -= 2;
                    gotoxy(x1, y1);
                    printf("@");
                    step++;
                    if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    else info(saveX1, saveY1, level, coin, seconds, key, step);
                }
                else if (move == 'd' && isSpace(x1 + 2, y1) && doorAcces(x1 + 2, y1, &key))
                {
                    if (isCoin(x1 + 2, y1))
                    {

                        gotoxy(x1 + 2, y1);
                        printf(" ");
                        coin++;
                        
                    }
                    else if (isKey(x1 + 2, y1))
                    {
                        gotoxy(x1 + 2, y1);
                        printf(" ");
                        key++;
                        
                    }
                    gotoxy(x1, y1);
                    printf(" ");
                    if(dual){
                        gotoxy(x2,y2);
                        printf("$");
                    }
                    x1 += 2;
                    gotoxy(x1, y1);
                    printf("@");
                    step++;
                    if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    else info(saveX1, saveY1, level, coin, seconds, key, step);
                }

                if(dual){
                    if (move == 'o' && isSpace(x2, y2 - 1) && doorAcces(x2, y2 - 1, &key2))
                    {
                        if (isCoin(x2, y2 - 2))
                        {

                            gotoxy(x2, y2 - 2);
                            printf(" ");
                            coin2++;
                            infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                        }
                        else if (isKey(x2, y2 - 2))
                        {
                            gotoxy(x2, y2 - 2);
                            printf(" ");
                            key2++;
                            infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                        }
                        gotoxy(x2, y2);
                        printf(" ");
                        gotoxy(x1,y1);
                        printf("@");
                        y2 -= 2;
                        gotoxy(x2, y2);
                        printf("$");
                    }
                    else if (move == 'l' && isSpace(x2, y2 + 1) && doorAcces(x2, y2 + 1, &key2))
                {
                    if (isCoin(x2, y2 + 2))
                    {

                        gotoxy(x2, y2 + 2);
                        printf(" ");
                        coin2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    else if (isKey(x2, y2 + 2))
                    {
                        gotoxy(x2, y2 + 2);
                        printf(" ");
                        key2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    gotoxy(x2, y2);
                    printf(" ");
                    gotoxy(x1,y1);
                    printf("@");
                    y2 += 2;
                    gotoxy(x2, y2);
                    printf("$");
                }
                    else if (move == 'k' && isSpace(x2 - 2, y2) && doorAcces(x2 - 2, y2, &key2))
                {
                    if (isCoin(x2 - 2, y2))
                    {

                        gotoxy(x2 - 2, y2);
                        printf(" ");
                        coin2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    else if (isKey(x2 - 2, y2))
                    {
                        gotoxy(x2 - 2, y2);
                        printf(" ");
                        key2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    gotoxy(x2, y2);
                    printf(" ");
                    gotoxy(x1,y1);
                    printf("@");
                    x2 -= 2;
                    gotoxy(x2, y2);
                    printf("$");
                }
                    else if (move == ';' && isSpace(x2 + 2, y2) && doorAcces(x2 + 2, y2, &key2))
                {
                    if (isCoin(x2 + 2, y2))
                    {

                        gotoxy(x2 + 2, y2);
                        printf(" ");
                        coin2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    else if (isKey(x2 + 2, y2))
                    {
                        gotoxy(x2 + 2, y2);
                        printf(" ");
                        key2++;
                        infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                    }
                    gotoxy(x2, y2);
                    printf(" ");
                    gotoxy(x1,y1);
                    printf("@");
                    x2 += 2;
                    gotoxy(x2, y2);
                    printf("$");
                }
                }
                
                if (move == 'q')
                {
                    
                    gotoxy(saveX1, saveY1);
                    printf("Are you sure you want to quit? (y/n):                             ");
                    move = getch();
                    while (move != 'y' && move != 'n')
                    {
                        move = getch();
                    }
                    if (move == 'y')
                        return;
                    else if (move == 'n')
                    {
                        gotoxy(saveX1, saveY1);
                        printf("                                                       \n");
                    }
                }

                if (x1 > 80 && y1 < 3)
                {
                    system("cls");
                    if(dual){
                        printf("Player 1 wins!\n");
                        printf("Player 1: %d coins and %d keys in %d seconds\n", coin, key, seconds);
                        printf("Player 2: %d coins and %d keys\n", coin2, key2);
                    } 
                    else{
                        printf("You complete Level %d!\n", level);
                        printf("Record: %d coins and %d keys in %d seconds and %d steps!\n",coin,key,seconds,step);
                        printf("Shortest path: %d steps\n",stepPath[level-1]);
                            if(step<stepBest[level-1]){
                                stepBest[level-1]=step;
                                fp = fopen("stepBest.txt", "w");
                                if (fp == NULL){
                                    printf("Could not open file");
                                    return;
                                }
                                for(int i=0;i<10;i++){
                                    fprintf(fp,"%d ",stepBest[i]);
                                }
                                fclose(fp);
                                printf("New record! %d steps!\n",stepBest[level-1]);
                            }else{
                                printf("Best record: %d steps\n",stepBest[level-1]);
                            }
                    } 
                    
                    
                    printf("Thanks for playing!\n");
                    printf("\n");
                    printf("Press any key to exit...");
                    Sleep(2000);
                    getch();
                    return;
                }
                if (x2 > 80 && y2 < 3)
                {
                    system("cls");
                    printf("Player 2 wins!\n");
                    printf("Player 1: %d coins and %d keys\n", coin, key);
                    printf("Player 2: %d coins and %d keys in %d seconds\n", coin2, key2, seconds);
                    printf("Thanks for playing!\n");
                    printf("\n");
                    printf("Press any key to exit...");
                    Sleep(2000);
                    getch();
                    return;
                }
            }

            if (difftime(time(NULL), t) >= 1)
            {
                seconds++;
                if(dual)infoDual(saveX1, saveY1, level, coin, seconds, key, coin2, key2);
                else info(saveX1, saveY1, level, coin, seconds, key, step);
                t=time(NULL);
            }
        }
    }

    if(mode==3){
        system("cls");
        printf("Select level (1~10)");
        level=enterInt(1,10);

        printAns(level);
        printf("\nPress any key to continue...");
        getch();
        return;

    }

    if(mode==4) return;
    


    return;
}

void input(int *inputMode){
    int mode=0;
    system("cls");
    printf("########################\n");
    printf("######   Maze   ########\n");
    printf("#### Set input keys ####\n");
    printf("##### 1 WASD Key  ######\n");
    printf("##### 2 Arrow Key ######\n");
    printf("##### 3   EXIT    ######\n");
    printf("########################\n");
    mode=enterInt(1,3);
    if(mode==1){
        *inputMode=1;
        printf("WASD keys set!\n");
        printf("Press any key to continue...");
        getch();
        return;
    }
    if(mode==2){
        *inputMode=2;
        printf("Arrow keys set!\n");
        printf("Press any key to continue...");
        getch();
        return;
    }
    if(mode==3) return;
    return;
}

void credit(){
    system("cls");
    printf("########################\n");
    printf("#######   Maze   #######\n");
    printf("####  NTUST PROJECT ####\n");
    printf("########################\n");
    printf("######   CREDIT   ######\n");
    printf("###### Jamie Chen ######\n");
    printf("######  DEC.2024  ######\n");
    printf("########################\n");
    printf("\nPress any key to continue...");
    getch();
    return;
}



int main(){

    setFullScreen();
    hidecursor();

    int mode=0;
    int inputMode=1;
    
    
    while(mode==0){

        system("cls");
        printf("########################\n");
        printf("#######   Maze   #######\n");
        printf("#### 1     Play     ####\n");
        printf("#### 2 Select Level ####\n");
        printf("#### 3 How to play? ####\n");
        printf("#### 4   Set input  ####\n");
        printf("#### 5     Exit     ####\n");
        printf("########################\n");
        mode=enterInt(1,6);
        if(mode==1){
            maze(inputMode);
            mode=0;
        }

        if(mode==2){
            selectLevel(inputMode);
            mode=0;
        }
        
        if(mode==3){
            guide();
            mode=0;
        }

        if(mode==4){
            input(&inputMode);
            mode=0;
        }

        if(mode==5) return 0;

        if(mode==6){
            credit();
            mode=0;
        }

    }

    return 0;
}
