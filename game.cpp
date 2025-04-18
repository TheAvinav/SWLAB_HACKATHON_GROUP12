#include "game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>


Player player;
std::map<std::string, Room> dungeon;
const int DUNGEON_WIDTH = 5;
const int DUNGEON_HEIGHT = 5;
bool bossDefeated = false;

std::string coordKey(int x, int y) {
    return std::to_string(x) + "_" + std::to_string(y);
}

void initializeDungeon(int width, int height) {
    srand(time(0));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Room room;
            int r = rand() % 10;
            if (x == width - 1 && y == height - 1) {
                room.type = BOSS;
            } else if (r < 3) room.type = BATTLE;
            else if (r < 5) room.type = TREASURE;
            else if (r < 7) room.type = TRAP;
            else room.type = EMPTY;

            dungeon[coordKey(x, y)] = room;
        }
    }
}

void showStatus() {
    std::cout << "\n=== Player Stats ===\n";
    std::cout << "HP: " << player.health << ", XP: " << player.xp << ", Level: " << player.level << "\n";
    std::cout << "Inventory: ";
    for (const auto& item : player.inventory) std::cout << item << " ";
    std::cout << "\nLocation: (" << player.x << ", " << player.y << ")\n";
    std::cout << "=====================\n";
    displayMap(); 
}


void handleRoomEvent() {
    std::string key = coordKey(player.x, player.y);
    Room& room = dungeon[key];
    if (room.visited && room.type != BOSS) 
    {   std::cout << "you have already visited this room\n";
        return; }
    room.visited = true;

    switch (room.type) {
        case BATTLE:
            std::cout << "A monster appears!\n";
            startCombat(rand() % 30 + 20, rand() % 10 + 5);
            break;
        case TREASURE:
            {  // Add these curly braces to create a new scope
                int treasureType = rand() % 4;
                switch (treasureType) {
                    case 0:
                        std::cout << "You found a Healing Potion!\n";
                        player.inventory.push_back("Healing Potion");
                        break;
                    case 1:
                        std::cout << "You found a Strength Potion!\n";
                        player.inventory.push_back("Strength Potion");
                        break;
                    case 2:
                        std::cout << "You found an Experience Scroll!\n";
                        player.inventory.push_back("Experience Scroll");
                        break;
                    case 3:
                        std::cout << "You found a chest of gold! +20 XP\n";
                        player.xp += 20;
                        break;
                }
            }  // Close the braces here
            break;
        case TRAP:
            if (player.characterClass == SCAVENGER && rand() % 2 == 0) {
                std::cout << "You dodged the trap!\n";
            } else {
                int damage = rand() % 20 + 5;
                std::cout << "A trap! You lose " << damage << " HP.\n";
                player.health -= damage;
            }
            break;
        case BOSS:
            std::cout << "You face the final boss!\n";
            startCombat(150, 20);
            if (player.health > 0) bossDefeated = true;
            break;
        default:
            std::cout << "This room is empty.\n";
    }

    levelUpIfNeeded();
}

void movePlayer(const std::string& dir) {
    int dx = 0, dy = 0;
    if (dir == "north") dy = -1;
    else if (dir == "south") dy = 1;
    else if (dir == "east") dx = 1;
    else if (dir == "west") dx = -1;
    else {
        std::cout << "Invalid direction.\n";
        return;
    }

    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX < 0 || newX >= DUNGEON_WIDTH || newY < 0 || newY >= DUNGEON_HEIGHT) {
        std::cout << "You can't move that way.\n";
    } else {
        player.x = newX;
        player.y = newY;
        handleRoomEvent();
    }
}

void startCombat(int enemyHealth, int enemyAttack) {
    int health=enemyHealth;
    int attack=enemyAttack;
    while (enemyHealth > 0 && player.health > 0) {
        std::cout << "\nEnemy HP: " << enemyHealth << " | Your HP: " << player.health << "\n";
        bool isBoss = (health == 150 && attack == 20); // Boss identifiers
        bool playerDouble = false, bossDouble = false;

    if (isBoss) {
         std::cout << "Flip a coin for a chance at double damage (yes/no)? ";
         std::string flip;
         std::getline(std::cin, flip);
        if (flip == "yes") {
        if (rand() % 2 == 0) {
            std::cout << "You win the flip! Your damage is doubled this fight.\n";
            playerDouble = true;
        } else {
            std::cout << "You lose the flip! Boss damage is doubled.\n";
            bossDouble = true;
        }
    }
}

        std::cout << "Choose action (attack/use [item]): ";
        std::string action;
        std::getline(std::cin, action);

        if (action == "attack") {
            int damageDealt = player.attack;
            if (playerDouble) damageDealt *= 2;
            enemyHealth -= damageDealt;
            std::cout << "You attack for " << damageDealt << " damage.\n";
        } else if (action.find("use ") == 0) {
            useItem(action.substr(4));
        } else {
            std::cout << "Invalid action.\n";
        }
        
        if (enemyHealth > 0) {
            int damageTaken = enemyAttack;
            if (bossDouble) damageTaken *= 2;
            player.health -= damageTaken;
            std::cout << "The enemy strikes for " << damageTaken << " damage!\n";
        }
        
    }

    if (enemyHealth <= 0) {
        std::cout << "You defeated the enemy!\n";
        player.xp += 20;
    }
}

void useItem(const std::string& item) {
    auto it = std::find(player.inventory.begin(), player.inventory.end(), item);
    if (it != player.inventory.end()) {
        if (item == "Healing Potion") {
            player.health += 30;
            std::cout << "You used a Healing Potion. +30 HP!\n";
        } else if (item == "Strength Potion") {
            player.attack += 5;
            std::cout << "You used a Strength Potion. +5 attack permanently!\n";
        } else if (item == "Experience Scroll") {
            player.xp += 25;
            std::cout << "You used an Experience Scroll. +25 XP!\n";
            levelUpIfNeeded();
        }
        player.inventory.erase(it);
    } else {
        std::cout << "You don't have that item.\n";
    }
}

void levelUpIfNeeded() {
    int xpForNext = player.level * 50;
    if (player.xp >= xpForNext) {
        player.level++;
        player.attack += 5;
        player.health += 20;
        std::cout << "Level up! You are now level " << player.level << "!\n";
    }
}

void displayMap() {
    std::cout << "\n===== MAP =====\n";
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            if (x == player.x && y == player.y) {
                std::cout << "P "; // Player position
            } else {
                std::string key = coordKey(x, y);
                if (dungeon[key].visited) {
                    switch (dungeon[key].type) {
                        case EMPTY: std::cout << ". "; break;
                        case BATTLE: std::cout << "B "; break;
                        case TREASURE: std::cout << "T "; break;
                        case TRAP: std::cout << "X "; break;
                        case BOSS: std::cout << "! "; break;
                    }
                } else {
                    std::cout << "? "; // Unexplored
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "==============\n";
}


