#include <HT1632.h>
#include <font_tinyfont.h>
#include <images.h>

const char PIN_LED_MATRIX_DATA = D0;
const char PIN_LED_MATRIX_WR = D1;
const char PIN_LED_MATRIX_RD = D2;
const char PIN_LED_MATRIX_CS = D3;

const char *text;
int text_width;
int text_pos;
int next_scroll_at = 0;
bool scroll_repeat = false;

void start_led_matrix() {
  HT1632.begin(PIN_LED_MATRIX_CS, PIN_LED_MATRIX_WR, PIN_LED_MATRIX_DATA);
  HT1632.clear();
}

void handle_led_matrix() {
  if (next_scroll_at == 0 || millis() < next_scroll_at) return;
  if (text_pos > text_width + OUT_SIZE) {
    if (scroll_repeat) {
      text_pos = 0;
    } else {
      next_scroll_at = 0;
      return;
    }
  }

  next_scroll_at = millis() + TEXT_SCROLL_DELAY;
  int x = OUT_SIZE - text_pos;
  int y = 0;
  HT1632.drawText(text, x, y, FONT_TINYFONT, FONT_TINYFONT_END, FONT_TINYFONT_HEIGHT, 0);
  HT1632.render();
  text_pos++;
}

void show_heart() {
  HT1632.begin(PIN_LED_MATRIX_CS, PIN_LED_MATRIX_WR, PIN_LED_MATRIX_DATA);
  // The definitions for IMG_HEART and its width and height are available in
  // images.h.
  // This step only performs the drawing in internal memory.
  HT1632.drawImage(IMG_HEART, IMG_HEART_WIDTH, IMG_HEART_HEIGHT,
                   (OUT_SIZE - IMG_HEART_WIDTH) / 2, 0);
  HT1632.render();  // This updates the display on the screen.
}

void _set_scroll_text(const char *t, bool repeat) {
  text = t;
  text_width = HT1632.getTextWidth(t, FONT_TINYFONT_END, FONT_TINYFONT_HEIGHT);
  text_pos = 0;
  next_scroll_at = millis();
  scroll_repeat = repeat;
}

void scroll_once(const char *t) {
  _set_scroll_text(t, false);
}

void scroll_forever(const char *t) {
  _set_scroll_text(t, true);
}
