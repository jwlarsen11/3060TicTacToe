#include <curses.h>
#include <iostream>
#include <ctype.h>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

//gbd g++ -g

//declaring classes here so they can be used in classes prior to actual declaration
class TicTacBoard;
class GameBoard;


class GameBoard{
public:
    TicTacBoard* entireGame[9];
    int currentBoard;
    int boardWinner;
    
public:
    //defined later because we need TicTacBoard methods which aren't defined at this point in the program.
    GameBoard();
    void displayGameBoard(int);
    
    //getter and setters
    int getCurrentBoard(){
        return currentBoard;
    }
    
    void setCurrentBoard(int newBoard){
        currentBoard = newBoard;
    }
    
};

//note: not a subclass of GameBoard like planned. Ncurses threw a "Segmentation Fault" during runtime
//removing the parent class fixed this.
class TicTacBoard{
private:
    char avaiableSquares[9];
public:
    TicTacBoard() {
        avaiableSquares[0] = '1';
        avaiableSquares[1] = '2';
        avaiableSquares[2] = '3';
        avaiableSquares[3] = '4';
        avaiableSquares[4] = '5';
        avaiableSquares[5] = '6';
        avaiableSquares[6] = '7';
        avaiableSquares[7] = '8';
        avaiableSquares[8] = '9';
    }
    
    void displayBoard(int currentCursorY,int currentCursorX, bool displayAll) {
        int count = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (j == 0)
                    printw(" |");
                else if(displayAll){
                    printw("%c|", avaiableSquares[count]);
                    count++;
                }
                else if(!isdigit(avaiableSquares[count])){
                    printw("%c|", avaiableSquares[count]);
                    count++;
                }
                else{
                    printw(" |");
                    count++;
                }
            }
            
            //update and move the cursor to avoid ovewriting other boards
            currentCursorY++;
            move(currentCursorY,currentCursorX);
        }
        printw(" ------- ");
        refresh();
    }
};

//these function are delcared here so the TicTacBoard class can be used inside them.
//if these are in the traditional spot the TicTacBoard class hasn't been defined yet and an error is thrown
GameBoard :: GameBoard() {
    for (int i = 0; i < 9; i++) {
        entireGame[i] = new TicTacBoard();
    }
    //start in the middle
    currentBoard = 4;
}

void GameBoard :: displayGameBoard(int boardToDisplay) {
    int currentCursorX = 0;
    int currentCursorY = 0;
    
    int currentBoard = 0;
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < 3; i++) {
            move(currentCursorY, currentCursorX);
            //logic will be if the board number is the current board being played display everything else just display a x and o.
            if(currentBoard == boardToDisplay)
            entireGame[k]->displayBoard(currentCursorY,currentCursorX,true);
            else
                entireGame[k]->displayBoard(currentCursorY,currentCursorX,false);
            //set cursor to right of it
            currentCursorX += 7;
            currentBoard++;
        }
        //set cursor to the needed recent column and reset x to the left hand side
        currentCursorY +=4;
        currentCursorX = 0;
    }
}

class logic{
public:
    //start a new game. This function is called when the game is first opened
    void gameOpened(){
        GameBoard game;
        if(menuGameOpened() == 1){
            playGame(game);
        }
        else{
            return;
        }
    }
    
    void playGame(GameBoard game){
        game.displayGameBoard(game.getCurrentBoard());
    }
    
    int menuGameOpened(){
        bool verified = false;
        //loop until valid choice
        while(!verified){
            printw("Quit program or play game?\n1.Play Game\n2.Quit Game\n3. Load Game\n");
            char input;
            input = getch();
            
            switch(input){
                case '1':
                    verified = true; //not needed because return exits function calls but clarity ya know
                    clear(); //clear the screen to display something new
                    return 1;
                    break;
                    
                case '2':
                    verified = true;
                    clear();
                    return 2;
                    break;
                default:
                    printw("Input not regonized. Please enter 1 or 2.\nEnter any key to continue.");
                    getch();
                    clear();
                    break;
                    
            }//end switch
            //need this refresh to print anyway
        }
        refresh();
        return -1;
    }
};


int main() {
    //stuff from example program
    initscr();            /* Start curses mode         */
    raw();                /* Line buffering disabled    */
    keypad(stdscr, TRUE);        /* We get F1, F2 etc..        */
    noecho();            /* Don't echo() while we do getch */
    
    GameBoard test;
    logic bigBrain;
    
    bigBrain.gameOpened();
    
    
    printw("\n\nThanks for playing! Press any key to quit.");
    refresh();
    getch(); //used to stall program to see input
    
    endwin();
    return 0;
}
