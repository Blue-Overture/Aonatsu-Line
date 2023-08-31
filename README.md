# Aonatsu Line - Blue, Summertime Blue English Translation

Original game developed by GIGA, published by [Entergram](https://www.entergram.co.jp/), English translation presented by Blue Overture.

## About the Game
- Published by [Entergram](https://www.entergram.co.jp/) on [Nintendo eShop](https://store-jp.nintendo.com/list/software/70010000044622.html) on Feb 24, 2022
- The final software made by the development studio GIGA<sup>rest in peace</sup>
### Game Summary, from Play Asia Store
- Tatsumi and his childhood friends Miki and Chihiro talked every day about what they would do during the upcoming summer vacation. By chance, they befriended the overly-honest ojou-sama Yui, who transferred to their school since she admired the co-education system, as well as kouhai Kotone, who had failed to make lasting friendships after trying too hard to fit in. This new group of five friends (?) began to plan for a memorable bittersweet summer vacation that happens only once in a lifetime.
## Requirements

- Aonatsu Line Demo Version for Nintendo Switch
- Retail version also works, but so far only the content found in the demo will be translated.


## Installation

### Nintendo Switch

You must have installed custom firmware `Atmosphere`.

- Copy folders `exefs` and `romfs` from unpacked release to folder on sdcard (if any of those folders don't exist, create them yourself):
  - For demo: `atmosphere/contents/010082E016CFC000/`
  - For full game: `atmosphere/contents/01009B7015E68000/`
- Enjoy!

### Yuzu

- In Yuzu, right-click `アオナツライン` in your games directory
- Select `Open Mod Data Location`
- A window will open showing the directory `yuzu\load\01009B7015E68000`
- In this folder, create a new folder named `English`
- Move folders `exefs` and `romfs` into `yuzu\load\01009B7015E68000\English` and close the window
- In Yuzu, right click `アオナツライン` and select `Properties`
- In the `Add-Ons` tab, make sure `English` is checked and close the window
- If using the demo version, the path will be `yuzu\load\010082E016CFC000` instead
- Enjoy!
For more information about using Yuzu mods, [check out their guide](https://yuzu-emu.org/help/feature/game-modding/).

## FAQ

### How was this made possible?

- It's important to note that we are a team of just two people, and this takes a large amount of meticulous work. This is our first time doing a translation of a video game, so keep that in mind too.

- We reverse-engineered the game assets to unpack them for modification. Afterwards we used [DeepL](https://www.deepl.com/en/whydeepl) deep learning translation as a base for the dialogue. This was then cross-referenced by other translation software and proofread by people. While we strive for a high-quality translation, you may encounter small errors, and we encourage readers to open a pull request for any errors they find so we can patch it out.

### Will this patch work with the PS4, PSVita or Steam release?

No, this patch has been tailor-made for the Nintendo Switch version of the game. There is a chance the assets may be useful on other platforms, but will likely require heavy modifications. Furthermore, we are unable to develop ports to any other system. If you would like to port it using our assets, please feel free to use them or contact us for additional assets.

### Why did you decide to translate this game?

Just felt like it, no other reason.

### Will you port the 18+ content of the game?

Due to the massive amount of work this would pose, we have decided not to.

### Are you working on any other projects now or in the future?

Blue Overture will continue developing the English language patch of Aonatsu Line for the forseeable future until it is complete. There are no other current projects, and there will probably be no other projects until development on this translation has ended.

### Where can I find a compatible copy of the game?

- [Nintendo eShop](https://store-jp.nintendo.com/list/software/70010000044622.html)
- [Play Asia](https://www.play-asia.com/aonatsu-line/13/70f0zz)

## Tools Used

- [DeepL](https://www.deepl.com/en/whydeepl)
- [Yuzu](https://yuzu-emu.org/)
- [GIMP](https://www.gimp.org/)
- [Python](https://www.python.org/)
