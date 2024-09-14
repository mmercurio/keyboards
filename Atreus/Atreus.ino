/* -*- mode: c++ -*-
 * Atreus -- Chrysalis-enabled Sketch for the Keyboardio Atreus
 * Copyright (C) 2018-2022  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built on " __DATE__ " at " __TIME__
#endif

#include "Kaleidoscope.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-FirmwareVersion.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-SpaceCadet.h"
#include "Kaleidoscope-DynamicMacros.h"
#include "Kaleidoscope-LayerNames.h"

#define MO(n) ShiftToLayer(n)
#define TG(n) LockLayer(n)

enum {
  MACRO_QWERTY,
  MACRO_VERSION_INFO,

  // These are special Qukeys macros to dynamically control Qukeys tap-repeat behavior
  // by modifying Qukeys.setMaxIntervalForTapRepeat(timeout) value.
  // The numeric references are for easily configuring the Macros from Chrysalis editor.

  // Type out current settings for debugging
  MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INFO,   // 2

  // Decrease the tap-repeat timeout
  MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_DEC,    // 3

  // Increase the tap-repeat timeout
  MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INC,    // 4

  // Toggle the tap-repeat timeout off, temporarily setting it to 0 and then back to previous value
  MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_OFF,  // 5

  // Toggle the tap-repeat timeout to max temporarily. Next activation will return to previous value.
  MACROS_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_MAX  // 6
};

#define QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT 120
#define QUKEYS_TAP_REPEAT_TIMEOUT_MIN     100
#define QUKEYS_TAP_REPEAT_TIMEOUT_MAX     220
#define QUKEYS_TAP_REPEAT_TIMEOUT_DELTA   10

// Qukeys Tap-Repeat State
typedef enum {
  QTR_STATE_ENABLED,
  QTR_STATE_OFF,
  QTR_STATE_MAX
} QtrState_t;

QtrState_t QukeysTapRepeatState = QTR_STATE_ENABLED;
uint8_t QukeysTapRepeatTimeout = QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT;

#define Key_Exclamation LSHIFT(Key_1)
#define Key_At          LSHIFT(Key_2)
#define Key_Hash        LSHIFT(Key_3)
#define Key_Dollar      LSHIFT(Key_4)
#define Key_Percent     LSHIFT(Key_5)
#define Key_Caret       LSHIFT(Key_6)
#define Key_And         LSHIFT(Key_7)
#define Key_Star        LSHIFT(Key_8)
#define Key_Plus        LSHIFT(Key_Equals)

enum {
  QWERTY,
  FUN,
  UPPER
};

// clang-format off
KEYMAPS(
  [QWERTY] = KEYMAP_STACKED
  (
       Key_Q   ,Key_W   ,Key_E       ,Key_R         ,Key_T
      ,Key_A   ,Key_S   ,Key_D       ,Key_F         ,Key_G
      ,Key_Z   ,Key_X   ,Key_C       ,Key_V         ,Key_B, Key_Backtick
      ,Key_Esc ,Key_Tab ,Key_LeftGui ,Key_LeftShift ,Key_Backspace ,Key_LeftControl

                     ,Key_Y     ,Key_U      ,Key_I     ,Key_O      ,Key_P
                     ,Key_H     ,Key_J      ,Key_K     ,Key_L      ,Key_Semicolon
       ,Key_Backslash,Key_N     ,Key_M      ,Key_Comma ,Key_Period ,Key_Slash
       ,Key_LeftAlt  ,Key_Space ,MO(FUN)    ,Key_Minus ,Key_Quote  ,Key_Enter
  ),

  [FUN] = KEYMAP_STACKED
  (
       Key_Exclamation ,Key_At           ,Key_UpArrow   ,Key_Dollar           ,Key_Percent
      ,Key_LeftParen   ,Key_LeftArrow    ,Key_DownArrow ,Key_RightArrow       ,Key_RightParen
      ,Key_LeftBracket ,Key_RightBracket ,Key_Hash      ,Key_LeftCurlyBracket ,Key_RightCurlyBracket ,Key_Caret
      ,TG(UPPER)       ,Key_Insert       ,Key_LeftGui   ,Key_LeftShift        ,Key_Delete         ,Key_LeftControl

                   ,Key_PageUp   ,Key_7 ,Key_8      ,Key_9 ,Key_Backspace
                   ,Key_PageDown ,Key_4 ,Key_5      ,Key_6 ,___
      ,Key_And     ,Key_Star     ,Key_1 ,Key_2      ,Key_3 ,Key_Plus
      ,Key_LeftAlt ,Key_Space    ,___   ,Key_Period ,Key_0 ,Key_Equals
   ),

  [UPPER] = KEYMAP_STACKED
  (
       Key_Insert            ,Key_Home                 ,Key_UpArrow   ,Key_End        ,Key_PageUp
      ,Key_Delete            ,Key_LeftArrow            ,Key_DownArrow ,Key_RightArrow ,Key_PageDown
      ,M(MACRO_VERSION_INFO) ,Consumer_VolumeIncrement ,XXX           ,XXX            ,___ ,___
      ,MoveToLayer(QWERTY)   ,Consumer_VolumeDecrement ,___           ,___            ,___ ,___

                ,Key_UpArrow   ,Key_F7              ,Key_F8          ,Key_F9         ,Key_F10
                ,Key_DownArrow ,Key_F4              ,Key_F5          ,Key_F6         ,Key_F11
      ,___      ,XXX           ,Key_F1              ,Key_F2          ,Key_F3         ,Key_F12
      ,___      ,___           ,MoveToLayer(QWERTY) ,Key_PrintScreen ,Key_ScrollLock ,Consumer_PlaySlashPause
   )
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(
  // ----------------------------------------------------------------------
  // Chrysalis plugins

  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,

  // FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
  // both debugging, and in backing up one's EEPROM contents.
  FocusEEPROMCommand,

  // The FirmwareVersion plugin lets Chrysalis query the version of the firmware
  // programmatically.
  FirmwareVersion,

  // The LayerNames plugin allows Chrysalis to display - and edit - custom layer
  // names, to be shown instead of the default indexes.
  LayerNames,

  // ----------------------------------------------------------------------
  // Keystroke-handling plugins

  // The Qukeys plugin enables the "Secondary action" functionality in
  // Chrysalis. Keys with secondary actions will have their primary action
  // performed when tapped, but the secondary action when held.
  Qukeys,

  // SpaceCadet can turn your shifts into parens on tap, while keeping them as
  // Shifts when held. SpaceCadetConfig lets Chrysalis configure some aspects of
  // the plugin.
  SpaceCadet,
  SpaceCadetConfig,

  // Enables the "Sticky" behavior for modifiers, and the "Layer shift when
  // held" functionality for layer keys.
  OneShot,
  OneShotConfig,
  EscapeOneShot,
  EscapeOneShotConfig,

  // The macros plugin adds support for macros
  Macros,

  // Enables dynamic, Chrysalis-editable macros.
  DynamicMacros,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys,
  MouseKeysConfig  //,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  // MagicCombo,

  // Enables the GeminiPR Stenography protocol. Unused by default, but with the
  // plugin enabled, it becomes configurable - and then usable - via Chrysalis.
  // GeminiPR,
);

void typeDigit(uint8_t digit) {
  // Using Macros type the single digit 0-9
  if (digit > 9) {
    Macros.type(PSTR("???"));
    return;
  }
  const char* digits[] = {
    PSTR("0"), PSTR("1"), PSTR("2"), PSTR("3"), PSTR("4"), PSTR("5"), PSTR("6"), PSTR("7"), PSTR("8"), PSTR("9")
  };
  Macros.type(digits[digit]);
}

void typeValue(uint8_t value) {
  // Using Macros type each digit of the value given an unsigned 8-bit value.
  // pretty pathetic but for small numbers it's okay.
  if (value >= 10) {
    uint8_t digit = value % 10;
    value = value / 10;
    typeValue(value); // will never recurse more than 3 times.
    typeDigit(digit);
  } else {
    typeDigit(value);
  }
}

void typeQukeysTapRepeatTimeout() {
  // Using Macros type the value for Qukeys tap-repeat timeout
  Macros.type(PSTR("Keyboardio Atreus Qukeys tap-repeat: "));
  typeValue(QukeysTapRepeatTimeout);
  if (QukeysTapRepeatState == QTR_STATE_OFF) {
    Macros.type(PSTR(" TOGGLED OFF"));
  }
  else if (QukeysTapRepeatState == QTR_STATE_MAX) {
    Macros.type(PSTR(" TOGGLED MAX"));
  }
  else if (QukeysTapRepeatTimeout >= QUKEYS_TAP_REPEAT_TIMEOUT_MAX) {
    Macros.type(PSTR(" MAX"));
  }
  else if (QukeysTapRepeatTimeout <= QUKEYS_TAP_REPEAT_TIMEOUT_MIN) {
    Macros.type(PSTR(" MIN"));
  }
  else if (QukeysTapRepeatTimeout == QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT)  {
    Macros.type(PSTR(" DEFAULT"));
  }
}

void increaseQukeysTapRepeatTimeout() {
  // increase the tap-repeat timeout value and switch state to enabled if in another state
  QukeysTapRepeatTimeout =
    min(QukeysTapRepeatTimeout + QUKEYS_TAP_REPEAT_TIMEOUT_DELTA, QUKEYS_TAP_REPEAT_TIMEOUT_MAX);
  Qukeys.setMaxIntervalForTapRepeat(QukeysTapRepeatTimeout);
  QukeysTapRepeatState = QTR_STATE_ENABLED;
}

void decreaseQukeysTapRepeatTimeout() {
  // decrease the tap-repeat timeout value and switch state to enabled if in another state
  QukeysTapRepeatTimeout =
    max(QukeysTapRepeatTimeout - QUKEYS_TAP_REPEAT_TIMEOUT_DELTA, QUKEYS_TAP_REPEAT_TIMEOUT_MIN);
  Qukeys.setMaxIntervalForTapRepeat(QukeysTapRepeatTimeout);
  QukeysTapRepeatState = QTR_STATE_ENABLED;
}

void toggleOffQukeysTapRepeatTimeout() {
  // toggle tap-repeat between off/previous value
  // if toggled off, toggle back to enabled.
  // if enabled or if toggled to max, toggle to off.
  if (QukeysTapRepeatState == QTR_STATE_OFF) {
    Qukeys.setMaxIntervalForTapRepeat(QukeysTapRepeatTimeout);
    QukeysTapRepeatState = QTR_STATE_ENABLED;
  }
  else {
    Qukeys.setMaxIntervalForTapRepeat(0);
    QukeysTapRepeatState = QTR_STATE_OFF;
  }
}

void toggleMaxQukeysTapRepeatTimeout() {
  // toggle tap-repeat between max/previous value
  // if toggled to max, toggle back to enabled
  // if enabled or togged to off, toggle to max
  if (QukeysTapRepeatState == QTR_STATE_MAX) {
    Qukeys.setMaxIntervalForTapRepeat(QukeysTapRepeatTimeout);
    QukeysTapRepeatState = QTR_STATE_ENABLED;
  }
  else {
    Qukeys.setMaxIntervalForTapRepeat(QUKEYS_TAP_REPEAT_TIMEOUT_MAX);
    QukeysTapRepeatState = QTR_STATE_MAX;
  }
}

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case MACRO_QWERTY:
      // This macro is currently unused, but is kept around for compatibility
      // reasons. We used to use it in place of `MoveToLayer(QWERTY)`, but no
      // longer do. We keep it so that if someone still has the old layout with
      // the macro in EEPROM, it will keep working after a firmware update.
      Layer.move(QWERTY);
      break;
    case MACRO_VERSION_INFO:
      Macros.type(("Keyboardio Atreus - Kaleidoscope "));
      Macros.type(PSTR(BUILD_INFORMATION));
      break;
    case MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INFO:
      typeQukeysTapRepeatTimeout();
      break;
    case MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INC:
      increaseQukeysTapRepeatTimeout();
      break;
    case MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_DEC:
      decreaseQukeysTapRepeatTimeout();
      break;
    case MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_OFF:
      toggleOffQukeysTapRepeatTimeout();
      break;
    case MACROS_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_MAX:
      toggleMaxQukeysTapRepeatTimeout();
    default:
      break;
    }
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
  EEPROMKeymap.setup(9);

  DynamicMacros.reserve_storage(48);

  LayerNames.reserve_storage(63);

  Layer.move(EEPROMSettings.default_layer());

  // To avoid any surprises, SpaceCadet is turned off by default. However, it
  // can be permanently enabled via Chrysalis, so we should only disable it if
  // no configuration exists.
  SpaceCadetConfig.disableSpaceCadetIfUnconfigured();

  // Tweak Qukeys to prevent unintended modifiers with home row mods:
  // https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html
  Qukeys.setOverlapThreshold(100);         // default 80
  Qukeys.setMinimumHoldTime(500);          // default 50
  Qukeys.setMinimumPriorInterval(350);      // default 75
  Qukeys.setMaxIntervalForTapRepeat(QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT);  // default 200
}

void loop() {
  Kaleidoscope.loop();
}
