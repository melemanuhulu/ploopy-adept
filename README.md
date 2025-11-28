# Ploopy Adept

The [Adept](https://ploopy.co/adept-trackball/) is an awsome open-source trackball by [Ploopy](https://ploopy.co/).

This repository contains custom 3D models and custom QMK firmware configuration files for the Ploopy Adept trackball.

---

![View from the top of the modified Ploopy Adept](https://github.com/melemanuhulu/ploopy-adept/blob/main/hardware/images/top-buttons-cropped.jpg)

The hardware:
 - Is shorter than the original (front-to-back, not top-to-bottom)
 - Has an extra layer that clamps around an [L-bracket](https://8020.net/4349.html) that can be attached to a standard tripod mount for easily adjustable, secure tenting
 - Accomodates [Bosch Rexroth ball transfer units](https://store.boschrexroth.com/en/us/p/ball-transfer-unit-r053010810)
 - Has one bearing postioned farther from the bottom of the mouse to shift the center of gravity and allow for a tenting at a steeper angle
 - Uses nuts and bolts to connect layers rather than screws threaded into plastic (4 each: 8mm M3 socket head cap screws, M3 hex nuts, 8mm M2 socket head cap screws, and M2 hex nuts)
 - Has raised buttons that connect to the main body using 4mm diameter × 2mm thick circular neodymium magnets (magnets are affixed using cyanoacrylate glue)

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
- A cut/copy/paste, browser back/forward, refresh layer
- And more!

---

The hardware and firmware were both designed for a left-handed person. Some modifications will likely be required for ideal use by a right-handed person.

Credit to the fine folk on the [Ploopy Discord](https://discord.gg/qYK9psKrNY) and the maintainers of the [Adept Anyball](https://github.com/adept-anyball) GitHub. Thier ideas and knowledge were very helpful while I was working on this project.
