
#include "delay.h"
#include "i2c.h"
#include "init.h"
#include "memory.h"
#include "ssd1315.h"
#include "ssd1315_UGUI.h"
#include "ugui.h"
#include "user_io.h"
#include <stdint.h>

static UG_GUI gui;
uint8_t *bbuf;
void game_init(void);
void game_step(void);
void game_draw(void);
void game_update(void);

// Size of one "cell" on the screen (in pixels)
#define CELL_SIZE   8
// Top margin (for score text)
#define TOP_MARGIN  8

// How many cells we have horizontally and vertically for the game area
#define GRID_W      (SSD1315_W / CELL_SIZE)                     // 128 / 8 = 16
#define GRID_H      ((SSD1315_H - TOP_MARGIN) / CELL_SIZE)      // (64-8)/8 = 7

#define MAX_SNAKE_LENGTH 64

// Game timing:
// - main loop runs every GAME_TICK_MS
// - snake moves every GAME_TICKS_PER_STEP ticks
#define GAME_TICK_MS        10   // ms between loop iterations
#define GAME_TICKS_PER_STEP 12   // 10 * 12 = 120 ms per snake move

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    DIR_UP = 0,
    DIR_RIGHT = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3
} Direction;

static Point     snake[MAX_SNAKE_LENGTH];
static int       snake_length;
static Direction dir;
static Point     apple;
static int       game_over;
static int       score;

// simple pseudo-random generator (LCG)
static uint32_t rnd_state = 1u;
static uint32_t rnd_next(void)
{
    rnd_state = rnd_state * 1664525u + 1013904223u;
    return rnd_state;
}

static void place_apple(void)
{
    while (1) {
        int x = (int)(rnd_next() % GRID_W);
        int y = (int)(rnd_next() % GRID_H);

        int on_snake = 0;
        for (int i = 0; i < snake_length; ++i) {
            if (snake[i].x == x && snake[i].y == y) {
                on_snake = 1;
                break;
            }
        }
        if (!on_snake) {
            apple.x = x;
            apple.y = y;
            break;
        }
    }
}

void game_init(void)
{
    // Start with a 3-block snake in the middle of the grid
    snake_length = 3;
    snake[0].x = GRID_W / 2;
    snake[0].y = GRID_H / 2;
    snake[1].x = snake[0].x - 1;
    snake[1].y = snake[0].y;
    snake[2].x = snake[1].x - 1;
    snake[2].y = snake[1].y;

    dir       = DIR_RIGHT;
    score     = 0;
    game_over = 0;

    rnd_state = 1u; // deterministic start, easy to debug
    place_apple();

    // Turn off LEDs initially
    user_led_write(0, 0);
    user_led_write(1, 0);
    user_led_write(2, 0);
}

static void handle_input(void)
{
    static int prev_btn0 = 0;
    static int prev_btn1 = 0;

    int btn0 = user_btn_read(0); // LEFT
    int btn1 = user_btn_read(1); // RIGHT

    // NOTE: if buttons are active-low, just invert:
    // int pressed0 = !btn0; int pressed1 = !btn1;
    // For now we assume "1 = pressed"
    int pressed0 = btn0;
    int pressed1 = btn1;

    // On rising edge: just pressed now (was 0, now 1)
    if (pressed0 && !prev_btn0) {
        // turn left: dir - 1 (mod 4)
        dir = (Direction)((dir + 3) & 3);
    }
    if (pressed1 && !prev_btn1) {
        // turn right: dir + 1 (mod 4)
        dir = (Direction)((dir + 1) & 3);
    }

    prev_btn0 = pressed0;
    prev_btn1 = pressed1;
}

void game_step(void)
{
    if (game_over) {
        return;
    }

    // current head
    Point head = snake[0];

    // move head one cell in current direction
    if (dir == DIR_UP) {
        head.y -= 1;
    } else if (dir == DIR_DOWN) {
        head.y += 1;
    } else if (dir == DIR_LEFT) {
        head.x -= 1;
    } else if (dir == DIR_RIGHT) {
        head.x += 1;
    }

    // wrap at edges
    if (head.x < 0)         head.x = GRID_W - 1;
    if (head.x >= GRID_W)   head.x = 0;
    if (head.y < 0)         head.y = GRID_H - 1;
    if (head.y >= GRID_H)   head.y = 0;

    // collision with our own body?
    for (int i = 0; i < snake_length; ++i) {
        if (snake[i].x == head.x && snake[i].y == head.y) {
            game_over = 1;
            user_led_write(2, 1); // red = dead :)
            return;
        }
    }

    // move body: from tail to head
    for (int i = snake_length; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    snake[0] = head;

    // apple eaten?
    if (head.x == apple.x && head.y == apple.y) {
        if (snake_length < MAX_SNAKE_LENGTH - 1) {
            snake_length++;
        }
        score++;

        // flash a LED when eating
        user_led_write(0, 1);
        user_led_write(1, 1);
        delay_ms(50);
        user_led_write(0, 0);
        user_led_write(1, 0);

        place_apple();
    }
}

void game_update(void)
{
    static int tick_counter = 0;

    handle_input(); // turn left/right as soon as button is pressed

    if (game_over) {
        return;
    }

    if (++tick_counter >= GAME_TICKS_PER_STEP) {
        tick_counter = 0;
        game_step(); // move snake one cell
    }
}

static void draw_cell(int gx, int gy, int filled)
{
    int x1 = gx * CELL_SIZE;
    int y1 = TOP_MARGIN + gy * CELL_SIZE; // game starts below top margin
    int x2 = x1 + CELL_SIZE - 1;
    int y2 = y1 + CELL_SIZE - 1;

    if (filled) {
        UG_FillFrame(x1, y1, x2, y2, C_WHITE);
    } else {
        UG_DrawFrame(x1, y1, x2, y2, C_WHITE);
    }
}

static void draw_score(void)
{
    char buf[16];
    int s = score;
    char tmp[10];
    int len = 0;

    if (s == 0) {
        tmp[len++] = '0';
    } else {
        while (s > 0 && len < 10) {
            tmp[len++] = (char)('0' + (s % 10));
            s /= 10;
        }
    }
    // reverse into buf
    for (int i = 0; i < len; ++i) {
        buf[i] = tmp[len - 1 - i];
    }
    buf[len] = '\0';

    UG_PutString(0, 0, "Score:");
    UG_PutString(40, 0, buf);
}

void game_draw(void)
{
    // draw snake
    for (int i = 0; i < snake_length; ++i) {
        draw_cell(snake[i].x, snake[i].y, 1);
    }

    // draw apple (as hollow cell)
    draw_cell(apple.x, apple.y, 0);

    // draw score on top line
    draw_score();

    if (game_over) {
        UG_PutString(32, 24, "GAME OVER");
    }
}

int main(void){

    clock_info_t ci;
    ci = init_clock();

    init_i2c3_pa8_pc9(&ci);
    init_ssd1315(I2C3);
    user_io_init();

    bbuf = ssd1315_backbuf();

    ssd1315_set_contrast(1);
    UG_Init(&gui, UG_DrawPixel_SSD1315, SSD1315_W, SSD1315_H);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    user_led_write(0,1); delay_ms(100); user_led_write(0,0);
    user_led_write(1,1); delay_ms(100); user_led_write(1,0);
    user_led_write(2,1); delay_ms(100); user_led_write(2,0);

    game_init();

    for (;;){

        ssd1315_flush();
        ssd1315_clear_back();
        game_update();
        game_draw();

        delay_ms(GAME_TICK_MS);
    }

    return 0;
}

