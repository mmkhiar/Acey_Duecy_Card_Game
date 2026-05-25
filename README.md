This project is a C++ implementation of the Acey-Deucy card game that demonstrates object-oriented design and extensive use of the Standard Template Library (STL). 
The program features multiple game modes, a save/load system, and a comprehensive statistics tracker that analyzes gameplay using containers such as lists, maps, sets, queues, stacks, and priority queues. 
STL algorithms are used to process and organize data, and the project is structured into modular classes for maintainability and clarity.

main.cpp
The entry point of the program. 
Handles user interaction, game flow, menu navigation, and coordinates all major components including the deck, player, game modes, and statistics tracking.

Card.h + Card.cpp
Defines the Card class, representing a single playing card. 
Includes rank/suit storage, comparison operators, and helper functions such as converting ranks to face cards (J, Q, K, A).

Deck.h + Deck.cpp
Manages the deck of cards using a container. Responsible for creating, shuffling, and dealing cards during gameplay.

Player.h + Player.cpp
Represents the player, including name, balance, and related operations such as adding or subtracting money.

GameMode.h
Abstract base class defining the interface for different game modes.

BetMode.h + BetMode.cpp
Implements gameplay where the player places bets and wins or loses money based on outcomes.

FunMode.h + FunMode.cpp
Implements a non-betting version of the game for casual play without affecting player balance.

SaveSystem.h + SaveSystem.cpp
Handles saving and loading player data to and from a file. 
Manages multiple save slots and allows users to persist game progress.

GameStats.h + GameStats.cpp
Tracks and analyzes game data across rounds. Uses multiple STL containers (list, map, set, queue, stack, priority queue) to store and organize information such as bets, outcomes, and card frequencies.
Also formats and displays a structured statistics report using STL algorithms.

GameTypes.h
Defines shared enumerations such as game modes.

player_game_info.dat
Binary data file used by the save system to store player information.
