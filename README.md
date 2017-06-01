# wow-utilities
A collection of programs that helps me improve my gameplay in WoW

## XP Log View
Compile and run this project, then load the *.lua logfile stored in "xp_log_view/example logs" and you can now see multiple graphs on a single plot. This project uses the LGPL license of the QCustomPlot library, which is located in "xp_log_view/qcustomplot" folder.

Right now, the project was designed for usage only of the author. This means that I have taken some shortcuts. For example, I could have added a way to toggle visibility of each graph. This would be trivial. Another change is to be able to change the window size of "total xp", "quest xp" and "mob xp" graphs, which are window avarages over time interval (t-window_size, t).

### Meanings
The axis on the left hand side denotes XP gained from a single XP source. In the plot, the XP sources are the bar graphs, with the green bars being the XP gained on a single mob kill and the blue bars the XP gained from a single quest turn in (or area discovery).

The axis on the right hand side denotes XP gained per hour. This implies that what is plotted against here must be a summed window average. The yellow graph is the total XP earned per hour, over the N1 last hours. The blue graph is the total XP earned per hour, over the last N2 hours. The green graph is the total XP earned per hour, over the last N3 hours. N1,N2 and N3 values are currently hardcoded. The red graph is the current maximum reached by the green graph, i.e. the maximum XP earned per hour from mobs so far. The dashed golden line is a reference line showing the current the XP earned per hour by killing 3 mobs the same level as you are that level. The level that the player character is currently at is marked just where it changes to it.

The colored background shows the zone that the player is currently in. To see which zone you are in, check the top axis.

### Create a new log
Install the addon located in "xp_log_view/log recording addons" named "Clock". This addon is an old addon that I added logging functionality to. It will automatically record all XP gains to a file "<WoWFolderPath>/WTF/Account/<AccountName>/<ServerName>/<CharacterName>/SavedVariables/Clock.lua"

## XP Path
The work on this utility started with the need to chart a "questing route". In order to level in WoW, as fast as possible, it is important to know what to do and in what order. Essentially, you start the game with a plan of what to do. In order to improve on this plan I wanted to have a way of documenting my current progress. So this is the basic functionality of this program; (1) building and viewing a list of what to do and in what order.

The next feature of this program (2), builds on the previous. Since I wanted a way to verify that each action in the list corresponds to an actual object in the game, I connected my program with a database and select each object used in my list from that database. But now it's also possible to verify important correctness properties of the plan. For example, to turn in a quest, I need to have picked it up first. If it isn't specified in the plan, the the program will point out which element in the list caused the error (turning in a quest the wasn't picked up). I can check many of these properties are correct for each state and so, if I want to change things in my plan, I'm alerted instantly of problems that crops up caused by my changes. This is essentially like a running a compiler, you check that the program is "correct", here I can check that my plan is "correct".

Since I'm now connected to a database, I can automatically pull all data associated with each in-game object that I interact with. This means that I can easily generate a game addon (3) that enables me to press a single button in-game to progress through my list of actions, in order. This means that most NPC interactions become very fast.

### Running the program
To run this program, you need a CMaNGOS database for WoW 1.12 installed, which means that you also need a DBMS and some Qt drivers.
