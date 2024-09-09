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

### Qukey Settings

Home row mods and secondary actions in general, such as secondary actions on the thumb cluster keys, are controlled in Kaleidoscope using the [Qukeys](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html) plugin.

In order to reduce unintentionally activating modifiers and secondary actions while typing quickly, the following settings are changed from the default behavior:

* `.setMaxIntervalForTapRepeat(timeout)`: set to less than the default and see dynamic adjustments below.
* `.setOverlapThreshold(percentage)`: set to 100% and still not perfect.
* `.setMinimumHoldTime(min_hold_time)`: increased significantly from the default.
* `.setMinimumPriorInterval(min_interval)`: increased significantly from the default.

### Dynamic Adjustments

I reduced [`.setMaxIntervalForTapRepeat(timeout)`](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#setmaxintervalfortaprepeat-timeout) to significantly lower values (e.g, 140 ms and below) than the default of 200 ms, in order to mitigate an issue where repeatedly quickly tapped home row keys would not register (e.g., typing "less" would only register one "s"). But this led to another problem: I could not tap and hold a key to have it repeat ([Tap-Repeat Behavior](https://kaleidoscope.readthedocs.io/en/latest/plugins/Kaleidoscope-Qukeys.html#tap-repeat-behaviour)). For most keys, this was not a problem for for many keys such as vi-style navigation arrows, backspace, space, etc., this was more of a nuisance.

To mitigate that, I created the ability to dynamically modify some Qukey settings using configured Macros on the `Fun` layer.

