


# Little Rook Chess

Little Rook Chess is available as an example file with version v1.09 of the library. This page will
describe details of the chess engine and implementation for the Arduino Environment.

## Development Goals

  * A chess game with a nice user interface
  * Should run on a Arduino Uno Board
  * Use a graphical LCD as user interface

## Artistic Challenge

A surprisingly high amount of time was spent on the design of the board and
the chess pieces. The big challenge has been the monochrome display with only 64x64 pixel size for the board. Please let me know if there is any better idea
for the design.

![http://wiki.dogm128.googlecode.com/hg/chess/chess_pieces.jpg](http://wiki.dogm128.googlecode.com/hg/chess/chess_pieces.jpg)


## Game Rules

Important rules like
  * castling and
  * en passant
are supported. Current limitations are
  * no minor pawn conversion (pawn is always converted to queen),
  * no detection of threefold repetition and
  * no implementation of the fifty move rule.

## Game Engine

The game engine is very simple:
  * Min/Max search algorithm
  * Material based evaluation with some position weight

## User Interface
  * Buttons of the DOGS102 and DOGM128 are supported
  * Only valid moves are allowd
  * Some "undo" is possible

## Little Rook Chess with the DOGM128 Shield
![http://wiki.dogm128.googlecode.com/hg/chess/dogm128_shield_chess_640.jpg](http://wiki.dogm128.googlecode.com/hg/chess/dogm128_shield_chess_640.jpg)

## Little Rook Chess with the DOGS102 Shield

![http://wiki.dogm128.googlecode.com/hg/chess/dogs102_shield_chess_512.jpg](http://wiki.dogm128.googlecode.com/hg/chess/dogs102_shield_chess_512.jpg)

# Links

  * [Wiki](dogm128.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)