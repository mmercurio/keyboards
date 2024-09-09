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
  MACRO_QUKEYS_REPEAT_DELAY_INFO,   // 2
  MACRO_QUKEYS_REPEAT_DELAY_DEC,    // 3
  MACRO_QUKEYS_REPEAT_DELAY_INC,    // 4
  MACRO_QUKEYS_REPEAT_DELAY_TOGGLE  // 5
};

#define QUKEYS_TAP_REPEAT_DELAY_DEFAULT 140
#define QUKEYS_TAP_REPEAT_DELAY_DEFAULT_STR "140"
#define QUKEYS_TAP_REPEAT_DELAY_MIN     100
#define QUKEYS_TAP_REPEAT_DELAY_MIN_STR "100"
#define QUKEYS_TAP_REPEAT_DELAY_MAX     220
#define QUKEYS_TAP_REPEAT_DELAY_MAX_STR "220"
#define QUKEYS_TAP_REPEAT_DELAY_DELTA   10
#define QUKEYS_TAP_REPEAT_DELAY_DELTA_STR "10"

bool Qukeys_Repeat_Delay_Enabled = true;
uint8_t Qukeys_Repeat_Delay = QUKEYS_TAP_REPEAT_DELAY_DEFAULT;

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

void tapRepeatDelayIndicatorTicks() {
  const char* low = PSTR("-");
  const char* high = PSTR("+");
  Macros.type(PSTR(" tick="QUKEYS_TAP_REPEAT_DELAY_DELTA_STR": "));
  int ticks = abs(Qukeys_Repeat_Delay - QUKEYS_TAP_REPEAT_DELAY_DEFAULT) / QUKEYS_TAP_REPEAT_DELAY_DELTA;
  const char* direction = Qukeys_Repeat_Delay < QUKEYS_TAP_REPEAT_DELAY_DEFAULT ? low : high;
  for (int i = 0; i < ticks; i++) {
    Macros.type(direction);
  }
}

void tapRepeatDelayIndicator() {
  // Using Macros, type out an indicator for the current value of
  // Qukeys_Repeat_Delay (i.e., Qukeys.setMaxIntervalForTapRepeat)
  if (! Qukeys_Repeat_Delay_Enabled) {
    Macros.type(PSTR("DISABLED."));
    return;
  }

  if (Qukeys_Repeat_Delay >= QUKEYS_TAP_REPEAT_DELAY_MAX) {
    Macros.type(PSTR("MAX="QUKEYS_TAP_REPEAT_DELAY_MAX_STR));
  }
  else if (Qukeys_Repeat_Delay <= QUKEYS_TAP_REPEAT_DELAY_MIN) {
    Macros.type(PSTR("MIN="QUKEYS_TAP_REPEAT_DELAY_MIN_STR));
  }
  else {
    Macros.type(PSTR("DEFAULT="QUKEYS_TAP_REPEAT_DELAY_DEFAULT_STR));
    if (Qukeys_Repeat_Delay != QUKEYS_TAP_REPEAT_DELAY_DEFAULT) {
      tapRepeatDelayIndicatorTicks();
    }
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
    case MACRO_QUKEYS_REPEAT_DELAY_INFO:
      Macros.type(PSTR("Keyboardio Atreus Qukeys tap repeat: "));
      tapRepeatDelayIndicator();
      break;
    case MACRO_QUKEYS_REPEAT_DELAY_INC:
      Qukeys_Repeat_Delay = min(Qukeys_Repeat_Delay + QUKEYS_TAP_REPEAT_DELAY_DELTA, QUKEYS_TAP_REPEAT_DELAY_MAX);
      Qukeys.setMaxIntervalForTapRepeat(Qukeys_Repeat_Delay);
      break;
    case MACRO_QUKEYS_REPEAT_DELAY_DEC:
      Qukeys_Repeat_Delay = max(Qukeys_Repeat_Delay - QUKEYS_TAP_REPEAT_DELAY_DELTA, QUKEYS_TAP_REPEAT_DELAY_MIN);
      Qukeys.setMaxIntervalForTapRepeat(Qukeys_Repeat_Delay);
      break;
    case MACRO_QUKEYS_REPEAT_DELAY_TOGGLE:
      if (Qukeys_Repeat_Delay_Enabled) {
        Qukeys_Repeat_Delay_Enabled = false;
        Qukeys.setMaxIntervalForTapRepeat(0);
      }
      else {
        Qukeys_Repeat_Delay_Enabled = true;
        Qukeys_Repeat_Delay = QUKEYS_TAP_REPEAT_DELAY_DEFAULT;
        Qukeys.setMaxIntervalForTapRepeat(Qukeys_Repeat_Delay);
      }
      break;
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
  Qukeys.setMaxIntervalForTapRepeat(QUKEYS_TAP_REPEAT_DELAY_DEFAULT);  // default 200
}

void loop() {
  Kaleidoscope.loop();
}
