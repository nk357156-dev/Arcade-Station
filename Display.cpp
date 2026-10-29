#include <Arduino.h>
#include "Config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void showBootScreen(int percent) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F(" SYSTEM LOADING "));
  lcd.setCursor(0, 1);
  lcd.print(F("  LOADING: "));
  if (percent < 10) lcd.print("0");
  lcd.print(percent);
  lcd.print(F("%"));
}

void showIdleScreen() {
  if (forceRedrawScreen || (int)lastSeconds != credits || lastState != WAIT_CREDIT) {
    lcd.setCursor(0, 0);
    if (credits >= playCost) {
      lcd.print(F("  PRESS START   "));
    } else {
      lcd.print(F(" INSERT "));
      lcd.print(playCost);
      lcd.print(F(" COIN "));
      int len = 8 + (playCost < 10 ? 1 : 2) + 5;
      for (int i = len; i < 16; i++) lcd.print(" ");
    }
    lcd.setCursor(0, 1);
    lcd.print(F(" CREDIT:"));
    if (credits < 10) lcd.print(" ");
    lcd.print(credits);
    lcd.print(F(" RUB  "));

    lastSeconds = (unsigned long)credits;
    lastState = WAIT_CREDIT;
  }
}

void showGameScreen() {
  if (forceRedrawScreen || (int)lastSeconds != credits || lastState != PLAYING) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("HAVE A NICE GAME"));
    lcd.setCursor(0, 1);
    lcd.print(F(" CREDIT:"));
    if (credits < 10) lcd.print(" ");
    lcd.print(credits);
    lcd.print(F(" RUB  "));

    lastSeconds = (unsigned long)credits;
    lastState = PLAYING;
  }
}

void showFreePlayScreen() {
  if (lastState != FREE_PLAYING) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("FREE PLAY        "));
    lcd.setCursor(0, 1);
    lcd.print(F("GOOD LUCK BOSS   "));
    lastState = FREE_PLAYING;
  }
}

void showServiceMenu() {
  if (lastState != SERVICE_MENU || menuIndex != lastMenuIndex) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("SERVICE MENU     "));
    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> BUZZER SETUP   ")); break;
      case 1: lcd.print(F("> GAME MODE      ")); break;
      case 2: lcd.print(F("> BOOT TIME      ")); break;
      case 3: lcd.print(F("> COIN TIME      ")); break;
      case 4: lcd.print(F("> TEST IR (TV)   ")); break;
      case 5: lcd.print(F("> TEST IR (PS2)  ")); break;
      case 6: lcd.print(F("> GAMES          ")); break;
      case 7: lcd.print(F("> INPUT SETTINGS ")); break;
      case 8: lcd.print(F("> PLAY COST      ")); break;
      default: lcd.print(F("                 ")); break;
    }
    lastState = SERVICE_MENU;
    lastMenuIndex = menuIndex;
  }
}

void showVolumeMenu() {
  if (lastState != SERVICE_VOLUME || menuIndex != lastMenuIndex) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("BUZZER SETUP     "));
    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> ON             ")); break;
      case 1: lcd.print(F("> OFF            ")); break;
      case 2: lcd.print(F("> BACK           ")); break;
    }
    lastState = SERVICE_VOLUME;
    lastMenuIndex = menuIndex;
  }
}

void showCoinTimeMenu() {
  int minutes = (int)(coinTimeSeconds / 60);

  if (lastState != SERVICE_COIN_TIME || menuIndex != lastMenuIndex || forceRedrawScreen ||
      minutes != (int)(coinTimeSeconds / 60)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("COIN TIME: "));
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(F("min"));

    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> +              ")); break;
      case 1: lcd.print(F("> -              ")); break;
      case 2: lcd.print(F("> BACK           ")); break;
    }

    lastState = SERVICE_COIN_TIME;
    lastMenuIndex = menuIndex;
  }
}

void showTestIRPS2Menu() {
  if (lastState != SERVICE_TEST_IR_PS2 || menuIndex != lastMenuIndex || forceRedrawScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("TEST IR PS2      "));
    lcd.setCursor(0, 1);

    switch (menuIndex) {
      case 0: lcd.print(F("> ON             ")); break;
      case 1: lcd.print(F("> OFF            ")); break;
      case 2: lcd.print(F("> RESET          ")); break;
      case 3: lcd.print(F("> BACK           ")); break;
      default: lcd.print(F("> BACK           ")); break;
    }

    lastState = SERVICE_TEST_IR_PS2;
    lastMenuIndex = menuIndex;
  }
}

