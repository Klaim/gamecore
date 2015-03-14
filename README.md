# gamecore
Game utility code I used in my **old** game projects.

Original description:
---------------------

GCore (for Game-Core) is my personal lib shared between my game projects (currently made on spare time).
This is a C++ lib that gather classes useful to build the core organization of a game.

Currently it is only tested on Windows XP but I'd like it to be cross-platform
(for at least Windows/Linux/Unix/MacOS and maybe Haiku too).
It should be thread-safe too as there is no static data nor any singleton class.
The user can use the singleton class provided to specify witch class should be a singleton.

This lib is useful when you want to build a game from scratch with other libs (like Ogre + RakNet? + OIS + OPCode etc)
as it provide some useful (and optional) basic systems often required in a game (like a console system, a log system etc).

The only dependency is boost (http://www.boost.org/) 1.42 at least.

IMPORTANT : THIS CODE IS CURRENTLY IN EXPERIMENTAL STATE!
Some part of the code are in heavy refactoring and comments might not always be in sync.
