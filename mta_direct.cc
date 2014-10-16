#include "led-matrix.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace rgb_matrix;

int main(int argc, char *argv[]) {
	int rows = 32;
	int chain = 1;
	int x_orig = 0;
	int y_orig = 0;

	rgb_matrix::Font font;
	font.LoadFont("fonts/4x6.bdf");
	GPIO io;
	if (!io.Init())
		return 1;
	

	Color color(255, 255, 0);

	RGBMatrix *canvas = new RGBMatrix(&io, rows, chain);
	bool all_extreme_colors = true;
	all_extreme_colors &= color.r == 0 || color.r == 255;
 	all_extreme_colors &= color.g == 0 || color.g == 255;
 	all_extreme_colors &= color.b == 0 || color.b == 255;
 	if (all_extreme_colors)
		canvas->SetPWMBits(1);

	int x = x_orig;
	int y = y_orig;

	char* line = "nah";
	const size_t last = strlen(line);
	bool line_empty = strlen(line) == 0;
	if ((y + font.height() > canvas->height()) || line_empty) {
		canvas->Clear();
		y = y_orig;
		}
    	DrawText(canvas, font, x, y + font.baseline(), color, line);
    	//y += font.height();

	char readin[15];
	fgets(readin, 10, stdin);
	canvas->Clear();
	delete canvas;

	return 0; 
}
