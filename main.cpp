#include <curses.h>
#include <iostream>
#include <ctype.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

//gbd g++ -g

//declaring classes here so they can be used in classes prior to actual declaration
class TicTacBoard;
class GameBoard;

class GameBoard{
private:
    TicTacBoard* entireGame[9];
    int currentBoard;
    int boardWinner;
    int currentTurn; //player 1 or 2
    
public:
    //defined later because we need TicTacBoard methods which aren't defined at this point in the program.
    GameBoard();
    void displayGameBoard(int);
    char getElementInOneSquare(TicTacBoard& square, int index);
    void setElementInOneSquare(int boardIndex, int squareindex,char newElement);
    TicTacBoard getOneTicTacBoard(int index);
    
    GameBoard& operator=(const GameBoard& other);
    
    
    //getter and setters
    int getCurrentBoardNumber(){
        return currentBoard;
    }
    
    void setCurrentBoard(int newBoard){
        currentBoard = newBoard;
    }
    
    int getBoardWinner(){
        return boardWinner;
    }
    void setBoardWinner(int newWinner){
        boardWinner = newWinner;
    }
    
    int getTurn(){
        return currentTurn;
    }
    void setTurn(int newTurn){
        currentTurn = newTurn;
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
    char getElement(int index){
        return avaiableSquares[index];
    }
    void setElement(int index, char newElemnet){
        avaiableSquares[index] = newElemnet;
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
    
    //****************************
    //for testing purposes shoudl be removed upon logic completion
    //******************************
    void fill(){
        for (int i = 0; i < 9; i++) {
            if(i == 0 || i==2)
                avaiableSquares[i] = 'x';
            else if(i == 5 || i==8)
                avaiableSquares[i] = 'o';
        }
    }
};

//these function are delcared here so the TicTacBoard class can be used inside them.
//if these are in the traditional spot the TicTacBoard class hasn't been defined yet and an error is thrown
GameBoard :: GameBoard() {
    for (int i = 0; i < 9; i++) {
        entireGame[i] = new TicTacBoard();
        entireGame[i]->fill();
        
    }
    //start in the middle
    currentBoard = 4;
    currentTurn = 1;
    boardWinner = false;
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
char GameBoard :: getElementInOneSquare(TicTacBoard& square,int index){
    return  square.getElement(index);
}

void GameBoard :: setElementInOneSquare(int boardIndex, int squareindex,char newElement){
    entireGame[boardIndex]->setElement(squareindex, newElement);
}

TicTacBoard GameBoard :: getOneTicTacBoard(int index){
    return *entireGame[index];
}
/*
//this overloads the '=' operator. Made for the load function
GameBoard& GameBoard::operator=(const GameBoard& other) {
    //see if they already equal each other
    if (this != &other) {
        //set the tic tac board arrays equal to each other
        for(int outer =0; outer<9;outer++){
            for(int index =0; index<9;index++){
                entireGame[outer]->setElement(index, other.entireGame[outer]->getElement(index));
            }
        }
        currentBoard = other.currentBoard;
        currentTurn = other.currentTurn;
        boardWinner = other.boardWinner;
    }
    return *this;
}
*/
class logic{
private:
    GameBoard game;
public:
    //start a new game. This function is called when the game is first opened
    void gameOpened(){
        int selection = menuGameOpened();
        if(selection == 1){
            playGame(game);
        }
        else if (selection == 3){
            loadGame(game);
            playGame(game);
            
        }
        else{
            return;
        }
        
    }
    
    void playGame(GameBoard& game){
        game.displayGameBoard(game.getCurrentBoardNumber());
        
        menuForPlayingGame(game);
        
    }
    
    char menuForPlayingGame(GameBoard& game){
        /*this function will take input and detect what is pressed. If a number is pressed it needs to check (or call function) that sees in the number pressed is a valid board space. Display should be the spots availiable in the current square, save, load, quit.
         */
        
        char input;
        /*
         *********************************************************************************
         This space is reservd for displaying the approiate numbers for a square selection. For now it will just display load or quit
         *********************************************************************************
         */
        
        move(15, 0);
        printw("(1-9)This would be were the available squares are displayed for selection\n(S/s)). Save Game\n(Q/q). Quit Game\nYour Input: \n");
        input = getch();
        
        input = toupper(input);
        
        switch(input){
                //cases for square selection should be before this code
            case 'S':
                printw("S was entered");
                saveGame(game);
                break;
                
            case 'Q':
                printw("Q was entered");
                break;
            default:
                printw("Input not regonized. Please enter 1 or 2.\nEnter any key to continue.");
                getch();
                clear();
                break;
                
        }
        return input;
    }
    
    //method that save a game to a file
    //************************************************************************************************************************************
    void saveGame(GameBoard& game){
        //code taken/inspired from code for geeks link in announcement on canvas.
        // Object to write in file
        ofstream file_obj;
        
        // Opening file in output mode
        file_obj.open("saveFile.txt", ios::out);
        
        //save the variables
        file_obj << game.getCurrentBoardNumber();
        file_obj << game.getBoardWinner();
        file_obj << game.getTurn();
        
        TicTacBoard toChange;
        //save the array values
        for(int outer = 0;outer<9;outer++){
            toChange = game.getOneTicTacBoard(outer);
            for(int inner = 0;inner<9;inner++){
                file_obj << toChange.getElement(inner);
            }
        }
        
        file_obj.close();
        
        return;
    }
    
    
    void loadGame(GameBoard& game){
        //code taken/inspired from code for geeks link in announcement on canvas.
        // Object to read from file
        ifstream file_obj;
        
        // Opening file in input mode
        file_obj.open("saveFile.txt", ios::in);
        
        //online resources were used for a effient way to convert chars to integers
        //variables
        
        char input;
        int intToChange;
        
        //board number
        file_obj >> input;
        intToChange = input - '0'; //this line could be moved into the method call but clarity ya know
        game.setCurrentBoard(intToChange);
        
        //board winner
        file_obj >> input;
        intToChange = input - '0'; //this line could be moved into the method call but clarity ya know
        game.setBoardWinner(intToChange);
        
        //current turn
        file_obj >> input;
        intToChange = input - '0'; //this line could be moved into the method call but clarity ya know
        game.setTurn(intToChange);
        
        //now for the arrays
        for(int outer = 0;outer<9;outer++){
            for(int inner = 0;inner<9;inner++){
                file_obj >> input;
                game.setElementInOneSquare(outer, inner, input);
            }
        }
        
        file_obj.close();
        
        return;
    }
    //************************************************************************************************************************************
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
                case '3':
                    verified = true;
                    loadGame(game);
                    clear();
                    return 3;
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
