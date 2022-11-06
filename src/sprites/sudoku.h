#ifndef sudoku_include_file
#define sudoku_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sudoku_width 32
#define sudoku_height 32
#define sudoku_size 1026
#define sudoku ((gfx_sprite_t*)sudoku_data)
extern unsigned char sudoku_data[1026];

#ifdef __cplusplus
}
#endif

#endif
