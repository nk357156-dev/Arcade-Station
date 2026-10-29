#include <Arduino.h>
#include "Config.h"

extern bool irSentOnBoot;
extern unsigned long bootStartTime;
unsigned long coinTimeSeconds = 180UL;

InputMode inputModes[5] = {MODE_AUTO, MODE_AUTO, MODE_AUTO, MODE_AUTO, MODE_AUTO};

void applyInputModes() {
  bool sessionActive = (currentState == PLAYING || currentState == FREE_PLAYING);

  for (uint8_t i = 0; i < 5; i++) {
    bool deviceOn;
    switch (inputModes[i]) {
      case MODE_ON:   deviceOn = true;          break;
      case MODE_OFF:  deviceOn = false;         break;
      case MODE_AUTO: deviceOn = sessionActive; break;
      default:        deviceOn = sessionActive; break;
    }
    digitalWrite(PIN_MOSFETS[i], deviceOn ? HIGH : LOW);
  }
}

// --- IR КОДЫ ---
const unsigned long IR_TV_CODE   = 0xA50UL;
const uint8_t       IR_TV_BITS   = 12;

const uint32_t      IR_PS2_CODE  = 0xA8B5BUL;
const uint8_t       IR_PS2_BITS  = 20;

const uint32_t IR_PS2_UP    = 0x6AB5B;
const uint32_t IR_PS2_OK    = 0x7AB5B;
const uint32_t IR_PS2_POWER = 0xA8B5B;

const uint8_t PS2_REPEAT_COUNT      = 10;
const unsigned long PS2_REPEAT_DELAY = 45;

const uint8_t TV_REPEAT_COUNT       = 5;
const unsigned long TV_REPEAT_DELAY  = 100;

const uint8_t PS2_GAME_REPEAT = 3;

const unsigned long PS2_OFF_HOLD_MS = 4000;
const unsigned long CMD_PAUSE_MS     = 500;

void beep(int pin, int durationMs) {
  if (!buzzerEnabled) return;
  unsigned long start = millis();
  while (millis() - start < durationMs) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(250);
    digitalWrite(pin, LOW);
    delayMicroseconds(250);
  }
}

static void sendPS2Cmd(uint32_t code, uint8_t count = PS2_REPEAT_COUNT) {
  for (uint8_t i = 0; i < count; i++) {
    irsend.sendSony(code, IR_PS2_BITS);
    delay(PS2_REPEAT_DELAY);
  }
}

void saveSettings() {
  EEPROM.write(0, (uint8_t)(buzzerEnabled ? 1 : 0));
  EEPROM.write(1, (uint8_t)bootScreenSeconds);
  EEPROM.write(2, (uint8_t)activeGameMode);
  EEPROM.write(3, (uint8_t)(coinTimeSeconds / 60));

  for (uint8_t i = 0; i < 5; i++) {
    EEPROM.write(4 + i, (uint8_t)inputModes[i]);
  }

  EEPROM.write(9, (uint8_t)playCost);   // [NEW]
}

void loadSettings() {
  uint8_t bz = EEPROM.read(0);
  bootScreenSeconds = EEPROM.read(1);
  uint8_t gm = EEPROM.read(2);
  uint8_t ct = EEPROM.read(3);

  buzzerEnabled = (bz != 0);
  activeGameMode = (GameMode)gm;
  coinTimeSeconds = ct * 60UL;

  if (bz == 255) buzzerEnabled = true;
  if (bootScreenSeconds < 10 || bootScreenSeconds > 60) bootScreenSeconds = 30;
  if (gm > 1) activeGameMode = TIMED_PLAY;
  if (ct < 1 || ct > 30) coinTimeSeconds = 180;

  for (uint8_t i = 0; i < 5; i++) {
    uint8_t m = EEPROM.read(4 + i);
    if (m > 2) m = 0;
    inputModes[i] = (InputMode)m;
  }

  // [NEW] — цена игры
  uint8_t pc = EEPROM.read(9);
  if (pc == 255 || pc < 1) playCost = 10;
  else playCost = pc;

  applyInputModes();
}

