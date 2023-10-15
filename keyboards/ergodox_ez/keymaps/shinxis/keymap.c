// move to             ~/qmk_firmware/keyboards/ergodox_ez/keymaps/shinxis/
// In Terminal, go to: ~/qmk_firmware
// and run:            make ergodox_ez:shinxis
// Link to this layout in (Ergodox EZ Glow) web editor https://configure.zsa.io/ergodox-ez/layouts/5BRRY/latest/0



// Features:
// 3 base layers: International English, Windows with swapped CTRL & GUI, and Mac with already customized keyboard layout (in software).
// 4 auxiliary layers accessible by chording of 4 thumb keys, that is, by fast hold & interrupt of otherwise normal thumb keys. Much like when performing a Copy-Paste.
// Auxiliary layers' lock-on with a shift key.
// Scandinavian AE and OE, that act as C (copy) and V (paste) with CTRL or GUI (ä and ö are not used for shortcuts anyway).
// Mirror keys for single-handed operation.
// Caps lock key and caps layer by chording of left & right shift keys.
// Windows alternative symbols (using Alt-keypad keycodes) with Alt double-hold. (Similar to Mac option-characters. Mostly unoccupied for now to save memory.)
/* Re-purposed basic keys and their shifted variants:    DOT .:    COMMA ,;    ASTERISK *?    SLASH /|    BACKSLASH \                               */
// Using of CTRL as SHIFT during Windows Alt-Tab application switching for easier reach and better resemblance to Mac behavior.

// caveats:
// while modified basics .,/\* are pressed, the code stays, so pressing SHIFT in the middle will result in the original character.
// The character ; cannot be held down on the first press when changing layers. Cannot alter modifiers mid-flight during some routines.
// Saving of current settings or layer to the flash does not work.

#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_bepo.h"

// #define KC_MAC_UNDO LGUI(KC_Z)
// #define KC_MAC_CUT LGUI(KC_X)
// #define KC_MAC_COPY LGUI(KC_C)
// #define KC_MAC_PASTE LGUI(KC_V)
// #define KC_PC_UNDO LCTL(KC_Z)
// #define KC_PC_CUT LCTL(KC_X)
// #define KC_PC_COPY LCTL(KC_C)
// #define KC_PC_PASTE LCTL(KC_V)
// #define ES_LESS_MAC KC_GRAVE
// #define ES_GRTR_MAC LSFT(KC_GRAVE)
// #define ES_BSLS_MAC ALGR(KC_6)
// #define NO_PIPE_ALT KC_GRAVE
// #define NO_BSLS_ALT KC_EQUAL
// #define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
// #define BP_NDSH_MAC ALGR(KC_8)

#define GENERAL_LAYER 0
#define WIN_LAYER 1
#define MAC_LAYER 2
#define CAPS_LAYER 3
#define CAPS_MAC_LAYER 4
#define MIRROR_LAYER 5
#define MIRROR_MAC_LAYER 6
#define MIRROR_NAVI_LAYER 7
#define NUM_LAYER 8                             // THUMB KEY L2
#define KP_LAYER 9                              // THUMB KEY R2
#define LNAVI_LAYER 10                          // THUMB KEY L1
#define RNAVI_LAYER 11                          // THUMB KEY R1
#define WIN_ALT_LAYER 12
#define GAME_LAYER 13
#define SETTINGS_LAYER 14

#define THUMB_TAP_TERM 200                      // Used in Thumb keys and Tap Dance.

// Aliases for per bit operations. Among other uses, used in debugging. a = the number, b = the tested bit, starting from the least singificant bit.
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))       // 1ULL is 1 as Unsigned Long Long. &= performs bitwise AND and stores it back to the variable. << shifts to left (b) bits.
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))    // ~ is bitwise NOT
#define BIT_TOGGLE(a,b) ((a) ^= (1ULL<<(b)))    // ^ is bitwise XOR
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))  // '!!' to make sure this returns 0 or 1



// For saving user settings to EEPROM:          // Does not seem to work
//typedef union {
//  uint32_t raw;
//  struct {
//    uint8_t my_base_layer :8;                 // uses 8 bits
//  };
//} user_config_t;
//user_config_t user_config;

//static bool DEBUGVAR = false;
//void debugprint(int _number);
void capslock_blink(void);                                                                                                              // Blink all signal LEDs
void capslayermods_keypress(keyrecord_t *record);                                                                                       // Combined code for modifiers for handling CapsLock layer
void scandic_keypress(keyrecord_t *record, uint16_t KC_A_O_keycode, uint16_t KC_C_V_keycode, bool *active_copy_paste, uint8_t charnum); // Combined code for scandinavian AE / OE keypress.
void win_alt_character(uint8_t charnum);                                                                                                // Output special characters in Windows using Alt-KeyPad method.
void winalt_normalpress(void);                                                                                                          // Combined code for WinAlt pressed (timeout) and WinAlt pressed (interrupted).
void shift_normalpress(void);                                                                                                           // Combined code for shift pressed (timeout) and shift pressed (interrupted).
void thumb_keypress(uint16_t thumb_key, uint16_t thumb_keycode, uint8_t thumb_layer, bool thumb_pressed, bool *KC_LRMOD_ACTIV );        // Combined code for thumb keys.

// -------------------------------------------------------------------------------- CUSTOM KEYCODES --------------------------------------------------------------------------------

enum custom_keycodes {
  KC_THUMB_L1 = EZ_SAFE_RANGE,                  // ----- 'THUMB' KEYS
  KC_THUMB_R1,
  KC_THUMB_L2,
  KC_THUMB_R2,
  KC_NEWLSHIFT,
  KC_NEWRSHIFT,
  KC_LMIRROR,
  KC_RMIRROR,
  KC_MIRROR_NAVI,                               // to MIRROR_NAVI layer from MIRROR layer
  KC_WINLALT,
  KC_WINRALT,
  KC_WINLCTRL,                                  // Used on Windows layer. When Alt-Tab is used, LCTRL acts as shift
  
  // KC_LALT,                                   // ----- MODIFIED BASIC KEYCODES
  // KC_RALT,
  // KC_LGUI,
  // KC_RGUI,
  // KC_LCTRL,
  // KC_RCTRL,
  // KC_NUMLOCK,
  // KC_TAB,
  
  KC_NEWDOT,                                    // ----- CUSTOM KEYCODES, NEW BASICS
  KC_NEWCOMMA,
  KC_NEWLABK,
  KC_NEWRABK,
  KC_NEWQUOTE,
  KC_NEWEQUAL,
  KC_NEWPLUS,
  KC_NEWASTERISK,
  KC_NEWSLASH,
  KC_NEWBSLASH,
  KC_NEWLBRACKET,
  KC_NEWRBRACKET,
                                                // ----- CUSTOM KEYCODES, EXOTIC
  KC_ADIA,                                      // Scandinavian Ä
  KC_ODIA,                                      // Scandinavian Ö
  KC_ARING,                                     // Scandinavian Å  (Windows)
  KC_AE,                                        // Scandinavian Æ  (Windows)
  KC_OSTROKE,                                   // Scandinavian Ø  (Windows)
  KC_ETH,                                       // Scandinavian Ð  (Windows)
  KC_THORN,                                     // Scandinavian Þ  (Windows)
  KC_UDIA,                                      // German Umlaut Ü (Windows)
  KC_ESZETT,                                    // German ß        (Windows)
  KC_MU,                                        // Greek µ         (Windows)
  KC_DEGREE,                                    // Degree sign     (Windows)
  KC_MACVOL_UP,                                 // Mac Volume fine step
  KC_MACVOL_DN,                                 // Mac Volume fine step
  
  KC_SETTING_NO_0,                              // ----- CUSTOM KEYCODES, SPECIALS
  KC_SETTING_NO_1,                              // Custom keys on Settings-layer for switching and saving the (default) base layer
  KC_SETTING_NO_2,
  KC_SETTING_NO_3,
  // KC_TEST1,
  // KC_TEST2,
  // KC_TEST3,
  RGB_SLD,                                      // Ergodox EZ default
};

// -------------------------------------------------------------------------------- THUMB KEYS --------------------------------------------------------------------------------

typedef struct {                                // Thumb keys
  uint16_t timer;                               // Timer value. Unsigned 16 bits.
  bool     active;                              // Timer is running.
  bool     pressed;                             // The thumb key status, up/dn.
  uint16_t key;                                 // Key and keycode, because some keys (like thumb spaces) send the same keycode but have different function. Also easier to track the original keycode.
  uint16_t keycode;                             // Keycode to be outputted during matrix scan and interrupt handling.
  uint8_t  layer;                               // Layer to swap to, if applicable.
  bool     twin;                                // Two symmetrical keys on opposite sides pressed at the same time, used as a trigger.
  uint8_t  tapcount;                            // Tap count. Limited to 4.
  uint8_t  locklayer;                           // Number of the (topmost) layer that is locked on. Prevent thumbkeys from turning off a layer if it is activated from somewhere else.
  bool     interrupter_active;                  // Layer changed due to an interrupted thumb key that is still pressed or not.
  uint8_t  interrupter_col;                     // Store location of the key that interrupts the thumb key and triggers the layer change. Needed for unregistering it properly.
  uint8_t  interrupter_row;                     // Normally QMK would only remember the original keycode from the earlier (lower) layer even on key release.
  uint16_t interrupter_keycode;                 // Converted/from-flash-re-fetched keycode to be unregistered on the release of the thumb-interrupting key.
} thumb_t;
static thumb_t thumb = {
   .timer = 0,
   .active = false,
   .pressed = false,
   .key = 0,
   .keycode = 0,
   .layer = 0,
   .twin = 0,
   .tapcount = 1,
   .locklayer = 0,
   .interrupter_active = false,
   .interrupter_col = 0,
   .interrupter_row = 0,
   .interrupter_keycode = 0,
 };

// -------------------------------------------------------------------------------- VARIABLES --------------------------------------------------------------------------------

