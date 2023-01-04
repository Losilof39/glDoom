// Doom COMMANDS - These are what keystrokes get turned into for
// interpretation by the game engine.  All text is passed through as
// the text characters with the "commands" being passed through as
// "control" characters.
// i.e. KEY_SPACE -> CMD_USE in the game engine but is still KEY_SPACE
// for text input

#ifndef DOOM_CMDS

#define DOOM_CMDS

#define CMD_NULL         0x00   // does nothing - deassigns key from action
#define CMD_SPEED        0x01   // turns on "run" mode
#define CMD_STRAFE       0x02   // turns on "sidestep" mode - xlate turn into sidestep
#define CMD_STRAFELEFT   0x03   // dodge to the left
#define CMD_STRAFERIGHT  0x04   // dodge to the right
#define CMD_FORWARD      0x05   // move forward
#define CMD_BACKWARD     0x06   // move backward
#define CMD_AUTOMAP      0x07   // toggle automap on and off
#define CMD_BACKSPACE    0x08   // go back up the menu chain or move text cursor backward
#define CMD_TURNLEFT     0x09   // turn to the left
#define CMD_TURNRIGHT    0x0A   // turn to the right
#define CMD_TURN180      0x0B   // turn around completely
#define CMD_USE          0x0C   // use the object (open doors, flip switches)
#define CMD_FIRE         0x0D   // fire current weapon
#define CMD_MENU         0x0E   // toggle the menu
#define CMD_CONSOLE      0x0F   // toggle the console
#define CMD_PDA          0x10   // toggle the pda
#define CMD_INVENTORY    0x11   // toggle inventory
#define CMD_INVLEFT      0x12   // scroll inventory to right
#define CMD_INVRIGHT     0x13   // scroll inventory to left
#define CMD_INVSELECT    0x14   // select item in inventory
#define CMD_ALTFIRE      0x15   // fire weapon in secondary mode
#define CMD_SAVEGAME     0x16   // save game menu option
#define CMD_LOADGAME     0x17   // load game menu option
#define CMD_QUICKSAVE    0x18   // quicksave the game
#define CMD_QUICKLOAD    0x19   // quickload the game
#define CMD_ENDGAME      0x1A   // end the current game
#define CMD_QUITGAME     0x1B   // leave the game program
#define CMD_HUDUP        0x1C   // next hud mode
#define CMD_HUDDOWN      0x1D   // previous hud mode
#define CMD_SAY          0x1E   // broadcast text message
#define CMD_SAYTEAM      0x1F   // team text message

#define CMD_SPACE        0x20   // the space bar

#define CMD_BANG         '!'
#define CMD_DQUOTE       '"'
#define CMD_POUND        '#'
#define CMD_DOLLAR       '$'
#define CMD_PERCENT      '%'
#define CMD_AMPERSAND    '&'
#define CMD_SQUOTE       '\''
#define CMD_LEFTPAREN    '('
#define CMD_RIGHTPAREN   ')'
#define CMD_ASTERISK     '*'
#define CMD_PLUS         '+'
#define CMD_COMMA        ','
#define CMD_MINUS        '-'
#define CMD_PERIOD       '.'
#define CMD_SLASH        '/'

#define CMD_0            '0'
#define CMD_1            '1'
#define CMD_2            '2'
#define CMD_3            '3'
#define CMD_4            '4'
#define CMD_5            '5'
#define CMD_6            '6'
#define CMD_7            '7'
#define CMD_8            '8'
#define CMD_9            '9'
#define CMD_COLON        ':'
#define CMD_SEMICOLON    ';'
#define CMD_LESSTHAN     '<'
#define CMD_EQUALS       '='
#define CMD_GREATERTHAN  '>'
#define CMD_QUESTION     '?'
#define CMD_ATSIGN       '@'


#define CMD_A            'A'
#define CMD_B            'B'
#define CMD_C            'C'
#define CMD_D            'D'
#define CMD_E            'E'
#define CMD_F            'F'
#define CMD_G            'G'
#define CMD_H            'H'
#define CMD_I            'I'
#define CMD_J            'J'
#define CMD_K            'K'
#define CMD_L            'L'
#define CMD_M            'M'
#define CMD_N            'N'
#define CMD_O            'O'
#define CMD_P            'P'
#define CMD_Q            'Q'
#define CMD_R            'R'
#define CMD_S            'S'
#define CMD_T            'T'
#define CMD_U            'U'
#define CMD_V            'V'
#define CMD_W            'W'
#define CMD_X            'X'
#define CMD_Y            'Y'
#define CMD_Z            'Z'

#define CMD_LEFTBRACKET  '['
#define CMD_BACKSLASH    '\\'
#define CMD_RIGHTBRACKET ']'
#define CMD_CARET        '^'
#define CMD_UNDERSCORE   '_'
#define CMD_ACCENTGRAVE  '`'

#define CMD_a            'a'
#define CMD_b            'b'
#define CMD_c            'c'
#define CMD_d            'd'
#define CMD_e            'e'
#define CMD_f            'f'
#define CMD_g            'g'
#define CMD_h            'h'
#define CMD_i            'i'
#define CMD_j            'j'
#define CMD_k            'k'
#define CMD_l            'l'
#define CMD_m            'm'
#define CMD_n            'n'
#define CMD_o            'o'
#define CMD_p            'p'
#define CMD_q            'q'
#define CMD_r            'r'
#define CMD_s            's'
#define CMD_t            't'
#define CMD_u            'u'
#define CMD_v            'v'
#define CMD_w            'w'
#define CMD_x            'x'
#define CMD_y            'y'
#define CMD_z            'z'

#define CMD_LEFTCBRACE   '{'
#define CMD_PIPE         '|'
#define CMD_RIGHTCBRACE  '}'
#define CMD_TILDE        '~'

#define CMD_SCREENSHOT   0x7F   // take a screenshot
#define CMD_JUMP         0x180
#define CMD_CROUCH       0x181
#define CMD_SAYTO        0x182
#define CMD_WAIT         0x183
#define CMD_NEXTWEAP     0x184
#define CMD_WEAPON1      0x185
#define CMD_WEAPON2      0x186
#define CMD_WEAPON3      0x187
#define CMD_WEAPON4      0x188 
#define CMD_WEAPON5      0x189
#define CMD_WEAPON6      0x18A
#define CMD_WEAPON7      0x18B
#define CMD_PING         0x18C

// These are input command sequences
// which are independent of what command
// they are bound to. 

#define MOUSE_B1         0x101
#define MOUSE_B2         0x102
#define MOUSE_B3         0x104

#define JOY_B01          0x201
#define JOY_B02          0x202
#define JOY_B03          0x203
#define JOY_B04          0x204
#define JOY_B05          0x205
#define JOY_B06          0x206
#define JOY_B07          0x207
#define JOY_B08          0x208
#define JOY_B09          0x209
#define JOY_B10          0x20A
#define JOY_B11          0x20B
#define JOY_B12          0x20C
#define JOY_B13          0x20D
#define JOY_B14          0x20E
#define JOY_B15          0x20F
#define JOY_B16          0x210
#define JOY_B17          0x211
#define JOY_B18          0x212
#define JOY_B19          0x213
#define JOY_B20          0x214
#define JOY_B21          0x215
#define JOY_B22          0x216
#define JOY_B23          0x217
#define JOY_B24          0x218
#define JOY_B25          0x219
#define JOY_B26          0x21A
#define JOY_B27          0x21B
#define JOY_B28          0x21C
#define JOY_B29          0x21D
#define JOY_B30          0x21E
#define JOY_B31          0x21F
#define JOY_B32          0x220

#endif