#include <conio.h>
#include <windows.h>
#include <vector>

#include "Random.hpp"

const short font_size = 16;
const short max_speed = 2;
const short height = 45;
const short width = 160;

class RainDrop {
  short length;
  short speed;
  short x;
  short y;

 public:
  RainDrop(std::vector<RainDrop>& input_drops);

  short display(char input_speed, char input_screen[],
                unsigned short input_colors[]);
  short get_x();
};

int main() {
  short speed = 1;
  char screen[height * width];
  unsigned short colors[height * width];

  for (int a = 0; a < height * width; a += 1) {
    screen[a] = 32;
    colors[a] = 0;
  }

  DWORD dword;
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

  SetConsoleScreenBufferSize(handle, {width, height});

  SMALL_RECT rect = {0, 0, width - 1, height - 1};
  SetConsoleWindowInfo(handle, 1, &rect);

  CONSOLE_FONT_INFOEX font;
  font.cbSize = sizeof(font);
  font.nFont = 0;
  font.dwFontSize.X = font_size;
  font.dwFontSize.Y = font_size;
  SetCurrentConsoleFontEx(handle, 0, &font);

  std::vector<RainDrop> drops;

  while (1) {
    speed += 1;

    if (max_speed < speed) {
      speed = 1;
    }

    if (0 != random(3, 0)) {
      drops.push_back(RainDrop(drops));
    }

    for (short a = 0; a < drops.size(); a += 1) {
      if (height < drops[a].display(speed, screen, colors) or
          width < drops[a].get_x()) {
        drops.erase(a + drops.begin());
        a -= 1;
      }
    }

    WriteConsoleOutputAttribute(handle, colors, height * width, {0, 0}, &dword);
    WriteConsoleOutputCharacter(handle, screen, height * width, {0, 0}, &dword);

    Sleep(25);
  }

  _getch();
}

RainDrop::RainDrop(std::vector<RainDrop>& input_drops)
    : speed(random<short>(max_speed, 1)), length(random(32, 16)), y(0) {
  x = -1;
  short xx = random(width - 1, 0);

  for (short a = 0; a < input_drops.size(); a += 1) {
    if (xx == input_drops[a].get_x()) {
      a = 0;
      xx = random(width - 1, 0);
    }
  }

  x = xx;
}

short RainDrop::display(char input_speed, char input_screen[],
                        unsigned short input_colors[]) {
  if (input_speed <= speed) {
    for (short a = min(height - 1, y); a >= max(y - length, 0); a -= 1) {
      char character = 32;

      if (length >= 1 + y - a) {
        character = random(127, 33);
      }

      if (a == y) {
        input_colors[x + a * width] = 15;
      } else if (a == y - 1) {
        input_colors[x + a * width] = 7;
      } else if (a == y - 2) {
        input_colors[x + a * width] = 8;
      } else if (a == y - 3) {
        input_colors[x + a * width] = 2;
      } else {
        input_colors[x + a * width] = 10;
      }

      input_screen[x + a * width] = character;
    }
    y += 1;
  }

  return 1 + y - length;
}

short RainDrop::get_x() { return x; }