typedef struct {
  bool    Copy;                                 // Scandinavian AE/OE with copy, key held down
  bool    Paste;                                // Scandinavian AE/OE with paste, key held down
  bool    LSHIFT;                               // Clone shift status for shift+key failsafe if shift is released before the key
  bool    RSHIFT;
  bool    LALT;                                 // Clone alt status for alt-character failsafe if layer changes before key release. Alt-characters are used on Mac layer.
  bool    RALT;
  bool    CapsLayerMods;                        // Modifiers will temporarily deactivate CapsLock layer for unshifted shortcuts.
  uint8_t CapsLayerModsCount;                   // Modifiers will temporarily deactivate CapsLock layer for unshifted shortcuts. Number of active mods (LGUI, RGUI, LCTRL, RCTRL).
  bool    WinTab;                               // When using Alt-Tab on Windows layer, adjacent CTRL acts as shift for better reach, making it similar to Mac layer
  bool    WinTabShift;                          // Associated with the above
  bool    BspaceDel;                            // SHIFT-BACKSPACE gives DELETE.
} active_t;
static active_t active = {
  .Copy = false,
  .Paste = false,
  .LSHIFT = false,
  .RSHIFT = false,
  .LALT = false,
  .RALT = false,
  .CapsLayerMods = false,
  .CapsLayerModsCount = 0,
  .WinTab = false,
  .WinTabShift = false,
  .BspaceDel = false,
};

// pre-calculate and store bit mask of left and right GUI and CTRL for later use:
const uint8_t SCANDIC_MODS = MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI) | MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL);
// shortens this in the if-condition: ((get_mods() & MOD_BIT(KC_LGUI) || get_mods() & MOD_BIT(KC_RGUI)) || (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTL)))

