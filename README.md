This game is about some cells life, on some hexagonal map, as my(Mohammad Safari) final project of first term in amirkabir university of tehran.In this game each player has equal number of cells with energy level 0, and some random name, and in each turn player can choose one of owned cells to 1)move in 6 directions if destinated house is not blocked 2)gain energy(just on energy houses and 100 level of energy as maximum) 3)split(divide) into two cells(with minimum 80 level of energy only in mitosis houses) and two new cells are born.

Besides, you can save your game in every turn and load it when you run game. also a score board is designed to see total energy gathered and total number of your cells of each player. 

I also added another option, a map editor source code should be compiled individualy in the same directory. You have also some options there such as making a new map that you can use it in your main game, reading a map and changing it then saving as a new map for loading into game, and load binary file of map into a text file name map_info.txt to make it possible for human to read content.

P.S : first I have compiled this code with mingw-64bit compiler in windows 10, using _sleep() and getch functions.
Second this game is kind of graphically handled, then should be run in a graphically capable console(in my experience Powershell in "Windows Terminal" not in itself, cmd(never) and not even basic bashes) 
