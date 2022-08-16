#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define BUTTON_PIN  3

#define DATA_PIN    2
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    84

#define BRIGHTNESS          128
#define FRAMES_PER_SECOND  120

CRGB leds_blend[NUM_LEDS];
CRGB leds[NUM_LEDS];
CRGB leds_output[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds_output, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

CRGB main_color =  CRGB(0, 128, 255);
uint8_t current_mode_last = 0;
uint8_t current_mode = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int spinner = 0;


void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 14);

  //mirror each emitter for smoothness
  for (int i = 0; i < NUM_LEDS; i += 12) {
    leds[i + 0] = leds[i + 11];
    leds[i + 1] = leds[i + 10];
    leds[i + 2] = leds[i + 9];
    leds[i + 3] = leds[i + 8];
    leds[i + 4] = leds[i + 7];
    leds[i + 5] = leds[i + 6];
  }
}


void rainbow2()
{
  // FastLED's built-in rainbow generator
  CRGB temp[7];
  fill_rainbow( temp, 7, gHue, 30);

  //mirror each emitter for smoothness
  for (int i = 0; i < 7; i++) {
    leds[i * 12 + 0] = temp[i];
    leds[i * 12 + 1] = temp[i];
    leds[i * 12 + 2] = temp[i];
    leds[i * 12 + 3] = temp[i];
    leds[i * 12 + 4] = temp[i];
    leds[i * 12 + 5] = temp[i];
    leds[i * 12 + 6] = temp[i];
    leds[i * 12 + 7] = temp[i];
    leds[i * 12 + 8] = temp[i];
    leds[i * 12 + 9] = temp[i];
    leds[i * 12 + 10] = temp[i];
    leds[i * 12 + 11] = temp[i];
  }
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void breath()
{
  float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 100;
  CRGB color = main_color;
  color.fadeToBlackBy(val);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void breath2()
{
  for (int i = 0; i < 7; i++) {
    float val = (exp(sin((millis() + i * 570) / 2000.0 * PI)) - 0.36787944) * 100;
    CRGB color = main_color;
    color.fadeToBlackBy(val);

    leds[i * 12 + 0] = color;
    leds[i * 12 + 1] = color;
    leds[i * 12 + 2] = color;
    leds[i * 12 + 3] = color;
    leds[i * 12 + 4] = color;
    leds[i * 12 + 5] = color;
    leds[i * 12 + 6] = color;
    leds[i * 12 + 7] = color;
    leds[i * 12 + 8] = color;
    leds[i * 12 + 9] = color;
    leds[i * 12 + 10] = color;
    leds[i * 12 + 11] = color;
  }
}


void breath3()
{

  uint8_t dimmer2 = beatsin8  (128, 0, 192, 0, 0);
  CRGB color1 = main_color;
  CRGB color2 = main_color;

  color1.fadeToBlackBy(dimmer2);
  color2.fadeToBlackBy(192 - dimmer2);

  for (int i = 0; i < 7; i++) {
    CRGB color;
    if (i % 2 == 0)
      color = color1;
    else
      color = color2;

    leds[i * 12 + 0] = color;
    leds[i * 12 + 1] = color;
    leds[i * 12 + 2] = color;
    leds[i * 12 + 3] = color;
    leds[i * 12 + 4] = color;
    leds[i * 12 + 5] = color;
    leds[i * 12 + 6] = color;
    leds[i * 12 + 7] = color;
    leds[i * 12 + 8] = color;
    leds[i * 12 + 9] = color;
    leds[i * 12 + 10] = color;
    leds[i * 12 + 11] = color;
  }
}

void solid() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = main_color;
  }
}

void loop()
{

#define DEBOUNCE 5
  static bool button_debounced_state = false;
  static bool button_last = false;
  static int button_count = 0;
  bool button = digitalRead(BUTTON_PIN);

  if (button == button_last && button_count < DEBOUNCE) {
    button_count++;
  } else {
    button_count = 0;
  }
  button_last = button;

  if (button_count == DEBOUNCE) {
    button_debounced_state = button;
  }

  static uint32_t button_pressed_time = 0;
  static uint32_t button_released_time = 0;
  static bool button_debounced_state_last = false;

  static int press_counter = 0;

  if (button_debounced_state != button_debounced_state_last) {
    if (button_debounced_state == 0) {
      press_counter++;
      button_pressed_time = millis();
    }
    if (button_debounced_state == 1) {
      button_released_time = millis();
    }
  }

  if (button_debounced_state == 0 && millis() - button_pressed_time > 1000) {
    current_mode = 0;
    press_counter = 0;
  }

  if (button_debounced_state == 1 && millis() - button_released_time > 300 && press_counter > 0) {
    current_mode = press_counter;
    press_counter = 0;
  }

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_MILLISECONDS( 50 ) {
    spinner++;
    if (spinner > 11)
      spinner = 0;

  }
  button_debounced_state_last = button_debounced_state;

  if ( current_mode_last != current_mode && current_mode != 0 &&  current_mode_last != 0) {
    for (int i = 0; i < NUM_LEDS ; i++) {

      leds_blend[i] = leds[i];

    }
  }



  switch (current_mode) {
    case 1 :
      solid();
      break;
    case 2 :
      solid();
      addGlitter(80);
      break;
    case 3 :
      breath2();
      break;
    case 4 : //meh
      breath();
      addGlitter(80);
      break;
    case 5 :
      breath();
      break;
    case 6 :  //good
      rainbow();
      break;
    case 7 :
      rainbow2();
      break;
    case 8 :   //good
      confetti();
      break;
    case 9:
      for (int i = 0; i < NUM_LEDS; i++) {
        if ( (i % 12) == spinner || ((i + 6) % 12) == spinner   )
          leds[i] = main_color;
        else
          leds[i] =  CRGB::Black;
      }
      break;
    case 10:
      for (int i = 0; i < NUM_LEDS; i++) {
        if ( (i % 12) == spinner || ((i + 6) % 12) == spinner || ((i + 3) % 12) == spinner || ((i + 9) % 12) == spinner)
          leds[i] = main_color;
        else
          leds[i] =  CRGB::Black;
      }
      break;
    case 11:
      breath3();
      break;
    default :
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
  }

  for (int i = 0; i < NUM_LEDS ; i++) {

    leds_blend[i].fadeToBlackBy(8);
    leds_output[i] = leds[i] + leds_blend[i];
  }


  current_mode_last = current_mode;
  FastLED.show();
  if (current_mode != 6 && current_mode != 7)
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}