// -------------------------------------------------------------------------------- BASE --------------------------------------------------------------------------------

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {      // PROGMEM saves (or loads) using flash
  [GENERAL_LAYER] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_NEWEQUAL,                                    KC_QUOTE,       KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           TG(SETTINGS_LAYER),
    KC_ESCAPE,      KC_Q,           KC_Y,           KC_U,           KC_W,           KC_F,           KC_NEWPLUS,                                     KC_NEWDOT,      KC_C,           KC_H,           KC_R,           KC_M,           KC_B,           TG(SETTINGS_LAYER),
    KC_TAB,         KC_I,           KC_O,           KC_E,           KC_A,           KC_G,                                                                           KC_P,           KC_L,           KC_N,           KC_S,           KC_T,           _______,
    KC_GRAVE,       KC_LMIRROR,     KC_Z,           KC_X,           KC_ADIA,        KC_ODIA,        KC_MINUS,                                       KC_NEWCOMMA,    KC_J,           KC_K,           KC_V,           KC_D,           KC_RMIRROR,     _______,
    _______,        KC_LCTRL,       KC_LALT,        KC_LGUI,        KC_NEWLSHIFT,                                                                                                   KC_NEWRSHIFT,   KC_RGUI,        KC_RALT,        KC_RCTRL,       _______,
                                                                                                    KC_DELETE,      TG(GAME_LAYER), _______,        _______,
                                                                                                                     TG(NUM_LAYER), TG(KP_LAYER),
                                                                                    KC_THUMB_L1,    KC_THUMB_L2,   TG(LNAVI_LAYER), TG(RNAVI_LAYER), KC_THUMB_R2,  KC_THUMB_R1
  ),
  [WIN_LAYER] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_NEWEQUAL,                                    KC_QUOTE,       KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           TG(SETTINGS_LAYER),
    KC_ESCAPE,      KC_Q,           KC_Y,           KC_U,           KC_W,           KC_F,           KC_NEWPLUS,                                     KC_NEWDOT,      KC_C,           KC_H,           KC_R,           KC_M,           KC_B,           TG(SETTINGS_LAYER),
    KC_TAB,         KC_I,           KC_O,           KC_E,           KC_A,           KC_G,                                                                           KC_P,           KC_L,           KC_N,           KC_S,           KC_T,           _______,
    KC_GRAVE,       KC_LMIRROR,     KC_Z,           KC_X,           KC_ADIA,        KC_ODIA,        KC_MINUS,                                       KC_NEWCOMMA,    KC_J,           KC_K,           KC_V,           KC_D,           KC_RMIRROR,     _______,
    _______,        KC_LGUI,        KC_WINLALT,     KC_WINLCTRL,    KC_NEWLSHIFT,                                                                                                   KC_NEWRSHIFT,   KC_RCTRL,       KC_WINRALT,     KC_RGUI,        _______,
                                                                                                    KC_DELETE,      TG(GAME_LAYER), _______,        _______,
                                                                                                                     TG(NUM_LAYER), TG(KP_LAYER),
                                                                                    KC_THUMB_L1,    KC_THUMB_L2,   TG(LNAVI_LAYER), TG(RNAVI_LAYER), KC_THUMB_R2,   KC_THUMB_R1
  ),
  [MAC_LAYER] = LAYOUT_ergodox_pretty(
    KC_ESCAPE,      KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_Y,                                           KC_H,           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           TG(SETTINGS_LAYER),
    KC_ESCAPE,      KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,           KC_EQUAL,                                       KC_N,           KC_U,           KC_I,           KC_O,           KC_P,           KC_LBRACKET,    TG(SETTINGS_LAYER),
    KC_TAB,         KC_A,           KC_S,           KC_D,           KC_F,           KC_G,                                                                           KC_J,           KC_K,           KC_L,           KC_SCOLON,      KC_QUOTE,       _______,
    KC_GRAVE,       KC_LMIRROR,     KC_Z,           KC_X,           KC_C,           KC_V,           KC_MINUS,                                       KC_B,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RMIRROR,     _______,
    _______,        KC_LCTRL,       KC_LALT,        KC_LGUI,        KC_NEWLSHIFT,                                                                                                   KC_NEWRSHIFT,   KC_RGUI,        KC_RALT,        KC_RCTRL,       _______,
                                                                                                    KC_DELETE,      TG(GAME_LAYER), _______,        _______,
                                                                                                                     TG(NUM_LAYER), TG(KP_LAYER),
                                                                                    KC_THUMB_L1,    KC_THUMB_L2,   TG(LNAVI_LAYER), TG(RNAVI_LAYER), KC_THUMB_R2,   KC_THUMB_R1
  ),
  [CAPS_LAYER] = LAYOUT_ergodox_pretty(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        LSFT(KC_Q),     LSFT(KC_Y),     LSFT(KC_U),     LSFT(KC_W),     LSFT(KC_F),     _______,                                        _______,        LSFT(KC_C),     LSFT(KC_H),     LSFT(KC_R),     LSFT(KC_M),     LSFT(KC_B),     TG(SETTINGS_LAYER),
    _______,        LSFT(KC_I),     LSFT(KC_O),     LSFT(KC_E),     LSFT(KC_A),     LSFT(KC_G),                                                                     LSFT(KC_P),     LSFT(KC_L),     LSFT(KC_N),     LSFT(KC_S),     LSFT(KC_T),     _______,
    _______,        _______,        LSFT(KC_Z),     LSFT(KC_X),     KC_ADIA,        KC_ODIA,        _______,                                        _______,        LSFT(KC_J),     LSFT(KC_K),     LSFT(KC_V),     LSFT(KC_D),     _______,        _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [CAPS_MAC_LAYER] = LAYOUT_ergodox_pretty(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        _______,        _______,        _______,        _______,        _______,                                                                        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [MIRROR_LAYER] = LAYOUT_ergodox_pretty(
    TG(SETTINGS_LAYER), KC_0,       KC_9,           KC_8,           KC_7,           KC_6,           KC_QUOTE,                                       KC_EQUAL,       KC_5,           KC_4,           KC_3,           KC_2,           KC_1,           KC_ESCAPE,
    TG(SETTINGS_LAYER), KC_B,       KC_M,           KC_R,           KC_H,           KC_C,           KC_NEWDOT,                                      KC_PLUS,        KC_F,           KC_W,           KC_U,           KC_Y,           KC_Q,           KC_ESCAPE,
    XXXXXXX,        KC_T,           KC_S,           KC_N,           KC_L,           KC_P,                                                                           KC_G,           KC_A,           KC_E,           KC_O,           KC_I,           KC_TAB,
    XXXXXXX,        _______,        KC_D,           KC_V,           KC_K,           KC_J,           KC_NEWCOMMA,                                    KC_MINUS,       KC_ODIA,        KC_ADIA,        KC_X,           KC_Z,           _______,        KC_GRAVE,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    KC_MIRROR_NAVI, KC_ENTER,       _______,        _______,        KC_BSPACE,      KC_MIRROR_NAVI
  ),
  [MIRROR_MAC_LAYER] = LAYOUT_ergodox_pretty(
    TG(SETTINGS_LAYER), KC_0,       KC_9,           KC_8,           KC_7,           KC_6,           KC_H,                                           KC_Y,           KC_5,           KC_4,           KC_3,           KC_2,           KC_1,           KC_ESCAPE,
    TG(SETTINGS_LAYER), KC_LBRACKET,KC_P,           KC_O,           KC_I,           KC_U,           KC_N,                                           KC_EQUAL,       KC_T,           KC_R,           KC_E,           KC_W,           KC_Q,           KC_ESCAPE,
    XXXXXXX,        KC_QUOTE,       KC_SCOLON,      KC_L,           KC_K,           KC_J,                                                                           KC_G,           KC_F,           KC_D,           KC_S,           KC_A,           KC_TAB,
    XXXXXXX,        _______,        KC_SLASH,       KC_DOT,         KC_COMMA,       KC_M,           KC_B,                                           KC_MINUS,       KC_V,           KC_C,           KC_X,           KC_Z,           _______,        KC_GRAVE,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    KC_MIRROR_NAVI, KC_ENTER,       _______,        _______,        KC_BSPACE,      KC_MIRROR_NAVI
  ),
  [MIRROR_NAVI_LAYER] = LAYOUT_ergodox_pretty(
    TG(SETTINGS_LAYER),  KC_0,      KC_9,           KC_8,           KC_7,           KC_6,           KC_NEWQUOTE,                                    KC_NEWEQUAL,    KC_5,           KC_4,           KC_3,           KC_2,           KC_1,           KC_ESCAPE,
    TG(SETTINGS_LAYER), KC_NEWSLASH, KC_HOME,       KC_UP,          KC_END,         KC_NEWQUOTE,    KC_NEWDOT,                                      KC_NEWPLUS,     KC_NEWEQUAL,    KC_HOME,        KC_UP,          KC_END,         KC_NEWBSLASH,   KC_ESCAPE,
    XXXXXXX,        KC_PGUP,        KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_NEWDOT,                                                                      KC_NEWPLUS,     KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_PGUP,        KC_TAB,
    XXXXXXX,        KC_PGDOWN,      KC_NEWLBRACKET, KC_NEWASTERISK, KC_NEWRBRACKET, KC_NEWCOMMA,    KC_NEWCOMMA,                                    KC_MINUS,       KC_MINUS,       KC_NEWLABK,     KC_NEWEQUAL,    KC_NEWRABK,     KC_PGDOWN,      KC_GRAVE,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [NUM_LAYER] = LAYOUT_ergodox_pretty(                                                                                                                      // Dynamic Macros would go here, but they crash the whole firmware with layer changes.
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        XXXXXXX,        KC_9,           KC_8,           KC_7,           KC_NEWCOMMA,    _______,                                        _______,        XXXXXXX,        KC_MEDIA_PREV_TRACK,KC_MEDIA_PLAY_PAUSE,KC_MEDIA_NEXT_TRACK, XXXXXXX, TG(SETTINGS_LAYER),
    _______,        XXXXXXX,        KC_6,           KC_5,           KC_4,           KC_NEWDOT,                                                                      XXXXXXX,        KC_AUDIO_MUTE,  KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP, XXXXXXX,     _______,
    _______,        XXXXXXX,        KC_3,           KC_2,           KC_1,           KC_0,           _______,                                        _______,        XXXXXXX,        XXXXXXX,        KC_MACVOL_DN,   KC_MACVOL_UP,   XXXXXXX,        _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [KP_LAYER] = LAYOUT_ergodox_pretty(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        XXXXXXX,        KC_HOME,        KC_UP,          KC_END,         KC_NEWEQUAL,    _______,                                        _______,        KC_NEWCOMMA,    KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_NUMLOCK,     TG(SETTINGS_LAYER),
    _______,        KC_PGUP,        KC_LEFT,        KC_DOWN,        KC_RIGHT,       XXXXXXX,                                                                        KC_KP_DOT,      KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_KP_PLUS,     KC_KP_ASTERISK,
    _______,        KC_PGDOWN,      XXXXXXX,        XXXXXXX,        KC_ADIA,        KC_ODIA,        _______,                                        _______,        KC_KP_0,        KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_KP_MINUS,    KC_KP_SLASH,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [LNAVI_LAYER] = LAYOUT_ergodox_pretty(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        KC_NEWBSLASH,   KC_HOME,        KC_UP,          KC_END,         KC_NEWEQUAL,    _______,                                        _______,        KC_MS_BTN2,     KC_MS_WH_LEFT,  KC_MS_UP,       KC_MS_WH_RIGHT, XXXXXXX,        TG(SETTINGS_LAYER),
    _______,        KC_PGUP,        KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_NEWPLUS,                                                                     KC_MS_BTN3,     KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_MS_WH_UP,    _______,
    _______,        KC_PGDOWN,      KC_NEWLABK,     KC_NEWEQUAL,    KC_NEWRABK,     KC_MINUS,       _______,                                        _______,        KC_MS_BTN1,     KC_MS_ACCEL0,   KC_MS_ACCEL1,   KC_MS_ACCEL2,   KC_MS_WH_DOWN,  _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [RNAVI_LAYER] = LAYOUT_ergodox_pretty(
    _______,        _______,        _______,        _______,        _______,        _______,        _______,                                        _______,        _______,        _______,        _______,        _______,        _______,        TG(SETTINGS_LAYER),
    _______,        KC_F13,         KC_F9,          KC_F8,          KC_F7,          XXXXXXX,        _______,                                        _______,        KC_NEWQUOTE,    KC_HOME,        KC_UP,          KC_END,         KC_NEWSLASH,    TG(SETTINGS_LAYER),
    _______,        KC_F12,         KC_F6,          KC_F5,          KC_F4,          XXXXXXX,                                                                        KC_NEWDOT,      KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_PGUP,        XXXXXXX   ,
    _______,        KC_F11,         KC_F3,          KC_F2,          KC_F1,          KC_F10,         _______,                                        _______,        KC_NEWCOMMA,    KC_NEWLBRACKET, KC_NEWASTERISK, KC_NEWRBRACKET, KC_PGDOWN,      XXXXXXX,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [WIN_ALT_LAYER] = LAYOUT_ergodox_pretty(
    _______,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        _______,                                        _______,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        TG(SETTINGS_LAYER),
    _______,        XXXXXXX,        XXXXXXX,        KC_UDIA,        XXXXXXX,        XXXXXXX,        _______,                                        _______,        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_MU,          KC_BSLASH,      TG(SETTINGS_LAYER),
    _______,        XXXXXXX,        KC_OSTROKE,     XXXXXXX,        KC_ARING,       XXXXXXX,                                                                        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_ESZETT,      KC_THORN,       _______,
    _______,        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_AE,          XXXXXXX,        _______,                                        _______,        XXXXXXX,        XXXXXXX,        KC_DEGREE,      KC_ETH,         XXXXXXX,        _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    _______,        _______,        _______,        _______,        _______,        _______
  ),
  [GAME_LAYER] = LAYOUT_ergodox_pretty(
    _______,        KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           _______,                                        _______,        KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           TG(SETTINGS_LAYER),
    _______,        KC_V,           KC_Q,           KC_W,           KC_E,           KC_T,           _______,                                        _______,        KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           TG(SETTINGS_LAYER),
    _______,        KC_R,           KC_A,           KC_S,           KC_D,           KC_G,                                                                           KC_H,           KC_J,           KC_K,           KC_L,           KC_SCOLON,      _______,
    _______,        KC_F,           KC_Z,           KC_X,           KC_C,           KC_B,           _______,                                        _______,        KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_SLASH,       _______,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    _______,        _______,        _______,        _______,
                                                                                                                    _______,        _______,
                                                                                    KC_SPACE,       KC_BSPACE,      _______,        _______,        KC_ENTER,       KC_SPACE
  ),
  [SETTINGS_LAYER] = LAYOUT_ergodox_pretty(
    TG(SETTINGS_LAYER), XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,                                        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        TG(SETTINGS_LAYER),
    TG(SETTINGS_LAYER), XXXXXXX,    XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,                                        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        TG(SETTINGS_LAYER),
    XXXXXXX,    KC_SETTING_NO_0, KC_SETTING_NO_1, KC_SETTING_NO_2, KC_SETTING_NO_3, XXXXXXX,                                                                        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,
    XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,                                        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,
    _______,        _______,        _______,        _______,        _______,                                                                                                        _______,        _______,        _______,        _______,        _______,
                                                                                                    RGB_MOD,        RGB_SLD,        TOGGLE_LAYER_COLOR,RGB_TOG,
                                                                                                                    XXXXXXX,        XXXXXXX,
                                                                                    RGB_HUD,        RGB_HUI,        XXXXXXX,        XXXXXXX,        RGB_VAD,        RGB_VAI
  )
};

extern bool g_suspend_state;
extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {                                         // Call the keymap level matrix init.
  rgb_matrix_enable();
  default_layer_set(0);
  layer_move(0);                                                             // Otherwise layer number 1 (WIN_LAYER) is active for some reason?
  ergodox_right_led_1_on();                                                  // This is to indicate that the keyboard has booted up, and layers are set to defaults. Cannot save user settings for now..
  ergodox_right_led_2_on();
  ergodox_right_led_3_on();
  // user_config.raw = eeconfig_read_user();                                 // Read the user config from EEPROM. Does not seem to work.
  // default_layer_set(user_config.my_base_layer);                           // Set default layer, if enabled
}

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
  //  HSV_RED     = {0,255,255}        HSV_CYAN    = {129,255,255}
  //  HSV_GREEN   = {86,255,255}       HSV_MAGENTA = {215,255,255}
  //  HSV_BLUE    = {172,255,255}      HSV_YELLOW  = {43,255,255}
  //  HSV_WHITE   = {0,0,255}          HSV_BLACK   = {0,0,0}
  
    /* LEFT SIDE LEDS MIRRORED */                                            /* RIGHT SIDE LEDS */
  [CAPS_LAYER] = {
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                                             {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                   {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  } },

  [CAPS_MAC_LAYER] = {
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                                                                                             {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
    {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  },
                   {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  } },

  [MIRROR_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [MIRROR_MAC_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                              {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
    {HSV_WHITE  }, {HSV_RED    }, {HSV_GREEN  }, {HSV_BLUE   }, {HSV_YELLOW },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [MIRROR_NAVI_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_GREEN  }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_CYAN   },
                                                                              {HSV_RED    }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
                                                                              {HSV_BLUE   }, {HSV_YELLOW }, {HSV_GREEN  }, {HSV_YELLOW }, {HSV_BLACK  },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_YELLOW }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_CYAN   },
    {HSV_YELLOW }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
    {HSV_YELLOW }, {HSV_GREEN  }, {HSV_MAGENTA}, {HSV_GREEN  }, {HSV_BLACK  },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [NUM_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_RED    }, {HSV_CYAN   }, {HSV_GREEN  }, {HSV_MAGENTA}, {HSV_RED    },
                                                                              {HSV_GREEN  }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_RED    }, {HSV_GREEN  },
                                                                              {HSV_BLUE   }, {HSV_BLACK  }, {HSV_BLUE   }, {HSV_RED    }, {HSV_BLACK  },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_BLACK  },
    {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_BLACK  },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [KP_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_BLACK  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_YELLOW },
                                                                              {HSV_BLACK  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_RED    },
                                                                              {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLUE   },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [LNAVI_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_RED    }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_RED    }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
                                                                              {HSV_RED    }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_BLACK  },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_GREEN  }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_CYAN   },
    {HSV_RED    }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
    {HSV_BLUE   }, {HSV_YELLOW }, {HSV_GREEN  }, {HSV_YELLOW }, {HSV_BLACK  },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [RNAVI_LAYER] = {
                                                                              {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
                                                                              {HSV_YELLOW }, {HSV_BLACK  }, {HSV_WHITE  }, {HSV_BLACK  }, {HSV_CYAN   },
                                                                              {HSV_YELLOW }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_BLACK  },
                                                                              {HSV_YELLOW }, {HSV_GREEN  }, {HSV_MAGENTA}, {HSV_GREEN  }, {HSV_BLACK  },
                                                                                             {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLACK  },
    {HSV_BLACK  }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLACK  },
    {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLUE   }, {HSV_BLACK  },
                   {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  }, {HSV_BLACK  } },

  [WIN_ALT_LAYER] = {
                                                                              {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
                                                                              {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
                                                                              {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
                                                                              {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
                                                                                             {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
    {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
    {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
    {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
    {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    },
                   {HSV_RED    }, {HSV_RED    }, {HSV_RED    }, {HSV_RED    } },

  [GAME_LAYER] = {
                                                                              {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
                                                                              {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
                                                                              {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
                                                                              {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
                                                                                             {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
    {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
    {HSV_GREEN  }, {HSV_GREEN  }, {HSV_WHITE  }, {HSV_GREEN  }, {HSV_GREEN  },
    {HSV_GREEN  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_WHITE  }, {HSV_GREEN  },
    {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  },
                   {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  }, {HSV_GREEN  } }
  };

void set_layer_color(int layer) {
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

void rgb_matrix_indicators_user(void) {
  if (g_suspend_state || keyboard_config.disable_layer_led) { return; }
  switch (biton32(layer_state)) {
    case CAPS_LAYER:
      set_layer_color(CAPS_LAYER);
      break;
    case CAPS_MAC_LAYER:
      set_layer_color(CAPS_MAC_LAYER);
      break;
    case MIRROR_LAYER:
      set_layer_color(MIRROR_LAYER);
      break;
    case MIRROR_MAC_LAYER:
      set_layer_color(MIRROR_MAC_LAYER);
      break;
    case MIRROR_NAVI_LAYER:
      set_layer_color(MIRROR_NAVI_LAYER);
      break;
    case NUM_LAYER:
      set_layer_color(NUM_LAYER);
      break;
    case KP_LAYER:
      set_layer_color(KP_LAYER);
      break;
    case LNAVI_LAYER:
      set_layer_color(LNAVI_LAYER);
      break;
    case RNAVI_LAYER:
      set_layer_color(RNAVI_LAYER);
      break;
    case WIN_ALT_LAYER:
      set_layer_color(WIN_ALT_LAYER);
      break;
    case GAME_LAYER:
      set_layer_color(GAME_LAYER);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
}

uint32_t layer_state_set_user(uint32_t state) {
  uint8_t layer = biton32(state);
  ergodox_board_led_off();
  ergodox_right_led_1_off();                                                 // reset the CapsLock etc. LEDS
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  // clear_keyboard();
  // clear_mods();
  clear_keyboard_but_mods();                                                 // Release all keys on layer change. Save modifiers, they may be active during layer change.
  if (active.LSHIFT) register_code(KC_LSHIFT);                               // Shifts to reflect the physical keys. Some custom keys may leave shifts stuck during layer change.
  else               unregister_code(KC_LSHIFT);
  if (active.RSHIFT) register_code(KC_RSHIFT);
  else               unregister_code(KC_RSHIFT);
  if (active.LALT) register_code(KC_LALT);                                   // The same for alts. The custom Mac Layer keys use alt in character output.
  else             unregister_code(KC_LALT);
  if (active.RALT) register_code(KC_RALT);
  else             unregister_code(KC_RALT);
  
  thumb.interrupter_active = false;                                          // The layer-triggering / interrupting key is also released, update the variable. No need to wait for any release.
  switch (layer) {
    case SETTINGS_LAYER:                                                     // turn on indication LEDs when moved to SETTINGS_LAYER
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
      clear_keyboard();
      // thumb.interrupter_active = false;
      thumb.locklayer = 0;                                                   // layerlock off, let all layers close
      break;
    default:
      break;
  }
  return state;
};

// -------------------------------------------------------------------------------- DEBUG & MISC --------------------------------------------------------------------------------

// void debugprint(int _number) {                                            // print out variable values for debugging purposes
//   tap_code(KC_SPACE);
//   for(int i=16; i>=0; i--) {
//     if (BIT_CHECK(_number, i)) {tap_code(KC_1);}
//     else                       {tap_code(KC_0);}
//   }
//   tap_code(KC_SPACE);
// }

void capslock_blink(void) {                                                  // saved almost 300 bytes in the KC_SETTING keys (?)
  for(int i=1;i<10;i++){
    ergodox_right_led_1_on(); wait_ms(50);
    ergodox_right_led_2_on(); wait_ms(50);
    ergodox_right_led_3_on(); wait_ms(50);
    ergodox_right_led_1_off(); wait_ms(50);
    ergodox_right_led_2_off(); wait_ms(50);
    ergodox_right_led_3_off(); wait_ms(50);
  }
}

void capslayermods_keypress(keyrecord_t *record) {                           // Combined code for modifier keys for handling CapsLock layer. Temporarily disable CapsLock layer while modifiers are pressed.
  if (record->event.pressed) {
    if (layer_state_is(CAPS_LAYER)) {                                        // if CapsLock layer is on, temporarily disable it. CAPS_MAC_LAYER is transparent and may be ignored.
      layer_off(CAPS_LAYER);
      active.CapsLayerMods = true;
    }
    if (active.CapsLayerMods) active.CapsLayerModsCount++;                   // One more modifier pressed. Even when CapsLock layer is not active anymore.
  }
  else {                                                                     // Key release.
    if (active.CapsLayerMods) {                                              // Re-activate the CapsLock layer, if it was on previously.
      active.CapsLayerModsCount--;                                           // one less modifier pressed
      if (active.CapsLayerModsCount == 0) {                                  // all 4 (Caps-) modifiers are up
        layer_on(CAPS_LAYER);
        active.CapsLayerMods = false;
      }
    }
  }
}

// -------------------------------------------------------------------------------- CUSTOM KEYCODES, SCANDINAVIAN AE OE COMBINED --------------------------------------------------------------------------------

void scandic_keypress(keyrecord_t *record, uint16_t KC_A_O_keycode, uint16_t KC_C_V_keycode, bool *active_copy_paste, uint8_t charnum) {
  if (record->event.pressed) {                                               // Do something when pressed
    uint8_t tempmods = get_mods();                                           // remember SHIFTs and others
    if ((tempmods & SCANDIC_MODS) != 0) {                                    // if the mods are down
      register_code(KC_C_V_keycode);                                         //
      *active_copy_paste = true; }                                           // enables holding of the key
    else {                                                                   // if the key is pressed normally without the mods
      if (!layer_state_is(KP_LAYER)) {                                       // (Re)Use only copy-paste part on KP layer. Alternative is to use KC_C and KC_V, but they also output the character.
      // if (get_highest_layer(layer_state) != KP_LAYER) {                   // This would be safer, but takes 14 bytes more (total 24), and KP_LAYER overrides all (lower layer) scandic_keypress character outputs anyway.
        if (layer_state_is(WIN_LAYER)) {                                     // ALT-keypad combo for windows, International English layout dead key for others
          if (((tempmods & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) { // if a shift key(s) was pressed or CAPS_LAYER is active
          // if (((tempmods & MOD_MASK_SHIFT) != 0) || (layer_state_is(CAPS_LAYER))) { // if a shift key(s) was pressed or CAPS_LAYER is active
            win_alt_character(charnum);
          }
          else {                                                             // if no shifts are pressed
            win_alt_character(charnum+0x20);                                 // lower case letter
          }
        }
        else {                                                               // non-Windows layout
          unregister_mods(MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT));              // Alt-Ä and Alt-Ö do not make sense here.
          // unregister_code(KC_LALT); unregister_code(KC_RALT);             // These work also. 'register' = send the key report immediately. register_mods is used here to combine the modifier presses.
          // del_mods(MOD_MASK_ALT);
          unregister_code(KC_A_O_keycode);                                   // if 'A' is already pressed, unregister it first to be able to re-register it as Ä
          register_code(KC_LSHIFT);
          // add_mods(MOD_BIT(KC_LSHIFT));
          tap_code(KC_QUOTE);
          if (layer_state_is(CAPS_LAYER)) {
            tap_code(KC_A_O_keycode);
            set_mods(tempmods);
          }
          else {
            set_mods(tempmods);                                              // Use shift or not, depending on the pre-keypress state.
            tap_code(KC_A_O_keycode);
          }
        }
      }
    }
  }
  else {                                                                     // on key release
    if (*active_copy_paste) {
      unregister_code(KC_C_V_keycode);
      *active_copy_paste = false;
    }
  }
}

// -------------------------------------------------------------------------------- CUSTOM KEYCODES, WINDOWS ALT-CODES --------------------------------------------------------------------------------

void win_alt_character(uint8_t charnum) {
  uint8_t digit;
  uint8_t decimal;
  uint8_t tempmods = get_mods();                                             // Store modifiers
  set_mods(MOD_BIT(KC_LALT));                                                // Only LALT pressed
  // clear_mods();                                                           // Clear modifiers.
  // register_code(KC_LALT);
  tap_code(KC_KP_0);                                                         // All Alt-keycodes of interest begin with 0.
  for(decimal=100; decimal>=1; decimal=decimal/10) {
    digit = charnum / decimal;                                               // Most significant decimal digit. Only integers, fractions are discarded because of the variable type.
    charnum = charnum - digit * decimal;                                     // Leave only the fraction part for the next turn.
    if (digit == 0) digit = 10;                                              // KC_KP_0 comes after KC_KP_9 in USB HID keycodes, therefore its ordinal is 10 instead of 0.
    tap_code( KC_KP_1 -1 + digit );                                          // digit = 1 corresponds to KC_KP_1.
  }
  if (active.LALT) register_code(KC_LALT);                                   // Just in case. set_mods does not seem to refresh the modifiers.
  else             unregister_code(KC_LALT);                                 //
  set_mods(tempmods);
}

// -------------------------------------------------------------------------------- THUMB KEYS, WINDOWS ALT --------------------------------------------------------------------------------

void winalt_normalpress(void) {                                                // 'Normal' WINALT keypress/output. Used in timeout (matrix_scan) and interrupt (early process_record).
  if (layer_state_is(WIN_ALT_LAYER))                                           // A double hold has occured if this layer is on. On this layer, Alts output normal keypress/release immediately, and this function is not called...
    { }                                                                        // .. unless if it is the initial layer-triggering event. So do nothing. Everything is already done in the interrupt-section.
  else {
    if (thumb.tapcount == 1) {
      register_code(thumb.keycode);                                            //
      if (!thumb.pressed)                                                      // if not pressed anymore, only TAP. Otherwise leave it as HOLD.
        unregister_code(thumb.keycode);
    }
    if (thumb.tapcount == 2) {                                                 // Double Tap. Double hold must not output anything, because it (always) activates Alt layer, already on the second keydown.
      if (!thumb.pressed) {                                                    //
        tap_code(thumb.keycode);
        tap_code(thumb.keycode);
      }
    }
  thumb.active = false;
  thumb.tapcount = 1;                                                        // tapcount is 1 at start, but increases during an interrupt event.
  }
}
  
// -------------------------------------------------------------------------------- THUMB KEYS, SHIFT --------------------------------------------------------------------------------

void shift_normalpress(void) {                                               // 'Normal' shift keypress/output. Used in timeout (matrix_scan) and interrupt (early process_record).
  uint8_t tempmods;
  uint8_t HighestLayer = get_highest_layer(layer_state);
  
  if (thumb.tapcount == 1) {
    if ((HighestLayer >= NUM_LAYER) && (HighestLayer <= RNAVI_LAYER)) {      // Thumb layer.
      if (thumb.pressed) {                                                   // ----- Thumb layer: SINGLE HOLD: Shift
        register_code(thumb.keycode);
      } else {                                                               // ----- Thumb layer: SINGLE TAP: Layer lock.
        if (thumb.locklayer == 0)                                            // If no layers locked, lock this one
          thumb.locklayer = HighestLayer;
        else {                                                               // If any layer locked, close it
          layer_off(thumb.locklayer);
          thumb.locklayer = 0;
        }
      }
    }
    else if (HighestLayer <= CAPS_MAC_LAYER) {                               // Basic layer
      if (thumb.pressed) {                                                   // ----- Basic layer: SINGLE HOLD: Shift
        register_code(thumb.keycode);
      }
      else {                                                                 // ----- Basic layer: SINGLE TAP: Braces
        register_code(thumb.keycode);                                        // here shift is pressed and released anyway (tap)
        if (thumb.key == KC_NEWLSHIFT) tap_code(KC_9);                       // LSHIFT
        else                           tap_code(KC_0);                       // RSHIFT
        unregister_code(thumb.keycode);
      }
    }
    else {                                                                   // other layers than basic layers or thumb layers: Normal shift
      if (thumb.pressed) register_code(thumb.keycode);
      else tap_code(thumb.keycode);
    }
  }
  
  else if (thumb.tapcount == 2) {
    if ((HighestLayer >= NUM_LAYER) && (HighestLayer <= RNAVI_LAYER)) {      // Thumb layer
      if (thumb.pressed) {                                                   // ----- Thumb layer: DOUBLE HOLD: Layer lock + Shift
        thumb.locklayer = HighestLayer;
        register_code(thumb.keycode);
      } else {                                                               // ----- Thumb layer: DOUBLE TAP: Layer lock with the other side's thumb layers
        switch (HighestLayer) {
          case   NUM_LAYER: thumb.locklayer = KP_LAYER;    break;
          case    KP_LAYER: thumb.locklayer = NUM_LAYER;   break;
          case LNAVI_LAYER: thumb.locklayer = RNAVI_LAYER; break;
          case RNAVI_LAYER: thumb.locklayer = LNAVI_LAYER; break;
        }
        layer_on(thumb.locklayer);
        layer_off(HighestLayer);
      }
    }
    else if (HighestLayer <= CAPS_MAC_LAYER) {                               // Basic layer
      if (thumb.pressed) {                                                   // ----- Basic layer: DOUBLE HOLD: Shift (tap + hold)
        tap_code(thumb.keycode);
        register_code(thumb.keycode);
      }
      else {                                                                 // ----- Basic layer: DOUBLE TAP: Brackets
          if (layer_state_is(MAC_LAYER)) {                                   // Mac layer
            tempmods = get_mods();
            register_code(KC_LALT);
            if (thumb.key == KC_NEWLSHIFT) tap_code(KC_9);
            else                           tap_code(KC_0);
            unregister_code(KC_LALT);                                        // Just to be sure.
            set_mods(tempmods);                                              // this does not seem to work here. Alt is stuck. Any mod press/release refreshes the proper status to Alt also.
          }
          else {                                                             // Non-Mac layer
            if (thumb.key == KC_NEWLSHIFT) tap_code(KC_LBRACKET);
            else                           tap_code(KC_RBRACKET);
          }
      }
    }
    else {                                                                   // other layers than basic layers or thumb layers: Normal shift
      if (thumb.pressed) register_code(thumb.keycode);
      else tap_code(thumb.keycode);
    }
  }
  
  else if (thumb.tapcount == 3) {
    if ((HighestLayer >= NUM_LAYER) && (HighestLayer <= RNAVI_LAYER)) {      // Thumb layer
      if (thumb.pressed) {                                                   // ----- Thumb layer: TRIPLE HOLD: Mirrored layer lock + Shift
        switch (HighestLayer) {
          case   NUM_LAYER: thumb.locklayer = KP_LAYER;    break;
          case    KP_LAYER: thumb.locklayer = NUM_LAYER;   break;
          case LNAVI_LAYER: thumb.locklayer = RNAVI_LAYER; break;
          case RNAVI_LAYER: thumb.locklayer = LNAVI_LAYER; break;
        }
        layer_on(thumb.locklayer);
        layer_off(HighestLayer);
        register_code(thumb.keycode);
      }
    }
    else if (HighestLayer <= CAPS_MAC_LAYER) {                               // Basic layer
      if (thumb.pressed) {                                                   // ----- Basic layer: TRIPLE HOLD: Shift (tap + tap + hold)
        tap_code(thumb.keycode);
        tap_code(thumb.keycode);
        register_code(thumb.keycode);
      }
      else {                                                                 // ----- Basic layer: TRIPLE TAP: Curly Braces (Shift-brackets)
        register_code(thumb.keycode);                                        // Shift
        if (layer_state_is(MAC_LAYER)) {                                     // Mac layer
          tempmods = get_mods();
          register_code(KC_LALT);
          if (thumb.key == KC_NEWLSHIFT) tap_code(KC_9);
          else                           tap_code(KC_0);
          unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else {
          if (thumb.key == KC_NEWLSHIFT) tap_code(KC_LBRACKET);              // Non-Mac layer
          else                           tap_code(KC_RBRACKET);
        }
        unregister_code(thumb.keycode);                                      // Shift. Here shift is pressed and released anyway (tap)
      }
    }
    else {                                                                   // other layers than basic layers or thumb layers: Normal shift
      if (thumb.pressed) register_code(thumb.keycode);
      else tap_code(thumb.keycode);
    }
  }
  
  else { }                                                                   // quad ignore?
  
  thumb.active = false;
  thumb.tapcount = 1;
}

// -------------------------------------------------------------------------------- THUMB KEYS, NORMAL COMBINED --------------------------------------------------------------------------------
// Queue commands for matrix scan.

void thumb_keypress(uint16_t thumb_key, uint16_t thumb_keycode, uint8_t thumb_layer, bool thumb_pressed, bool *KC_LRMOD_ACTIV) {
  if ((thumb_key == KC_THUMB_L2) && (active.BspaceDel))                      // Backspace-Delete
    thumb_keycode = KC_DELETE;
  if (thumb_pressed) {                                                       // ----- PRESS
    if (KC_LRMOD_ACTIV != NULL)                                              // clone status of physical modifier key if applicable. KC_LRMOD_ACTIV is a pointer variable holding the memory address of the original variable. Should be NULL with other than a modifier (Shift/WinAlt) key event.
      *KC_LRMOD_ACTIV = true;                                                // Now referencing to the original variable; active.LSHIFT, active.RSHIFT, active.WINLALT, or active.WINRALT.
    // uint8_t HighestLayer = get_highest_layer(layer_state);
    if ((get_highest_layer(layer_state) > CAPS_MAC_LAYER) && (thumb_key != KC_NEWLSHIFT) && (thumb_key != KC_NEWRSHIFT)) // Normal/direct press/release on upper layers, only with the 4 real thumb keys and WinAlt keys.
      register_code(thumb_keycode);                                          // Only press. Release affects on all layers anyway.
    else {                                                                   //
      thumb.key = thumb_key;
      thumb.keycode = thumb_keycode;
      thumb.layer = thumb_layer;
      thumb.timer = timer_read();                                            // (re)start the counter
      thumb.active = true;                                                   // let the matrix-part take care of the rest
      thumb.pressed = true;                                                  // clone status of thumb keys for use in matrix_scan
    }
  } else {                                                                   // ----- RELEASE
    thumb.pressed = false;                                                   // Clone the status.
    unregister_code(thumb_keycode);                                          // Release after timeout (hold)
    if (KC_LRMOD_ACTIV != NULL)                                              // Should be reset before the layer changes!
      *KC_LRMOD_ACTIV = false;
    
    if ((thumb_key == KC_NEWLSHIFT) || (thumb_key == KC_NEWRSHIFT)) {        // ----- Only for SHIFTs
      unregister_code(KC_CAPSLOCK);                                          // Shift up also releases KC_CAPSLOCK.
      if (active.BspaceDel == true) {                                        // Quick (interrupted) Shift-Backspace outputs Delete.
        active.BspaceDel = false;
        unregister_code(KC_DELETE);                                          // So that it won't get stuck
      }
    }
    else if ((thumb_key == KC_WINLALT) || (thumb_key == KC_WINRALT)) {       // ----- Only for WINALTs
      if (thumb_key == KC_WINLALT) active.WinTab = false;                    // This is not needed for the other side (WINRALT) !
      // if (thumb.active = false)                                           // if the timer is not running and no more keypresses are expected/waited, close the alt layer
      if (layer_state_is(thumb_layer)) layer_off(thumb_layer);               // ... the layer is not on after one press anyway.
    }
    else {                                                                   // ----- Only for the 4 real thumb keys
      if (thumb_layer != thumb.locklayer)                                    // if layers are activated from somewhere else, do not close them.
        if (layer_state_is(thumb_layer)) layer_off(thumb_layer);             // if the layer is on, close it (check first to minimize the number of layer reloads?)
    }
  }
}

// -------------------------------------------------------------------------------- THUMB KEYS, TIMEOUT --------------------------------------------------------------------------------

void matrix_scan_user(void) {
  if (thumb.active) {                                                        // quick check for minimal impact on scanning
    if (timer_elapsed(thumb.timer) > (THUMB_TAP_TERM)) {                     // Timeout.

      // switch (thumb.key) { }                                              // switch case would be here, but if is used instead (to combine some code)
      if ((thumb.key >= KC_THUMB_L1) && (thumb.key <= KC_THUMB_R2)) {        // ---------- THUMB keys L1, R1, L2, R2
        if (thumb.pressed) register_code(thumb.keycode);                     // on HOLD. Thumb key taps are not handled here; release is processed immediately in (interrupting) key release.
      }
      
      else if ((thumb.key == KC_NEWLSHIFT) || (thumb.key == KC_NEWRSHIFT)) { // ---------- LSHIFT, RSHIFT
        if (thumb.twin) {                                                    // twin press, keys from both sides. The first key is only trigger, the last key counts here.
          thumb.twin = false;                                                // mark as processed
          if (thumb.pressed) {                                               // ----- TWIN HOLD: CapsLock
            register_code(KC_CAPSLOCK);                                      // release in normal keypress/release
            ergodox_right_led_2_on();
            wait_ms(50);
            ergodox_right_led_2_off();
          } else {                                                           // ----- TWIN TAP: CAPS_LAYER toggle
            layer_invert(thumb.layer);                                       // CAPS_LAYER or CAPS_MAC_LAYER
          }
        }
        else {                                                               // Normal keypress. Not twin.
          shift_normalpress();                                               //
        }
      }

      else if ((thumb.key == KC_WINLALT) || (thumb.key == KC_WINRALT)) {     // ---------- WINLALT, WINRALT
        winalt_normalpress();
      }
      
      thumb.active = false;                                                  // After timeout there's no active thumb keys, even if the key is pressed.
      thumb.tapcount = 1;                                                    // Reset counter / mark multi-press handled.
    }
  }
}

// -------------------------------------------------------------------------------- THUMB KEYS, INTERRUPTED --------------------------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint8_t tempmods;                                                          // Introduce local temporary variables
  // uint8_t tempmods = get_mods();                                          // Using this here for all keys would save ~50 bytes, but causes some overhead for every keypres/release.
  bool thumb_interrupted = false;                                            // If a thumb key is interrupted.
  uint16_t fetched_keycode = XXXXXXX;                                        // Temporary variable for storing the keycode fetched from flash.
  //  if (DEBUGVAR == true) debugprint(keycode);
  
  if (thumb.active) {                                                        // A thumb key was pressed on previous round and matrix_scan has not yet timed out. This is false if it's the first thumb keypress.
      
    if ((thumb.key >= KC_THUMB_L1) && (thumb.key <= KC_THUMB_R2)) {          // ---------- Thumb keys L1, R1, L2, R2 are interrupted. No double-taps here; the 4 actual thumb keys are reset immediately on key release.
      if (record->event.pressed) {                                           // if interrupting key is pressed. Magic happens on keydowns, skip releases (for now).
        layer_on(thumb.layer);                                               // Move to the layer of the specific thumb key.
        fetched_keycode = pgm_read_word(&keymaps[(thumb.layer)][(record->event.key.row)][(record->event.key.col)]); // fetch the upper layer keycode from the 'keymap' constant stored in flash (PROGMEM means 'store in flash').
        if (!((fetched_keycode == _______) || (fetched_keycode == XXXXXXX))) // If the keycode is not KC_TRANSPARENT or KC_NO
          keycode = fetched_keycode;                                         // update the keycode according to the upper layer.
        thumb.interrupter_active = true;
        thumb.interrupter_col = record->event.key.col;                       // Memorize the key and the new keycode to be able to unregister it later (on key release).
        thumb.interrupter_row = record->event.key.row;
        thumb.interrupter_keycode = keycode;
        thumb_interrupted = true;                                            // If a thumb key is interrupted by a (basic) keycode, output the (flash-fetched) corrected keycode after other processing.
        active.BspaceDel = false;                                            // Normal layer change: Reset Backspace-Delete.
      }
      else {                                                                 // ----- RELEASE. Interrupting release. A key is released before timeout.
        // if (thumb.key == keycode) {                                       // If the same key is (pressed /) released that is associated with the timer. The timer is shared among all thumb keys, and press/release may occur out-of-order.
        register_code(thumb.keycode);                                        // Register normal thumb keycode like it was a timeout. Enables rolling mod-thumb-shortcuts.
        // }                                                                 // ... The interrupting modifier (or other key) will be unregistered normally in the keycodes case handler below. The thumb key is registered right before the mod is released.
      }
      thumb.active = false;
      thumb.tapcount = 1;
    }
    
    else if ((thumb.key == KC_NEWLSHIFT) || (thumb.key == KC_NEWRSHIFT)) { // ---------- LSHIFT, RSHIFT interrupted.
      if (record->event.pressed) {
        if (thumb.twin) {                                                    // if twin already on the previous round; reset all. Do nothing else. The right action is too ambiguous to define here.
          thumb.twin = false;                                                // The next key may output as normal.
          thumb.active = false;
          // thumb.tapcount = 1; ???
          unregister_code(KC_LSHIFT);
          unregister_code(KC_RSHIFT);
          unregister_code(KC_CAPSLOCK);
        }
        else {                                                               // normal keypress, not twin (at least not previously)
          if ((keycode == KC_NEWLSHIFT) || (keycode == KC_NEWRSHIFT)) {      // shift interrupted by shift
            if (keycode != thumb.key) {                                      // ----- NEW TWIN: Shift interrupted with a shift, but not a double-tap; it's a twin press.
              // if (((keycode == KC_NEWLSHIFT) && (thumb.key == KC_NEWRSHIFT)) || ((keycode == KC_NEWRSHIFT) && (thumb.key == KC_NEWLSHIFT))) { // Twin. This may also be used.
              if (thumb.pressed) {
                thumb.twin = true;                                           // Mark for matrix_scan to handle it at timeout.
                // thumb.active = true;                                      // Do not reset thumb.active here!
                thumb.tapcount = 1;                                          // Reset tap counter after twin taps
              }
              else {                                                         //
                shift_normalpress();                                         // First half of Twin Shift was released before timeout: normal shift press
                // thumb.active = false;                                     // Already in shift_normalpress()
                // thumb.tapcount = 1;                                       // Already in shift_normalpress()
              }
            }
            else {                                                           // ----- DOUBLE-TAP with the same key (shift). Never together with twin.
              thumb.tapcount++;                                              // Increment counter. Do nothing else, let matrix_scan handle it.
              if (thumb.tapcount >4) thumb.tapcount = 4;                     // Keep counter realistic and prevent overflow.
              // thumb.active = true;                                        // Do not reset thumb.active here!
            }
          }                                                                  // shift interrupted by:
          else if ((keycode == KC_THUMB_L2) && (thumb.pressed)) {            // ----- BACKSPACE-DELETE. Shift interrupted by backspace (keydown) while Shift still held down. Shift timer does not reset on keyup yet.
            active.BspaceDel = true;                                         // SHIFT is interrupted by BACKSPACE: Propose DELETE instead. Ignored if the thumb key is interrupted (layer change).
            //                                                               // if SHIFT is released before timeout and before the interrupt event (thumb.pressed == false): normal shift press (below)
            // thumb.active = true;                                          // Do not reset thumb.active here!
            thumb.tapcount = 1;                                              // Reset tap counter (failsafe). Also, earlier taps are ignored.
          }
          else {
            shift_normalpress();                                             // If a normal key or other thumb key interrupts (press) a shift, do the same as on timer timeout.
            // thumb.active = false;                                         // Already in shift_normalpress()
            // thumb.tapcount = 1;                                           // Already in shift_normalpress()
          }
        }
      }
      else {                                                                 // ----- RELEASE is interrupting a shift
        //  thumb.active = true;                                             // Already true here.
        if ((keycode >= KC_THUMB_L1) && (keycode <= KC_THUMB_R2)){           // Shift's timer is running: if any thumb is lifted: Do the same as on timeout. Enables quick rolling layer lock.
          thumb.pressed = false;                                             // so that shift_normalpress() does not interprete a hold
          shift_normalpress();                                               // do the same resets as on timeout
          // thumb.active = false;                                           // Already in shift_normalpress()
          // thumb.tapcount = 1;                                             // Already in shift_normalpress()
        }
        // if same key releases whose timer is running - interrupt by itself
      }
    }
    
    else if ((thumb.key == KC_WINLALT) || (thumb.key == KC_WINRALT)) {       // ---------- LALT, RALT interrupted.
      if (record->event.pressed) {
        if (keycode == thumb.key) {                                          // ----- DOUBLE-TAP with the same key.
          thumb.tapcount++;                                                  // Increment counter. Let matrix_scan handle everything else.
          if (thumb.tapcount >2) {                                           //
            thumb.tapcount = 2;                                              // Keep counter realistic and prevent overflow
            tap_code(thumb.keycode);                                         // Tap out extra taps from the beginning, saving the latest two keypresses to decide the action taken
            // thumb.active = true;                                          // Leave thumb.active = true for the timeout to handle the rest.
          }
          thumb.timer = timer_read();                                        // Re-start the timer. Do it here and not in the cases part below, because the case part is skipped (below). Other thumb variables remain the same.
          active.LALT = false;                                               // Alts are pressed, but not registered! Layer change would register these otherwise, therefore false.
          active.RALT = false;                                               //
          layer_on(WIN_ALT_LAYER);                                           // Alt layer activates immediately on second keydown. No need for keycode fetching. No point in waiting; layer change does not output anything.
          // thumb.active = true;                                            // The timer is still left running; Alt may still release, close the layer, and act as double tap within the tapping term.
          // thumb.tapcount = ?;                                             // Do not touch thumb.count here!!
          return false;                                                      // Skip the cases part and prevent further processing of Alt. Otherwise it would register Alt keypress, because we're on high number WinAlt layer.
        }
        else {                                                               // ----- Any other key (press) interrupting: Do the same as on timeout.
          winalt_normalpress();                                              //
          // thumb.active = false;                                           // Already in winalt_normalpress()
          // thumb.tapcount = 1;                                             // Already in winalt_normalpress()
        }
      }
      // else {}                                                             // ----- RELEASE interrupting Alt: do not care.
    }
  }
  
// -------------------------------------------------------------------------------- THUMB KEYS, INTERRUPTED (LATE RELEASE) --------------------------------------------------------------------------------
  
  if (thumb.interrupter_active == true) {                                    // If the key that triggered the layer (and interrupted a thumb key) is (still) pressed.
    if (!record->event.pressed) {                                            // Check release events
      if ((record->event.key.col == thumb.interrupter_col) && (record->event.key.row == thumb.interrupter_row)) { // Compare the location of the released key and the layer-triggering key.
        thumb.interrupter_active = false;
        keycode = thumb.interrupter_keycode;                                 // Use the new modified keycode on release also, to have the correct keycode processed on release (below).
        // unregister_code(keycode);                                         // Not this here, let unregistering happen in following key checks.
        thumb_interrupted = true;                                            // This is release event of the interrupting / triggering key.
      }                                                                      //
    }
  }

// -------------------------------------------------------------------------------- THUMB KEYS, NORMAL --------------------------------------------------------------------------------
  
  switch (keycode) {
      
      
      
    case KC_THUMB_L1:
      thumb_keypress(KC_THUMB_L1, KC_SPACE, LNAVI_LAYER, record->event.pressed, NULL);
      return false;
      break;

    case KC_THUMB_R1:
      thumb_keypress(KC_THUMB_R1, KC_SPACE, RNAVI_LAYER, record->event.pressed, NULL);
      return false;
      break;
      
    case KC_THUMB_L2:
      thumb_keypress(KC_THUMB_L2, KC_BSPACE, NUM_LAYER, record->event.pressed, NULL);
      return false;
      break;
      
    case KC_THUMB_R2:
      thumb_keypress(KC_THUMB_R2, KC_ENTER, KP_LAYER, record->event.pressed, NULL);
      return false;
      break;
      
      
      
    case KC_NEWLSHIFT:
      thumb_keypress(KC_NEWLSHIFT, KC_LSHIFT, CAPS_LAYER, record->event.pressed, &active.LSHIFT);
      return false;
      break;
    
    case KC_NEWRSHIFT:
      thumb_keypress(KC_NEWRSHIFT, KC_RSHIFT, CAPS_LAYER, record->event.pressed, &active.RSHIFT);
      return false;
      break;
                                                                             // Mirror keys. Not used the normal way in anywhere, hogging a rare basic keycode is also fine for TD compatibility.
                                                                             // If there is multiple TD's and switching to layer with custom keycodes,
                                                                             // things happen out-of-order if at all, and basic/custom keys work differently.
    case KC_LMIRROR:                                                         // LEFT MIRROR.
      if (record->event.pressed) {                                           // key down
        if (get_highest_layer(layer_state) <= MIRROR_MAC_LAYER) {            // If other (upper) layers are active, do not activate mirror layer in the background
          if (layer_state_is(MAC_LAYER)) layer_on(MIRROR_MAC_LAYER);         // if Mac layer is on
          else                           layer_on(MIRROR_LAYER);
        }
      } else {                                                               // key release
        layer_off(MIRROR_MAC_LAYER);
        layer_off(MIRROR_LAYER);
      }
      return false; // debug
      break;

    case KC_RMIRROR:                                                         // RIGHT MIRROR.
        if (record->event.pressed) {                                         // key down
          if (get_highest_layer(layer_state) <= MIRROR_MAC_LAYER) {          // If other (upper) layers are active, do not activate mirror layer in the background
            if (layer_state_is(MAC_LAYER)) layer_on(MIRROR_MAC_LAYER);       // if Mac layer is on
            else                           layer_on(MIRROR_LAYER);
          }
        } else {                                                             // key release
          layer_off(MIRROR_MAC_LAYER);
          layer_off(MIRROR_LAYER);
        }
        return false; // debug
        break;

    case KC_MIRROR_NAVI:
      if (record->event.pressed) {
        layer_on(MIRROR_NAVI_LAYER);
      }
      else {
        layer_off(MIRROR_LAYER);
        layer_off(MIRROR_MAC_LAYER);
        layer_off(MIRROR_NAVI_LAYER);
      }
      return false; // debug
      break;

      
      
    case KC_WINLALT:
      thumb_keypress(KC_WINLALT, KC_LALT, WIN_ALT_LAYER, record->event.pressed, &active.LALT);
      return false;
      break;
      
    case KC_WINRALT:
      thumb_keypress(KC_WINRALT, KC_RALT, WIN_ALT_LAYER, record->event.pressed, &active.RALT);
      return false;
      break;
      
  
            
    case KC_WINLCTRL:                                                        // on Windows layer
      if (record->event.pressed) {
        if (active.WinTab) {                                                 // If TAB was used with LALT, output LSHIFT instead of LCTRL. LCTRL may still be used by pressing it down before TAB.
          register_code(KC_LSHIFT);
          active.WinTabShift = true;
        } else {                                                             // Normal LCTRL
          register_code(KC_LCTRL);
        }
      } else {                                                               // release
        if (active.WinTabShift) {
          unregister_code(KC_LSHIFT);                                        // LSHIFT up, if it was down
          active.WinTabShift = false;
        }
        unregister_code(KC_LCTRL);                                           // LCTRL up with the physical key anyway
      }
      capslayermods_keypress(record);                                        // CapsLock layer compatibility like in 'normal' KC_LCTRL
      return false;
      break;
      
// -------------------------------------------------------------------------------- MODIFIED BASIC KEYCODES --------------------------------------------------------------------------------

    case KC_LALT:
      if (record->event.pressed) active.LALT = true;
      else                       active.LALT = false;
      return true;
      break;
    case KC_RALT:
      if (record->event.pressed) active.RALT = true;
      else                       active.RALT = false;
      return true;
      break;
      
      
      
    case KC_LGUI:                                                            // Modifier compatibility for CapsLock layer (temporary disabling it)
      capslayermods_keypress(record);
      return true;
      break;
    case KC_RGUI:
      capslayermods_keypress(record);
      return true;
      break;
    case KC_LCTRL:
      capslayermods_keypress(record);
      return true;
      break;
    case KC_RCTRL:
      capslayermods_keypress(record);
      return true;
      break;

      

    case KC_NUMLOCK:
      if (record->event.pressed) {
        ergodox_right_led_1_on();
        wait_ms(50);                                                         // minimum time the led is on
      }
      else
        ergodox_right_led_1_off();
      if (thumb_interrupted) return false;                                   // If a thumb key is interrupted, deny further processing of the interrupting (wrong/real/original) keycode. Otherwise process normally.
      else                   return true;                                    // All keycodes that are output from upper layer with thumb-interrupt need their original (lower layer) keycode suppressed.
      break;                                                                 //


      
    case KC_TAB:                                                             //
      if (record->event.pressed) {
        if (layer_state_is(WIN_LAYER)) {
          if ((get_mods() & MOD_BIT(KC_LALT)) == MOD_BIT(KC_LALT))  {        // Only LALT
            active.WinTab = true;
          }
        }
      }
      return true;
      break;

// -------------------------------------------------------------------------------- CUSTOM KEYCODES, NEW BASICS --------------------------------------------------------------------------------
      
    // All of these following keycodes need also their release events handled in case that they interrupted a thumb key, so that they are not stuck when the layer changes back.
    // A solution is to use only taps, and/or clearing all pressed keys when the layer changes.
            
    case KC_NEWDOT:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_N);
        else
          unregister_code(KC_N);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0) // or you may use: if (!(active.LSHIFT || active.RSHIFT))
            register_code(KC_DOT);                                           // "." no shift
          else
            register_code(KC_SCOLON);                                        // ":" shift
        } else {
          unregister_code(KC_DOT);                                           // on key release, release all key combinations for this key
          unregister_code(KC_SCOLON);                                        // this may limit functionality when holding several keys simultaneously
        }
      }
      return false;
      break;                                                                 // Or, all keys may be reset at layer change.
      
    case KC_NEWCOMMA:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_B);
        else
          unregister_code(KC_B);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0)
            register_code(KC_COMMA);                                         // "," no shift
          else {
            unregister_mods((MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)));      //
            // unregister_code(KC_LSHIFT);
            // unregister_code(KC_RSHIFT);
            register_code(KC_SCOLON);                                        // ";" shift pressed
          }
        } else {                                                             // on key release
          unregister_code(KC_COMMA);
          unregister_code(KC_SCOLON);
          if (active.LSHIFT) register_code(KC_LSHIFT);                       // workaround for when shift is released before the key
          // else            unregister_code(KC_LSHIFT);
          if (active.RSHIFT) register_code(KC_RSHIFT);
          // else            unregister_code(KC_RSHIFT);
        }
      }
      return false;
      break;

      
        
    case KC_NEWLABK:                                                         // "<"
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LALT);
          tap_code(KC_B);
          // unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else {}
      }
      else {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LSHIFT);
          tap_code(KC_COMMA);
          // unregister_code(KC_LSHIFT);
          set_mods(tempmods);
        }
        else {}
      }
      return false;
      break;
      
    case KC_NEWRABK:                                                         // ">"
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LALT);
          tap_code(KC_N);
          // unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else {}
      }
      else {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LSHIFT);
          tap_code(KC_DOT);
          // unregister_code(KC_LSHIFT);
          set_mods(tempmods);
        }
        else {}
      }
      return false;
      break;
        
      
        
    case KC_NEWQUOTE:                                                        // "'"
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_H);
        else
          unregister_code(KC_H);
      }
      else {
        if (record->event.pressed)
          register_code(KC_QUOTE);
        else
          unregister_code(KC_QUOTE);
      }
      return false;
      break;
      
      
      
    case KC_NEWEQUAL:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_Y);
        else
          unregister_code(KC_Y);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0) {
            register_code(KC_EQUAL);                                         // "=" no shift
          } else {}                                                          // "" shift pressed
        } else
          unregister_code(KC_EQUAL);
      }
      return false;
      break;
      
      
      
    case KC_NEWPLUS:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_EQUAL);
        else
          unregister_code(KC_EQUAL);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0) {
            register_code(KC_LSHIFT);
            register_code(KC_EQUAL);                                         // "+" no shift
          } else {}                                                          // "" shift pressed
        } else {
          unregister_code(KC_EQUAL);
          if (active.LSHIFT) register_code(KC_LSHIFT);
          else               unregister_code(KC_LSHIFT);
          // if (active.RSHIFT) register_code(KC_RSHIFT);
          // else               unregister_code(KC_RSHIFT);
        }
      }
      return false;
      break;
            
            
        
    case KC_NEWASTERISK:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_BSLASH);
        else
          unregister_code(KC_BSLASH);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0) {
            register_code(KC_LSHIFT);
            register_code(KC_8);                                             // "*" no shift
          } else
            register_code(KC_SLASH);                                         // "?" shift pressed
        } else {
          unregister_code(KC_8);
          unregister_code(KC_SLASH);
          if (active.LSHIFT) register_code(KC_LSHIFT);
          else               unregister_code(KC_LSHIFT);
          // if (active.RSHIFT) register_code(KC_RSHIFT);
          // else               unregister_code(KC_RSHIFT);
        }
      }
      return false;
      break;
          
          
      
    case KC_NEWSLASH:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed)
          register_code(KC_RBRACKET);
        else
          unregister_code(KC_RBRACKET);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0)
            register_code(KC_SLASH);                                         // "/" no shift
          else
            register_code(KC_BSLASH);                                        // "|" pipe with shift
        } else {                                                             // on key release
          unregister_code(KC_SLASH);
          unregister_code(KC_BSLASH);
        }
      }
      return false;
      break;
          
    case KC_NEWBSLASH:
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LALT);
          register_code(KC_RBRACKET);
          // unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else
          unregister_code(KC_RBRACKET);
      }
      else {
        if (record->event.pressed) {
          if ((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) == 0)
            register_code(KC_BSLASH);                                        // "\" no shift
          // else {}                                                         // ""  with shift
        }
        else
          unregister_code(KC_BSLASH);
      }
      return false;
      break;
        
        
        
    case KC_NEWLBRACKET:                                                     // "["
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LALT);
          tap_code(KC_9);
          // unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else {}
      }
      else {
        if (record->event.pressed)
          register_code(KC_LBRACKET);
        else
          unregister_code(KC_LBRACKET);
      }
      return false;
      break;
      
    case KC_NEWRBRACKET:                                                     // "]"
      if (layer_state_is(MAC_LAYER)) {
        if (record->event.pressed) {
          tempmods = get_mods();
          register_code(KC_LALT);
          tap_code(KC_0);
          // unregister_code(KC_LALT);
          set_mods(tempmods);
        }
        else {}
      }
      else {
        if (record->event.pressed)
          register_code(KC_RBRACKET);
        else
          unregister_code(KC_RBRACKET);
      }
      return false;
      break;

