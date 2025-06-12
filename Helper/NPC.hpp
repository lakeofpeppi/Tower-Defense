//
// Created by Angelie Melisa on 2025/6/11.
//

#ifndef NPCTALKER_HPP
#define NPCTALKER_HPP
//
// Created by Angelie Melisa on 2025/6/11.
//
#pragma once
#include "Engine/Sprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "Scene/PlayScene.hpp"

class NPC : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 1.0f; // 5 fps, adjust to taste
    int animationFrame = 0; // 0-2 for 3 frames
    std::string baseImagePath;

    NPC(float x, float y, std::string baseImagePath, std::string targetSceneName)
        : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
          targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

    void Update(float deltaTime) override {
        animationTimer += deltaTime;
        if (animationTimer >= animationInterval) {
            animationTimer = 0.0f;
            animationFrame = (animationFrame + 1) % 3; // Loop 0-3 for 4 frames
            std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
            SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
        }
    }

    void OnTouch(){
        if (hasSpoken) return;
        PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
        if (playScene) {
            playScene->ShowDialogue({
                "...", //0 ->toma shock
                "You're not from here! We haven't seen a traveler in years.", //1 -> toma shock
                "Maybe you’ve been sent to", //2 -> toma shock
                "save our dear himesama…", //3 -> toma happy
                "Oh forgive me traveller,", //4 -> toma worry
                "I havent introduced myself", //5 -> toma worry
                "I am Toma, Nakamura Toma", //6 ->toma happy
                "Himesama?", // 7 -. rin worry
                "She has been gone for 7 years,", //8 -> toma worry
                "ever since she left us the weather is bad", //9 -> toma worry
                "and the crops wont grow", //10 -> toma worry
                "Princess Sayuki,", //11 -> toma worry
                "I bet she’s scared and alone.", //12 -> toma worry
                "You must find her!", //13 -> toma shock
                "……What will I get from this?", //14 rin clos
                "Everything! The village has saved a lot of money to survive", //15 -> toma shock
                "But with the princess here everything will be good again", //16 ->toma happy
                "Ill talk with the other villagers they’ll accept!", // 17 -> toma happy
                "Toma quickly gathered everyone and introduced Rin", //18 -> black
                "everyone agreed to give Rin the money.", //19 -> black
                "...Okay then, but keep your promise.", //20 -> rin normal
                "Thank you! To save the princess you’ll need this", //21 -> toma happy
                "Toma hands Rin a pocket with coins", //22 -> no character
                "use this to buy any items you need.", //23 -> toma happy
                "Again thank you traveller!", //24 -> toma happy
            });
            hasSpoken = true;
        }
    }

};

#endif //NPCTALKER_HPP
