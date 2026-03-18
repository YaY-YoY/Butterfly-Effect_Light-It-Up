# Butterfly Effect: Light-It-Up

**Arduino-based LED puzzle game**: toggle lights in a 3x3 grid with chain reactions inspired by the Butterfly Effect.

## Project Overview
This project implements a 3x3 LED puzzle game using an Arduino Mega 2560.  
Pressing one LED toggles its cross-shaped neighbors, and the goal is to turn all LEDs on.  
The game includes a scoring system based on speed and number of moves, creating a challenging and engaging puzzle.

The project emphasizes hardware interaction, game logic, and real-time response.

## Game Rules
- There are **9 LEDs** in total.
- Pressing one LED toggles the state of its **cross-shaped neighbors** (on → off, off → on).
- **All LEDs on** means you win the level.

### Scoring
| Condition                        | Points               |
|----------------------------------|----------------------|
| Complete a level                 | +10                  |
| Finish within 10 seconds         | +5 per second faster |
| Exceed 15 seconds                | -3 per second over   |
| Complete in <3 steps             | +10 per step less    |
| Complete in >5 steps             | -3 per extra step    |
| Total score <0                   | Game Over            |

## Hardware Requirements
- Arduino Mega 2560 ×1
- Breadboard ×1
- LEDs ×9
- Push buttons ×10
- Male-to-male jumper wires ×20
- Single-core wires ×63 (optional)

## Hardware Setup
- **LEDs:** 9 LEDs connected to digital pins **22 to 30** on the Arduino Mega 2560.  
  Each pin controls one LED for turning it on or off.

- **Push Buttons:** 10 buttons connected to digital pins **31 to 39** and **pin 45**.  
  Each button corresponds to a LED toggle input in the game.

## Project Structure
- Light_It_Up.ino
- demo.mp4
  
## Demo Video
You can see the game in action:
https://github.com/YaY-YoY/Butterfly-Effect_Light-It-Up/raw/refs/heads/main/demo.mp4

## How to Run
1. Open `Light_It_Up.ino` in **Arduino IDE**.
2. Connect the Arduino Mega 2560 and wire the LEDs/buttons.
3. Upload the sketch to the Arduino.
4. Press the buttons to play the game.

## Author
Yi-An, Yang

## Contributions
- I coded the **entire game logic** in Arduino: LED control, scoring, and game state.
- Recorded a short **demo video** to show how it works.

## Acknowledgements
- All LEDs and buttons are **standard Arduino components** purchased off-the-shelf.
