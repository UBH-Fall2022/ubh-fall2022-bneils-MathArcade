#include <tice.h>
#include <graphx.h>

static void palette_init() {

}

int main(void) {
	gfx_Begin();
	gfx_SetDrawBuffer();
	palette_init();
	// begin program logic now
	gfx_End();
}
