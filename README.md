# ElevatorSimulation

## Playtest
- Multiplayer could only be tested in the editor as I didn't do the join server functionality
- In the editor, use the default editor map L_TestLevel to test the elevator mechanic
-Otherwise, feel free to test the shipped version

## Function that I have done
- Elevator Mechanic, each floor will have its own control panel to call the elevator
- The player can control the elevator through the control panel in each elevator
- Simple UI for select floor/prompt input
- WASD control movement, E to interact with Control Panel
- Goal, Touch the white square at the end of the correct floor and generate a new goal

## Programming highlight
- Most of the functionality was done in C++, only the goal mechanic and widget related were mostly done in the blueprint.
- In terms of code formatting and organization, I organized it in descending order of public, protected and private sections. Each section will be having descending order from function to variable.
- For naming convention, most of the naming convention should follow Unreal Guideline. The only difference is I will add a m_ prefix for private members. (exp: m_CharacterPawn)
