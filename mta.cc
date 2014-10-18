// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how write text.
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"
#include "threaded-canvas-manipulator.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace rgb_matrix;

// Simple generator that pulses through RGB and White.
class ColorPulseGenerator : public ThreadedCanvasManipulator {
public:
  ColorPulseGenerator(Canvas *m) : ThreadedCanvasManipulator(m) {}
  void Run() {
    uint32_t continuum = 0;
    while (running()) {
      usleep(5 * 1000);
      continuum += 1;
      continuum %= 3 * 255;
      int r = 0, g = 0, b = 0;
      if (continuum <= 255) {
        int c = continuum;
        b = 255 - c;
        r = c;
      } else if (continuum > 255 && continuum <= 511) {
        int c = continuum - 256;
        r = 255 - c;
        g = c;
      } else {
        int c = continuum - 512;
        g = 255 - c;
        b = c;
      }
      canvas()->Fill(r, g, b);
    }
  }
};

int main(int argc, char *argv[]) {
  Color color(255, 255, 255);
  Color color_mn(237, 201, 81);
  Color color_bk(0, 160, 176);
  const char *bdf_font_file = "matrix/fonts/4x6.bdf";
  const char *bdf_font_file_large = "matrix/fonts/7x13O.bdf";
  int rows = 32;
  int chain = 1;
  int x_orig = 0;
  int y_orig = 0;

  /*
   * Load font. This needs to be a filename with a bdf bitmap font.
   */
  Font font;
  Font large_font;

  if (!font.LoadFont(bdf_font_file)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
    return 1;
  }
  if (!large_font.LoadFont(bdf_font_file_large)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file_large);
    return 1;
  }


  /*
   * Set up GPIO pins. This fails when not running as root.
   */
  GPIO io;
  if (!io.Init())
    return 1;
    
  /*
   * Set up the RGBMatrix. It implements a 'Canvas' interface.
   */
  RGBMatrix *canvas = new RGBMatrix(&io, rows, chain);

/*
  bool all_extreme_colors = true;
  all_extreme_colors &= color.r == 0 || color.r == 255;
  all_extreme_colors &= color.g == 0 || color.g == 255;
  all_extreme_colors &= color.b == 0 || color.b == 255;
  if (all_extreme_colors)
    canvas->SetPWMBits(1);
*/
  const int x = x_orig;
  int y = y_orig;

  if (isatty(STDIN_FILENO)) {
    // Only give a message if we are interactive. If connected via pipe, be quiet
    printf("Enter lines. Full screen or empty line clears screen.\n"
           "Supports UTF-8. CTRL-D for exit.\n");
  }

  char line[1024];
  while (fgets(line, sizeof(line), stdin)) {
    //if subway view
   if(strncmp(line, "[subway]", 8) == 0) {
      const size_t last = strlen(line);
      if (last > 0) line[last - 1] = '\0';  // remove newline.
      bool line_empty = strlen(line) == 0;
      canvas->Clear();
      y = y_orig;
      if (line_empty)
        continue;
      int north, south;
      sscanf(line, "[subway]%d,%d", &north, &south);
      char str_north[10];
      char str_south[10];
      sprintf(str_north, "%d", north);
      sprintf(str_south, "%d", south);
      DrawText(canvas, large_font, 1, y + large_font.baseline(), color, str_north);
      y += large_font.height();
      DrawText(canvas, font, 1, y + font.baseline() - 2, color_mn, "min");
      DrawText(canvas, large_font, 17, y + large_font.baseline(), color, str_south);
      y += large_font.height();
      DrawText(canvas, font, 17, y + font.baseline() - 2, color_bk, "min");
      DrawText(canvas, large_font, 17, large_font.baseline(), color_mn, "MN");
      DrawText(canvas, large_font, 1, large_font.baseline() * 2 + 7, color_bk, "BK");
    }
  }

  // Finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
