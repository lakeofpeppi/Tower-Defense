cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images/play DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resource/images)
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_TowerDefense
        Helper/House.cpp
        Helper/House.hpp
        Helper/NPC.cpp
        Helper/NPC.hpp
    Bullet/Bullet.cpp
    Bullet/Bullet.hpp
    Bullet/FireBullet.cpp
    Bullet/FireBullet.hpp
    Bullet/LaserBullet.cpp
    Bullet/LaserBullet.hpp
        Character/RinCharacter.cpp
        Character/RinCharacter.hpp

    # TODO HACKATHON-3 (3/3): Add the new enemy source files here
    Enemy/Enemy.cpp
    Enemy/Enemy.hpp
    Enemy/SoldierEnemy.cpp
    Enemy/SoldierEnemy.hpp
    Enemy/TankEnemy.cpp
    Enemy/TankEnemy.hpp
        Enemy/AquaTankEnemy.cpp
        Enemy/AquaTankEnemy.cpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    
    # TODO HACKATHON-2 (3/3): Add the new scene source files here

    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.h
    Scene/WinScene.cpp
    Scene/WinScene.hpp
        Scene/SettingsScene.cpp
        Scene/SettingsScene.hpp
        Scene/IntroScene.cpp
        Scene/IntroScene.hpp
    Turret/FireTurret.cpp
        Turret/FireTurret.hpp
        Turret/RocketTurret.hpp
        Turret/RocketTurret.cpp
        Turret/LaserTurret.cpp
    Turret/LaserTurret.hpp
    Turret/MachineGunTurret.cpp
    Turret/MachineGunTurret.hpp
    Turret/Turret.cpp
    Turret/Turret.hpp

    Turret/TurretButton.cpp
    Turret/TurretButton.hpp




        UI/Animation/DirtyEffect.cpp
    UI/Animation/DirtyEffect.hpp
    UI/Animation/ExplosionEffect.cpp
    UI/Animation/ExplosionEffect.hpp
    UI/Animation/Plane.cpp
    UI/Animation/Plane.hpp
        UI/Animation/ShockwaveEffect.cpp
        UI/Animation/ShockwaveEffect.hpp


        UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
        UI/Component/TextBox.cpp
        UI/Component/TextBox.hpp





    
    main.cpp
        Enemy/PlaneEnemy.cpp
        Enemy/PlaneEnemy.hpp
        Scene/ScoreboardScene.cpp
        Scene/ScoreboardScene.hpp
        UI/Component/TextBox.cpp
        UI/Component/TextBox.hpp
        Turret/FireTurret.cpp
        Turret/FireTurret.hpp
        Bullet/BurningBullet.cpp
        Bullet/BurningBullet.hpp
        Turret/RocketTurret.cpp
        Turret/RocketTurret.hpp
        Bullet/RocketBullet.cpp
        Bullet/RocketBullet.hpp
        UI/Animation/FlashEffect.cpp
        UI/Animation/FlashEffect.hpp
        Turret/ShovelTool.cpp
        Turret/ShovelTool.hpp
        Enemy/AquaTankEnemy.cpp
        Enemy/AquaTankEnemy.hpp
        UI/Animation/ShockwaveEffect.cpp
        UI/Animation/ShockwaveEffect.hpp

        Scene/IntroScene.cpp
        Scene/IntroScene.hpp
        Scene/VillageScene.cpp
        Scene/VillageScene.hpp
        Helper/House.hpp
        Helper/House.cpp
        Helper/NPC.cpp
        Helper/NPC.hpp
        Scene/BookScene.cpp
        Scene/BookScene.hpp
        Scene/OceanScene.cpp
        Scene/OceanScene.hpp
        Helper/Fence.hpp
        Scene/SaharaScene.cpp
        Scene/SaharaScene.hpp
        Scene/ForestScene.cpp
        Scene/ForestScene.hpp
        Scene/GameData.cpp
        Scene/GameData.hpp
        Scene/PrincessScene.cpp
        Scene/PrincessScene.hpp
        Enemy/ScorpionEnemy.cpp
        Enemy/ScorpionEnemy.hpp
        Enemy/OrcEnemy.cpp
        Enemy/OrcEnemy.hpp
        Enemy/FrogEnemy.cpp
        Enemy/FrogEnemy.hpp
        Enemy/CrabEnemy.cpp
        Enemy/CrabEnemy.hpp
        Enemy/JellyfishEnemy.cpp
        Enemy/JellyFishEnemy.hpp
        Enemy/SharkEnemy.cpp
        Enemy/SharkEnemy.hpp
        Scene/GoodScene.cpp
        Scene/GoodScene.hpp
        Scene/BadScene.cpp
        Scene/BadScene.hpp
        Scene/NormalScene.cpp
        Scene/NormalScene.hpp
        Helper/Creature.cpp
        Helper/Creature.hpp
        Scene/BattleOrc.cpp
        Scene/BattleOrc.hpp
        Scene/BattleScorpion.cpp
        Scene/BattleScorpion.hpp
        Scene/BattleSea.cpp
        Scene/BattleSea.hpp
        Helper/Frog.cpp
        Helper/Frog.hpp
        Scene/MemoryScene.cpp
        Scene/MemoryScene.hpp

)

IF (WIN32)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC lib)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()