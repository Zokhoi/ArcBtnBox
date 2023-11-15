#include "Arduino.h"
#include "Keyboard.h"
#include "string.h"
#include "keys.h"

/* pins */
int btnPins[5] = {10, 16, 14, 15, A0};
int btnKeys[5] = {KEY_D, KEY_F, KEY_ENTR, KEY_J, KEY_K};

/* buttons are active low */
int prevState[5] = {1, 1, 1, 1, 1};
int state[5] = {1, 1, 1, 1, 1};

/* timer interrupt to check keypress */
ISR(TIMER3_COMPA_vect) {
  for (size_t i = 0; i < 5; i++) {
    state[i] = digitalRead(btnPins[i]);
  }
  
  TXLED0; /* disable TX LED on any key event sent to computer */
  for (size_t i = 0; i < 5; i++) {
    if (btnKeys[i]!=KEY_NONE && prevState[i] != state[i]) {
      if (state[i]) Keyboard.release(btnKeys[i]);
      else Keyboard.press(btnKeys[i]);
    }
  }
  TXLED0; /* disable TX LED on any key event sent to computer */

  memcpy(prevState, state, 5*sizeof(int));
}

void setup() {
  cli(); /* disable global interrupts */

  for (size_t i = 0; i < 5; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  Serial.begin(9600);

  
  /* setup timer interrupts */
  
  /* reset configs and counter */
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;

  /**
   * setup compare match register
   * Clock speed (16MHz) / (prescaler * interrupt freq) -1
   */
  OCR3A = 249; /* interrupt every ~1 ms */

  /**
  Prescaler:
    (timer speed(Hz)) = (Arduino clock speed(16MHz)) / prescaler
      So 1Hz = 16000000 / 1 --> Prescaler: 1
      Prescaler can equal the below values and needs the relevant bits setting
      1    [CS10]
      8    [CS11]
      64   [CS11 + CS10]
      256  [CS12]
      1024 [CS12 + CS10]
  */
  TCCR3A |= (1 << WGM31);  /* turn on CTC mode */
  TCCR3B |= (1 << CS11) | (1 << CS10);  /* set to 64 prescaler */

  TIMSK3 |= (1 << OCIE3A); /* enable timer compare interrupt */

  Keyboard.begin();

  sei(); /* enable global interrupts */
}

void loop() {}
