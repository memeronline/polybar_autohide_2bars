# PolyBar_Autohide

PolyBar_Autohide is a programs that implements autohide features for polybar and other status bars.
PolyBar_Autohide is wm agnostic and bar agnostic, it should work with any wm and bar only on X11.

## How to Install

1. Clone the git repo in your home:

     ```[bash]
     git clone https://github.com/memeronline/polybar_autohide_2bars ~/polybar_autohide_2bars
     ```

     - Install dependencies: `libxdo-dev` from [xdotool](https://github.com/jordansissel/xdotool)

2. Make adjustments to the script directly from the source code `autohide.h`.

     - To find the WM_CLASS of your bar you can use the `getwmclass.sh` script.

          ```[bash]
          ./getwmclass.sh
          ```

3. Compile the script  with `make` or install it to `/usr/bin/autohide` with `make install`

4. Run the script with whatever kind of startup script you are using.

5. Uninstall with `make uninstall`

## Additional info about this fork

This is my first time ever putting out something like this (a.k.a; something which I expect will work for not only me, but also other people having this niche issue), so feel more than free to submit issues or any unintended behaviour

P.S Im terrible at documenting things, if something doesn't work the way it should or you expect it to, also feel free to open an issue about it

________________________________________________________________________________
[![Donate (to the original creator!) (obviously)](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=LZDKH4PL5Z3XN&source=url)
