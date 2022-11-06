#ifndef sudoku_include_file
#define sudoku_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define sudoku_width 128
#define sudoku_height 64
#define sudoku_size 8194
#define sudoku ((gfx_sprite_t*)sudoku_data)
extern unsigned char sudoku_data[8194];

#ifdef __cplusplus
}
#endif

#endif
