# Ploopy Adept

The [Adept](https://ploopy.co/adept-trackball/) is an powerful open-source trackball by [Ploopy](https://ploopy.co/) running [QMK](https://qmk.fm/).

This repository contains custom 3D models and custom QMK firmware configuration files for the Ploopy Adept trackball.

---

![View from the top of the modified Ploopy Adept](https://github.com/melemanuhulu/ploopy-adept/blob/main/hardware/images/top-buttons-cropped.jpg)

The hardware:
 - Is shorter than the original (front-to-back, not top-to-bottom)
 - Has an extra layer that clamps around an L-bracket that can be attached to a ball-head tripod mount for easy adjustments and secure tenting
 - Uses ball transfer units rather than single-axis bearings
 - Has one bearing postioned higher in the shell of the mouse to allow for steeper tenting
 - Uses nuts and bolts to connect layers rather than screws threaded into plastic
 - Has raised buttons that connect to the main body using magnets

---

The firmware includes:
- A custom scroll layer that
  - is accessed by a tap-toggle, allowing for one button on the base layer to be
    - held for momentary scrolling
    - double-tapped to toggle scrolling on
  - can be locked in vertical scroll, horizontal scroll, snap scroll, or unconstrained scroll mode
  - can be used for media control
    - up and down to control volume
    - left and right to scrub
- A navigation layer to open, close, and cycle through open tabs and apps, and a layer to do the same with virtual desktops
- An editing layer with backspace, delete, tab, escape, and enter keys, and quick access to arrow keys
- A files laver with shortcuts to save, search, rename, download, open file browser, select all, print, and print screen
- A cut/copy/paste and browser back/forward/refresh layer
- A 10° offset to so that trackball movements feel natural while maintaining all five fingers in "home positions" on five of the mouse's six buttons
- And more!

---

The hardware and firmware were both designed for a left-handed person. Some modifications will likely be required for ideal use by a right-handed person.

Credit to the fine folk on the [Ploopy Discord](https://discord.gg/qYK9psKrNY) and the maintainers of the [Adept Anyball](https://github.com/adept-anyball) GitHub. Thier ideas and knowledge were very helpful while I was working on this project.
