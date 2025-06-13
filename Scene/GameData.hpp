#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

class GameData {
public:
    static int lives;
    static int money;
    static int knowledge;
    static int strength;
    static int speed;
    static int CurrentMapId;
    static int orcHP;
    static int orcStrength;
    static int scorpionHP;

    static void Reset() {
        lives = 50;
        money = 0;
        knowledge = 0;
        strength = 100;
        speed = 100;
        CurrentMapId = 1;
    }
};

#endif // GAME_DATA_HPP
