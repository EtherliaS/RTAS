# Rapid Trigger Arduino Script
Script for using rapid trigger on 2 button keypad. Can be expanded to any number of buttons, depends on your arduino.\
There are no strong mathematical ideas behind the code, but it works for me. Feel free to modify it to suit your needs.
# Guide
You can simply build one by following [this guide](https://www.youtube.com/watch?v=4rrDy9KakRI).\
The code presented in this guide did not work for me, so I made my own version.
## Parts I used:
* Adrduino Pro Micro
* 2x gateron brown switches (you can buy Lekker L60 & L45 and similar)
* 4x 1mm magnets (2 in each switch)
* 2x hall sensors SS49E (you can use more accurate sensors, but I have not tried)
* 3D printed case and keycaps
# Code
Versions:\
**v4** - deprecated old *(This version is easier to understand how the script works. yep, this is dogwater code)*\
**v5** - simplified new.\
**v6** - *v5* with exponential moving average\
Depending on the keypad, version 6 can either help or make things worse.
Also included is a "test" sketch for testing values ​​for manual tuning.
# Constants that should be specified
|     Variable     |                                 Meaning                                          |
|------------------|----------------------------------------------------------------------------------|
| `pins`           | Pins on your arduino                                                             |
| `keyCodes`       | Keycodes on kb (direct relationship with pins array pins[i] => keyCodes[i])      |
| `pinMax`         | Maximum pin values, needed only at startup (does not affect the work in any way) |
| `activatePoints` | Points below which the press is counted                                          |
| `releasePoints`  | Points of unconditional release                                                  |
| `pressDelay`     | Minimal delay between taps                                                       |
| `alpha`          | Smoothing factor for EMA (only in **v6**) must be 0<alpha<1                      |
