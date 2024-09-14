# Atreus

This is my custom Kaleidoscope firmware for the [Keyboardio Atreus](https://shop.keyboard.io/products/keyboardio-atreus#), a minimal 44 key, split, columnar, ergonomic mechanical keyboard.

My Atreus layout is inspired by [Miryoku](https://github.com/manna-harbour/miryoku/), with the following notable differences and features:

* QWERTY layout for base layer
* vi-style navigation vs Miryoku arrows on home keys
* Additional macOS shortcuts on Nav and Mouse layers
* Kaleidoscope [MouseKeys Warping](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-MouseKeys.html#warping) with 2x2 grid
* Implementation differences to accommodate Kaleidoscope firmware and customizations for secondary modifier actions
* The Keyboardio Atreus has 44 keys and I make regular use of 38 (the extras are redundant)

Many of these differences from standard Miryoku are documented as [Alternative Layout](https://github.com/manna-harbour/miryoku/tree/master/docs/reference#alternative-layouts).

## Qukeys and Home Row Mods

Until I learned about and started using [home row mods](https://precondition.github.io/home-row-mods), I was mostly unimpressed and ineffective with the Atreus. There just were not enough keys to comfortably accommodate how I was using the keyboard. That changed when I started using home row mods -- i.e., secondary actions on the home row keys that take on modifier keys such as `shift`, `control`, `alt`, when held and normal keys when tapped.

My only problem was that when typing quickly (and sometimes not so quickly) with the default Kaleidoscope firmware, I would frequently accidentally trigger unintentional modifier actions and run into other problems such as repeated home row keys not getting registered. This is what led me to customize and compile my own firmware.

### My Qukeys Settings

Home row mods and secondary actions in general, such as secondary actions on the thumb cluster keys, are controlled in Kaleidoscope using the [Qukeys](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html) plugin.

In order to reduce unintentionally activating modifiers and secondary actions while typing quickly, the following settings are changed from the default behavior:

* `.setMaxIntervalForTapRepeat(timeout)`: set to less than the default and see dynamic adjustments below.
* `.setOverlapThreshold(percentage)`: set to 100% and still not perfect.
* `.setMinimumHoldTime(min_hold_time)`: increased significantly from the default.
* `.setMinimumPriorInterval(min_interval)`: increased significantly from the default.

### Dynamic Adjustments

I reduced [`.setMaxIntervalForTapRepeat(timeout)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setmaxintervalfortaprepeat-timeout) to significantly lower values (e.g, 140 ms and below) than the default of 200 ms, in order to mitigate an issue where repeatedly quickly tapped home row keys would not register (e.g., typing "less" would only register one "s"). But this led to another problem: I could not tap and hold a key to have it repeat ([Tap-Repeat Behavior](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#tap-repeat-behaviour)). For most keys this is not a problem, but for keys such as vi-style navigation arrows, backspace, space, etc., this was much more of a nuisance.

To mitigate issues with the Qukeys Tap-Repeat behavior, I created the ability to dynamically modify some Qukey settings using configured Macros which I map to keys on the Function layer. The Macros are:

* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INFO`: type out info on current settings for debugging.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_DEC`: decrease the tap-repeat timeout.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INC`: increase the tap-repeat timeout.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_OFF`: toggle the tap-repeat timeout between current value and 0 to temporarily disable it.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_MAX`: toggle the tap-repeat timeout between current value and its max, to temporarily allow easier repeating of held keys.

In general, I prefer values of 130-140 ms for the tap-repeat timeout interval. But when I want to occasionally take advantage of the tap-repeat behavior, I'm now able to quickly temporarily modify the settings.

## Layout Experiments

### Alternative 01

Experimental alternative layout to solve these specific issues with my original layout:

1. Using home row mods for `shift` is often challenging when typing uppercase or mixed case letters that alternate between hands, such as "MACRO". It would be nice to have access to `shift` on the thumb keys that do not conflict with other letters.
1. Copying and pasting is not easy when the right hand is on the mouse. On a normal QWERTY keyboard this is trivial with the left hand. However, with the original layout above it requires the left hand crossing to the right half of the split keyboard or a difficult contortion or chording two keys with the left hand.

Ideas:

* add `shift` as secondary modifier on outter thumb keys, repositioning layer shift keys for `fun` and `media` layers.
* mirror paste, copy, cut keys from right hand to left hands on the `mouse` layer so these keys can easily be used with either hand when the right hand is on the mouse.
