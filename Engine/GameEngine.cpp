#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "Allegro5Exception.hpp"
#include "GameEngine.hpp"
#include "IScene.hpp"
#include "LOG.hpp"
#include "Point.hpp"
#include "Resources.hpp"

std::string Engine::GameEngine::currentActivePlayerName = "PLAYER";
std::string Engine::GameEngine::currentActiveScene = "";

namespace Engine {
    void GameEngine::initAllegro5() {
        if (!al_init()) throw Allegro5Exception("failed to initialize allegro");

        // Initialize add-ons.
        if (!al_init_primitives_addon()) throw Allegro5Exception("failed to initialize primitives add-on");
        al_init_font_addon();
        // Use the code below if you're using a newer version or Allegro5, the code above is for compatibility.
        // if (!al_init_font_addon()) throw Allegro5Exception("failed to initialize font add-on");
        if (!al_init_ttf_addon()) throw Allegro5Exception("failed to initialize ttf add-on");
        if (!al_init_image_addon()) throw Allegro5Exception("failed to initialize image add-on");
        // Enable antialias by linear interpolation.
        al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
        if (!al_install_audio()) throw Allegro5Exception("failed to initialize audio add-on");
        if (!al_init_acodec_addon()) throw Allegro5Exception("failed to initialize audio codec add-on");
        if (!al_reserve_samples(reserveSamples)) throw Allegro5Exception("failed to reserve samples");
        // Can initialize other add-ons here, such as video, ...
        // Install peripherals.
        if (!al_install_keyboard()) throw Allegro5Exception("failed to install keyboard");
        if (!al_install_mouse()) throw Allegro5Exception("failed to install mouse");

        // Setup game display.
        display = al_create_display(screenW, screenH);
        if (!display) throw Allegro5Exception("failed to create display");
        al_set_window_title(display, title);
        // Set alpha blending mode.
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

        // Load and set window icon.
        if (icon) {
            static std::shared_ptr<ALLEGRO_BITMAP> iconReference = Resources::GetInstance().GetBitmap(icon);
            al_set_display_icon(display, iconReference.get());
            LOG(INFO) << "Loaded window icon from: " << icon;
        }

        // Setup update timer.
        update_timer = al_create_timer(1.0f / fps);
        if (!update_timer) throw Allegro5Exception("failed to create timer");

        // Setup event queue.
        event_queue = al_create_event_queue();
        if (!event_queue) throw Allegro5Exception("failed to create event queue");

        // Check how many mouse buttons are supported.
        const unsigned m_buttons = al_get_mouse_num_buttons();
        LOG(INFO) << "There are total " << m_buttons << " supported mouse buttons";

        // Register display, timer, keyboard, mouse events to the event queue.
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_mouse_event_source());
        // Can register other event sources, such as timer, video, ...

