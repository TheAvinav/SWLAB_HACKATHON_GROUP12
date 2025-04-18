#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <map>

enum RoomType { EMPTY, BATTLE, TREASURE, TRAP, BOSS };

struct Room {
    RoomType type;
    bool visited = false;
};

enum CharacterClass { WARRIOR, MAGE, SCAVENGER };

struct Player {
    int health;
    int attack;
    int xp;
    int level;
    int x, y;
    std::vector<std::string> inventory;
    CharacterClass characterClass;
    bool tempBuff = false;
};


void initializeDungeon(int width, int height);
void showStatus();
void handleRoomEvent();
void movePlayer(const std::string& dir);
void startCombat(int enemyHealth, int enemyAttack);
void useItem(const std::string& item);
bool saveGame(const std::string& filename);
bool loadGame(const std::string& filename);
std::string sha256(const std::string& filename);
void levelUpIfNeeded();
void displayMap();


extern std::map<std::string, Room> dungeon;
extern Player player;
extern const int DUNGEON_WIDTH;
extern const int DUNGEON_HEIGHT;
extern bool bossDefeated;

#endif
