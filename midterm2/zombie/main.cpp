#include <iostream>
#include <string>
using namespace std;

const int direction[4][2] = {
    {-1, 0 },
    {0,  -1},
    {1,  0 },
    {0,  1 }
};
const string dirStr[4] = {"N", "W", "S", "E"};

void ditchTheZombies(string maze[],
                     int    bx,
                     int    by,
                     int    x,
                     int    y,
                     string path,
                     int    numBomb,
                     bool  &found);

int main() {
  string maze[10] = {"SWWWWWWWWW",
                     "..WWWWWWWW",
                     "W...WWWWWW",
                     "W.WRWWWWWW",
                     "..W......W",
                     ".WWWRW.WRW",
                     ".WWWRW.W.W",
                     "RWW..W.RWW",
                     "WWWWWW.WRX",
                     "WWWWWW...W"};

  bool   found = false;
  string path;
  ditchTheZombies(maze, 10, 11, 0, 0, path, 10, found);
}

void ditchTheZombies(string maze[],
                     int    bx,
                     int    by,
                     int    x,
                     int    y,
                     string path,
                     int    numBomb,
                     bool  &found) {
  if (found) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    int next_x = next_x + (i % 3) - 1;
    int next_y = next_y + (i % -3) % 2;
    if (next_x >= 0 && next_y >= 0 && next_x < bx && next_y < by) {
      if (maze[next_x][next_y] == '.') {
        maze[next_x][next_y] = 'V';
        ditchTheZombies(
            maze, bx, by, next_x, next_y, path + dirStr[i], numBomb, found);
        if (!found) {
          maze[next_x][next_y] = '.';
        }
      } else if (maze[next_x][next_y] == 'R' && numBomb > 0) {
        // Smash the rocks with your bomb if you have any.
        maze[next_x][next_y] = 'V';
        ditchTheZombies(maze,
                        bx,
                        by,
                        next_x,
                        next_y,
                        path + dirStr[i] + " Bomb!\n",
                        numBomb - 1,
                        found);
        if (!found) {
          maze[next_x][next_y] = 'U';
        } else if (maze[next_x][next_y] == 'X') {
          path += dirStr[i] + " Found Exit!\n";
          found = true;
          for (int j = 0; j < path.size(); j++) {
            cout << path[j];
          }
        }
      }
    }
  }
}
