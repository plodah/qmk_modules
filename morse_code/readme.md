## Morse Code
This is 95% taken from [@daliusd](https://github.com/daliusd/qmk_firmware/blob/morsilka/keyboards/a_dux/keymaps/morsilka/keymap.c).

I put in the small amount of work needed to turn this into a module:-
- The original over-rode every key, this just creates a keycode so it can be assigned wherever.
- I changed the use of matrix_scan_ to a housekeeping_task_
- In addition to the single manual key, I've added seperate DIT and DASH keys.


Disclaimer: *This readme was **not** written using this morse code module.*

## Keycodes
| Full Keycode | Short Code | Notes |
| --- | --- | --- | 
| `COMMUNITY_MODULE_MORSE_CODE_MANUAL` | `CM_MORS` | "Classic" mode. A short press is a Dit `.` and a longer press is a Dash `-` |
| `COMMUNITY_MODULE_MORSE_CODE_DIT` | `CM_MORT` | Always sends a dit. |
| `COMMUNITY_MODULE_MORSE_CODE_DASH` | `CM_MORH` | Always sends a dash. |

## Config
| Variable | Default | Notes |
| --- | --- | --- |
| `DIT_DURATION` | 100ms | The duration of a Dit |
| `SHORT_GAP` | 3 * `DIT_DURATION` | Duration of pause between encoded characters |