#ifndef snake_include_file
#define snake_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define snake_width 128
#define snake_height 64
#define snake_size 8194
#define snake ((gfx_sprite_t*)snake_data)
extern unsigned char snake_data[8194];

#ifdef __cplusplus
}
#endif

#endif
