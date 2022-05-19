# 〇〇〇

by Sébastien Abilla

GNU/GPL v3 (voir LICENSE)

![Aperçu](res/illustration.png)

#### Play online:

[itch.io]https://seb46.itch.io/sanmaru

#### Description:

* Pop 3 bubbles of the same color in a row.
* Done with a 50ko/s internet... First time I don't use google/stackoverflow to make a game. :)
* I used a simplified version of the physical engine made for [my CS50 final project](https://github.com/sebabilla/balls_in_the_wind) and
tried to do a real little game with it. (or hope so)

[Source code in French](https://github.com/sebabilla/sanmaru)



#### Credits

* Sound effects reused from my previous games, they were adapted from recordings in [LaSonothèque](https://lasonotheque.org/apropos.html) of Joseph Sardin (Free of right)

###### Font

* Orbitron  par Matt McInerney (Open Font License)

#### Compilation

GNU/Linux running file

```
gcc *.c -Wall -o 〇〇〇 -lm $(sdl2-config --cflags --libs) -lSDL2_gfx -lSDL2_ttf -lSDL2_mixer
```

Web Assembly compilation and companion files running in a web browser

```
emcc *c -O2 -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file res/ -s USE_SDL_GFX=2 -s USE_SDL_MIXER=2 -s ALLOW_MEMORY_GROWTH -o 〇〇〇.html

```
