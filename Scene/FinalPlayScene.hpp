//
// Created by wuru8 on 2024/6/20.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_FINALPLAYSCENE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_FINALPLAYSCENE_HPP


#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <allegro5/allegro_font.h>
#include <queue>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Utility/Conductor.hpp"
#include "Utility/Note.hpp"
#include "UI/Component/ToggledTextButton.hpp"

class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}  // namespace Engine

class FinalPlayScene final : public Engine::IScene {
    // Tower part
private:
    const float l[10] = {0.5f, 1.f, 2.f, 4.f, 6.f, 8.f, 12.f};
    int a24th = 0;
    int p24th = 0;

    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED,
    };
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

    // Rhythm Game part

    int x_shift = 1600;
    int wid = 300;
    int tmpscore=0;

    static const int lanes = 4;
    std::vector<Note> notes;
    ALLEGRO_SAMPLE* backgroundMusic;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Conductor conductor;

    int score;
    Engine::Label* score_label;
    std::string score_text;

    Engine::Label* fps_label;
    mutable std::string fps;

    Engine::Label* judgement_label;
    std::string current_judgement;


    int combo;
    Engine::Label* combo_label;
    std::string combo_text;

    ALLEGRO_FONT* font;

    ALLEGRO_COLOR white;
    ALLEGRO_COLOR cyan;
    ALLEGRO_COLOR yellow;
    ALLEGRO_COLOR red;
    ALLEGRO_COLOR blue;

    mutable std::queue<float> frame_time;

    bool last_note_is_hold[lanes];

    float last_hit_time[lanes];
    float last_pressed_time[lanes];
    float last_up_time[lanes];
    enum class Judgement {
        perfect, good, missed
    };
    Judgement last_judgement[lanes];
    bool allperfect;
    bool fullcombo;
    bool ap_fc_triggered;
    float endtime;

    int lane_key[lanes] = {ALLEGRO_KEY_D, ALLEGRO_KEY_F, ALLEGRO_KEY_J, ALLEGRO_KEY_K};

    static const int pressed_fx_max_brightness = 20;
    constexpr static const float pressed_fx_attack = 0.02;  // in sec
    constexpr static const float pressed_fx_decay = 0.4;    // in sec

    bool cheat_mode;
    Engine::ToggledTextButton* cheat_btn;

protected:
    int lives;
    int money;
    int SpeedMult;
public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    Group* TileMapGroup;
    Group* GroundEffectGroup;
    Group* DebugIndicatorGroup;
    Group* BulletGroup;
    Group* TowerGroup;
    Group* EnemyGroup;
    Group* EffectGroup;
    Group* UIGroup;
    Engine::Label* UIMoney;
    Engine::Label* UILives;
    Engine::Image* imgTarget;
    Engine::Sprite* dangerIndicator;
    Turret* preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;

    static Engine::Point GetClientSize();
    explicit FinalPlayScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadEnemyWave();
    void ConstructUI();
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    std::vector<std::vector<int>> CalculateBFSDistance();
    // void ModifyReadMapTiles();


    // rhythm game part
    void ReadNotes(int songID);
    void OnKeyUp(int keyCode) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_FINALPLAYSCENE_HPP
