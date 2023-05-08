#include <curses.h>
#include <iostream>
#include <ctype.h>
#include <string>
//files
#include <iostream>
#include <fstream>
//STL requirements
#include<array> // for array, at(), and for STL requirement, used in tic tac board character array
#include<iterator> // for iterators
using namespace std;

//declaring classes here so they can be used in classes prior to actual declaration
class TicTacBoard;
class GameBoard;

class GameBoard{
private:
    TicTacBoard* entireGame[9];
    int currentBoard;
    int boardWinner;
    
public:
    //defined later because we need TicTacBoard methods which aren't defined at this point in the program.
    GameBoard();
    int currentTurn; //player 1 or 2
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
//this was changed to implement the STL requirement (array), due to time some methods were switched to iterators and others were left with standard c++ syntax.
//example of iterator can be found in the constructor of tictacboard
class TicTacBoard{
private:
    array <char,9> avaiableSquares;
    char winner;
public:
    TicTacBoard() {
        array<char,9>::iterator ptr;
        char value = '1';
        
        for(ptr = avaiableSquares.begin(); ptr <avaiableSquares.end();ptr++){
            *ptr = value;
            value++;
        }
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

    bool checkWin()
    {
        bool completed = false;
        bool gameOver = false;

        int index = 0;
        int maxCompares = 3;
        int compares = 0;

        //horizontals
        while (!gameOver && compares < maxCompares) {
            //notice that string compare was not used. Because we're comparing 2 chars == is fine.
            if (avaiableSquares[index] == avaiableSquares[index + 1] && avaiableSquares[index] == avaiableSquares[index + 2]) {
                completed = true;
                if (avaiableSquares[index] == 'x') {
                    winner = 'x';
                }
                else {
                    winner = 'o';
                }
            }
            //next row
            //could be used with a larger board via a loop for length of board and index+=siseOfBoard
            index += 3;
            compares++;
        }//end horizontal check

        //verticals
        index = 0;
        maxCompares = 3;
        compares = 0;
        while (!gameOver && compares < maxCompares) {
            if (avaiableSquares[index] == avaiableSquares[index + 3] && avaiableSquares[index] == avaiableSquares[index + 6]) {
                completed = true;
                if (avaiableSquares[index] == 'x') {
                    winner = 'x';
                }
                else {
                    winner = 'o';
                }
            }
            //next column
            index += 1;
            compares++;
        }//end vertical check

        //diagonals are not related so 2 if statements will do.
        //could be used in a larger board with [0],[counter*(sizeOfBoard+1)]
        if (avaiableSquares[0] == avaiableSquares[4] && avaiableSquares[0] == avaiableSquares[8]) {
            completed = true;
            if (avaiableSquares[0] == 'x') {
                winner = 'x';
            }
            else {
                winner = 'o';
            }
        }
        if (avaiableSquares[2] == avaiableSquares[4] && avaiableSquares[0] == avaiableSquares[6]) {
            completed = true;
            if (avaiableSquares[2] == 'x') {
                winner = 'x';
            }
            else {
                winner = 'o';
            }
        }
        return completed;
    }
    
    //****************************
    //for testing purposes should be removed upon logic completion
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
    //array<TicTacBoard,9>::iterator ptr;
  //  ptr = entireGame.begin();
    
    for (int i = 0; i < 9; i++) {
        entireGame[i] = new TicTacBoard();
        //****************************
        //for testing purposes should be removed upon logic completion
        //******************************
        //entireGame[i]->fill();
        
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
//this overloads the '=' operator. Made for the load function (potentially)
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

//logic for the game
class logic{
private:
    GameBoard game;
    int currentBoardNum = 4;
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

    void ifBoardAlreadyWon(GameBoard& game)
    {
        refresh();
        printw("Associated Board has been completed! Where would you like to go?");
        char inputChar = getch();
        int input = inputChar - '0';
        switch(input)
        {
            case 1:
                if(game.getOneTicTacBoard(0).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 2:
                if(game.getOneTicTacBoard(1).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 3:
                if(game.getOneTicTacBoard(2).checkWin()== true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 4:
                if(game.getOneTicTacBoard(3).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 5:
                if(game.getOneTicTacBoard(4).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 6:
                if(game.getOneTicTacBoard(5).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 7:
                if(game.getOneTicTacBoard(6).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 8:
                if(game.getOneTicTacBoard(7).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            case 9:
                if(game.getOneTicTacBoard(8).checkWin() == true)
                {
                    printw("Board Already Completed! Please try again!");
                    ifBoardAlreadyWon(game);
                }
                else
                {
                    game.setCurrentBoard(input-1);
                    menuForPlayingGame(game);
                }
                break;
            default:
                printw("Input not recognized! Try Again!");
                ifBoardAlreadyWon(game);
                break;
        }
    }
    
    char menuForPlayingGame(GameBoard& game){
        /*this function will take input and detect what is pressed. If a number is pressed it needs to check (or call function) that sees in the number pressed is a valid board space. Display should be the spots availiable in the current square, save, load, quit.
         */
        game.displayGameBoard(currentBoardNum);
        refresh();
        
        char inputChar = 'M';
        int input = -1;
        move(15, 0);
        printw("Choose Save(S/s)\nQuit(Q/q)\nOr one of the following squares(1-9): ");
        for (int i = 0; i < 9; i++)
        {
            if (game.getOneTicTacBoard(currentBoardNum).getElement(i) != 'x' && game.getOneTicTacBoard(currentBoardNum).getElement(i) != 'o')
            {
                printw("%d ",i+1);
                
            }
        }
        printw("\n");
        inputChar = getch();
        input = inputChar - '0';
        inputChar = toupper(inputChar);
        if(inputChar == 'S')
        {
            printw("S was entered");
            saveGame(game);
            menuForPlayingGame(game);
        }
        else if(inputChar == 'Q')
        {
            printw("Q was entered");
            
        }
        else if(inputChar == '1' || inputChar == '2' || inputChar == '3' || inputChar == '4' || inputChar == '5' || inputChar == '6' || inputChar == '7' || inputChar == '8' || inputChar == '9')
        {
            //input = (int)inputChar;
            
            if(game.getOneTicTacBoard(currentBoardNum).getElement(input-1) != 'x' && game.getOneTicTacBoard(currentBoardNum).getElement(input-1) != 'o')
                {
                    if(game.getTurn() == 1)
                    {
                        game.setElementInOneSquare(currentBoardNum, input-1, 'x');
                        if(game.getOneTicTacBoard(input-1).checkWin())
                        {
                            ifBoardAlreadyWon(game);
                        }
                        else
                        {
                            game.setCurrentBoard(input-1);
                            currentBoardNum = input-1;
                            menuForPlayingGame(game);
                        }
                    }
                    else
                    {
                        game.getOneTicTacBoard(currentBoardNum).setElement(input-1, 'o');
                        if(game.getOneTicTacBoard(input-1).checkWin())
                        {
                            ifBoardAlreadyWon(game);
                        }
                        else
                        {
                            game.setCurrentBoard(input-1);
                            currentBoardNum = input-1;
                            menuForPlayingGame(game);
                        }
                    }
                }
                else
                {
                    printw("Square Already Occupied!");
                    menuForPlayingGame(game);
                }
        }
        else
        {
            printw("Input not regonized. Try again!\n");
            clear();
            menuForPlayingGame(game);
        }
        return input;
    }
    
    
    //method that saves a game to a file
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
    
    
    //method that loads a game. If no game is found it will either error out and start a new game or just load a blank game.
    bool loadGame(GameBoard& game){
        //code taken/inspired from code for geeks link in announcement on canvas.
        // Object to read from file
        ifstream file_obj;
        
        try{ //for try catch requirement.
            // Opening file in input mode
            file_obj.open("saveFile.txt", ios::in);
            
            if(!file_obj.is_open()){
                throw (0);
            }
            else{
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
                return true;
            }
        }
        catch(int error){
            printw("A save file could not be found please choose another option. Press any key to continue");
            getch();
            clear();
            refresh();
            return false;
        }
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
                case '3':
                    
                    if(loadGame(game)){
                        verified = true;
                        clear();
                        return 3;
                    }
                    clear();
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