void checkCoinInput() {
  static bool lastCoin = HIGH;
  bool coin = digitalRead(PIN_COIN);

  if (coin == LOW && lastCoin == HIGH) {
    credits++;
    beep(PIN_BUZZER, 50);
    forceRedrawScreen = true;
  }
  lastCoin = coin;
}

// [CHANGED] — используем playCost вместо CREDITS_PER_PLAY
void checkStartButtons() {
  static bool lastStartP1 = HIGH;
  static bool lastStartP2 = HIGH;

  bool p1 = digitalRead(PIN_START_P1);
  bool p2 = digitalRead(PIN_START_P2);

  bool startPressed = (p1 == LOW && lastStartP1 == HIGH) || (p2 == LOW && lastStartP2 == HIGH);

  if (startPressed) {
    if (currentState == WAIT_CREDIT) {
      if (credits >= playCost) {
        credits -= playCost;
        sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
        currentState = PLAYING;
        gameActive = true;
        beep(PIN_BUZZER, 100);
      } else {
        beep(PIN_BUZZER, 50);
      }
    }
    else if (currentState == PLAYING) {
      if (credits >= playCost) {
        credits -= playCost;
        sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
        beep(PIN_BUZZER, 100);
      } else {
        currentState = WAIT_CREDIT;
        gameActive = false;
        beep(PIN_BUZZER, 50);
      }
    }
    else if (currentState == FREE_PLAYING) {
      sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
      beep(PIN_BUZZER, 50);
    }

    forceRedrawScreen = true;
  }

  lastStartP1 = p1;
  lastStartP2 = p2;
}

void updateTimer() {
  // Кредитная система: таймер не используется
}

void sendBootIR() {
  if (irSentOnBoot) return;

  unsigned long now = millis();
  unsigned long elapsed = now - bootStartTime;

  const unsigned long DELAY_BEFORE_SEND = 5000;

  if (elapsed >= DELAY_BEFORE_SEND) {
    irSentOnBoot = true;

    for (uint8_t i = 0; i < TV_REPEAT_COUNT; i++) {
      irsend.sendSony(IR_TV_CODE, IR_TV_BITS);
      delay(TV_REPEAT_DELAY);
    }

    delay(500);

    sendPS2Cmd(IR_PS2_CODE, 8);
  }
}

void sendManualIR() {
  for (uint8_t i = 0; i < TV_REPEAT_COUNT; i++) {
    irsend.sendSony(IR_TV_CODE, IR_TV_BITS);
    delay(TV_REPEAT_DELAY);
  }
}

void sendManualIRPS2() {
  sendPS2Cmd(IR_PS2_CODE, 8);
}

void sendManualIRPS2Off() {
  unsigned long startHold = millis();
  while (millis() - startHold < PS2_OFF_HOLD_MS) {
    irsend.sendSony(IR_PS2_CODE, IR_PS2_BITS);
    delay(PS2_REPEAT_DELAY);
  }
}

void sendPS2Reset() {
  sendManualIRPS2Off();
  delay(2000);
  sendPS2Cmd(IR_PS2_CODE, 8);
}

void sendGameOutRun() {
  sendPS2Cmd(IR_PS2_OK, PS2_GAME_REPEAT);
}

void sendGameTekken5() {
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_OK, PS2_GAME_REPEAT);
}

void sendGameTimeCrisis3() {
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_OK, PS2_GAME_REPEAT);
}

void sendGameTimeCrisisCZ() {
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_UP, PS2_GAME_REPEAT);
  delay(CMD_PAUSE_MS);
  sendPS2Cmd(IR_PS2_OK, PS2_GAME_REPEAT);
}

void sendGameClose() {
  sendManualIRPS2Off();
  delay(2000);
  sendPS2Cmd(IR_PS2_CODE, 8);
}