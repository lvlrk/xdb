#define RELEASE // define or undefine to toggle
#ifdef RELEASE
#define VERSION_MAX 0
#define VERSION_MIN 3

#include <iostream>
#include <string>
#include <fmt/core.h>
#include <vector>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define STYLE_COUNT 11
std::string styles[STYLE_COUNT] = {"ashes", "candy", "cyber",
	"enefete", "lavanda", "terminal",
	"bluish", "cherry", "dark",
	"jungle", "sunny"};
int styleIdx = 2;

const int fontSize = 20;

#define CHANGE_STYLE GuiLoadStyle(TextFormat("styles/%s.rgs", styles[styleIdx].c_str()));\
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

int Gui(int argc, char **argv) {
  InitWindow(800, 600, "xdb");

  GuiLoadStyle(TextFormat("styles/%s.rgs", styles[styleIdx].c_str()));
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

  while(!WindowShouldClose()) {
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    if(IsKeyPressed(KEY_LEFT)) {
      if(styleIdx - 1 < 0) styleIdx = STYLE_COUNT - 1;
      else styleIdx--;

      CHANGE_STYLE;
    }
    if(IsKeyPressed(KEY_RIGHT)) {
      if(styleIdx + 1 > STYLE_COUNT - 1) styleIdx = 0;
      else styleIdx++;

      CHANGE_STYLE;
    }

    BeginDrawing();
		  DrawText(TextFormat("styles/%s.rgs", styles[styleIdx].c_str()),
			  0, 0, 20, GRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

int main(int argc, char **argv)
{
  const std::string usage = "Usage: xdb [-vg]\n"
    "\txdb [--help]\n";
  const std::string help = "Usage: xdb [OPTION...] [FILE...]\n"
    "The best FOSS viewer\n\n"
    "      --help      display this help and exit\n"
    "  -v, --version   output version information and exit\n"
    "  -g, --gui       open gui mode\n\n"
    "Report bugs to https://github.com/lvlrk/xdb/issues\n";

  if(argc < 2) {
    std::cerr << usage;

    return 1;
  }

  std::string cOpt;

  for(int i = 1; i < argc; i++) {
    cOpt = std::string(argv[i]);

    if(cOpt == "--help") {
      std::cout << help;

      return 0;
    }
    if(cOpt == "-v" || cOpt == "--version") {
      std::cout << fmt::format("xdb-{}.{}\n", VERSION_MAX, VERSION_MIN);

      return 0;
    }
    if(cOpt == "-g" || cOpt == "--gui") {
      return Gui(argc, argv);
    }
  }

  return 0;
}
#endif

#ifndef RELEASE
#include "xdb.h"

int main(int argc, char **argv)
{
  Xdb x;

  x.PushBackFilename("Makefile");
  // x.PushBackFilename("xdb");

  x.WriteToFile("debug.xdb");

  return 0;
}
#endif