// -------------------------------------------------------------------------------- CUSTOM KEYCODES, EXOTIC --------------------------------------------------------------------------------
            
    case KC_ADIA:                                     // 196, (228)
      scandic_keypress(record, KC_A, KC_C, &active.Copy, 0xC4);              // Key record, Base character A/O, Copy/Paste C/V with modifiers, Copy/Paste active, Uppercase letter codetable code point.
      return false;
      break;

    case KC_ODIA:                                      // 214, (246)
      scandic_keypress(record, KC_O, KC_V, &active.Paste, 0xD6);
      return false;
      break;

    case KC_ARING: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xC5); // 197
        }
        else {
          win_alt_character(0xE5); // 229
        }
      }
      return false;
      break;

    case KC_AE: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xC6); // 198
        }
        else {
          win_alt_character(0xE6); // 230
        }
      }
      return false;
      break;

    case KC_OSTROKE: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xD8); // 216
        }
        else {
          win_alt_character(0xF8); // 248
        }
      }
      return false;
      break;

    case KC_ETH: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xD0); // 208
        }
        else {
          win_alt_character(0xF0); // 240
        }
      }
      return false;
      break;

    case KC_THORN: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xDE); // 222
        }
        else {
          win_alt_character(0xFE); // 254
        }
      }
      return false;
      break;

    case KC_UDIA: // on Windows layer
      if (record->event.pressed) {
        if (((get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) != 0) || (layer_state_is(CAPS_LAYER))) {
          win_alt_character(0xDC); // 220
        }
        else {
          win_alt_character(0xFC); // 252
        }
      }
      return false;
      break;
      
    case KC_ESZETT: // on Windows layer
      if (record->event.pressed) {
        win_alt_character(0xDF); // 223
      }
      return false;
      break;
      
    case KC_MU: // on Windows layer
      if (record->event.pressed) {
        win_alt_character(0xB5); // 181
      }
      return false;
      break;

    case KC_DEGREE: // on Windows layer
      if (record->event.pressed) {
        win_alt_character(0xB0); // 176
      }
      return false;
      break;



    case KC_MACVOL_UP:
      if (record->event.pressed) {
        register_code(KC_LALT);
        wait_ms(50);                                                         // This is necessary, it seems.
        register_code(KC_LSHIFT);
        wait_ms(50);
        register_code(KC_AUDIO_VOL_UP);
      } else {
        unregister_code(KC_AUDIO_VOL_UP);
        unregister_code(KC_LSHIFT);
        unregister_code(KC_LALT);
      }
      return false;
      break;

    case KC_MACVOL_DN:
      if (record->event.pressed) {
        register_code(KC_LALT);
        wait_ms(50);
        register_code(KC_LSHIFT);
        wait_ms(50);
        register_code(KC_AUDIO_VOL_DOWN);
      } else {
        unregister_code(KC_AUDIO_VOL_DOWN);
        unregister_code(KC_LSHIFT);
        unregister_code(KC_LALT);
      }
      return false;
      break;