        // Start the timer to update and draw the game.
        al_start_timer(update_timer);
    }
    void GameEngine::startEventLoop() {
        bool done = false;
        ALLEGRO_EVENT event;
        int redraws = 0;
        auto timestamp = std::chrono::steady_clock::now();
        while (!done) {
            al_wait_for_event(event_queue, &event);
            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    // Event for clicking the window close button.
                    LOG(VERBOSE) << "Window close button clicked";
                    done = true;
                    break;
                case ALLEGRO_EVENT_TIMER:
                    // Event for redrawing the display.
                    if (event.timer.source == update_timer)
                        // The redraw timer has ticked.
                        redraws++;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    // Event for keyboard key down.
                    LOG(VERBOSE) << "Key with keycode " << event.keyboard.keycode << " down";
                    activeScene->OnKeyDown(event.keyboard.keycode);
                    key_state[event.keyboard.keycode] = true;
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    // Event for keyboard key up.
                    LOG(VERBOSE) << "Key with keycode " << event.keyboard.keycode << " up";
                    activeScene->OnKeyUp(event.keyboard.keycode);
                    key_state[event.keyboard.keycode] = false;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    // Event for mouse key down.
                    LOG(VERBOSE) << "Mouse button " << event.mouse.button << " down at (" << event.mouse.x << ", " << event.mouse.y << ")";
                    activeScene->OnMouseDown(event.mouse.button, event.mouse.x, event.mouse.y);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    // Event for mouse key up.
                    LOG(VERBOSE) << "Mouse button " << event.mouse.button << " down at (" << event.mouse.x << ", " << event.mouse.y << ")";
                    activeScene->OnMouseUp(event.mouse.button, event.mouse.x, event.mouse.y);
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                    if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                        // Event for mouse move.
                        LOG(VERBOSE) << "Mouse move to (" << event.mouse.x << ", " << event.mouse.y << ")";
                        activeScene->OnMouseMove(event.mouse.x, event.mouse.y);
                    } else if (event.mouse.dz != 0) {
                        // Event for mouse scroll.
                        LOG(VERBOSE) << "Mouse scroll at (" << event.mouse.x << ", " << event.mouse.y << ") with delta " << event.mouse.dz;
                        activeScene->OnMouseScroll(event.mouse.x, event.mouse.y, event.mouse.dz);
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    LOG(VERBOSE) << "Mouse leave display.";
                    ALLEGRO_MOUSE_STATE state;
                    al_get_mouse_state(&state);
                    // Fake mouse out.
                    activeScene->OnMouseMove(-1, -1);
                    break;
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    LOG(VERBOSE) << "Mouse enter display.";
                    break;
                default:
                    // Ignore events that we're not interested in.
                    break;
            }
            // Can process more events and call callbacks by adding more cases.

            // Redraw the scene.
            if (redraws > 0 && al_is_event_queue_empty(event_queue)) {
                if (redraws > 1)
                    LOG(VERBOSE) << redraws - 1 << " frame(s) dropped";
                // Calculate the timeElapsed and update the timestamp.
                auto nextTimestamp = std::chrono::steady_clock::now();
                std::chrono::duration<float> timeElapsed = nextTimestamp - timestamp;
                timestamp = nextTimestamp;
                // Update and draw the next frame.
                update(timeElapsed.count());
                draw();
                redraws = 0;
            }
        }
    }
    void GameEngine::update(float deltaTime) {
        if (!nextScene.empty()) {
            changeScene(nextScene);
            nextScene = "";
        }
        // Force lag to avoid bullet-through-paper issue.
        if (deltaTime >= deltaTimeThreshold)
            deltaTime = deltaTimeThreshold;
        activeScene->Update(deltaTime);
    }
    void GameEngine::draw() const {
        activeScene->Draw();
        al_flip_display();
    }
    void GameEngine::destroy() {
        // Destroy allegro5 window resources.
        al_destroy_timer(update_timer);
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        // Free all scenes.
        for (const auto &pair : scenes)
            delete pair.second;
    }
    void GameEngine::changeScene(const std::string &name) {
        if (scenes.count(name) == 0)
            throw std::invalid_argument("Cannot change to a unknown scene.");
        // Terminate the old scene.
        activeScene->Terminate();
        activeScene = scenes[name];
        // Release unused resources.
        if (freeMemoryOnSceneChanged)
            Resources::GetInstance().ReleaseUnused();
        // Initialize the new scene.
        activeScene->Initialize();
        LOG(INFO) << "Changed to " << name << " scene";
    }
    void GameEngine::Start(const std::string &firstSceneName, int fps, int screenW, int screenH,
                           int reserveSamples, const char *title, const char *icon, bool freeMemoryOnSceneChanged, float deltaTimeThreshold) {
        LOG(INFO) << "Game Initializing...";
        // Update Allegro5 configs.
        this->fps = fps;
        this->screenW = screenW;
        this->screenH = screenH;
        this->reserveSamples = reserveSamples;
        this->title = title;
        this->icon = icon;
        this->freeMemoryOnSceneChanged = freeMemoryOnSceneChanged;
        this->deltaTimeThreshold = deltaTimeThreshold;
        if (scenes.count(firstSceneName) == 0)
            throw std::invalid_argument("The scene is not added yet.");
        activeScene = scenes[firstSceneName];

        initAllegro5();
        LOG(INFO) << "Allegro5 initialized";
        LOG(INFO) << "Game begin";

        activeScene->Initialize();
        LOG(INFO) << "Game initialized";
        // Draw the first frame.
        draw();
        LOG(INFO) << "Game start event loop";
        // This call blocks until the game is finished.
        startEventLoop();
        LOG(INFO) << "Game Terminating...";
        activeScene->Terminate();
        LOG(INFO) << "Game terminated";
        LOG(INFO) << "Game end";
        destroy();
    }
    void GameEngine::AddNewScene(const std::string &name, IScene *scene) {
        if (scenes.count(name) != 0)
            throw std::invalid_argument("Cannot add scenes with the same name.");
        scenes[name] = scene;
    }
    void GameEngine::ChangeScene(const std::string &name) {
        nextScene = name;
    }
    IScene *GameEngine::GetActiveScene() const {
        return activeScene;
    }
    IScene *GameEngine::GetScene(const std::string &name) {
        if (scenes.count(name) == 0)
            throw std::invalid_argument("Cannot get scenes that aren't added.");
        return scenes[name];
    }
    Point GameEngine::GetScreenSize() const {
        return Point(screenW, screenH);
    }
    int GameEngine::GetScreenWidth() const {
        return screenW;
    }
    int GameEngine::GetScreenHeight() const {
        return screenH;
    }
    Point GameEngine::GetMousePosition() const {
        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);
        return Point(state.x, state.y);
    }
    bool GameEngine::IsKeyDown(int keyCode) const {
        ALLEGRO_KEYBOARD_STATE state;
        al_get_keyboard_state(&state);
        return al_key_down(&state, keyCode);
    }
    GameEngine &GameEngine::GetInstance() {
        // The classic way to lazy initialize a Singleton.
        static GameEngine instance;
        return instance;
    }
    bool GameEngine::IsKeyDown(int keycode) {
        if (keycode < 0 || keycode >= ALLEGRO_KEY_MAX) return false;
        return key_state[keycode];
    }
    /*SaveFileData GameEngine::LoadSaveFile(){

        SaveFileData svData;
        std::ifstream inFile(saveFilePath);
        std::string line;

        if (inFile.is_open()) {
            while (std::getline(inFile, line)) {
                if (line.find("PlayerName: ") == 0) {
                    svData.playerName = line.substr(std::string("PlayerName: ").length());
                } else if (line.find("SFXVolume: ") == 0) {
                    svData.sfxVolume = std::stof(line.substr(std::string("SFXVolume: ").length()));
                } else if (line.find("BGMVolume: ") == 0) {
                    svData.bgmVolume = std::stof(line.substr(std::string("BGMVolume: ").length()));
                }
            }
            inFile.close();
        } else {
            std::cerr << "Unable to open file for reading.\n";

            return {"Player", 1.0f, 1.0f};
        }

        return svData;
    }
    void GameEngine::WriteSaveFile(SaveFileData svData){
        SaveFileData old = LoadSaveFile();
        ofstream outFile(saveFilePath);
        if (outFile.is_open()){
            outFile << "PlayerName: " << (svData.playerName.empty() ? old.playerName : svData.playerName) << endl;
            outFile << "BGMVolume: " << svData.bgmVolume << endl;
            outFile << "SFXVolume: " << svData.sfxVolume << endl;
        } else {
            cerr << "Unable to open file for writing " << saveFilePath << "\n";
        }
    }
    vector<scoreBoardData> GameEngine::LoadScoreBoard() {
		vector<scoreBoardData> result;
		ifstream file(leaderBoardFilePath);
		if (!file.is_open()){
			cerr << "Failed to open file " << leaderBoardFilePath << endl;
			return result;
		}
		string line;
		while (getline(file, line)){
			istringstream iss(line);
			scoreBoardData entry;
			char slash, colon;
			iss >> entry.saveTime.day >> slash >> entry.saveTime.month >> entry.saveTime.hour >> colon >> entry.saveTime.minute >> colon >> entry.saveTime.second;
			iss >> entry.playerName >> entry.highestScore;

			if (iss.fail()) {
				std::cerr << "Error parsing line: " << line << std::endl;
				continue;
			}

			// result.resize(result.size() + 1);
			cout << " LOAD : Found " << entry.playerName << endl;
			result.push_back(entry);
		}

		file.close();

		return result;
	}

	void GameEngine::WriteScoreBoard(vector<scoreBoardData> oldSbData,scoreBoardData entry) {
		std::ofstream file(leaderBoardFilePath, std::ios::out); // Open file in output mode
		if (!file.is_open()) {
			return;
		}

		// std::vector<scoreBoardData> oldSbData = Engine::GameEngine::GetInstance().LoadScoreBoard();
		bool contains = false;
		for (auto& data : oldSbData) {
			cout << "Iterating through " << data.playerName << endl;
			if (data.playerName == entry.playerName) {
				contains = true;
				data = entry; // Update existing entry with new data
				break;
			}
		}

		if (!contains) {
			std::cout << "No One named " << entry.playerName << " Creating one... \n";
			oldSbData.push_back(entry); // Add new entry
		}

		// Write all entries back to the file
		for (const auto& data : oldSbData) {
			cout << "WRITING : " << data.playerName<< endl;
			file << data.saveTime.day << "/" << data.saveTime.month << " "
				<< (data.saveTime.hour < 10 ? "0" : "") << data.saveTime.hour << ":"
				<< (data.saveTime.minute < 10 ? "0" : "") << data.saveTime.minute << ":"
				<< (data.saveTime.second < 10 ? "0" : "") << data.saveTime.second << " "
				<< data.playerName << " " << data.highestScore << std::endl;
		}

		std::cout << "Scoreboard Save Success\n";
		file.close();
	}
    DateTime GameEngine::GetCurrentDateTime(){
        auto now = std::chrono::system_clock::now();

        // Convert time point to time_t (used for representing calendar time)
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

        // Convert time_t to tm structure for local time representation
        std::tm* now_tm = std::localtime(&now_time_t);

        // Create a DateTime structure and populate it with the current date and time
        DateTime currentDateTime;
        currentDateTime.year = now_tm->tm_year + 1900; // tm_year is years since 1900
        currentDateTime.month = now_tm->tm_mon + 1;    // tm_mon is months since January [0, 11]
        currentDateTime.day = now_tm->tm_mday;
        currentDateTime.hour = now_tm->tm_hour;
        currentDateTime.minute = now_tm->tm_min;
        currentDateTime.second = now_tm->tm_sec;

        return currentDateTime;
    }

    Point GameEngine::GridToXYPosition(int gridY, int gridX, int blockSize) const {
        // int screenW = Engine::GameEngine::GetInstance().GetScreenWidth();
        // int screenH = Engine::GameEngine::GetInstance().GetScreenHeight();

        return Point(gridY * blockSize, gridX * blockSize);
    }
    std::vector<PlayerEntry> GameEngine::LoadProfileBasedSaving() {
        std::vector<PlayerEntry> result;
        std::ifstream file(profileListFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file " << profileListFilePath << std::endl;
            return result;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            PlayerEntry entry;
            std::string token;

            // Parsing each field based on the '|' delimiter
            std::getline(iss, entry.name, '|');
            std::getline(iss, token, '|');
            entry.avatarID = std::stoi(token);
            std::getline(iss, entry.difficulty, '|');  // Correctly parse difficulty as string
            std::getline(iss, token, '|');
            entry.money = std::stoi(token);
            std::getline(iss, token, '|');
            entry.x = std::stof(token);
            std::getline(iss, token, '|');
            entry.y = std::stof(token);
            std::getline(iss, token, '|');
            entry.speed = std::stof(token);
            std::getline(iss, token, '|');
            entry.currentHP = std::stof(token);
            std::getline(iss, token, '|');
            entry.maxHP = std::stof(token);
            std::getline(iss, token, '|');
            entry.atkDMG = std::stof(token);
            std::getline(iss, token, '|');
            entry.healthPotion = std::stoi(token);
            std::getline(iss, token, '|');
            entry.missile = std::stoi(token);
            std::getline(iss, token, '|');
            entry.shield = std::stoi(token);
            std::getline(iss, token, '|');
            entry.currentEXP = std::stoi(token);
            std::getline(iss, token, '|');
            entry.maxEXP = std::stoi(token);
            std::getline(iss, token, '|');
            entry.playerLevel = std::stoi(token);
            std::getline(iss, entry.lastScene); // Correctly parse the last field as string

            if (iss.fail()) {
                std::cerr << "Error parsing line: " << line << std::endl;
                continue;
            }

            std::cout << "LOAD : Found " << entry.name << std::endl;
            result.push_back(entry);
        }

        file.close();

        return result;
    }
    void GameEngine::WriteProfileBasedSaving(std::vector<PlayerEntry> oldEntryData, PlayerEntry currPlayerEntry) {
        std::ofstream file(profileListFilePath, std::ios::out); // Open file in output mode
        if (!file.is_open()) {
            return;
        }

        bool contains = false;
        for (auto& data : oldEntryData) {
            std::cout << "Iterating through " << data.name << std::endl;
            if (data.name == currPlayerEntry.name) {
                contains = true;
                data = currPlayerEntry; // Update existing entry with new data
                break;
            }
        }

        if (!contains) {
            std::cout << "No One named " << currPlayerEntry.name << " Creating one... \n";
            oldEntryData.push_back(currPlayerEntry); // Add new entry
        }

        // FORMAT : <NAME>|<AVATARID>|<DIFF>|<MONEY>|<POSX>|<POSY>|<SPEED>|<CURRHP>|<MAXHP>|<ATKDMG>|<HEALTHPOTION>|<MISSILE>|<CURREXP>|<MAXEXP>|<CURRLVL>|<LASTSCENENAME>
        for (const auto& data : oldEntryData) {
            std::cout << "WRITING : " << data.name << std::endl;
            file << data.name << "|"
                << data.avatarID << "|"
                << data.difficulty << "|"
                << data.money << "|"
                << data.x << "|"
                << data.y << "|"
                << data.speed << "|"
                << data.currentHP << "|"
                << data.maxHP << "|"
                << data.atkDMG << "|"
                << data.healthPotion << "|"
                << data.missile << "|"
                << data.shield << "|"
                << data.currentEXP << "|"
                << data.maxEXP << "|"
                << data.playerLevel << "|"
                << data.lastScene << std::endl;
        }

        std::cout << "Profile for " << currPlayerEntry.name << " Save Success\n";
        file.close();
    }
    */

    /*
    void GameEngine::SetCurrentActivePlayer(std::string name, PlayerEntry playerEntry){
        currentActivePlayerName = name;
        currentActivePlayerEntry = playerEntry;
    }

    PlayerEntry GameEngine::GetCurrentActivePlayer(){
        return currentActivePlayerEntry;
    }
    */
}

