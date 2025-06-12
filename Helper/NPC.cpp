//
// Created by Angelie Melisa on 2025/6/11.
//
#include "Helper/NPC.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/VillageScene.hpp" // FULL include allowed here

NPC::NPC(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void NPC::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 3;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}

void NPC::OnTouch() {
    if (hasSpoken) return;
    VillageScene* villageScene = dynamic_cast<VillageScene*>(Engine::GameEngine::GetInstance().GetScene("village"));
    if (villageScene) {
        villageScene->ShowDialogue({
            "...", //0 -> toma shock
            "You're not from here! We haven't seen a traveler in years.",
            "Maybe you’ve been sent to",
            "save our dear himesama…",
            "Oh forgive me traveller,",
            "I havent introduced myself",
            "I am Toma, Nakamura Toma",
            "Himesama?",
            "She has been gone for 7 years,",
            "ever since she left us the weather is bad",
            "and the crops wont grow",
            "Princess Sayuki,",
            "I bet she’s scared and alone.",
            "You must find her!",
            "……What will I get from this?",
            "Everything! The village has saved a lot of money to survive",
            "But with the princess here everything will be good again",
            "I'll talk with the other villagers they’ll accept!",
            "Toma quickly gathered everyone and introduced Rin",
            "everyone agreed to give Rin the money.",
            "...Okay then, but keep your promise.",
            "Thank you! To save the princess you’ll need this",
            "Toma hands Rin a pocket with coins",
            "use this to buy any items you need.",
            "Again thank you traveller!",
        });
        hasSpoken = true;
    }
}
