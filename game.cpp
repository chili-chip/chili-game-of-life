#include "game.hpp"
#include <cstdlib>
#include <ctime>

using namespace blit;

static const int WIDTH = 128;
static const int HEIGHT = 128;

static bool grid[WIDTH][HEIGHT];
static bool next[WIDTH][HEIGHT];

static uint32_t last_update = 0;
static const uint32_t UPDATE_INTERVAL = 100;

void init() {
    set_screen_mode(ScreenMode::hires);

    // seed random
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            grid[x][y] = (blit::random() % 2);
        }
    }
}

int count_neighbors(int x, int y) {
    int count = 0;
    for(int dx=-1; dx<=1; dx++) {
        for(int dy=-1; dy<=1; dy++) {
            if(dx == 0 && dy == 0) continue; // skip self
            int nx = (x + dx + WIDTH) % WIDTH;
            int ny = (y + dy + HEIGHT) % HEIGHT;
            if(grid[nx][ny]) count++;
        }
    }
    return count;
}

void update(uint32_t time) {
    if(time - last_update < UPDATE_INTERVAL) return;

    last_update = time;

    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            int n = count_neighbors(x, y);
            if(grid[x][y]) {
                next[x][y] = (n == 2 || n == 3);
            } else {
                next[x][y] = (n == 3);
            }
        }
    }

    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            grid[x][y] = next[x][y];
        }
    }
        
}

void render(uint32_t time) {
    screen.pen = Pen(0, 0, 0);
    screen.clear();
    screen.pen = Pen(255, 255, 255);

    for(int x=0; x<WIDTH; x++) {
        for(int y=0; y<HEIGHT; y++) {
            if(grid[x][y]) {
                screen.pixel(Point(x,y));
            }
        }
    }
}
