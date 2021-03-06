# divbox

Branch|[![Travis CI logo](pics/TravisCI.png)](https://travis-ci.org)|[![Codecov logo](pics/Codecov.png)](https://www.codecov.io)
---|---|---
`master`|[![Build Status](https://travis-ci.org/richelbilderbeek/divbox.svg?branch=master)](https://travis-ci.org/richelbilderbeek/divbox) | [![codecov.io](https://codecov.io/github/richelbilderbeek/divbox/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/divbox?branch=master)
`develop`|[![Build Status](https://travis-ci.org/richelbilderbeek/divbox.svg?branch=develop)](https://travis-ci.org/richelbilderbeek/divbox) | [![codecov.io](https://codecov.io/github/richelbilderbeek/divbox/coverage.svg?branch=develop)](https://codecov.io/github/richelbilderbeek/divbox?branch=develop)

General purpose box with divisions, written by Martijn Versteeg. Adaptations by Richel Bilderbeek.

![](test.png)

## Download

 * Windows executable: [here](http://www.richelbilderbeek.nl/divbox.zip)
 * Linux executable: [here](http://www.richelbilderbeek.nl/divbox_linux.zip)

## Compiling

You need the cairo libs (libcairo2-dev package on ubuntu 10.10).

Then do:

```
qmake
make
```

## Useage

Example useage:

```
divbox testbox
```

[testbox](testbox) is a setup file.
`divbox` will use that file to create a file called `test.eps`.

```
Usage: divbox [file]\n
  Reads a description of a box from a file or stdin and writes an eps file for the lasercutter\n\n
Syntax of the input.\n
  #                     Lines starting with # are ignored.
  B <w> <l> <h> [dh]    Specify box dimensions, Width, Length, Height   and optionally the height of the divisions (if omitted dh will equal h).
  L <w> <h>             Specify lasser cutter dimensions: width and height, in millimeters
  N <name.eps>          Set the name of the outputfile
  T <t> [tw] [taper]    Set the thickness of the material, and optionally the size of the teeth and the amount of tapering  of the teeth
  D <pos>               Add a horizontal division at location <pos> along the <l> side of the box
  V <pos>               Add a vertical division in the current horizontal box at location <pos> along the <w> side of the box
  X (or EOF)            Exit the program and write the file
```

## Files

Filename|Description
---|---
divbox.cpp      | The classes implementing the box layout code
divbox.h        | The classes implementing the box layout code
lasercutter.cpp |	General lasercutter utility functions (layers over cairo) might be useful in other projects.
lasercutter.h   | General lasercutter utility functions (layers over cairo) might be useful in other projects.
main.cpp			  | Parser for inputfile
makefile        | Makefile
testbox 			  | Example

## Known Bugs

 * Two vdivs in adjacent gaps on the same location don't work yet.
 * multipage doesn't work yet

## Example

 * see [testbox](testbox)

