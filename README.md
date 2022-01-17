# LiveTime
## A simple Linux live speedrun timer.
LiveTime can be used to time anything in a live setting. 
It displays a timer on the screen with a centisecond time resolution.
The timer can be started (default: `shift+q`), paused (default: `shift+f`) and reset (default: `shift+t`) using hotkeys.
LiveTime does not support splits.

## Dependencies
LiveTime is written in python 3 and C.
LiveTime requires `yad` to display the timer as well as the pip `system_hotkey` module to register global hotkeys.
`system_hotkeys` has its own dependencies: `libxcb-render0-dev` as well as the pip modules `xcffib` and `xpybutil`.

To install them (debian):
```
sudo apt-get install libxcb-render0-dev
sudo apt_get install yad
pip3 install xcffib
pip3 install xpybutil
pip3 install system_hotkey
```

## Installation
To use Livetime, you must download the clivetime.c timer script, compile it, and add it to your path, as well as download the livetime python script and add it to your path.
You can use the following commands to do so:
```
wget https://raw.githubusercontent.com/NicolasLalonde/LiveTime/main/clivetime.c
gcc clivetime.c -o $HOME/.local/bin/clivetime
sudo rm clivetime.c
wget https://raw.githubusercontent.com/NicolasLalonde/LiveTime/main/livetime
sudo chmod +x ./livetime
mv ./livetime $HOME/.local/bin/livetime
```

## Configuration
You can configure Livetime to change the hotkeys and the size and position of the displayed timer.
To do this, you can modify the corresponding variables in the livetime python script.
You can modify the livetime python script with your favourite text editor. 
If you followed the installation steps, it should be located in `$HOME/.local/bin`.
For details on supported hotkeys, see the `system_hotkeys` module page: https://pypi.org/project/system_hotkey/

## Usage
To use the timer, simply type in `livetime` in your terminal.
To quit, you can simply use `ctrl+c` in the terminal to send the kill signal.

## Known issues
The `system_hotkeys` module sometimes eats the inputed keys, such that they will not be carried over to whatever other processes you are running. 
The way it does this is somewhat inconsistent. 
Noting this, you may want to use a seperate hotkey combination for the timer and whatever keys you use to start your run or your recording.

Because of the way `yad` updates its windows, the timer looks relatively consistent (although you may see some flashing every now and then) but the window is actually cleared out between every update. Because of this, if you record your screen and pause on any given frame, it's quite possible the timer window will be empty.

In any desktop environment I've tested in, `yad` won't display a window over a fullscreen window. 
The timer window will default to displaying over any other window, but not fullscreen ones. 
To see the timer over your games, you will have to run them in windowed mode.

