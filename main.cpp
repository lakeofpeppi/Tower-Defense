// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/IntroScene.hpp"
#include "Scene/VillageScene.hpp"
#include "Scene/BookScene.hpp"



int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO HACKATHON-2 (2/3): Register Scenes here
	game.AddNewScene("start", new StartScene());
    game.AddNewScene("stage-select", new StageSelectScene());
	//game.AddNewScene("play", new PlayScene());
	game.AddNewScene("village", new VillageScene());

	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("scoreboard-scene", new ScoreboardScene());
	game.AddNewScene("intro", new IntroScene());
	game.AddNewScene("book", new BookScene());

    // TODO HACKATHON-1 (1/1): Change the start scene (DONE)
	game.Start("start", 60, 1792, 1216);
	return 0;
}
