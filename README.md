# Ploopy Adept

The [Ploopy](https://ploopy.co/) [Adept](https://ploopy.co/adept-trackball/) is an awsome open-source trackball.

This repository contains my custom QMK firmware configuration files and custom 3D printer files for the Ploopy Adept trackball.

The hardware:
 - Is shorter than the original (front-to-back, not top-to-bottom)
 - Has an extra layer that clamps around an L-bracket that can be attached to a standard tripod mount for easily adjustable, secure tenting
 - Accomodates Bosch Rexroth ball transfer units
 - Has one bearing postioned farther from the bottom of the mouse to shift the center of gravity and allow for a tenting at a steeper angle

The firmware includes:
- Custom pointer acceleration and deceleration
- Drag-scroll mode with hold/tap logic
- Tap dance actions for additional buttons
- Combos for quick navigation keys
- Per-layer pointer behavior
- Custom timers for gestures
- Multi-layer trackball configuration
- Optional userspace-style reusable code

The hardware and firmware were both designed for a left-handed person. Some modifications will likely be required for ideal use by a right-handed person.

Credit to the fine folk on the [Ploopy Discord](https://discord.gg/qYK9psKrNY) and the maintainers of the [Adept Anyball](https://github.com/adept-anyball) GitHub. Thier ideas and knowledge were very helpful while I was working on this project.
