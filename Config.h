#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>
#include <IRremote.h>

enum State {
  BOOTING, WAIT_CREDIT, PLAYING, FREE_PLAYING,
  SERVICE_MENU, SERVICE_VOLUME,
  SERVICE_BOOT_TIME, SERVICE_GAME_MODE, SERVICE_COIN_TIME,
  SERVICE_TEST_IR, SERVICE_TEST_IR_PS2,
  SERVICE_GAMES,
  SERVICE_INPUT_SETTINGS,
  SERVICE_PLAY_COST                      // [NEW]
};

enum GameMode { TIMED_PLAY, FREE_PLAY };

enum InputMode { MODE_AUTO = 0, MODE_ON = 1, MODE_OFF = 2 };

// --- Пины ---
const int PIN_COIN   = 2;
const int PIN_BUZZER = 7;
const int PIN_MENU   = 4;
const int PIN_NEXT   = 5;
const int PIN_OK     = 6;

const int PIN_MOSFETS[5] = {8, 9, 10, 11, 12};

const int PIN_START_P1 = A0;
const int PIN_START_P2 = A1;

extern int credits;
extern int playCost;                      // [NEW]
extern bool gameActive;
extern State currentState;
extern int menuIndex;
extern bool buzzerEnabled;
extern int bootScreenSeconds;
extern GameMode activeGameMode;
extern unsigned long coinTimeSeconds;

extern IRsend irsend;
extern bool irSentOnBoot;
extern unsigned long bootStartTime;

extern State lastState;
extern int lastMenuIndex;
extern unsigned long lastSeconds;
extern bool forceRedrawScreen;

extern InputMode inputModes[5];

void beep(int pin, int durationMs);
void saveSettings();
void loadSettings();
void checkCoinInput();
void updateTimer();
void checkServiceButtons();
void checkStartButtons();
void applyInputModes();

void sendBootIR();
void sendManualIR();
void sendManualIRPS2();
void sendManualIRPS2Off();
void sendPS2Reset();

void sendGameOutRun();
void sendGameTekken5();
void sendGameTimeCrisis3();
void sendGameTimeCrisisCZ();
void sendGameClose();

void showBootScreen(int percent);
void showIdleScreen();
void showGameScreen();
void showFreePlayScreen();

void showServiceMenu();
void showVolumeMenu();
void showBootTimeMenu();
void showGameModeMenu();
void showCoinTimeMenu();
void showTestIRMenu();
void showTestIRPS2Menu();
void showGamesMenu();
void showInputSettingsMenu();
void showPlayCostMenu();                  // [NEW]

#endif