# How to run Saltwater mods
**SALTWATER IS EXPERIMENTAL.** It also does not patch the RomFS, you'll need to use Luma's patching to load files to RomFS.
## Step 1 - Install Luma3DS plugin loader fork
You can check whether you have it or not by opening the Rosalina menu (L + Select + Down). If an option there is 
"Plugin Loader"., you already have the version we need; skip to Step 2. Otherwise, you can get it from:

* [The GitHub releases page](https://github.com/Nanquitas/Luma3DS/releases/tag/v10.2.1). You'll need to replace your current boot.firm.
* UniversalUpdater: it can be found very easily by searching "Nanquitas" (the author of the fork)

![](img/UniversalUpdater.png)

You will need to reconfigure Luma3DS' settings afterwards.

## Step 2 - Set up
Before anything, you want to make sure the plugin loader is enabled- open Rosalina and toggle the option if it's disabled.

You'll need the Saltwater plugin file and the C00.bin file for your region (which might have C00.bin.EU as a name, for example).
DO NOT REMOVE THE .EU/.US/.KR/.JP FROM THE FILE NAME, or Saltwater won't recognize it.

Place the C00 file in the `spicerack` folder in the root of your SD. (EG, for the US region: `spicerack/C00.bin.US`).

Then, you want to place the Saltwater.3gx file in the `luma/plugins/TITLEID` folder in your SD, where TITLEID is:

- `0004000000155a00` for Japan (Rhythm Tengoku: The Best +) [Patching not implemented]
- `000400000018a400` for United States (Rhythm Heaven Megamix)
- `000400000018a500` for Europe (Rhythm Paradise Megamix)
- `000400000018a600` for Korea (Rhythm World: The Best +)

## Step 3 - Running the game
Just open the game as usual. Yeah, that's it!

Enjoy your modded gaming!

(You can also play unmodded by removing the C00 for a specific region - no need for empty C00s anymore!)