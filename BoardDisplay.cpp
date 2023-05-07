
#include <ncurses.h>
#include<iostream>

//XMAX and YMAX must be a multiple of 9
#define XMAX 72
#define YMAX 36
static char fixchax[] = {1,1,0,0,1,1,
                        0,0,1,1,0,0,
                        1,1,0,0,1,1
                        };

static char fixchao[] = {0,1,1,1,1,0,
                        1,1,0,0,1,1,
                        0,1,1,1,1,0
                        };
static char beegx[] = {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,
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

static char beego[] = {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
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

typedef struct {
    int x;
    int y;
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

//Drawing the chips
void drawChips(int *dest, char chip, Gameboard *board)
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
}

void drawBeegChips(int *dest, char chip, Gameboard *board){
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
 
int main()
{
    Gameboard Board;

    // Centering the gameboard on the screen.
    Board.x = (80-XMAX)/2;
    Board.y = 3;
    int dest[9];
  
    initscr();
    start_color();
    noecho();
    cbreak;
    curs_set(FALSE);        // Disable the console cursor.
    init_pair(1, COLOR_CYAN, COLOR_BLACK); //3x3 color
    init_pair(2, COLOR_WHITE,COLOR_BLACK); // 9x9 color
    init_pair(3, COLOR_YELLOW,COLOR_BLACK);// o color
    init_pair(4, COLOR_GREEN,COLOR_BLACK);//x color

    drawBoard(&Board,2,9);
    drawBoard(&Board,1,3);
    drawChips(dest, 'x', &Board);
    
    getch();
    resetty();
    endwin();

    return 0;
}