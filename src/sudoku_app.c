#include <graphx.h>
#include <tice.h>
#include <sys/util.h>

#include <stdbool.h>
#include <string.h>

#include "gfx.h"

#define pos(x, y) ((y) * 9 + (x))

static void redraw_entire_screen(int x, int y, unsigned char *board, unsigned char *board_initial_state);
static void draw_sel(int x, int y);
static bool validate_num_insert(int x, int y, unsigned char * board, int n);
static unsigned char * load_random_board();
static bool all81(unsigned char *arr);

// most values here are experimental (aka magic numbers), sowwy

// The amount of space on the left/right sides of the screen when
// trying to fit the largest possible square inside the screen.
#define SQUARE_LRMARGIN ((LCD_WIDTH - LCD_HEIGHT) / 2)
#define CELL_WIDTH (LCD_HEIGHT / 9)

#define NUM_SUDOKU_BOARDS 32

#define CELL_NUM_PADDING ((CELL_WIDTH - 8) / 2)

// The amount of pixels on the left/right
#define BOX_THICKNESS 2
#define CHAR_HEIGHT 8

// How many pixels need to be subtracted from the lines due to integer
// divison.
#define MAGIC_INTEGER_ERR 7

// Sudoku data will be kept in sudoku_data.c

/* Draws a grayed box representing the user's selection */
static void draw_sel(int x, int y) {
    gfx_SetColor(GRAY1);
    gfx_FillRectangle(SQUARE_LRMARGIN*2 + x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
}

/* Yep, redraws the entire board. Does not fill the screen. */
static void redraw_entire_screen(int curx, int cury, unsigned char *board, unsigned char *board_initial_state) {
    gfx_FillScreen(WHITE);
    draw_sel(curx, cury);
    gfx_SetColor(BLACK);

    for (int i = 0; i < 10; ++i) {
        if (board_initial_state[pos(curx, cury)] == 0 && validate_num_insert(curx, cury, board, i)) {
            gfx_SetTextXY(5, 5 + i * CHAR_HEIGHT);
            gfx_PrintChar('0' + i); //thanks, C standard for letting me do this
        }
    }

    for (int i = 0; i <= 9; ++i) {
        // the offset uses the smaller dimension, which in this
        // case is the height of the screen.
        int offset = LCD_HEIGHT / 9 * i;
        gfx_HorizLine(SQUARE_LRMARGIN*2, offset, LCD_WIDTH - SQUARE_LRMARGIN * 2 - MAGIC_INTEGER_ERR);
        gfx_VertLine(offset + SQUARE_LRMARGIN*2, 0, LCD_HEIGHT - MAGIC_INTEGER_ERR);
    }
    
    // draw the thick lines to recognize the boxes
    for (int i = 0; i <= 9; i += 3) {
        int offset = LCD_HEIGHT / 9 * i;
        //horiz
        gfx_FillRectangle(SQUARE_LRMARGIN*2 - BOX_THICKNESS, offset - BOX_THICKNESS, 
            LCD_HEIGHT + BOX_THICKNESS * 2 - MAGIC_INTEGER_ERR, BOX_THICKNESS * 2);
        //vert
        gfx_FillRectangle(offset + SQUARE_LRMARGIN*2 - BOX_THICKNESS, 0,
            BOX_THICKNESS * 2, LCD_HEIGHT + BOX_THICKNESS * 2 - MAGIC_INTEGER_ERR - 1);
    }
    
    for (int y = 0; y < 9; ++y) {
        for (int x = 0; x < 9; ++x) {
            int i = pos(x, y);
            if (board[i] == 0) // empty cells are repr with 0. Don't render
                continue;
            char digit = '0' + board[i];
            gfx_SetTextFGColor( (board_initial_state[i] == 0) ? RED : BLACK );
            
            gfx_SetTextXY(x * CELL_WIDTH + SQUARE_LRMARGIN*2 + CELL_NUM_PADDING, y * CELL_WIDTH + CELL_NUM_PADDING);
            gfx_PrintChar(digit);
        }
    }
}

static unsigned char * load_random_board() {
    extern unsigned char sudoku_boards[32][9][9];
    int i = random() % NUM_SUDOKU_BOARDS;
    return (unsigned char *) sudoku_boards[i];
}

void sudoku_mainloop(void) {
    int curx = 0;
    int cury = 0;

    unsigned char *board;
    unsigned char boardcpy[9][9]; // used to identify what was there originally

    board = load_random_board();
    memcpy(boardcpy, board, sizeof(boardcpy));

    for (;;) {
        redraw_entire_screen(curx, cury, board, (unsigned char *) boardcpy);
        gfx_SwapDraw();
        uint8_t key;
        while (!(key = os_GetCSC()))
            ;
        if (key == sk_Up) cury--;
        else if (key == sk_Down) cury++;
        else if (key == sk_Left) curx--;
        else if (key == sk_Right) curx++;
        else if (key == sk_Clear) return;   // to quit the game prematurely
        else {
            int num_to_insert = -1;
            switch (key) {
                case sk_Del:
                    num_to_insert = 0;
                    break;
                case sk_1:
                    num_to_insert = 1;
                    break;
                case sk_2:
                    num_to_insert = 2;
                    break;
                case sk_3:
                    num_to_insert = 3;
                    break;
                case sk_4:
                    num_to_insert = 4;
                    break;
                case sk_5:
                    num_to_insert = 5;
                    break;
                case sk_6:
                    num_to_insert = 6;
                    break;
                case sk_7:
                    num_to_insert = 7;
                    break;
                case sk_8:
                    num_to_insert = 8;
                    break;
                case sk_9:
                    num_to_insert = 9;
                    break;
                
            }
            // basically:
            //   the number they pressed needs to be valid
            //   in order to place in this cell, they need to have replaced something they put down or it must already be empty
            //   and lastly, they need to be able to put it there to begin with (no row, col, or box duplicates)
            if (num_to_insert > 0 && (boardcpy[cury][curx] == 0 || board[pos(curx, cury)] == 0) && validate_num_insert(curx, cury, board, num_to_insert)) {
                board[pos(curx, cury)] = num_to_insert;
            } else if (num_to_insert == 0 && boardcpy[cury][curx] == 0) {
                board[pos(curx, cury)] = 0;
            }
        }

        if (cury < 0) cury = 0;
        else if (cury > 8) cury = 8;
        if (curx < 0) curx = 0;
        else if (curx > 8) curx = 8;

        if (all81(board)) {
            // this means that they completed the game :)
            const char *wonlines[] = {
                "You",
                "solved",
                "the",
                "board!",
                NULL
            };

            redraw_entire_screen(curx, cury, board, (unsigned char *) boardcpy);
            g_list(wonlines, 5, 5);
            gfx_SwapDraw();

            while (!os_GetCSC())
                ;
            return;
        }
    }
}

/* eat shit, i don't give a fuck if this looks pretty */
static bool validate_num_insert(int x, int y, unsigned char *board, int n) {
    for (int i = 0; i < 9; ++i) {
        int idx;
        idx = pos(x, i);
        if (board[idx] == n) return false;
        idx = pos(i, y);
        if (board[idx] == n) return false;
        idx = pos(x / 3 * 3 + i % 3,
            y / 3 * 3 + i / 3);
        if (board[idx] == n) return false;
    }
    return true;
}

/* checks that all values in an array of size 9*9 are non-zero */
static bool all81(unsigned char *arr) {
    for (int i = 0; i < 9*9; ++i)
        if (!arr[i]) return false;
    return true;
}
