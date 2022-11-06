#ifndef snake_include_file
#define snake_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define snake_width 32
#define snake_height 32
#define snake_size 1026
#define snake ((gfx_sprite_t*)snake_data)
extern unsigned char snake_data[1026];

#ifdef __cplusplus
}
#endif

#endif
