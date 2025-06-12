#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/SettingsScene.hpp"
#include "IntroScene.hpp"

#include <iostream>
#include <ostream>
#include "BookScene.hpp"

#include "PlayScene.hpp"

void BookScene::Initialize() {

    float w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    float h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    float halfW = w / 2.0f;
    float halfH = h / 2.0f;

    float wpic = 1900.0f;
    float hpic = 1300.0f;
    AddNewObject(new Engine::Image("play/bookstore.png", 0.0f, 0.0f, wpic, hpic));
    float startX = halfW - 600;
    float startY = halfH - 100;


    Engine::ImageButton *btn;

    // Fill dialogue lines
    dialogueLines = {
        " ", //0 -> black
        "    The smell of paper and incense fill your nose  ", //1 -> black
        "Gain as much knowledge traveller,", //2 -> narrator
        "as it will aid you in your journey",//3 ->narrator
        "As you look around, a strange note catches your eye",//4 -> black
        "What is this?", //5 -> rin normal
        " ", //make the first page show up // 6
        "……trees…Forest?", //7 ->rin normal
        " And sands is abundant in the desert,", //8 -> rin normal
        "but how can there be a desert near here? ", //9 -> rin worried
        "And the last one the ocean.", //10 ->rin close
        "How the hell am I supposed to find that?", //11 ->rin worried
        " Don’t joke with me! ", //12 ->rin worried
        "             Frustrated, you flip the page          ", //13 ->black
        " ",//14 -> page 2 will show up here
        "...", // 15 -> rin normal
        " ",//16 -> page 3 will show up here
        "It seems I need to take this mission seriously",//make this postion on the bottom //17 ->rin closed
        "       As you let go of the note, it vanishes,      ", //18 -> black
        "      like it never existed in the first place.     ", //19 ->black
        "Outlander, it seems you have sufficient knowledge,", //20 ->narrator
        "it is best to hurry up and not waste time here anymore" // 21 -> narator
    };

    // Create the first line label
    float lineHeight = 70;
    // Black screen
    blackScreen = new Engine::Image("play/black.png", 0, 0, w, h);
    blackScreen->Visible = true;
    AddNewObject(blackScreen);

    // Page dimensions
    float pageWidth = w * 0.8f;
    float pageHeight = h * 0.8f;

    // Page 1
    page1 = new Engine::Image("play/page 1.png", (w - pageWidth) / 2, (h - pageHeight) / 2, pageWidth, pageHeight);
    page1->Visible = false;
    AddNewObject(page1);

    // Page 2
    page2 = new Engine::Image("play/page 2.png", (w - pageWidth) / 2, (h - pageHeight) / 2, pageWidth, pageHeight);
    page2->Visible = false;
    AddNewObject(page2);

    // Page 3
    page3 = new Engine::Image("play/page 3.png", (w - pageWidth) / 2, (h - pageHeight) / 2, pageWidth, pageHeight);
    page3->Visible = false;
    AddNewObject(page3);

    // Dialogue box
    dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, h - 210, 1250, 150);
    dialogueBoxImage->Visible = false;
    AddNewObject(dialogueBoxImage);

    // Rin normal
    rin_normal = new Engine::Image("play/rin normal.png", halfW - 950, h - 480, 720, 480);
    rin_normal->Visible = false;
    AddNewObject(rin_normal);

    // Rin worried
    rin_worry = new Engine::Image("play/rin worried.png", halfW - 950, h - 480, 720, 480);
    rin_worry->Visible = false;
    AddNewObject(rin_worry);

    // Rin close
    rin_close = new Engine::Image("play/rin close.png", halfW - 950, h - 480, 720, 480);
    rin_close->Visible = false;
    AddNewObject(rin_close);



    dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, startX, startY, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(dialogueLabel);

    int btnX = 1792 - 400 - 20; // Right edge - button width - padding
    int btnY = 1216 - 100 - 20; // Bottom edge - button height - padding

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    btn->SetOnClickCallback([this]() {
    this->NextOnClick(1);
    });
    AddNewControlObject(btn);






    // Center the label on the button
    AddNewObject(new Engine::Label("next", "To The Point.ttf", 80,
        btnX + 300,         // btnX + width/2
        btnY + 50,          // btnY + height/2
        255, 255, 255, 255,       // black color
        0.5, 0.5));         // centered alignment

   // bgmInstance = AudioHelper::PlaySample("book.mp3", true, AudioHelper::BGMVolume);
}
void BookScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();  // safer and clearer
    }
    IScene::Terminate();
}
void BookScene::BackOnClick(int stage) {

}
void BookScene::NextOnClick(int stage) {
    if (currentLine < (int)dialogueLines.size() - 1) {
        currentLine++;
        if (dialogueLabel)dialogueLabel->Text = dialogueLines[currentLine];

        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = screenW / 2;
        int halfH = screenH / 2;

        // Default position and visibility
        if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
        if (dialogueLabel)dialogueLabel->Position.y = halfH - 100;
        if (dialogueBoxImage) dialogueBoxImage->Visible = false;
        if (rin_normal) rin_normal->Visible = false;
        if (rin_worry) rin_worry->Visible = false;
        if (rin_close) rin_close->Visible = false;
        if (page1) page1->Visible = false;
        if (page2) page2->Visible = false;
        if (page3) page3->Visible = false;



        // Hide black screen by default
        if (blackScreen) blackScreen->Visible = false;

        // Special cases: Show dialogue box + appropriate Rin expression
        if (currentLine == 5 || currentLine == 11 || currentLine == 7 || currentLine == 8 || currentLine == 9 || currentLine == 10|| currentLine == 12 || currentLine == 15 || currentLine == 17) {
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 300;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
            if (dialogueBoxImage) dialogueBoxImage->Visible = true;

            if (currentLine == 9|| currentLine == 11 || currentLine == 12) {
                if (rin_worry) rin_worry->Visible = true;
            } else if (currentLine == 5 || currentLine == 8 || currentLine == 7 || currentLine == 15) {
                if (rin_normal) rin_normal->Visible = true;
            }else if (currentLine == 10 || currentLine == 17){
                if (rin_close) rin_close->Visible = true;
            }
        }

        // Show page images at specific lines
        if (currentLine == 6) {
            if (page1) page1->Visible = true;
            if (dialogueBoxImage) dialogueBoxImage->Visible = false;
        }
        if (currentLine == 14) {
            if (page2) page2->Visible = true;
            if (dialogueBoxImage) dialogueBoxImage->Visible = false;
        }
        if (currentLine == 16) {
            if (page3) page3->Visible = true;
            if (dialogueBoxImage) dialogueBoxImage->Visible = false;
        }

        // Show black screen on "You slowly approached the villager"
        if (currentLine == 0|| currentLine == 1 || currentLine == 4 || currentLine == 13 || currentLine == 18 || currentLine == 19) {
            if (blackScreen) blackScreen->Visible = true;

            if (dialogueBoxImage) dialogueBoxImage->Visible = false;
            if (rin_normal) rin_normal->Visible = false;
            if (rin_worry) rin_worry->Visible = false;
            if (rin_close) rin_close->Visible = false;
            if (page1) page1->Visible = false;
            if (page2) page2->Visible = false;
            if (page3) page3->Visible = false;


            // Position dialogue label at bottom center
            if (dialogueLabel) dialogueLabel->Position.x = halfW - 350;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;  // near bottom with some padding

            // Optionally, make sure the label is visible if you hid it earlier
            if (dialogueLabel)dialogueLabel->Visible = true;
        }

    } else {
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("village"));
        if (scene) {
            scene->MapId = stage;
            Engine::GameEngine::GetInstance().ChangeScene("village");
        }
    }
}



void BookScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void BookScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
