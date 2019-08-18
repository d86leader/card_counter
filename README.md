# Card Counter

An anroid/iphone/desktop app for card-collecting games like Rebirth.
Use it to track cards in your deck or discard pile or both.

This app is stable but incomplete.
Expect new features soon.

## Requirements

Requires Qt 5.12 (because of Qt Quick Controls Icons).

IPhone/OSX build also requires XCode or something. I didn't try it yet.

## Building

The simplest solution would be to open the `card_counter.pro` file in Qt Creator
which you should have with your Qt installation, and build it for the desired platform.

Or you can use these command line instructions to build for your desktop:
`mkdir build && cd build && qmake_qt5 .. && make`
