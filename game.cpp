#include "game.hpp"

using namespace blit;

static const int WIDTH = 128;
static const int HEIGHT = 128;

// Two buffers we will swap each generation
static bool buffer_a[WIDTH * HEIGHT];
static bool buffer_b[WIDTH * HEIGHT];
static bool *grid = buffer_a;   // current state
static bool *next = buffer_b;   // next state being written

static inline int idx(int x, int y) { return x + y * WIDTH; }

static uint32_t last_update = 0;
static const uint32_t UPDATE_INTERVAL = 100;

void init() {
    set_screen_mode(ScreenMode::hires);

    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            grid[idx(x,y)] = (blit::random() % 2);
        }
    }
}

// Count Moore neighborhood with toroidal wrap
static inline int count_neighbors(int x, int y) {
    int count = 0;
    // precompute wrapped coordinates to avoid repeated modulo
    int xm1 = (x == 0 ? WIDTH - 1 : x - 1);
    int xp1 = (x == WIDTH - 1 ? 0 : x + 1);
    int ym1 = (y == 0 ? HEIGHT - 1 : y - 1);
    int yp1 = (y == HEIGHT - 1 ? 0 : y + 1);

    // Unrolled neighbor accesses (8)
    count += grid[idx(xm1, ym1)];
    count += grid[idx(x,   ym1)];
    count += grid[idx(xp1, ym1)];
    count += grid[idx(xm1, y  )];
    count += grid[idx(xp1, y  )];
    count += grid[idx(xm1, yp1)];
    count += grid[idx(x,   yp1)];
    count += grid[idx(xp1, yp1)];
    return count;
}

void update(uint32_t time) {
    if(blit::buttons.released & Button::MENU) {
        for(int x = 0; x < WIDTH; x++) {
            for(int y = 0; y < HEIGHT; y++) {
                grid[idx(x,y)] = (blit::random() % 2);
            }
        }
    }
    if(time - last_update < UPDATE_INTERVAL) return;
    last_update = time;

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            int n = count_neighbors(x, y);
            bool alive = grid[idx(x,y)];
            next[idx(x,y)] = (alive ? (n == 2 || n == 3) : (n == 3));
        }
    }

    // swap buffers instead of copying
    std::swap(grid, next);
}

void render(uint32_t time) {
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.pen = Pen(255, 255, 255);

    // Batch horizontal runs of live cells into rectangle fills to reduce per-pixel overhead
    for(int y = 0; y < HEIGHT; y++) {
        int run_start = -1;
        for(int x = 0; x < WIDTH; x++) {
            bool alive = grid[idx(x,y)];
            if(alive) {
                if(run_start == -1) run_start = x; // start new run
            } else if(run_start != -1) {
                // end previous run
                screen.rectangle(Rect(run_start, y, x - run_start, 1));
                run_start = -1;
            }
        }
        if(run_start != -1) {
            screen.rectangle(Rect(run_start, y, WIDTH - run_start, 1));
        }
    }
}
