#include <iostream>
#include <ncurses.h>
using namespace std;

// XMAX y YMAX must be multiple of 9.
#define XMAX 54
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
} GameBoard;

void drawGameboard(GameBoard *Gameboard)      // Draw gameboard
{
    int i, j;

    for(i=0; i<=YMAX; i++)
        for(j=0; j<=XMAX; j++)
        {
            if (i == 0)   // First row of the gameboard.
            {
                if (j == 0)              // Upper-Left corner.
                    mvaddch(Gameboard->y+i, Gameboard->x+j, ACS_ULCORNER);
                else if (j == XMAX)     // Upper-Right corner.
                    mvaddch(Gameboard->y+i, Gameboard->x+j, ACS_URCORNER);
                else if (j % (XMAX / 9) == 0)   // Middle columns.
                    mvaddch(Gameboard->y+i, Gameboard->x+j, ACS_TTEE);
                else                    // Normal lines.
                    mvaddch(Gameboard->y+i, Gameboard->x+j, ACS_HLINE);
            }

            else if (j % (XMAX / 9) == 0)   // Middle lines.
            {
                mvaddch(Gameboard->y+i, Gameboard->x+j, ACS_VLINE);
            }
        }
}//drawGameboard

int main(int argc, char **argv){

    GameBoard Gameboard;

    // Centering the gameboard on the screen.
    Gameboard.x = (80-XMAX)/2;
    Gameboard.y = 3;

        //initialize screen
        initscr();

         drawGameboard(&Gameboard);

        getch();
        endwin();
}
