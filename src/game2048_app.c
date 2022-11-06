#include "game2048_app.h"
#include <graphx.h>
#include <tice.h>
#include <stdbool.h>
#include <sys/util.h>
#include "gfx.h"

#define LEFTPAD (LCD_WIDTH - LCD_HEIGHT)
#define CELL_WIDTH (LCD_HEIGHT / 4)

static void draw_board(void);
static bool spawn2(void);

static uint24_t board[4][4];

void game2048_mainloop(void) {
    draw_board();
    gfx_SwapDraw();

    while (!os_GetCSC())
        ;
    
}

static void draw_board(void) {
    gfx_FillScreen(WHITE);
    gfx_SetColor(BLACK);
    for (int i = 0; i <= 4; ++i) {
        int v = i * CELL_WIDTH;
        gfx_HorizLine(LEFTPAD, v, LCD_HEIGHT);
        gfx_VertLine(LEFTPAD + v, 0, LCD_HEIGHT);
    }
}

/*I'M CRYING RIGHT NOW. WHY WOULD I EVER WRITE THIS?*/
/* returns false if it failed, true if it succeeded*/
static bool spawn2(void) {
    for (int i = 0; i < 16; ++i)
        if (((uint24_t *)board)[i] == 0) goto has_zero;
    return false;//no place to put a 2
has_zero:
    for (;;) {
        int24_t pos = random() % 16;
        if (((uint24_t *)board)[pos]==0) {
            ((uint24_t*)board)[pos]=2;
            break;
        }
    }
    return true;
}