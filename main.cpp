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
    bool checkGameOver();
    
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

    char getWinner()
    {
        return winner;
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
       // entireGame[i]->fill();
        
    }
    //start in the middle
    currentBoard = 4;
    currentTurn = 1;
    boardWinner = false;
}

bool GameBoard :: checkGameOver()
    {
        bool completed = false;
        bool gameOver = false;

        int index = 0;
        int maxCompares = 3;
        int compares = 0;

        //horizontals
        while (!gameOver && compares < maxCompares) {
            //notice that string compare was not used. Because we're comparing 2 chars == is fine.
            if (entireGame[index]->getWinner() == entireGame[index + 1]->getWinner() && entireGame[index]->getWinner() == entireGame[index + 2]->getWinner()) {
                completed = true;
                if (entireGame[index]->getWinner() == 'x') {
                    boardWinner = 1;
                }
                else {
                    boardWinner = 2;
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
            if (entireGame[index]->getWinner() == entireGame[index + 3]->getWinner() && entireGame[index]->getWinner() == entireGame[index + 6]->getWinner()) {
                completed = true;
                if (entireGame[index]->getWinner() == 'x') {
                    boardWinner = 1;
                }
                else {
                    boardWinner = 2;
                }
            }
            //next column
            index += 1;
            compares++;
        }//end vertical check

        //diagonals are not related so 2 if statements will do.
        //could be used in a larger board with [0],[counter*(sizeOfBoard+1)]
        if (entireGame[0]->getWinner() == entireGame[4]->getWinner() && entireGame[0]->getWinner() == entireGame[8]->getWinner()) {
            completed = true;
            if (entireGame[0]->getWinner() == 'x') {
                boardWinner = 1;
            }
            else {
                boardWinner = 2;
            }
        }
        if (entireGame[2]->getWinner() == entireGame[4]->getWinner() && entireGame[0]->getWinner() == entireGame[6]->getWinner()) {
            completed = true;
            if (entireGame[2]->getWinner() == 'x') {
                boardWinner = 1;
            }
            else {
                boardWinner = 2;
            }
        }
        return completed;
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
                entireGame[currentBoard]->displayBoard(currentCursorY,currentCursorX,true);
            else
                entireGame[currentBoard]->displayBoard(currentCursorY,currentCursorX,false);
            //set cursor to right of it
            currentCursorX += 7;
            currentBoard++;
        }
        //set cursor to the needed recent column and reset x to the left hand side
        currentCursorX = 0;
        currentCursorY +=4;
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

//Class that handles all of the display needs of the Gameboard and therefore needs to inherit properties of the Gameboard
class Display: protected TicTacBoard{
  private:
  int XMAX = 72;
  int YMAX = 36;
  char fixchax[18] = {1,1,0,0,1,1,
                    0,0,1,1,0,0,
                    1,1,0,0,1,1
                    };

  char fixchao[18] = {0,1,1,1,1,0,
                    1,1,0,0,1,1,
                    0,1,1,1,1,0
                   };
  char beegx[160] = {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
                  1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
                  0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                  0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                  0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                  0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                  0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                  0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                  1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
                  1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1
                  };

  char beego[160] = {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                  1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,
                  1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                  1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                  1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,
                  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                  0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0
                  };
  public:
    void drawGrid(GameBoard *board, int color, int grid)      // Draw gameboard
  {
      int i, j;
      attron(COLOR_PAIR(color));
      for(i=0; i<=YMAX; i++)
          for(j=0; j<=XMAX; j++)
          {
              if (i == 0)   // Firs row of the gameboard.
              {
                  if (j == 0)              // Upper-Left corner.
                      mvaddch(board->y+i, board->x+j, ACS_ULCORNER);
                 else if (j == XMAX)     // Upper-Right corner.
                     mvaddch(board->y+i, board->x+j, ACS_URCORNER);
                 else if (j % (XMAX / grid) == 0)   // Middle column.
                      mvaddch(board->y+i, board->x+j, ACS_TTEE);
                 else                    // Normal lines.
                      mvaddch(board->y+i, board->x+j, ACS_HLINE);
              }
            
              else if (i % (YMAX / grid) == 0 && i != YMAX)   // Dividers.
              {
                if (j == 0)             // First column.
                    mvaddch(board->y+i, board->x+j, ACS_LTEE);
                else if (j == XMAX)     // Last column.
                    mvaddch(board->y+i, board->x+j, ACS_RTEE);
                else if (j % (XMAX / grid) == 0)   // Middle column.
                    mvaddch(board->y+i, board->x+j, ACS_PLUS);
                else                    // Normal lines.
                    mvaddch(board->y+i, board->x+j, ACS_HLINE);
              }
            
              else if (i == YMAX)             // Last row
              {
                if (j == 0)              // Lower-Left corner.
                    mvaddch(board->y+i, board->x+j, ACS_LLCORNER);
                else if (j == XMAX)     // Lower-Right corner.
                    mvaddch(board->y+i, board->x+j, ACS_LRCORNER);
                else if (j % (XMAX / grid) == 0)   // Middle column.
                    mvaddch(board->y+i, board->x+j, ACS_BTEE);
                else                    // Normal lines.
                    mvaddch(board->y+i, board->x+j, ACS_HLINE);
              }
            
              else if (j % (XMAX / grid) == 0)   // Middle lines.
              {
                mvaddch(board->y+i, board->x+j, ACS_VLINE);
              }
        }
    attroff(COLOR_PAIR(color));
  }//drawGrid

  //Drawing the chips
  void drawChips(int *dest, char chip, GameBoard *board)
  {
    int i, j;
    int y, x;
    
    for (i=0; i<3; i++)     // Chip height
        for (j=0; j<6; j++)     // Chip width
        {
            y = YMAX/9 * dest[0] + i + board->y + 1;
            x = XMAX/9 * dest[1] + j + board->x + 2;
            if (fixchax[j+6*i] == 1 && chip == 'x')          // If X chip...
            {
                attron(COLOR_PAIR(4));
                mvaddch(y, x, '#');
                attroff(COLOR_PAIR(4));
            }
            
            else if (fixchao[j+6*i] == 1 && chip == 'o')     // If O chip...
            {
                attron(COLOR_PAIR(3));
                mvaddch(y, x, '#');
                attroff(COLOR_PAIR(3));
            }
            
            else    // Print spaces.
            {
                //
            }
        }
  }//drawChips

  void drawBeegChips(int *dest, char chip, GameBoard *board){
    int i, j;
    int y, x;
    
    for (i=0; i<10; i++)     // Chip height
        for (j=0; j<16; j++)     // Chip width
        {
            y = YMAX/3 * dest[0] + i + board->y + 2;
            x = XMAX/3 * dest[1] + j + board->x + 5;
            if (fixchax[j+16*i] == 1 && chip == 'x')          // If X chip...
            {
                attron(COLOR_PAIR(4));
                mvaddch(y, x, '#');
                attroff(COLOR_PAIR(4));
            }
            
            else if (fixchao[j+16*i] == 1 && chip == 'o')     // If O chip...
            {
                attron(COLOR_PAIR(3));
                mvaddch(y, x, '#');
                attroff(COLOR_PAIR(3));
            }
            
            else    // Print spaces.
            {
                //
            }
        }
  }//drawBeegChips

  void drawBoard(GameBoard *board){
    init_pair(1, COLOR_CYAN, COLOR_BLACK); //3x3 color
    init_pair(2, COLOR_WHITE,COLOR_BLACK); // 9x9 color
    init_pair(3, COLOR_YELLOW,COLOR_BLACK);// o color
    init_pair(4, COLOR_GREEN,COLOR_BLACK);//x color

    //drawing the grid
    drawGrid(&board,2,9);
    drawGrid(&board,1,3);    
  } //drawBoard

//converts the information from the boards to be used in the display properly
void convertToCoordsBeeg(int boardNum, int *coord){
  switch(boardNum){
    case 0:
    coord[0] = 0;
    coord[1] = 0;
    break;
    case 1:
    coord[0] = 0;
    coord[1] = 1;
    break;
    case 2:
    coord[0] = 0;
    coord[1] = 2;
    break;
    case 3:
    coord[0] = 1;
    coord[1] = 0;
    break;
    case 4:
    coord[0] = 1;
    coord[1] = 1;
    break;
    case 5:
    coord[0] = 1;
    coord[1] = 2;
    break;
    case 6:
    coord[0] = 2;
    coord[1] = 0;
    break;
    case 7:
    coord[0] = 2;
    coord[1] = 1;
    break;
    case 8:
    coord[0] = 2;
    coord[1] = 2;
    break;
  }
}//convertToCoordsBeeg

void startLittle(int boardNum, int *coord){
  switch(boardNum){
    case 0:
    coord[0] = 0;
    coord[1] = 0;
    break;
    case 1:
    coord[0] = 0;
    coord[1] = 3;
    break;
    case 2:
    coord[0] = 0;
    coord[1] = 6;
    break;
    case 3:
    coord[0] = 3;
    coord[1] = 0;
    break;
    case 4:
    coord[0] = 3;
    coord[1] = 3;
    break;
    case 5:
    coord[0] = 3;
    coord[1] = 6;
    break;
    case 6:
    coord[0] = 6;
    coord[1] = 0;
    break;
    case 7:
    coord[0] = 6;
    coord[1] = 3;
    break;
    case 8:
    coord[0] = 6;
    coord[1] = 6;
    break;
  }
}//startLittle

  void addLittle(int *coord, int place){
    switch(place){
      case 0:
      break;
      case 1:
      coord[1] += 1;
      break;
      case 2:
      coord[1] += 2;
      break;
      case 3:
      coord[0] += 1;
      break;
      case 4:
      coord[0] += 1;
      coord[1] += 1;
      break;
      case 5:
      coord[0] += 1;
      coord[1] += 2;
      break;
      case 6:
      coord[0] += 2;
      break;
      case 7:
      coord[0] += 2;
      coord[1] += 1;
      break;
      case 8:
      coord[0] += 2;
      coord[1] += 2;
      break;
    }
  }//addLittle

int convertLittle(int *coord,int boardNum, int place){
  startLittle(boardNum, coord);
  addLittle(coord, place);
  return 0;
}//convertLittle

};//Display Class

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
    int turn = 2;
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
        printw("Associated Board has been completed! Where would you like to go?\n");
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                    if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
                    menuForPlayingGame(game);
                }
                break;
            default:
                printw("Input not recognized! Try Again!");
                ifBoardAlreadyWon(game);
                break;
        }
    }
    
    int menuForPlayingGame(GameBoard& game){
        /*this function will take input and detect what is pressed. If a number is pressed it needs to check (or call function) that sees in the number pressed is a valid board space. Display should be the spots availiable in the current square, save, load, quit.
         */
        game.displayGameBoard(currentBoardNum);
        refresh();
        
        if(!game.checkGameOver())
        {
            if (turn == 1){
                printw("Player 1 wins!");
            }
            else if(turn ==2){
                printw("Player 2 wins!");
                
            }
            return turn;
        }
        
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
                            if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
                            if(turn == 1)
                                turn = 2;
                            else
                                turn = 1;
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
