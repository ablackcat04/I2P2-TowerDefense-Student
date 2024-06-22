cmake_minimum_required(VERSION 3.27)
project(2024_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TowerDefense
        Engine/Allegro5Exception.cpp
        Engine/Allegro5Exception.hpp
        Engine/AudioHelper.cpp
        Engine/AudioHelper.hpp
        Bullet/Bullet.cpp
        Bullet/Bullet.hpp
        Engine/Collider.cpp
        Engine/Collider.hpp
        UI/Animation/DirtyEffect.cpp
        UI/Animation/DirtyEffect.hpp
        Enemy/Enemy.cpp
        Enemy/Enemy.hpp
        UI/Animation/ExplosionEffect.cpp
        UI/Animation/ExplosionEffect.hpp
        Bullet/FireBullet.cpp
        Bullet/FireBullet.hpp
        Engine/GameEngine.cpp
        Engine/GameEngine.hpp
        Engine/Group.cpp
        Engine/Group.hpp
        Engine/IControl.cpp
        Engine/IControl.hpp
        UI/Component/Image.cpp
        UI/Component/Image.hpp
        UI/Component/ImageButton.cpp
        UI/Component/ImageButton.hpp
        Engine/IObject.cpp
        Engine/IObject.hpp
        Engine/IScene.cpp
        Engine/IScene.hpp
        UI/Component/Label.cpp
        UI/Component/Label.hpp
        Bullet/LaserBullet.cpp
        Bullet/LaserBullet.hpp
        Turret/LaserTurret.cpp
        Turret/LaserTurret.hpp
        Engine/LOG.cpp
        Engine/LOG.hpp
        Scene/LoseScene.cpp
        Scene/LoseScene.hpp
        Turret/MachineGunTurret.cpp
        Turret/MachineGunTurret.hpp
        main.cpp
        Bullet/MissileBullet.cpp
        Bullet/MissileBullet.hpp
        Turret/MissileTurret.cpp
        Turret/MissileTurret.hpp
        UI/Animation/Plane.cpp
        UI/Animation/Plane.hpp
        Enemy/PlaneEnemy.cpp
        Enemy/PlaneEnemy.hpp
        Scene/PlayScene.cpp
        Scene/PlayScene.hpp
        Engine/Point.cpp
        Engine/Point.hpp
        Engine/Resources.cpp
        Engine/Resources.hpp
        UI/Component/Slider.cpp
        UI/Component/Slider.hpp
        Enemy/SoldierEnemy.cpp
        Enemy/SoldierEnemy.hpp
        Engine/Sprite.cpp
        Engine/Sprite.hpp
        Scene/StageSelectScene.cpp
        Scene/StageSelectScene.hpp
        Enemy/TankEnemy.cpp
        Enemy/TankEnemy.hpp
        Turret/Turret.cpp
        Turret/Turret.hpp
        Turret/TurretButton.cpp
        Turret/TurretButton.hpp
        Scene/WinScene.cpp
        Scene/WinScene.hpp

        # ODO: [HACKATHON-1-SCENE] (2/4) Add the new scene source files here
        Scene/StartScene.cpp
        Scene/StartScene.hpp
        Scene/SettingsScene.cpp
        Scene/SettingsScene.hpp
        Scene/ScoreboardScene.cpp
        Scene/ScoreboardScene.hpp
        UI/Component/Scoreboard.cpp
        UI/Component/Scoreboard.hpp
        Enemy/FlameTank.cpp
        Enemy/FlameTank.hpp
        Turret/MomoiTurret.cpp
        Turret/MomoiTurret.hpp
        Bullet/MomoiBullet.cpp
        Bullet/MomoiBullet.hpp
        Utility/TimeHelper.cpp
        Utility/TimeHelper.hpp
        Scene/PlotScene.cpp
        Scene/PlotScene.hpp
        UI/Component/RefImage.cpp
        UI/Component/RefImage.hpp
        UI/Component/ToggledTextButton.cpp
        UI/Component/ToggledTextButton.hpp
        Scene/RhythmGameScene.hpp
        Scene/RhythmGameScene.cpp
        Utility/Conductor.cpp
        Utility/Conductor.hpp
        Scene/FinalPlayScene.cpp
        Scene/FinalPlayScene.hpp
        Utility/Note.cpp
        Utility/Note.hpp
        #        UI/Component/TextInput.cpp
#        UI/Component/TextInput.hpp
)

IF (WIN32)
    target_include_directories(TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(TowerDefense PUBLIC lib)
    target_include_directories(TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()