void showBootTimeMenu() {
  int currentSec = bootScreenSeconds;

  if (lastState != SERVICE_BOOT_TIME || menuIndex != lastMenuIndex || forceRedrawScreen ||
      currentSec != bootScreenSeconds) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("BOOT TIME: "));
    lcd.print(currentSec);
    lcd.print(F("sec        "));

    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> +              ")); break;
      case 1: lcd.print(F("> -              ")); break;
      case 2: lcd.print(F("> BACK           ")); break;
    }

    lastState = SERVICE_BOOT_TIME;
    lastMenuIndex = menuIndex;
  }
}

void showGameModeMenu() {
  if (lastState != SERVICE_GAME_MODE || menuIndex != lastMenuIndex) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("GAME MODE        "));
    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> TIMED PLAY     ")); break;
      case 1: lcd.print(F("> FREE PLAY      ")); break;
      case 2: lcd.print(F("> BACK           ")); break;
    }
    lastState = SERVICE_GAME_MODE;
    lastMenuIndex = menuIndex;
  }
}

void showTestIRMenu() {
  if (lastState != SERVICE_TEST_IR || menuIndex != lastMenuIndex || forceRedrawScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("TEST IR (TV)     "));
    lcd.setCursor(0, 1);

    switch (menuIndex) {
      case 0: lcd.print(F("> CONFIRM        ")); break;
      case 1: lcd.print(F("> BACK           ")); break;
      default: lcd.print(F("> BACK           ")); break;
    }

    lastState = SERVICE_TEST_IR;
    lastMenuIndex = menuIndex;
  }
}

void showGamesMenu() {
  if (lastState != SERVICE_GAMES || menuIndex != lastMenuIndex || forceRedrawScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("GAMES            "));
    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> OUTRUN         ")); break;
      case 1: lcd.print(F("> TEKKEN 5       ")); break;
      case 2: lcd.print(F("> TIME CRISIS 3  ")); break;
      case 3: lcd.print(F("> TIME CRISIS CZ ")); break;
      case 4: lcd.print(F("> CLOSE GAME     ")); break;
      case 5: lcd.print(F("> BACK           ")); break;
      default: lcd.print(F("> BACK           ")); break;
    }
    lastState = SERVICE_GAMES;
    lastMenuIndex = menuIndex;
  }
}

void showInputSettingsMenu() {
  if (lastState != SERVICE_INPUT_SETTINGS || menuIndex != lastMenuIndex || forceRedrawScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("INPUT SETTINGS  "));

    lcd.setCursor(0, 1);
    lcd.print(F("> "));
    switch (menuIndex) {
      case 0: lcd.print(F("GUNCON"));   break;
      case 1: lcd.print(F("GAMEPAD1")); break;
      case 2: lcd.print(F("GAMEPAD2")); break;
      case 3: lcd.print(F("JOYSTICK1")); break;
      case 4: lcd.print(F("JOYSTICK2")); break;
    }
    // Добиваем пробелами до выравнивания режима справа
    int nameLen;
    switch (menuIndex) {
      case 0: nameLen = 6; break;
      case 1: nameLen = 8; break;
      case 2: nameLen = 8; break;
      case 3: nameLen = 9; break;
      case 4: nameLen = 9; break;
      default: nameLen = 0; break;
    }
    for (int i = nameLen; i < 9; i++) lcd.print(" ");
    lcd.print(" ");

    // Режим
    int modeLen;
    switch (inputModes[menuIndex]) {
      case MODE_AUTO: lcd.print(F("AUTO")); modeLen = 4; break;
      case MODE_ON:   lcd.print(F("ON"));   modeLen = 2; break;
      case MODE_OFF:  lcd.print(F("OFF"));  modeLen = 3; break;
    }
    for (int i = modeLen; i < 4; i++) lcd.print(" ");

    lastState = SERVICE_INPUT_SETTINGS;
    lastMenuIndex = menuIndex;
  }
}

void showPlayCostMenu() {
  if (lastState != SERVICE_PLAY_COST || menuIndex != lastMenuIndex || forceRedrawScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("PLAY COST: "));
    lcd.print(playCost);
    lcd.print(F(" COIN"));
    int len = 11 + (playCost < 10 ? 1 : 2) + 4;
    for (int i = len; i < 16; i++) lcd.print(" ");

    lcd.setCursor(0, 1);
    switch (menuIndex) {
      case 0: lcd.print(F("> +              ")); break;
      case 1: lcd.print(F("> -              ")); break;
      case 2: lcd.print(F("> BACK           ")); break;
    }

    lastState = SERVICE_PLAY_COST;
    lastMenuIndex = menuIndex;
  }
}