// -------------------------------------------------------------------------------- CUSTOM KEYCODES, SPECIALS --------------------------------------------------------------------------------
      
    case KC_SETTING_NO_0:                                                    // DF(0);  Equivalent macro keycode (in keymap) for switching the base layer.
      if (record->event.pressed) {
        if (active.LSHIFT && active.RSHIFT) {                                // if both shifts are down.
        // if (((get_mods() & (MOD_BIT(KC_LSHIFT))) != 0) && ((get_mods() & (MOD_BIT(KC_RSHIFT))) != 0)) { // alternative example
          capslock_blink();
          // set_single_persistent_default_layer(0);                         // Does not seem to work.
          // user_config.my_base_layer = 0;                                  // Does not seem to work.
          // eeconfig_update_user(user_config.raw);                          // Write custom settings to EEPROM.
        }
        default_layer_set(0);
        layer_move(0); }
      return false;                                                          // true would output the character on lower layer also
      break;                                                                 // just to be safe


    case KC_SETTING_NO_1:                                                    // DF(0);  Equivalent macro keycode (in keymap) for switching the base layer.
      if (record->event.pressed) {
        if (active.LSHIFT && active.RSHIFT) {                                // if both shifts are down.
          capslock_blink();
          // set_single_persistent_default_layer(1);                         // Does not seem to work.
        }
        default_layer_set(1);
        layer_move(1); }
      return false;
      break;

    case KC_SETTING_NO_2:                                                    // DF(0);  Equivalent macro keycode (in keymap) for switching the base layer.
      if (record->event.pressed) {
        if (active.LSHIFT && active.RSHIFT) {                                // if both shifts are down.
          capslock_blink();
          // set_single_persistent_default_layer(2);                         // Does not seem to work.
        }
        default_layer_set(2);
        layer_move(2); }
      return false;
      break;
        
    case KC_SETTING_NO_3:
      if (record->event.pressed) {
        layer_move(GAME_LAYER); }
      return false;
      break;

      

