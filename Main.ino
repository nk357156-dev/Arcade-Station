#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

int credits = 0;
int playCost = 10;                         // [NEW]
bool gameActive = false;
State currentState = BOOTING;
int menuIndex = 0;
bool buzzerEnabled = true;
int bootScreenSeconds = 30;
GameMode activeGameMode = TIMED_PLAY;
bool forceRedraw = false;
bool forceRedrawScreen = false;

State lastState = (State)-1;
int lastMenuIndex = -1;
int lastBootPercent = -1;
unsigned long lastSeconds = 0;

State lastLoopState = (State)-1;

IRsend irsend;
bool irSentOnBoot = false;
unsigned long bootStartTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_COIN, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_MENU, INPUT_PULLUP);
  pinMode(PIN_NEXT, INPUT_PULLUP);
  pinMode(PIN_OK, INPUT_PULLUP);

  pinMode(PIN_START_P1, INPUT_PULLUP);
  pinMode(PIN_START_P2, INPUT_PULLUP);

  for (int i = 0; i < 5; i++) {
    pinMode(PIN_MOSFETS[i], OUTPUT);
    digitalWrite(PIN_MOSFETS[i], HIGH);
  }

  lcd.init();
  lcd.backlight();
  delay(200);

  loadSettings();

  bootStartTime = millis();
  irSentOnBoot = false;
}

void loop() {
  sendBootIR();
  checkCoinInput();
  checkServiceButtons();
  checkStartButtons();
  updateTimer();

  if (lastLoopState != currentState) {
    lcd.clear();

    if (currentState == SERVICE_VOLUME ||
        currentState == SERVICE_BOOT_TIME ||
        currentState == SERVICE_GAME_MODE ||
        currentState == SERVICE_COIN_TIME ||
        currentState == SERVICE_TEST_IR ||
        currentState == SERVICE_TEST_IR_PS2 ||
        currentState == SERVICE_GAMES ||
        currentState == SERVICE_INPUT_SETTINGS ||
        currentState == SERVICE_PLAY_COST ||          // [NEW]
        currentState == SERVICE_MENU) {
      menuIndex = 0;
    }

    lastLoopState = currentState;
    forceRedrawScreen = true;

    applyInputModes();
  }

  switch (currentState) {
    case BOOTING: {
        static unsigned long bootStart = millis();
        static int lastPercent = -1;
        unsigned long now = millis();
        int percent = (int)((now - bootStart) * 100 / (bootScreenSeconds * 1000L));
        if (percent > 100) percent = 100;

        if (percent != lastPercent) {
          showBootScreen(percent);
          lastPercent = percent;
        }

        if (percent >= 100) {
          currentState = WAIT_CREDIT;
        }
        break;
      }

    case WAIT_CREDIT:
      showIdleScreen();
      break;

    case PLAYING:
      showGameScreen();
      break;

    case FREE_PLAYING:
      showFreePlayScreen();
      break;

    case SERVICE_MENU:
      showServiceMenu();
      break;

    case SERVICE_VOLUME:
      showVolumeMenu();
      break;

    case SERVICE_COIN_TIME:
      showCoinTimeMenu();
      break;

    case SERVICE_TEST_IR_PS2:
      showTestIRPS2Menu();
      break;

    case SERVICE_BOOT_TIME:
      showBootTimeMenu();
      break;

    case SERVICE_GAME_MODE:
      showGameModeMenu();
      break;

    case SERVICE_TEST_IR:
      showTestIRMenu();
      break;

    case SERVICE_GAMES:
      showGamesMenu();
      break;

    case SERVICE_INPUT_SETTINGS:
      showInputSettingsMenu();
      break;

    case SERVICE_PLAY_COST:                           // [NEW]
      showPlayCostMenu();
      break;
  }

  forceRedrawScreen = false;
}