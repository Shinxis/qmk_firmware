/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
// #define ORYX_CONFIGURATOR
// #define TAPPING_TERM 175

// Animations in use: // = enable:
   #define DISABLE_RGB_MATRIX_ALPHAS_MODS         // Static dual hue, speed is hue for secondary hue
   #define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN    // Static gradient top to bottom, speed controls how much gradient changes
   #define DISABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT // Static gradient left to right, speed controls how much gradient changes
// #define DISABLE_RGB_MATRIX_BREATHING           // Single hue brightness cycling animation
   #define DISABLE_RGB_MATRIX_BAND_SAT            // Single hue band fading saturation scrolling left to right
// #define DISABLE_RGB_MATRIX_BAND_VAL            // Single hue band fading brightness scrolling left to right
   #define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT   // Single hue 3 blade spinning pinwheel fades saturation
// #define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL   // Single hue 3 blade spinning pinwheel fades brightness
   #define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT     // Single hue spinning spiral fades saturation
   #define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL     // Single hue spinning spiral fades brightness
   #define DISABLE_RGB_MATRIX_CYCLE_ALL           // Full keyboard solid hue cycling through full gradient
   #define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT    // Full gradient scrolling left to right
// #define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN       // Full gradient scrolling top to bottom
   #define DISABLE_RGB_MATRIX_CYCLE_OUT_IN        // Full gradient scrolling out to in
   #define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL   // Full dual gradients scrolling out to in
   #define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON // Full gradient Chevron shapped scrolling left to right
   #define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL      // Full gradient spinning pinwheel around center of keyboard
   #define DISABLE_RGB_MATRIX_CYCLE_SPIRAL        // Full gradient spinning spiral around center of keyboard
// #define DISABLE_RGB_MATRIX_DUAL_BEACON         // Full gradient spinning around center of keyboard
   #define DISABLE_RGB_MATRIX_RAINBOW_BEACON      // Full tighter gradient spinning around center of keyboard
// #define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS   // Full dual gradients spinning two halfs of keyboard
   #define DISABLE_RGB_MATRIX_RAINDROPS           // Randomly changes a single key's hue
   #define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS // Randomly changes a single key's hue and saturation
   #define DISABLE_RGB_MATRIX_HUE_BREATHING        // Hue shifts up a slight ammount at the same time, then shifts back
   #define DISABLE_RGB_MATRIX_HUE_PENDULUM         // Hue shifts up a slight ammount in a wave to the right, then back to the left
   #define DISABLE_RGB_MATRIX_HUE_WAVE             // Hue shifts up a slight ammount and then back down in a wave to the right

// Space optimization:
#ifndef NO_DEBUG // !NO_DEBUG
#define NO_DEBUG
#endif
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE) // !NO_PRINT
#define NO_PRINT
#endif
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define DISABLE_LEADER

// tap dance fine-tuning:
// #define PERMISSIVE_HOLD

// Misc settings, originally from Oryx configurator:
#undef RGB_MATRIX_HUE_STEP
#define RGB_MATRIX_HUE_STEP 4 // default: 16
// #undef IGNORE_MOD_TAP_INTERRUPT
// #define USB_SUSPEND_WAKEUP_DELAY 0
#define CAPS_LOCK_STATUS
// #define RGB_MATRIX_STARTUP_SPD 60