//    case KC_TEST1:
//      if (record->event.pressed) {
//        if (DEBUGVAR) DEBUGVAR = false;
//        else          DEBUGVAR = true;
//      }
//      else {
//
//      }
//      return true;

//    case KC_TEST2:
//      if (record->event.pressed) {
//        tempmods = get_mods();
//        clear_mods();
//        register_code(KC_LALT); register_code(KC_KP_0); unregister_code(KC_KP_0); register_code(KC_KP_1); unregister_code(KC_KP_1); register_code(KC_KP_9); unregister_code(KC_KP_9); register_code(KC_KP_7); unregister_code(KC_KP_7);
//        set_mods(tempmods);
//      }
//      else {
//        register_code(KC_LALT); tap_code(KC_KP_0); tap_code(KC_KP_1); tap_code(KC_KP_9); tap_code(KC_KP_7); unregister_code(KC_LALT);
//      }
//      return true;
//
//    case KC_TEST3:
//      if (record->event.pressed) {
//        if (DEBUGVAR == 1) DEBUGVAR = 0;
//        else              DEBUGVAR = 1;
//      } else clear_keyboard();
//      return false;
      
      
      
    case RGB_SLD:                                                            // Ergodox EZ default
      if (record->event.pressed) rgblight_mode(1);
      return false;
      
      

    default:                                                                 // all other (basic) keycodes from base layer - or interrupted upper layer
      if (thumb_interrupted) {
        if ((keycode >= KC_A) && (keycode <= KC_EXSEL)) {                    // If basic keycode, KC_A (0x04) >= keycode <= KC_EXSEL (0xA4).
          if (record->event.pressed) register_code(keycode);
          else                       unregister_code(keycode);
          // A tap would be better here, because we cannot alter the original keycode here (to release it later); it is just passed to process_record_user and the original is still in the other QMK internals.
          // However, we use keycode-agnostic clear_keyboard to unregister all keycodes at a layer change in thumb key release. Unless the keycode is unregistered by the key release on the upper layer first.
          return false;                                                      // Processed. Do not let the original wrong keycode to continue.
        }
        // else if ((keycode >= KC_LCTRL) && (keycode <= KC_RGUI))           // Let modifiers through even if they interrupt a thumb key.
        //  return true;                                                     //
        // else                                                              // All other keycodes as interrupters. Process normally.
        //  return true;                                                     //
      }
      else
        return true;                                                         // No interrupt, process normally.
      break;
    // end of cases
  } // end of switch
  return true;                                                             //
}
