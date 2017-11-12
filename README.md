# Toy Press Counter

This is a project for Prof. Hyowon Gweon's lab in order to more accurately measure button presses in a child's toy.

# How to use

## Arduino program
The program given in `main.cpp` needs to be compiled for the Arduino (any of them will do). There are a few constants of interest which can be changed depending on requirements:

- `MIN_BUTTON_PRESS` is the number of milliseconds required for a number to be considered a 'button press.' Any press/release cycle that is shorter than the given one will be ignored.
- `BUTTON_PIN` defines the pin on which the button should be on. Note that we treat a button press as a digital 1->0.

## Python program
The Python 3 (note the 3!) script is contained under `python/main.py`. To install the requirements to run this python script run
```
pip install -r python/requirements.txt
```
from the main directory. Note that the third line of this program should be changed depending on where your Arduino is connected (e.g. on which port it is connected to).