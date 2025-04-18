#include "game.cpp"
#include"saveload.cpp"
#include <iostream>

int main() {
    while (true) {
        std::string choice;
        std::cout << "Load previous game? (yes/no): ";
        std::getline(std::cin, choice);

        bool loaded = false;
        if (choice == "yes" && loadGame("data/savegame.txt")) {
            std::cout << "Game loaded!\n";
            loaded = true;
        }

        if (!loaded) {
            std::cout << "Available characters:\n";
            std::cout << "1. Warrior - HP: 150, Attack: 10\n";
            std::cout << "2. Mage - HP: 80, Attack: 20\n";
            std::cout << "3. Scavenger - HP: 100, Attack: 15 (dodges traps sometimes)\n";
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
            player.tempBuff = false;
            player.inventory.clear();
            bossDefeated = false;
            initializeDungeon(DUNGEON_WIDTH, DUNGEON_HEIGHT);

            std::cout << "New game started.\n";
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
                return 0;
            }

            if (player.tempBuff && command.rfind("move ", 0) == 0) {
                player.attack /= 2;
                player.tempBuff = false;
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

        std::string again;
        std::cout << "Play again? (yes/no): ";
        std::getline(std::cin, again);
        if (again != "yes") {
            std::cout << "Thanks for playing!\n";
            break;
        }

        // Reset global game state
        dungeon.clear();
        bossDefeated = false;
    }

    return 0;
}
