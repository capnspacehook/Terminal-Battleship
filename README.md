# Battleship
Commandline classic battleship game (mostly) with smart AI

The difficulty affects the grid size, the number of ships that
can be placed on the grid, and the chance the computer has to hit you.

After you choose a difficulty, follow the onscreen prompts to choose your fleet.
You can place any combination of ships, providing that you are under the ship parts count.
Unlike classic battleship, the composition of your fleet is not predetirmined. 

Choose ships until you run out of ship parts. Then place them on the grid.
When you choose a coordinate, the program places the ship starting at that
coordinate and going to the right or up, depending if you placed the ship 
horizontally or vertically. 

After you have placed your ships, enter coordinates to fire. After you take
your turn, the computer will take a turn. The grid before and after the computer
takes a turn will be shown so you can more easliy see where the computer hit.

There also may be a few explosive barrels on your and your oponent's grid... if either of you
hit one it will hit everything around it in a large radius so be careful!!

### Symbol key:

Cell Type | Symbol 
--- | :---:
Destroyer | D
Submarine | U  
Frigate | F
Battleship | B
Carrier | C
Hit | H
Miss | X
Sunk | S
Explosive Barrel | *
Hit Barrel | #

### AI:
Of course, the AI is the real star of the show. It fires based on where ships are most probable to be. 
[Its design is heavily based off of the blog post from DataGenetics.](http://www.datagenetics.com/blog/december32011/)

Basically, the algorithm tries to place every type of ship in every position and orientation possible, and increment the cells that ships fit in. Where the ship can't fit, such as a hit or sunk ship cell, will remain untouched unless a ship in a different orientation or position or of a different type fits that cell. The cells with the highest number will be the most probable to contain a ship. 

When a ship is hit, things change slightly. If when updating the probability matrix, a ship passes through a hit cell, the ship's cells are incremented by 20. This is to ensure that the algorithm continues to try to sink a ship it hit, and not fire on the next probable cell somewhere completely different. When a ship passes 2 cells is where my algorithm makes some changes to the original laid out in the blog post above. If a ship passes through 2 or more hit points, its cells is incremented by 200 * the number of hit cells it passes through. This small but crucial change greatly improves the efficiency of the algorithm when it is trying to sink a hit ship. 
Before this change, the algorithm would sometimes fire on all the adjacent cells of a hit ship, because they were of equal probability. It might get 2 or 3 hits and divert and fire on an obvious miss to a human player. 

Thanks for playing!
