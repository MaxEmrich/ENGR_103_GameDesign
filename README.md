# Interactive Game Description

## Main Idea:

This will be a bop-it inspired game.

## Rules:

The gameplay cycle will work by producing pre-defined lights and sounds to prompt the user to a button, change switch, or press a capacitance pad (the A1 capacitance pad) on the board. The game will go through 5 pre-defined rounds, where each function call (prompt) is matched with an input that the user needs to give before time runs out. Each round, the time for each interaction goes down.

## Inputs & Outputs

Inputs: The inputs from the user are as follows: left and right buttons, switch, and the single A1 capacitance pad.

Outputs: The outputs are as follows: all of the built-in board LEDS are output devices. The speaker on the board will also be activated as a core gameplay mechanic. The console may be used also to keep track of a player’s score and current level, as well as print out the rule set at the beginning of the game.

## Mapping:

Left button press => interacts based on the left side of the board lighting up.
Right button press => interacts based on the right side of the board lighting up.
Switch on/off => Interact based on two green flashes.
Capacitance pad press => Interact with cap pad after a tone is played.
