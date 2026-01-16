# Saltwater
An experimental CTRPlugin for running mods in Rhythm Heaven Megamix, with focus on supporting all regional versions of the game and loading mods in a modular way.

This tool is part of the **SpiceRack** project, which aims to create more advanced tools for Rhythm Heaven Megamix modding. Join our Discord server for information at https://discord.gg/xAKFPaERRG !

Join the RHModding server for information and support regarding modding Rhythm Heaven games in general: https://discord.com/invite/ps4rq53

## How to use
The following guides explain how to install Saltwater and its launcher, Barista:

- Hardware: https://patataofcourse.github.io/spicerack-guide/
- Citra: https://patataofcourse.github.io/spicerack-guide/citra.html


## Building
You'll need the following:

- devKitARM
- libctru
- libctrpf (CTRPluginFramework)
- 3gxtool

To install them, you'll need [devkitPro pacman](https://devkitpro.org/wiki/devkitPro_pacman), with the [ThePixellizerOSS package database](https://gitlab.com/thepixellizeross/ctrpluginframework#adding-package-database).

Run `make` to build the plugin.

> If you're trying to use clangd, you'll need to add `--query-driver=$DEVKITARM/bin/arm-none-eabi-*` to its arguments

## Credits
* patataofcourse, 0xadk, and TheAlternateDoctor for programming the actual plugin
* EstexNT for a LOT of the research used in this project - thank you so much!
* Nanquitas for the Luma3DS plugin loader, CTRPF, and the template
* PabloMK7 for CTRPF, the template, CTGP-7 (which inspired this project), and a LOT of support (ty)
* NeoBeo for RHMPatch
* Everyone who's cheered on and helped with testing, thank you so much, everyone <3

###### RHMPatch was released in 2017 guys it's kinda outdated
