# ukui-menu

![build](https://github.com/ukui/ukui-menu/workflows/Check%20build/badge.svg?branch=master)

An advanced menu for UKUI, providing common and all software mixed sort module, alphabet classification module, functional classification module and search module.

# How to report bugs
Bugs should be reported to the UKUI bug tracking system:

	https://github.com/ukui/ukui-menu/issues

You will need to create an account for yourself.

Please read the HACKING file for information on where to send changes or
bugfixes for this package.

# How to build
## Preparation
You can build pre-depends with such command:

`sudo apt install libglib2.0-dev qtbase5-dev libqt5svg5-dev libqt5x11extras5-dev libgsettings-qt-dev libbamf3-dev libxrandr-dev libxtst-dev libx11-dev`

Note: build-depends references the paragraph in debian/control.

## Build from source and test

> git clone https://github.com/ukui/ukui-menu

> cd ukui-menu && mkdir build && cd build

> qmake .. && make

> sudo make install

Note: If you want to run ukui-menu, You just need to execute the ukui-menu command at the terminal.
