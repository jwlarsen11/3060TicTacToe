
#include <ncurses.h>
#include<iostream>

//XMAX and YMAX must be a multiple of 9
#define XMAX 72
#define YMAX 36

typedef struct {
    int x;
    int y;
    int fichasant[2];           // Check if the chip is in the same place.
    int fichasjuego[2];         // Number of chips in game for each player.
    char fichas[3][3];          // Chips of the gameboard. (0 -> Empty / 1 -> X / 2 -> O)
    int jugador;                // Actual player.
    int movimientos;            // Number of moves.
    int mover;                  // Is moving any chip?
} Gameboard;


void drawBoard(Gameboard *board, int color, int grid)      // Draw gameboard
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
}//drawBoard

int main()
{
    Gameboard Board;

    // Centering the gameboard on the screen.
    Board.x = (80-XMAX)/2;
    Board.y = 3;

    initscr();
    start_color();
    noecho();
    cbreak;
    curs_set(FALSE);        // Disable the console cursor.
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE,COLOR_BLACK);

    drawBoard(&Board,2,9);
    drawBoard(&Board,1,3);
    
    getch();
    resetty();
    endwin();

    return 0;
}