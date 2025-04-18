#include "game.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

bool saveGame(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) return false;

    out << player.health << "\n" << player.attack << "\n" << player.xp << "\n"
        << player.level << "\n" << player.x << " " << player.y << "\n"
        << player.tempBuff << "\n"; // Add this line
    
    for (const auto& item : player.inventory) {
        out << item << " ";
    }
    out << "\n" << bossDefeated << "\n";


    for (const auto& pair : dungeon) {
        out << pair.first << " " << pair.second.type << " " << pair.second.visited << "\n";
    }

    out.close();
    std::string hash = sha256(filename);
    std::ofstream hfile("data/save_hash.txt");
    hfile << hash;
    hfile.close();
    return true;
}

bool loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    std::ifstream hfile("data/save_hash.txt");
    std::string savedHash;
    getline(hfile, savedHash);
    hfile.close();

    if (savedHash != sha256(filename)) {
        std::cout << "Save file has been tampered with!\n";
        return false;
    }

    in >> player.health >> player.attack >> player.xp >> player.level  >> player.x >> player.y >> player.tempBuff; 
    in.ignore();
    std::string line;
    getline(in, line);
    std::stringstream ss(line);
    player.inventory.clear();
    std::string item;
    while (ss >> item) player.inventory.push_back(item);

    in >> bossDefeated;
    std::string key;
    int type;
    bool visited;
    dungeon.clear();
    while (in >> key >> type >> visited) {
        dungeon[key] = {static_cast<RoomType>(type), visited};
    }

    return true;
}

std::string sha256(const std::string& filename) {
    #ifdef _WIN32
        // Windows implementation
        std::string command = "certutil -hashfile " + filename + " SHA256 | findstr /v \"hash\"";
        FILE* pipe = _popen(command.c_str(), "r");
    #else
        // Linux/Unix implementation
        std::string command = "sha256sum " + filename + " | cut -d ' ' -f1";
        FILE* pipe = popen(command.c_str(), "r");
    #endif
    
    if (!pipe) return "HASH_ERROR";

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }
    
    #ifdef _WIN32
        _pclose(pipe);
    #else
        pclose(pipe);
    #endif
    
    // Clean up the result (remove whitespace and newlines)
    result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
    result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1);
    
    return result;
}
