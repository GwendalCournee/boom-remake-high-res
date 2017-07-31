<a href="https://scan.coverity.com/projects/lifish">
	<img alt="Coverity Scan Build Status"
	     src="https://scan.coverity.com/projects/5674/badge.svg"/>
</a>

## ABOUT ##
Lifish is an arcade old-style game inspired by Factor Software's BOOM.

Coded with [vim](http://www.vim.org/).

Lifish comes with a (Work In Progress) level editor, currently working only on Linux and MacOS: [LifishEdit](https://github.com/silverweed/lifish-edit)

### Planned release ###
Once the main work is done (read: the assets), the game will be released 
for the current platforms:

* Windows Vista or later 64 bit (if you want 32 bit, you can build it yourself)
* Linux (somewhat recent versions of `libc` and `libstdc++` are needed; to be precise,
  your distro must support `GLIBC_2.17` and `GLIBCXX_3.4.20`.)
* OS X
* Probably FreeBSD

See later for instructions on how to build the game from the sources.

## DEPENDENCIES ##

* [SFML](https://github.com/SFML/SFML) >= 2.4
* [nlohmann/json](https://github.com/nlohmann/json) (included in the source)

### Building ###
The graphics you'll find in `assets` is a placeholder. No graphic asset is even close to being final, and the final
assets won't be uploaded on this repo, as they'll be available for purchase in the official release.

#### Linux, OS X, BSD, etc ####
The game can be compiled on *nix (tested on Linux, OSX and FreeBSD) via `cmake . && make`
(you need to have SFML >=2.4 installed).

#### Windows ####
On Windows you can either use VS2015 (or later), or a Unix-like environment like MinGW-w64 (I use the TDM-GCC compiler).

If you use the latter, you can follow the steps on
[this guide](http://ascend4.org/Setting_up_a_MinGW-w64_build_environment) until the *MinGW-w64*
chapter (included -- you can skip the *First steps* section, as all the listed dependencies like Python etc
aren't required). Remember to `cmake -G "MSYS Makefiles" .`, otherwise cmake will probably
generate a VS makefile.

#### Multithreaded mode ####
The game can be compiled in multithreaded mode (separate logic and rendering), but
at the moment it's not really a good idea to do so, as:
- the framerate probably won't improve (and it needs not, should be already smooth 120fps)
- odd behaviours and occasional crashes may occur.


### LICENSE ###
Copyright (c) Giacomo Parolini, 2017

See the LICENSE file for licensing information.
