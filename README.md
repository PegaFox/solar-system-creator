
# Solar System Creator

A simple n body simulator

## Installation

```

git clone --recursive https://github.com/PegaFox/solar-system-creator

cd solar-system-creator

mkdir build

cd build

cmake ..

make

```

## User Interface

Mouse Scroll: Zoom

WASD: Pan camera

Space: pause/unpause

R: Clear universe

Right click: Create new body

Middle click: Select body

Left click: Drag selected body

Backspace: Delete selected body

Up/Down arrow keys: Change simulation speed

When paused, some options will show up

Turning up lookahead will show a prediction of where orbits will go

The save button will save the current state of the universe to a file called "save.txt"

The load button load the universe with whatever is in "save.txt"

When a body is selected, some additional options may appear

The follow button will make the simulation act as if the followed planet is pinned to the center of the universe

Velocity can be manipulated with the ang slider to change the angle, and changing the strength with the value to the right of the ang slider