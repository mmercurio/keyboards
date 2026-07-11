// -*- mode: c++ -*-

/**
 * Custom Qukey settings and macros.
 */

#define QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT 120
#define QUKEYS_TAP_REPEAT_TIMEOUT_MIN     100
#define QUKEYS_TAP_REPEAT_TIMEOUT_MAX     220
#define QUKEYS_TAP_REPEAT_TIMEOUT_DELTA   10

class MMDebug
{
  public:

    static void typeDigit(uint8_t digit) {
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

    static void typeValue(uint8_t value) {
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
};

class MMQukeys
{
  public:

  // Qukeys Tap-Repeat State
  typedef enum {
    QTR_STATE_ENABLED,
    QTR_STATE_OFF,
    QTR_STATE_MAX
  } QtrState_t;

  MMQukeys() {
    reset();
  }

  ~MMQukeys() {
  }

  void reset() {
    _qtr_state = QTR_STATE_ENABLED;
    _qtr_timeout = QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT;
    Qukeys.setOverlapThreshold(100);
    Qukeys.setMinimumHoldTime(500);
    Qukeys.setMinimumPriorInterval(350);
    Qukeys.setMaxIntervalForTapRepeat(QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT);

    // I perfer to keep the tap-repeat timeout off by default.
    // It needs to bet set to such a high value in order to not
    // interfere with secondary actions it's essentially disabled.
    toggleOffQukeysTapRepeatTimeout();
  }

  void increaseQukeysTapRepeatTimeout() {
    // increase the tap-repeat timeout value and switch state to enabled if in another state
    _qtr_timeout = min(_qtr_timeout + QUKEYS_TAP_REPEAT_TIMEOUT_DELTA, QUKEYS_TAP_REPEAT_TIMEOUT_MAX);
    Qukeys.setMaxIntervalForTapRepeat(_qtr_timeout);
    _qtr_state = QTR_STATE_ENABLED;
  }

  void decreaseQukeysTapRepeatTimeout() {
    // decrease the tap-repeat timeout value and switch state to enabled if in another state
    _qtr_timeout = max( _qtr_timeout - QUKEYS_TAP_REPEAT_TIMEOUT_DELTA, QUKEYS_TAP_REPEAT_TIMEOUT_MIN);
    Qukeys.setMaxIntervalForTapRepeat( _qtr_timeout);
    _qtr_state = QTR_STATE_ENABLED;
  }

  void toggleOffQukeysTapRepeatTimeout() {
    // toggle tap-repeat between off/previous value
    // if toggled off, toggle back to enabled.
    // if enabled or if toggled to max, toggle to off.
    if (_qtr_state == QTR_STATE_OFF) {
      Qukeys.setMaxIntervalForTapRepeat(_qtr_timeout);
      _qtr_state = QTR_STATE_ENABLED;
    }
    else {
      Qukeys.setMaxIntervalForTapRepeat(0);
      _qtr_state = QTR_STATE_OFF;
    }
  }

  void toggleMaxQukeysTapRepeatTimeout() {
    // toggle tap-repeat between max/previous value
    // if toggled to max, toggle back to enabled
    // if enabled or togged to off, toggle to max
    if (_qtr_state == QTR_STATE_MAX) {
      Qukeys.setMaxIntervalForTapRepeat(_qtr_timeout);
      _qtr_state = QTR_STATE_ENABLED;
    }
    else {
      Qukeys.setMaxIntervalForTapRepeat(QUKEYS_TAP_REPEAT_TIMEOUT_MAX);
      _qtr_state = QTR_STATE_MAX;
    }
  }

  void typeQukeysTapRepeatTimeout() {
    // Using Macros type the value for Qukeys tap-repeat timeout
    Macros.type(PSTR("Keyboardio Qukeys tap-repeat: "));
    MMDebug::typeValue(_qtr_timeout);
    if (_qtr_state == QTR_STATE_OFF) {
      Macros.type(PSTR(" [TOGGLED OFF]"));
    }
    else if (_qtr_state == QTR_STATE_MAX) {
      Macros.type(PSTR(" [TOGGLED MAX]"));
    }
    Macros.type(PSTR(" MIN="));
    MMDebug::typeValue(QUKEYS_TAP_REPEAT_TIMEOUT_MIN);
    Macros.type(PSTR(" MAX="));
    MMDebug::typeValue(QUKEYS_TAP_REPEAT_TIMEOUT_MAX);
    Macros.type(PSTR(" DEFAULT="));
    MMDebug::typeValue(QUKEYS_TAP_REPEAT_TIMEOUT_DEFAULT);
    Macros.type(PSTR(" DELTA="));
    MMDebug::typeValue(QUKEYS_TAP_REPEAT_TIMEOUT_DELTA);
  }

  private:
    QtrState_t _qtr_state;
    uint8_t _qtr_timeout;
};
