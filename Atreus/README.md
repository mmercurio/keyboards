# Atreus

This is my custom Kaleidoscope firmware for the [Keyboardio Atreus](https://shop.keyboard.io/products/keyboardio-atreus#), a minimal 44 key, split, columnar, ergonomic mechanical keyboard.

My Atreus layout is inspired by [Miryoku](https://github.com/manna-harbour/miryoku/), with the following notable differences and features:

* QWERTY layout for base layer
* vi-style navigation vs Miryoku arrows on home keys
* Additional macOS shortcuts on `nav` and `mouse` layers
* Swap `space` and `backspace` to match the layout on the [Keyboardio Model 100](https://shop.keyboard.io/products/model-100) I'm already familiar with.
* Kaleidoscope [MouseKeys Warping](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-MouseKeys.html#warping) with 2x2 grid
* Implementation differences to accommodate Kaleidoscope firmware and customizations for secondary modifier actions
* The Keyboardio Atreus has 44 keys and I make regular use of 38. The three outer keys on the last row of each hand are mostly ignored for regular use.

See [Miryoku Alternative Layout](https://github.com/manna-harbour/miryoku/tree/master/docs/reference#alternative-layouts) for some documented differences from default Miryoku layout.

## Qukeys and Home Row Mods

Until I learned about and started using [home row mods](https://precondition.github.io/home-row-mods), I was mostly unimpressed and ineffective with the Atreus. There just were not enough keys to comfortably accommodate how I was using the keyboard. That changed when I started using home *row mods*, or secondary actions on the home row keys that take on modifier keys such as `shift`, `control`, `alt`, when held and normal keys when tapped.

My only problem was that when typing quickly (and sometimes not so quickly) with the default Kaleidoscope firmware, I would frequently accidentally trigger unintentional modifier actions and run into other problems such as repeated home row keys not getting registered. This is what led me to customize and compile my own firmware.

### My Qukeys Settings

Home row mods and secondary actions in general, such as secondary actions on the thumb cluster keys, are controlled in Kaleidoscope using the [Qukeys](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html) plugin.

In order to reduce unintentionally activating modifiers and secondary actions while typing quickly, the following settings are changed from the default behavior:

* [`.setMaxIntervalForTapRepeat(timeout)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setmaxintervalfortaprepeat-timeout): set to less than the default and see [Dynamic Adjustments](#dynamic-adjustments) below.
* [`.setOverlapThreshold(percentage)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setoverlapthreshold-percentage): set to 100% and still not perfect.
* [`.setMinimumHoldTime(min_hold_time)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setoverlapthreshold-percentage): increased significantly from the default.
* [`.setMinimumPriorInterval(min_interval)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setminimumpriorinterval-min-interval): increased significantly from the default.

### Dynamic Adjustments

I reduced [`.setMaxIntervalForTapRepeat(timeout)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setmaxintervalfortaprepeat-timeout) to significantly lower values (e.g, less than 140 ms) than the default of 200 ms, in order to mitigate an issue where repeated quickly tapped home row keys would not register (e.g., typing "less" would only register one "s"). But this led to another problem: I could not tap and hold a key to have it repeat ([Tap-Repeat Behavior](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#tap-repeat-behaviour)). For most keys not having tap-repeat is not a huge problem, but for keys such as vi-style navigation, `backspace`, `space`, etc., this was much more of a nuisance.

To mitigate issues with the Qukeys Tap-Repeat behavior, I created the ability to dynamically modify some Qukey settings using configured macros which I map to keys on the `fun` layer. The macros are:

* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INFO`: type out info on current settings for debugging.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_DEC`: decrease the tap-repeat timeout.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_INC`: increase the tap-repeat timeout.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_OFF`: toggle the tap-repeat timeout between current value and 0 to temporarily disable it.
* `MACRO_QUKEYS_TAP_REPEAT_TIMEOUT_TOGGLE_MAX`: toggle the tap-repeat timeout between current value and its max, to temporarily allow easier repeating of held keys.

In general, I prefer values of 130-140 ms for the tap-repeat timeout interval. But when I want to occasionally take advantage of the tap-repeat behavior, I'm now able to quickly temporarily modify the settings.

## Layout Experiments

Some alternative layouts to solve specific issues which I'm still experimenting with.

### Alternative 01

Experimental alternative layout to solve specific issues with the original layout:

1. Using home row mods for `shift` is challenging when typing uppercase or mixed case words that alternate between hands, especially words containing letters on the home row. It would be nice to have access to `shift` on the thumb keys.
1. Copying and pasting is not easy when the right hand is on the mouse. On a normal QWERTY keyboard this is trivial with the left hand. However, with the original layout it requires a difficult contortion or chording two keys with the left hand.

Updates to address these issues:

1. add `shift` as secondary modifier on outer thumb keys (third from center on Atreus), repositioning layer shift keys for `fun` and `media` layers.
1. mirror the `paste`, `copy`, `cut` keys on the `mouse` layer to left hand so these keys can easily be used with either hand when the right hand is on the mouse.

See `layout_alt01.json` for details.
