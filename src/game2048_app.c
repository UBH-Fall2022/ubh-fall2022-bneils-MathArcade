#include "game2048_app.h"
#include <graphx.h>
#include <tice.h>
#include <stdbool.h>
#include <sys/util.h>
#include <stdio.h>
#include <string.h>
#include "gfx.h"

#define LEFTPAD (LCD_WIDTH - LCD_HEIGHT)
#define CELL_WIDTH (LCD_HEIGHT / 4)
//AGHAGOHAHOA I CANT DO IT ANYMORE END MY PAINNNN
static void draw_board(void);
static bool spawn2(void);

static uint24_t board[4][4];

//these functions make it slightly more inefficient, but allow us to only implement the algorithm once.
static void reflectboard_oververt(void);
//static void reflectboard_overhor(void);
static void reflectboard_rot90(void);
static void swap(uint24_t *a, uint24_t *b);
static int shl(void);

void game2048_mainloop(void) {
    memset(board, 0, sizeof(board)); // we can't allow it to retain state since we arent already overwriting it like we were in snake
    spawn2();   

    uint24_t hiscore=0;

    for (;;) {
        uint24_t key;

        draw_board();
        gfx_SwapDraw();

        while (!(key = os_GetCSC()))
            ;
        
        if (key == sk_Left) {
            hiscore+=shl();
        }
        else if (key == sk_Right) {
            reflectboard_oververt();
            hiscore+=shl();
            reflectboard_oververt();
        }
        else if (key == sk_Up) {
            reflectboard_rot90();
            reflectboard_rot90();
            reflectboard_rot90();
            hiscore+=shl();
            reflectboard_rot90();
        }
        else if (key == sk_Down) {
            reflectboard_rot90();//360 rot cancels out, yo
            hiscore+=shl();
            reflectboard_rot90();
            reflectboard_rot90();
            reflectboard_rot90();
        }
        else if (key == sk_Clear) {
            return;
        } else {
            continue;
        }
        if (!spawn2()) break; // ded
    }
    draw_board();
    gfx_SetTextFGColor(GREEN);
    gfx_PrintStringXY("Your ", 10, 92);
    gfx_PrintStringXY("score is",10,100);
    char s[10];
    sprintf(s,"%d",hiscore); 
    gfx_PrintStringXY(s, 10, 108);
    gfx_SwapDraw();

    usleep(1000000);//1sec delay
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
    gfx_SetTextFGColor(BLACK);
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            int py = y * CELL_WIDTH + 5 + (CELL_WIDTH - 8) / 2;
            int px;
            int n = board[y][x];
            if (!n) continue;
            char str_n[16];
            sprintf(str_n, "%d", n);
            px = x * CELL_WIDTH + LEFTPAD + 5 + (CELL_WIDTH - gfx_GetStringWidth(str_n)) / 2;
            gfx_PrintStringXY(str_n, px, py);
        }
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

static void swap(uint24_t *a, uint24_t *b) {
    uint24_t t = *a;
    *a = *b;
    *b = t;
}

static void reflectboard_oververt(void) {
    for (int y = 0; y < 4; ++y) {
        swap(&board[y][0], &board[y][3]);
        swap(&board[y][1], &board[y][2]);
    }
}
/*
static void reflectboard_overhor(void) {
    for (int x = 0; x < 4; ++x) {
        swap(&board[0][x], &board[3][x]);
        swap(&board[1][x], &board[2][x]);
    }
}*/

static int shl(void) {
    int addscore=0;//score to add to the total from this shift*
    for (int y = 0; y < 4; ++y) {
        bool firstrun=true;
doitagain:
        //shift everything to the left w/o combining em, we just want the zeros on the right,
        //and the numbers on the left. We also need to preserve the order, though.
        for (int x = 0; x < 3; ++x) {
            if (!board[y][x]) {
                for (int i = x+1; i < 4; ++i) {
                    if (board[y][i]) {
                        swap(&board[y][i],&board[y][x]);
                        break;
                    }
                }
            }
        }
        if (!firstrun) continue;
        int x = 0;
        while (x < 3) {
            if (board[y][x] == board[y][x+1]) {
            addscore += (board[y][x] *= 2);
            board[y][x+1] = 0;
            x+=2;
            }else x++;
        }
        firstrun=false;
        goto doitagain;
    }
    return addscore;
}

//0 1 2 3
//4 5 6 7
//8 9 A B
//C D E F

//THIS CODE IS FUCKINGINSANE. I WAS GOING TO DO IT MANUALLY BUT I CAUGHT THE PATTERN!
static void reflectboard_rot90(void) {
    uint24_t temp[4][4];

    for (int a = 0; a < 4; ++a) {
        for (int b = 0; b < 4; ++b) {
            temp[b][3-a]=board[a][b];
        }
    }
    memcpy(board,temp,sizeof(temp));
}
/*
    temp[0][3]=board[0][0];
    temp[1][3]=board[0][1];
    temp[2][3]=board[0][2];
    temp[3][3]=board[0][3];
    
    temp[0][2]=board[1][0];
    temp[1][2]=board[1][1];
    temp[2][2]=board[1][2];
    temp[3][2]=board[1][3];

    temp[0][1]=board[2][0];
    temp[1][1]=board[2][1];
    temp[2][1]=board[2][2];
    temp[3][1]=board[2][3];

    temp[0][0]=board[3][0];
    temp[1][0]=board[3][1];
    temp[2][0]=board[3][2];
    temp[3][0]=board[3][3];
*/
