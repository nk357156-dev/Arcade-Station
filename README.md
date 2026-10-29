# Arcade-Station
> ## О проекте  
> ArcadeStation — система управления аркадным игровым автоматом на базе PlayStation 2 Slim. Контролирует оплату игрового времени, блокировку управления, запуск консоли и телевизора, ИК-команды, сервисное меню и отображение информации на LCD-дисплее.

| Параметр           | Значение                      |
| ------------------ | ----------------------------- |
| Основная платформа | PlayStation 2 Slim SCPH-70000 |
| Контроллер         | Arduino Nano / ATmega328P     |
| Дисплей            | LCD 1602 I2C                  |
| Оплата             | Импульсный монетоприёмник     |
| Видеосистема       | CRT-телевизор Sony            |
| Питание автомата   | 220 В / 12 В / 5 В            |

## 📑 Содержание

- [Комплектующие](#1.комплектующие)
- [[#2. Принцип работы|Принцип работы]]
- [[#3. Меню выбора игры|Меню выбора игры]]
- [[#4. Сервисное меню|Сервисное меню]]
- [[#5. Питание|Питание]]
- [[#6. Подключение компонентов|Подключение компонентов]]
- [[#7. Прошивка Arduino|Прошивка Arduino]]
- [[#8. Код|Код]]
- [[#8. Конструкция корпуса|Конструкция корпуса]]
- [[#9. Логика оплаты и завершения раунда|Логика оплаты и завершения раунда]]
- [[#10. План доработок|План доработок]]

## 1. Комплектующие

- [ ] Динамики с усилителем
- [ ] Корпус
- [ ] Переходная рамка
- [ ] Аркадные джойстики и кнопки × 2 шт.
- [ ] Панель разъёмов
- [ ] IEC C14 с выключателем
- [ ] Монетоприёмник
- [ ] USB-удлинители × 2 шт.
- [ ] Блок питания 12 В
- [ ] Удлинители DualShock × 4 шт.
- [ ] Энкодеры PlayStation 2 × 2 шт.
- [ ] Гофра для провода пистолета
- [ ] Провода
- [x] TV
- [x] PlayStation 2
- [x] GunCon 2
- [x] ИК-светодиоды × 2 шт.
- [x] Резисторы 220 Ом × 11 шт.
- [x] Дисплей LCD 1602
- [x] Arduino Nano
- [x] Дополнительный дисплей
- [x] Кнопки управления Arduino
- [x] Транзисторы × 5 шт.
- [x] Сервисные кнопки
- [x] Печатная плата
- [x] ИК-передатчики × 2 шт.
- [x] Зуммер
- [x] Сетевой адаптер
- [x] Прошивка Arduino

## 2. Принцип работы

### 2.1. Включение автомата

1. На Arduino подаётся питание.
2. Сервопривод имитирует нажатие кнопки питания PlayStation 2: перемещается в заданные положения, выдерживая необходимую последовательность.
3. Arduino отправляет ИК-команду включения телевизора.
4. На LCD отображается процесс загрузки консоли.
5. Длительность загрузки задаётся в сервисном меню — BOOT TIME.
6. На время загрузки кнопка START заблокирована.
7. После загрузки активируется игровой режим, сохранённый до предыдущего выключения.

>[!warning] Уточнение реализации  
ИК-команда включения телевизора и управление кнопкой питания PS2 — разные механизмы. Для сервопривода нужно обеспечить корректное нажатие без чрезмерного усилия. Поведение после перезапуска питания и сохранение режима необходимо проверить отдельно.

### 2.2. Режим игры по времени — TIMED PLAY

Состояние ожидания оплаты
>[!important] На дисплее:
 >            INSERT COIN
 >            TIME: 00:00

После внесения необходимой суммы:

1. Монетоприёмник выдаёт импульсы.
2. Arduino определяет внесённую сумму.
3. При достижении установленной цены начисляется заданное количество игрового времени.
4. Управление разблокируется.
5. Воспроизводится мелодия начала игры.
6. На дисплее появляется сообщение:
>[!important] На дисплее:
>          HAVE A NICE GAME
>          TIME: 15:00

7. Начинается обратный отсчёт оставшегося времени.

При достижении нуля:

- управление блокируется;
- кнопка START блокируется;
- воспроизводится мелодия окончания игры;
- автомат возвращается в состояние ожидания оплаты.

>[!important] Принцип списания времени  
Если оплата вносится заранее, необходимо хранить доступный баланс игрового времени. Если оплата вносится только после завершения раунда, необходимо отдельно учитывать сумму, доступную для продолжения игры.

### 2.3. Режим свободной игры — FREE PLAY

Управление доступно без оплаты, таймер не ограничивает игру.

LCD:

FREE GAME

GOOD LUCK BOSS

В этом режиме кнопка START работает по правилам выбранной игры, а монетоприёмник не влияет на доступность управления.

### 2.4. Выбор и сохранение режима

Система поддерживает два режима:

- FREE PLAY — бесплатная игра.
- TIMED PLAY — игра за деньги и время.

После перезапуска автомата должен восстанавливаться режим, сохранённый перед выключением.

  

## 3. Меню выбора игры

Предполагаемый список игр:

1. Tekken 5
2. Time Crisis 3
3. OutRun 2006: Coast 2 Coast
4. Time Crisis: Crisis Zone
5. CLOSE GAME

Для каждой игры необходимо предусмотреть:

- выбор через меню;
- запуск соответствующей игры;
- настройку режима управления;
- корректную обработку завершения игры;
- возврат в меню либо в режим ожидания оплаты.

>[!note] Важное ограничение  
Меню выбора игры и запуск игр требуют отдельной реализации. Само наличие пунктов меню не означает, что Arduino уже умеет запускать игры на PS2.

  

## 4. Сервисное меню

Вход осуществляется физической кнопкой SERVICE, установленной внутри корпуса.

Управление:

- NEXT — следующий пункт или увеличение значения;
- BACK — возврат на предыдущий уровень;
- OK — подтверждение выбора.

### 4.1. Параметры и функции

|                |                                                                              |        |
| -------------- | ---------------------------------------------------------------------------- | ------ |
| Пункт меню     | Функциональность                                                             | Статус |
| VOLUME GAME    | Регулировка громкости от 0 до 100% с шагом 1%; отправка ИК-команд телевизору | ❗      |
| GAME MODE      | Выбор FREE PLAY / TIMED PLAY                                                 | ✅      |
| INPUT MODE     | Выбор ARCADE / GUN / DUALSHOCK                                               | ✅      |
| GAME PRICE     | Установка стоимости игры                                                     | ✅      |
| COIN TIME      | Настройка игрового времени за сумму                                          | ✅      |
| BUZZER         | Включение и отключение зуммера                                               | ✅      |
| BOOT TIME      | Время загрузки с шагом 5 секунд                                              | ✅      |
| IR TEST        | Проверка ИК-передатчика                                                      | ✅      |
| SERVO ROTATION | Настройка углов сервопривода                                                 | ✅      |
| RESET          | Имитация нажатия кнопки сброса PS2                                           | ✅      |
| BACK           | Возврат в предыдущее меню                                                    | ✅      |

### 4.2. Требования к отдельным настройкам

VOLUME GAME

- Диапазон: 0–100%.
- Шаг: 1%.
- Отображение текущего значения.
- Кнопки изменения: +, −, BACK.
- При изменении отправляется соответствующая ИК-команда телевизору.

>[!warning] Совместимость ИК  
Не каждый телевизор поддерживает установку громкости абсолютным значением. Если доступны только команды увеличения и уменьшения громкости, Arduino должна учитывать это ограничение.

INPUT MODE

- ARCADE — аркадные джойстики и кнопки.
- GUN — режим светового пистолета.
- DUALSHOCK — режим контроллера DualShock.

Переключение режима должно согласовываться с физическим подключением органов управления и блокировкой ввода.

GAME PRICE

- Установка стоимости игрового сеанса.
- Увеличение и уменьшение значения.
- Отображение текущей цены.
- Возврат в предыдущее меню.

COIN TIME

- Установка количества игрового времени за заданную сумму.
- Сохранение параметров после перезапуска.

BOOT TIME

- Изменение времени ожидания загрузки.
- Шаг настройки — 5 секунд.
- Сохранение значения в энергонезависимой памяти.

IR TEST

- Отправка тестовой ИК-команды.
- Проверка работы ИК-передатчика.

SERVO ROTATION

- Настройка положения сервопривода.
- Подбор углов для корректного нажатия кнопки питания.
- Проверка повторяемости срабатывания.

  

## 5. Питание

|   |   |
|---|---|
|Узел|Питание|
|Arduino Nano|5 В DC|
|LCD 1602 I2C|5 В DC|
|ИК-передатчики|По схеме драйвера|
|Зуммер|По характеристикам компонента|
|Сервопривод|От отдельного источника 5 В|
|Монетоприёмник|12 В DC|
|Подсветка|12 В DC|
|PlayStation 2|220 В AC|
|CRT-телевизор|220 В AC|

>[!danger] Электробезопасность  
Сетевое напряжение 220 В должно быть отделено от низковольтной электроники. Необходимо использовать изолированные соединения, подходящий сетевой ввод, защиту от натяжения кабеля и надёжное крепление проводов.

  

Сервопривод и другие нагрузки нельзя питать непосредственно от вывода Arduino. При питании от отдельного низковольтного источника общий GND соединяется там, где это необходимо по схеме. Сетевые цепи не должны соединяться с GND Arduino.

  

## 6. Подключение компонентов

### 6.1. Основная таблица пинов Arduino

|   |   |   |   |
|---|---|---|---|
|Компонент|Пин Arduino|Тип подключения|Примечание|
|Выход монетоприёмника|D2|INPUT_PULLUP|Второй контакт кнопочного/импульсного сигнала — GND, если это совместимо с выходом монетоприёмника|
|Управление реле|D3|OUTPUT|Реле или драйвер, соответствующий нагрузке|
|Зуммер|D8|OUTPUT|Подключение согласно характеристикам зуммера|
|Сервопривод|D9|SERVO|Питание от отдельного источника 5 В|
|LCD SDA|A4|I2C|Линия данных|
|LCD SCL|A5|I2C|Линия тактирования|
|ИК-передатчик|D11|OUTPUT|Через подходящий транзисторный драйвер|
|Питание LCD|5V|Питание|От стабилизированного источника|
|Общий провод|GND|GND|Для совместимых низковольтных цепей|

### 6.2. Дополнительные требования

- ☐ Проверить распиновку монетоприёмника и уровень сигнала на его выходе.
- ☐ При несовместимом напряжении использовать оптрон или подходящий интерфейсный каскад.
- ☐ Обеспечить отдельное питание сервопривода.
- ☐ Проверить допустимый ток выходного каскада ИК-передатчика.
- ☐ Подобрать транзисторный драйвер для реле, если он не встроен в модуль.
- ☐ Проверить, что общий GND предусмотрен только для соответствующих низковольтных цепей.
- ☐ Проверить питание и ток потребления LCD с подсветкой.

>[!warning] Не путать сигнальный выход и питание  
Нельзя подключать 12-вольтовый выход монетоприёмника непосредственно к входу Arduino. Перед подключением необходимо установить, является ли выход импульсным открытым коллектором, логическим сигналом или отдельным выходом с напряжением.

  

## 7. Прошивка Arduino

Основная прошивка должна поддерживать:

- обработку импульсов монетоприёмника;
- расчёт внесённой суммы;
- начисление и обратный отсчёт игрового времени;
- блокировку и разблокировку управления;
- управление сервоприводом;
- отправку ИК-команд телевизору;
- работу LCD 1602;
- звуковые сигналы;
- сервисное меню;
- сохранение настроек;
- восстановление игрового режима после перезапуска.

### 7.1. Настройка ИК-пульта телевизора

В комплекте прошивки должен быть отдельный скетч для сканирования ИК-пульта.

Порядок настройки:

1. Загрузить скетч сканера на Arduino.
2. Подключить ИК-приёмник к Arduino согласно его распиновке.
3. Нажать необходимые кнопки оригинального пульта.
4. Записать полученные коды.
5. Перенести коды в отмеченные места основной прошивки.
6. Проверить команды включения и регулировки громкости.

### 7.2. Поддержка светового пистолета

Если режим GunCon 2 не используется, соответствующие части прошивки можно отключить для упрощения программы.

При этом необходимо проверить, что удаление кода не затрагивает общую логику меню, обработку кнопок и управление игровым временем.
## 8. Код
### Config.h
```cpp
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
```
#### Main.ino
```cpp
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
```
### GameLogic.cpp
```cpp
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
```
### Display.cpp
```cpp
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
```
### ServiceMenu.cpp
```cpp
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
```
## 9. Конструкция корпуса

### 8.1. Материалы

- Основные панели — ЛДСП толщиной 16 мм.
- Торцы боковин — Т-образный кант.
- Видимые торцы панелей — бумажная кромка.
- Декоративные элементы — самоклеящаяся плёнка с принтами.
- Переходная рамка — PLA.
- Корпус LCD-дисплея — PLA.

### 8.2. Сборка

1. Изготовить панели по чертежам.
2. Обработать торцы.
3. Нанести декоративную плёнку.
4. Установить телевизор и PlayStation 2.
5. Установить аркадные органы управления.
6. Закрепить монетоприёмник и панель разъёмов.
7. Установить LCD и Arduino.
8. Организовать проводку, вентиляцию и доступ к сервисным элементам.

### 8.3. Фотографии и чертежи

Добавить фотографии корпуса и сборки в эту папку заметки:

Разработка/

└── PS2 Arcade Cabinet/

    ├── IMG_9844.jpeg

    └── IMG_9842.jpeg

После размещения файлов замени этот блок на встроенные изображения Obsidian:

![[IMG_9844.jpeg]]

![[IMG_9842.jpeg]]

  

## 10. Логика оплаты и завершения раунда

>[!important] Основное правило CoinTime  
Оплата должна давать право на продолжение игры. Нельзя разрешать бесконечное нажатие START без оплаты, но и нельзя списывать деньги за каждое нажатие START без подтверждения завершения предыдущего раунда.

### 9.1. Требуемое поведение

|   |   |   |   |
|---|---|---|---|
|Ситуация|Управление|START|Оплата|
|Баланс 0 ₽|Заблокировано|Заблокирована|Ожидается|
|Внесено 15 ₽ или больше|Разблокировано|Контролируется системой|Доступен оплаченный сеанс|
|Раунд завершён, средств достаточно|По правилам режима|Обрабатывается системой|Списывается стоимость продолжения|
|Раунд завершён, средств нет|Заблокировано|Заблокирована|Ожидается|
|Свободная игра|Разблокировано|Разрешена|Не требуется|

### 9.2. Работа кнопки START

Планируемая логика:

1. При нулевом балансе игровые кнопки и START заблокированы.
2. После оплаты управление разблокируется.
3. Система отслеживает состояние раунда.
4. После завершения раунда нажатие START должно быть обработано по правилам оплаты.
5. Если средств хватает, стоимость продолжения списывается и управление остаётся доступным.
6. Если средств не хватает, управление блокируется до следующей оплаты.

### 9.3. Важное техническое ограничение

Arduino не может надёжно определить окончание раунда Tekken 5 только по факту нажатия START. Необходимо отдельное подтверждение состояния игры — например, анализ доступных сигналов или игровой памяти PS2 при наличии подходящего интерфейса.

ИК-передатчик сам по себе не считывает состояние игры и не подтверждает, что раунд действительно закончился.

>[!warning] Зафиксировать до реализации  
Нужно определить способ обнаружения конца раунда, а также способ блокировки START без помех для обычного управления. Это отдельная техническая задача, которую нельзя считать реализованной только подключением ИК-светодиода.

## 11. План доработок

Приоритет 1 — оплата и управление

- ☐ Определить точный тип выходного сигнала монетоприёмника.
- ☐ Реализовать подсчёт импульсов и перевод их в сумму.
- ☐ Утвердить правила начисления и списания времени.
- ☐ Реализовать блокировку игрового управления.
- ☐ Разработать способ обнаружения окончания раунда Tekken 5.
- ☐ Реализовать обработку START с учётом баланса.
- ☐ Проверить сохранение состояния при отключении питания.

Приоритет 2 — сервисное меню

- ☐ Реализовать VOLUME GAME.
- ☐ Реализовать INPUT MODE.
- ☐ Реализовать GAME PRICE.
- ☐ Проверить сохранение всех параметров.
- ☐ Проверить переходы между пунктами меню.

Приоритет 3 — телевизор и PS2

- ☐ Считать ИК-коды телевизора.
- ☐ Настроить включение телевизора.
- ☐ Настроить регулировку громкости.
- ☐ Проверить последовательность включения PS2 сервоприводом.
- ☐ Проверить функцию RESET.

Приоритет 4 — корпус и финальная сборка

- ☐ Изготовить корпус и переходную рамку.
- ☐ Установить динамики и усилитель.
- ☐ Смонтировать органы управления.
- ☐ Организовать кабельные трассы.
- ☐ Проверить безопасность питания.
- ☐ Провести длительный тест игрового автомата.

  

## 📝 Заметки автора

Основная цель: создать автономный аркадный автомат на базе PS2 Slim, в котором монетоприёмник управляет доступом к игре, Arduino отвечает за логику автомата, а телевизор и консоль включаются автоматически.

Главная нерешённая задача: надёжное определение окончания раунда Tekken 5 и реализация платного продолжения без возможности обхода оплаты.