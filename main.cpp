#include "game.hpp"
#include"saveload.cpp"
#include <iostream>
using namespace std;
#include <string>

int main() {
    std::string choice;
    std::cout << "Load previous game? (yes/no): ";
    std::getline(std::cin, choice);

    if (choice == "yes" && loadGame("data/savegame.txt")) {
        std::cout << "Game loaded!\n";
    } else {
        if (choice != "yes" || !loadGame("data/savegame.txt")) {
            std::cout << "Choose your character (warrior, mage, scavenger): ";
            std::string cls;
            std::getline(std::cin, cls);
        
            if (cls == "warrior") {
                player.characterClass = WARRIOR;
                player.health = 150;
                player.attack = 10;
            } else if (cls == "mage") {
                player.characterClass = MAGE;
                player.health = 80;
                player.attack = 20;
            } else {
                player.characterClass = SCAVENGER;
                player.health = 100;
                player.attack = 15;
            }
        
            player.level = 1;
            player.xp = 0;
            player.x = 0;
            player.y = 0;
            initializeDungeon(DUNGEON_WIDTH, DUNGEON_HEIGHT);
            std::cout << "New game started.\n";
        }
        
    }

    while (true) {
        showStatus();
        std::cout << "Enter command (move [dir], use [item], save, exit): ";
        std::string command;
        std::getline(std::cin, command);

        if (command.rfind("move ", 0) == 0) {
            movePlayer(command.substr(5));
        } else if (command.rfind("use ", 0) == 0) {
            useItem(command.substr(4));
        } else if (command == "save") {
            saveGame("data/savegame.txt");
            std::cout << "Game saved.\n";
        } else if (command == "exit") {
            std::cout << "Goodbye!\n";
            break;
        }

        if (player.health <= 0) {
            std::cout << "You have died.\n";
            break;
        }

        if (bossDefeated) {
            std::cout << "You have defeated the boss and won the game!\n";
            break;
        }
    }

    return 0;
}
