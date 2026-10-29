#include <Arduino.h>
#include "Config.h"

static bool menuBtnLast = HIGH;
static bool nextBtnLast = HIGH;
static bool okBtnLast   = HIGH;

const uint8_t DEBOUNCE_STABLE_COUNT = 4;

static uint8_t menuBtnStable = 0;
static uint8_t nextBtnStable = 0;
static uint8_t okBtnStable   = 0;

static bool menuPressed = false;
static bool nextPressed = false;
static bool okPressed   = false;

void checkServiceButtons() {
  bool menuBtn = digitalRead(PIN_MENU);
  bool nextBtn = digitalRead(PIN_NEXT);
  bool okBtn   = digitalRead(PIN_OK);

  // --- MENU: вход/выход ---
  if (menuBtn == LOW) {
    if (menuBtnLast == HIGH) {
      menuBtnStable = 0;
      menuPressed = false;
    } else {
      if (menuBtnStable < DEBOUNCE_STABLE_COUNT) menuBtnStable++;
    }
    menuBtnLast = LOW;
  } else {
    menuBtnLast = HIGH;
    menuBtnStable = 0;
    menuPressed = false;
  }

  if (!menuPressed && menuBtnStable >= DEBOUNCE_STABLE_COUNT) {
    if (currentState >= SERVICE_MENU) {
      currentState = WAIT_CREDIT;
      menuIndex = 0;
    } else {
      currentState = SERVICE_MENU;
      menuIndex = 0;
    }
    menuPressed = true;
  }

  // --- NEXT: листание ---
  if (nextBtn == LOW) {
    if (nextBtnLast == HIGH) {
      nextBtnStable = 0;
      nextPressed = false;
    } else {
      if (nextBtnStable < DEBOUNCE_STABLE_COUNT) nextBtnStable++;
    }
    nextBtnLast = LOW;
  } else {
    nextBtnLast = HIGH;
    nextBtnStable = 0;
    nextPressed = false;
  }

  if (!nextPressed && nextBtnStable >= DEBOUNCE_STABLE_COUNT) {
    if (currentState == SERVICE_MENU) {
      menuIndex = (menuIndex + 1) % 9;   // [CHANGED] было 8, стало 9
    }
    else if (currentState == SERVICE_VOLUME) {
      menuIndex = (menuIndex + 1) % 3;
    }
    else if (currentState == SERVICE_BOOT_TIME) {
      menuIndex = (menuIndex + 1) % 3;
    }
    else if (currentState == SERVICE_GAME_MODE) {
      menuIndex = (menuIndex + 1) % 3;
    }
    else if (currentState == SERVICE_COIN_TIME) {
      menuIndex = (menuIndex + 1) % 3;
    }
    else if (currentState == SERVICE_TEST_IR) {
      menuIndex = (menuIndex + 1) % 2;
    }
    else if (currentState == SERVICE_TEST_IR_PS2) {
      menuIndex = (menuIndex + 1) % 4;
    }
    else if (currentState == SERVICE_GAMES) {
      menuIndex = (menuIndex + 1) % 6;
    }
    else if (currentState == SERVICE_INPUT_SETTINGS) {
      menuIndex = (menuIndex + 1) % 5;
    }
    else if (currentState == SERVICE_PLAY_COST) {   // [NEW]
      menuIndex = (menuIndex + 1) % 3;
    }
    nextPressed = true;
  }

  // --- OK: выбор ---
  if (okBtn == LOW) {
    if (okBtnLast == HIGH) {
      okBtnStable = 0;
      okPressed = false;
    } else {
      if (okBtnStable < DEBOUNCE_STABLE_COUNT) okBtnStable++;
    }
    okBtnLast = LOW;
  } else {
    okBtnLast = HIGH;
    okBtnStable = 0;
    okPressed = false;
  }

  if (!okPressed && okBtnStable >= DEBOUNCE_STABLE_COUNT) {
    if (currentState == SERVICE_MENU) {
      if (menuIndex == 0) currentState = SERVICE_VOLUME;
      else if (menuIndex == 1) currentState = SERVICE_GAME_MODE;
      else if (menuIndex == 2) currentState = SERVICE_BOOT_TIME;
      else if (menuIndex == 3) currentState = SERVICE_COIN_TIME;
      else if (menuIndex == 4) currentState = SERVICE_TEST_IR;
      else if (menuIndex == 5) currentState = SERVICE_TEST_IR_PS2;
      else if (menuIndex == 6) currentState = SERVICE_GAMES;
      else if (menuIndex == 7) currentState = SERVICE_INPUT_SETTINGS;
      else if (menuIndex == 8) currentState = SERVICE_PLAY_COST;   // [NEW]
    }
    else if (currentState == SERVICE_VOLUME) {
      if (menuIndex == 0) { buzzerEnabled = true; saveSettings(); beep(PIN_BUZZER, 50); }
      else if (menuIndex == 1) { buzzerEnabled = false; saveSettings(); beep(PIN_BUZZER, 50); }
      else if (menuIndex == 2) { currentState = SERVICE_MENU; }
    }
    else if (currentState == SERVICE_TEST_IR) {
      if (menuIndex == 0) {
        sendManualIR();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 1) {
        currentState = SERVICE_MENU;
      }
    }
    else if (currentState == SERVICE_TEST_IR_PS2) {
      if (menuIndex == 0) {
        sendManualIRPS2();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 1) {
        sendManualIRPS2Off();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 2) {
        sendPS2Reset();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 3) {
        currentState = SERVICE_MENU;
      }
    }
    else if (currentState == SERVICE_GAMES) {
      if (menuIndex == 0) { sendGameOutRun(); beep(PIN_BUZZER, 50); forceRedrawScreen = true; }
      else if (menuIndex == 1) { sendGameTekken5(); beep(PIN_BUZZER, 50); forceRedrawScreen = true; }
      else if (menuIndex == 2) { sendGameTimeCrisis3(); beep(PIN_BUZZER, 50); forceRedrawScreen = true; }
      else if (menuIndex == 3) { sendGameTimeCrisisCZ(); beep(PIN_BUZZER, 50); forceRedrawScreen = true; }
      else if (menuIndex == 4) { sendGameClose(); beep(PIN_BUZZER, 50); forceRedrawScreen = true; }
      else if (menuIndex == 5) { currentState = SERVICE_MENU; }
    }
    else if (currentState == SERVICE_BOOT_TIME) {
      if (menuIndex == 0) {
        bootScreenSeconds = min(bootScreenSeconds + 5, 60);
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 1) {
        bootScreenSeconds = max(bootScreenSeconds - 5, 10);
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 2) {
        currentState = SERVICE_MENU;
      }
    }
    else if (currentState == SERVICE_GAME_MODE) {
      if (menuIndex == 0) { activeGameMode = TIMED_PLAY; saveSettings(); beep(PIN_BUZZER, 50); currentState = WAIT_CREDIT; }
      else if (menuIndex == 1) { activeGameMode = FREE_PLAY; saveSettings(); beep(PIN_BUZZER, 50); currentState = FREE_PLAYING; gameActive = true; }
      else if (menuIndex == 2) { currentState = SERVICE_MENU; }
    }
    else if (currentState == SERVICE_COIN_TIME) {
      int minutes = (int)(coinTimeSeconds / 60);
      if (menuIndex == 0) {
        minutes = min(minutes + 1, 30);
        coinTimeSeconds = (unsigned long)minutes * 60UL;
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 1) {
        minutes = max(minutes - 1, 1);
        coinTimeSeconds = (unsigned long)minutes * 60UL;
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 2) {
        currentState = SERVICE_MENU;
      }
    }
    else if (currentState == SERVICE_INPUT_SETTINGS) {
      inputModes[menuIndex] = (InputMode)(((int)inputModes[menuIndex] + 1) % 3);
      saveSettings();
      applyInputModes();
      beep(PIN_BUZZER, 50);
      forceRedrawScreen = true;
    }
    // [NEW] ---- PLAY COST ----
    else if (currentState == SERVICE_PLAY_COST) {
      if (menuIndex == 0) {
        playCost = min(playCost + 1, 99);
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 1) {
        playCost = max(playCost - 1, 1);
        saveSettings();
        beep(PIN_BUZZER, 50);
        forceRedrawScreen = true;
      }
      else if (menuIndex == 2) {
        currentState = SERVICE_MENU;
      }
    }
    okPressed = true;
  }
}