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
#include "PrincessScene.hpp"
#include "Scene/BadScene.hpp"


#include "PlayScene.hpp"

void PrincessScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    // Engine::ImageButton *good;
    // Engine::ImageButton *bad;
    // Engine::ImageButton *normal;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/enter.png", 0, 0, wpic, hpic));
    //AddNewObject(new Engine::Image("play/rin dialogue expressions.png", 0, 620, 2700, 600));
    //house fire.jpg
    //princess room.jpg

    // Fill dialogue lines
    dialogueLines = {
      dialogueLines = {
    "              At the end of the path is a small, quiet wooden house,            ", //0 ->enter
    "              You've finally arrived at the final destination outlander,         ", //1 -> enter
    "                 Finish this mission quickly to gain your rewards.              ", //2 ->black
    "                               Tired, you opened the door                           ", //3 -. black
    "                               your mission is almost over                          ", //4 -> black
    "                                                                             ", //5 -> black
    "                         Inside, sits a girl, cold and beautiful.                    ", //6 -> black
    "(She must be the princess)                            ", //7 -> narrate
    "                               Have you come to save me?                            ", //8 ->sayki far
    "Yes. The villagers", //9 -> rin normal
    "           Tell me, Outlander… what makes you think I need saving?           ", //10 -> sayuki normal
    "(her face...., what a large scar)", //11 -> rin normal
    "       Because of the color of my hair, they told me I'm a heaven sent       ", //12 ->sayuki close
    "      They burned my house and killed my parents to 'cleanse' my origins     ", //13 -> house fire
    "        I didn’t understand. I just remember the silence that followed       ", //14 -> house fire
    "             They called me divine. Holy. Said I could save them             ", //15 -> worship
    "                  So, they locked me away… and made me pray.                 ", //16 -> pray shrine
    "                Pray for sunlight. For rain. For crops to grow               ", //17 -> pray shrine
    "                 And when nothing changed....they punished me                ", //18 -> abuse
    "           They starved me, hit me, told me that I was failing them.         ", //19 -> sayuki scars
    "                         That their pain was my fault                           ", //20 -> sauki scars
    "       And then… one day… it worked. Rain fell. The earth bloomed again      ", //21 -> rain
    "      So I prayed more. And more. And somehow… my wishes kept coming true    ", //22 -> rain
    "                            Until one day it didn’t.                          ", //23 -> rain
    "  They grew angry again. I saw it in their eyes. I knew what would come next ", //24 ->angry
    "       So I made one last wish. Not for them. Not for rain. Just for me      ", //25 -> pray
    "      I wished…To be hidden. To be free. To be rescued from the villagers    ", //26 -> pray
    "            I’ve been safe here. Eating herbs. Sleeping in silence.          ", //27 -> sayuki normal
    "                 I don’t want to go back. And yet… here you are              ", //28 -> sayuki close
    "                   So tell me… what will you do to me now?                   ", //29 -> sayuki worry
          " ", // 30
          }

    };

    // Create the first line label
    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;
    // Black screen
    // Black screen
    blackScreen = new Engine::Image("play/black.png", 0, 0, w, h);
    blackScreen->Visible = false;
    AddNewObject(blackScreen);

    AngryScreen = new Engine::Image("play/angry.png", 0, 0, w, h);
    AngryScreen->Visible = false;
    AddNewObject(AngryScreen);

    RoomScreen = new Engine::Image("play/princess room.jpg", 0, 0, w + 100, h + 100);
    RoomScreen->Visible = false;
    AddNewObject(RoomScreen);

    FireScreen = new Engine::Image("play/house fire.jpg", 0, 0, w, h);
    FireScreen->Visible = false;
    AddNewObject(FireScreen);

    RainScreen = new Engine::Image("play/rain.jpg", 0, 0, w, h);
    RainScreen->Visible = false;
    AddNewObject(RainScreen);

    AbuseScreen = new Engine::Image("play/abuse.png", 0, 0, w, h);
    AbuseScreen->Visible = false;
    AddNewObject(AbuseScreen);

    PrayScreen = new Engine::Image("play/pray.png", 0, 0, w, h);
    PrayScreen->Visible = false;
    AddNewObject(PrayScreen);

    WorshipScreen = new Engine::Image("play/worship.png", 0, 0, w, h);
    WorshipScreen->Visible = false;
    AddNewObject(WorshipScreen);



    // Rin normal
    sayuki_normal = new Engine::Image("play/sayuki normal.png", halfW -300 , h - 800, 1200, 800);
    sayuki_normal->Visible = false;
    AddNewObject(sayuki_normal);

    // Rin worried
    sayuki_worry = new Engine::Image("play/sayuki worried.png", halfW -300 , h - 800, 1200, 800);
    sayuki_worry->Visible = false;
    AddNewObject(sayuki_worry);

    sayuki_close = new Engine::Image("play/sayuki close.png", halfW -300 , h - 800, 1200, 800);
    sayuki_close->Visible = false;
    AddNewObject(sayuki_close);

    sayuki_far= new Engine::Image("play/sayuki far.png", halfW -300 , h - 800, 1200, 800);
    sayuki_far->Visible = false;
    AddNewObject(sayuki_far);

    sayuki_scars= new Engine::Image("play/sayuki scars.png", halfW -300 , h - 800, 1200, 800);
    sayuki_scars->Visible = false;
    AddNewObject(sayuki_scars);

    // Dialogue box
    dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, h - 210, 1250, 150);
    dialogueBoxImage->Visible = false;
    AddNewObject(dialogueBoxImage);

    rin_normal= new Engine::Image("play/rin normal.png", halfW - 950, h - 480, 720, 480);
    rin_normal->Visible = false;
    AddNewObject(rin_normal);

    dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, halfW-550, h-150, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(dialogueLabel);



    int btnX = 1792 - 400 - 20; // Right edge - button width - padding
    int btnY = 1216 - 100 - 20; // Bottom edge - button height - padding

    //next
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    btn->SetOnClickCallback(std::bind(&PrincessScene::NextOnClick, this, 1));
    AddNewControlObject(btn);

    // Center the label on the button
    AddNewObject(new Engine::Label("next", "To The Point.ttf", 80,
                                   btnX + 300, // btnX + width/2
                                   btnY + 50, // btnY + height/2
                                   255, 255, 255, 255, // black color
                                   0.5, 0.5)); // centered alignment


    int buttonWidth = 200;
    int buttonHeight = 80;
    int spacing = 40;
    int centerX = 896 - buttonWidth / 2;

    int btnY1 = 300;                    // good
    int btnY2 = btnY1 + buttonHeight + spacing;  // bad
    int btnY3 = btnY2 + buttonHeight + spacing;  // normal

    // GOOD button
    good = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", centerX - 50, btnY1, buttonWidth + 100, buttonHeight);
    good -> Visible = false;
    good->SetOnClickCallback(std::bind(&PrincessScene::GoodOnClick, this));
    AddNewControlObject(good);
    labelGood = new Engine::Label("Stay with her", "To The Point.ttf", 80, centerX + buttonWidth / 2, btnY1 + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelGood->Visible = false;
    AddNewObject(labelGood);
    // BAD button
    bad = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", centerX - 50, btnY2, buttonWidth + 100, buttonHeight);
    bad -> Visible = false;
    bad->SetOnClickCallback(std::bind(&PrincessScene::BadOnClick, this));
    AddNewControlObject(bad);
    labelBad = new Engine::Label("Take her back", "To The Point.ttf", 80, centerX + buttonWidth / 2, btnY2 + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelBad->Visible = false;
    AddNewObject(labelBad);
    // NORMAL button
    normal = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", centerX - 50, btnY3, buttonWidth + 100, buttonHeight);
    normal -> Visible = false;
    normal->SetOnClickCallback(std::bind(&PrincessScene::NormalOnClick, this));
    AddNewControlObject(normal);
    labelNormal = new Engine::Label("Leave her be", "To The Point.ttf", 80, centerX + buttonWidth / 2, btnY3 + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelNormal->Visible = false;
    AddNewObject(labelNormal);


    bgmInstance = AudioHelper::PlaySample("princess-backstory.mp3", true, AudioHelper::BGMVolume);
}
void PrincessScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    }
    IScene::Terminate();
}
void PrincessScene::BackOnClick(int stage) {

}
void PrincessScene::NextOnClick(int stage) {
    AudioHelper::PlaySample("press.mp3");
    if (currentLine + 1 < (int)dialogueLines.size()) {
        currentLine++;
        dialogueLabel->Text = dialogueLines[currentLine];

        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = screenW / 2;
        int halfH = screenH / 2;
        if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
        if (dialogueLabel)dialogueLabel->Position.y = halfH - 100;
        if (dialogueBoxImage)dialogueBoxImage->Visible = false;
        if (sayuki_close)sayuki_close->Visible = false;
        if (sayuki_far)sayuki_far->Visible = false;
        if (sayuki_scars)sayuki_scars->Visible = false;
        if (sayuki_worry)sayuki_worry->Visible = false;
        if (sayuki_normal) sayuki_normal->Visible = false;
        if (rin_normal)rin_normal-> Visible= false;
        if (RoomScreen)RoomScreen-> Visible= false;
        if (FireScreen)FireScreen-> Visible= false;
        if (RainScreen)RainScreen-> Visible= false;
        if (AbuseScreen) AbuseScreen -> Visible= false;
        if (PrayScreen) PrayScreen-> Visible= false;
        if (WorshipScreen) WorshipScreen-> Visible= false;
        if (AngryScreen) AngryScreen -> Visible= false;
        if (blackScreen) blackScreen->Visible = false;
        if (good) good->Visible= false;
        if (bad) bad->Visible= false;
        if (normal) normal->Visible= false;
        if (normal) normal->Visible= false;
        if (labelBad) labelBad->Visible= false;
        if (labelGood) labelGood->Visible= false;


        if (currentLine == 1)
        {
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;

        }

        if (currentLine == 7 || currentLine == 8 || currentLine == 9 || currentLine == 10 || currentLine == 11 || currentLine == 12 || currentLine == 19 || currentLine == 20 || currentLine ==27 || currentLine == 28 || currentLine == 29 || currentLine == 25 || currentLine == 26) {
            if (RoomScreen)RoomScreen-> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;

            if (currentLine == 8 || currentLine == 7 || currentLine == 9)
            {
                if (sayuki_far)sayuki_far->Visible = true;
                if (currentLine == 9 || currentLine == 7)
                {
                    if (rin_normal)rin_normal-> Visible = true;
                    if (dialogueBoxImage)dialogueBoxImage->Visible = true;
                    if (dialogueLabel)dialogueLabel->Position.x = halfW - 300;
                    if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;

                }
            }else if (currentLine == 10 || currentLine == 11 || currentLine == 27)
            {
                if (sayuki_normal)sayuki_normal->Visible = true;
                if (currentLine == 11)
                {
                    if (rin_normal)rin_normal-> Visible = true;
                    if (dialogueLabel)dialogueLabel->Position.x = halfW - 300;
                    if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
                    if (dialogueBoxImage)dialogueBoxImage->Visible = true;
                }
            }else if (currentLine == 12 || currentLine == 28)
            {
                if (sayuki_close) sayuki_close->Visible = true;
            }
            else if (currentLine == 25 || currentLine == 26 || currentLine == 29)
            {
                if (sayuki_worry)sayuki_worry->Visible = true;
            }
        }

        //House
        if (currentLine == 13 || currentLine == 14)
        {
            if (FireScreen)FireScreen-> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 15)
        {
            if (WorshipScreen)WorshipScreen-> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 16 || currentLine == 17)
        {
            if (PrayScreen)PrayScreen-> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 18 || currentLine == 19 || currentLine == 20)
        {
            if (AbuseScreen) AbuseScreen -> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 21 || currentLine == 22 || currentLine == 23)
        {
            if (RainScreen)RainScreen-> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 24)
        {
            if (AngryScreen) AngryScreen -> Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 2 || currentLine == 3 || currentLine == 4 || currentLine == 5 || currentLine == 6) {
            if (blackScreen) blackScreen->Visible = true;
            if (dialogueLabel)dialogueLabel->Visible = true;
            if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
            if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        }
        if (currentLine == 30) {
                if (sayuki_worry) sayuki_worry->Visible = true;
                if (RoomScreen) RoomScreen->Visible = true;

                if (good) good->Visible = true;
                if (labelGood) labelGood->Visible = true;

                if (bad) bad->Visible = true;
                if (labelBad) labelBad->Visible = true;

                if (normal) normal->Visible = true;
                if (labelNormal) labelNormal->Visible = true;

        }


    } else {
      }
}

void PrincessScene::GoodOnClick()
{
    Engine::GameEngine::GetInstance().ChangeScene("good");
    }
void PrincessScene::BadOnClick()
{
    Engine::GameEngine::GetInstance().ChangeScene("bad");

}
void PrincessScene::NormalOnClick()
{
    Engine::GameEngine::GetInstance().ChangeScene("normal");

}

void PrincessScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void PrincessScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
