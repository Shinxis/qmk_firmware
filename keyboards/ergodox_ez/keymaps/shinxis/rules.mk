# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file
LTO_ENABLE = yes # in previous qmk version this tag was LINK_TIME_OPTIMIZATION_ENABLE
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
DYNAMIC_MACRO_ENABLE = no # takes up 562 bytes, just the core. Dynamic Macros may crash the whole firmware with timer-interrupting layer changes.
SRC = matrix.c
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
TAP_DANCE_ENABLE = no

# ---------- Space optimization ----------
# # EXTRAFLAGS += -flto  # same as link time optimization above ???
# COMMAND_ENABLE    = no # Commands for debug and configuration
UNICODE_ENABLE    = no   # Unicode
SWAP_HANDS_ENABLE = no   #
