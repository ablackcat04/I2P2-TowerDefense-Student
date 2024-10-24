#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "allegro5/allegro_primitives.h"
#include <allegro5/allegro.h>

#include "Engine/AudioHelper.hpp"
#include "Engine/Resources.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/MissileTurret.hpp"
#include "Turret/MomoiTurret.hpp"
#include "Turret/TurretButton.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Enemy/FlameTank.hpp"
#include "Enemy/BossEnemy.hpp"
#include "WinScene.hpp"
#include "PlayScene.hpp"
#include "MapScene.hpp"
#include "Utility/UsefulConstants.hpp"
#include "UI/Component/FPSLabel.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = {Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
                                          ALLEGRO_KEY_LEFT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_RIGHT,
                                          ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

PlayScene::PlayScene() : backgroundMusic(nullptr), bgmInstance(nullptr) , conductor() {
    white = al_map_rgb(255,255,255);
    cyan = al_map_rgb(0, 220, 220);
    yellow = al_map_rgb(220, 220, 0);
    red = al_map_rgb(255, 0, 0);
    blue = al_map_rgb(0,0,255);
}

void PlayScene::Initialize() {
    al_resize_display(Engine::GameEngine::GetInstance().GetDisplay(), 1920, 832);
    mapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 10;
    money = 150;
    SpeedMult = 1;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    ReadMap();
    ReadEnemyWave();
    mapDistance = CalculateBFSDistance();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    // Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    //bgmId = AudioHelper::PlayBGM("play.ogg");

    al_resize_display(Engine::GameEngine::GetInstance().GetDisplay(), 1900, 832);

    for (int i = 0; i < lanes; ++i) {
        last_hit_time[i] = -10000;
        last_judgement[i] = Judgement::missed;
        last_pressed_time[i] = -10000;
    }
    combo = 0;
    current_judgement = "";
    notes.clear();
    is_all_perfect = true;
    is_full_combo = true;

    for (int i = 0; i < lanes; ++i) {
        last_note_is_hold[i] = false;
    }

    score = 0;
    font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 40, 0);

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    //int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    if (MapId == 1) {
        bgmInstance = AudioHelper::PlaySample("Beyond_Apocalypse.ogg", false, AudioHelper::BGMVolume);
    } else if (MapId == 2) {
        bgmInstance = AudioHelper::PlaySample("Monochrize.ogg", false, AudioHelper::BGMVolume);
    } else {
        bgmInstance = AudioHelper::PlaySample("Salad_Savior_D1AB0Lic_DEV0Ti0N.ogg", false, AudioHelper::BGMVolume);
    }

    if (MapId == 1) conductor.init(152, 0);
    else if (MapId == 2) conductor.init(160, 0);
    else if (MapId == 3) conductor.init(250, 0);

    ReadNotes(MapId);

    score_label = new Engine::Label(&score_text, "BoutiqueBitmap7x7_1.7.ttf", 40, x_shift, 0, 255, 255, 255, 255);
    AddRefObject(*score_label);
    judgement_label = new Engine::Label(&current_judgement, "BoutiqueBitmap7x7_1.7.ttf", 64, x_shift + 45, 490, 255, 255, 255, 255, 1, 1);
    AddRefObject(*judgement_label);
    combo_label = new Engine::Label(&combo_text, "BoutiqueBitmap7x7_1.7.ttf", 64, x_shift + 121, 400, 255, 255, 255, 255, 1, 1);
    AddRefObject(*combo_label);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 100, 50, 50, 50);
    btn->SetOnClickCallback([] { Engine::GameEngine::GetInstance().ChangeScene(SceneNames::select);});
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 24, w-100, 75, 0, 0, 0, 255, 0.5, 0.5));

    cheat_mode = false;
    cheat_btn = new Engine::ToggledTextButton("cheat", &cheat_mode, 1400, 400, al_map_rgb(0,0,0), al_map_rgb(50,50,50),
                                            al_map_rgb(100,100,100));
    AddRefControlObject(*cheat_btn);

    ap_fc_triggered = false;

    Engine::FPSLabel* fps = new Engine::FPSLabel("BoutiqueBitmap7x7_1.7.ttf", 40, x_shift + wid - 50, 0, 255, 255, 255, 255, 0, 0);
    AddRefObject(*fps);
}
void PlayScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
    delete score_label;
    delete combo_label;
    delete judgement_label;

    WinScene* win_scene = dynamic_cast<WinScene*>(Engine::GameEngine::GetInstance().GetScene(SceneNames::win));
    win_scene->SetLastGameInfo(money, lives);

    al_resize_display(Engine::GameEngine::GetInstance().GetDisplay(), 1600, 832);

    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void PlayScene::Update(float deltaTime) {
    combo_text = std::to_string(combo);
    conductor.update();
    score_text = std::to_string(score);

    a24th = ((conductor.song_position / conductor.length_per_beat_in_seconds) * 6);

    for (float f : l) {
        if ((int)(a24th / (24/f)) > (int)(p24th / (24/f))) {
            for (auto i: TowerGroup->GetObjects()) {
                reinterpret_cast<Turret*>(i)->TriggerByRhythm(f);
            }
            for (auto i : EnemyGroup->GetObjects()) {
                reinterpret_cast<Enemy*>(i)->TriggerByRhythm(f);
            }
        }
    }

    p24th = a24th;

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        n->update(conductor);
        float t;
        if (n->is_hold) {
            float addtime= (float)n->height_in_pixel / 700.0;
            t = conductor.song_position - n->start_time * conductor.length_per_beat_in_seconds - 1 - addtime;

            if (cheat_mode) {
                float tt = conductor.song_position - n->start_time * conductor.length_per_beat_in_seconds - 1;
                if (!n->will_note_be_hit_by_cheat && tt > 0) {
                    last_note_is_hold[n->lane] = true;
                    last_hit_time[n->lane] = conductor.song_position;
                    last_judgement[n->lane] = Judgement::perfect;
                    current_judgement = "Perfect";
                    ++combo;

                    score += 100;
                    EarnMoney(5);

                    n->will_note_be_hit_by_cheat = true;

                    for (auto i : TowerGroup->GetObjects()) {
                        dynamic_cast<Turret *>(i)->TriggerByHit();
                    }
                }
            }
        } else {
            t = conductor.song_position - n->start_time * conductor.length_per_beat_in_seconds - 1;
        }

        if (cheat_mode) {
            if (t > 0) {
                last_note_is_hold[n->lane] = false;
                if (!n->is_hold) {
                    last_hit_time[n->lane] = conductor.song_position;
                }
                last_up_time[n->lane] = conductor.song_position;

                last_judgement[n->lane] = Judgement::perfect;
                current_judgement = "Perfect";
                ++combo;

                score += 100;
                EarnMoney(5);

                for (auto i : TowerGroup->GetObjects()) {
                    dynamic_cast<Turret *>(i)->TriggerByHit();
                }

                n = notes.erase(n);
                --n;
            }
        }

        if (t> 0.1) {
            is_all_perfect= false;
            is_full_combo = false;
            last_judgement[n->lane] = Judgement::missed;
            n = notes.erase(n);
            current_judgement = "Missed";
            --n;
            combo = 0;
        }
    }
    if(notes.empty()){
        if (!ap_fc_triggered) {
            if(is_all_perfect){
                AddNewObject(new Engine::Label("Allperfect", "pirulen.ttf", 100, 804, 400, 255, 0, 0, 255, 0.5, 0.5));
                EffectGroup->AddNewObject(new Plane());
                EarnMoney(10000);
            } else if(is_full_combo){
                AddNewObject(new Engine::Label("Fullcombo", "pirulen.ttf", 100, 804, 400, 255, 0, 0, 255, 0.5, 0.5));
                EffectGroup->AddNewObject(new Plane());
                EarnMoney(5000);
            }
            ap_fc_triggered = true;
        }
    }

    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    // Calculate danger zone.
    std::vector<float> reachEndTimes;
    for (auto& it : EnemyGroup->GetObjects()) {
        reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
    }
    // Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
    std::sort(reachEndTimes.begin(), reachEndTimes.end());
    float newDeathCountDown = -1;
    int danger = lives;
    for (auto& it : reachEndTimes) {
        if (it <= DangerTime) {
            danger--;
            if (danger <= 0) {
                // Death Countdown
                float pos = DangerTime - it;
                if (it > deathCountDown) {
                    // Restart Death Count Down BGM.
                    AudioHelper::StopSample(deathBGMInstance);
                    if (SpeedMult != 0)
                        deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
                }
                float alpha = pos / DangerTime;
                alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
                dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
                newDeathCountDown = it;
                break;
            }
        }
    }
    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0)
        AudioHelper::StopSample(deathBGMInstance);
    if (deathCountDown == -1 && lives > 0) {
        AudioHelper::StopSample(deathBGMInstance);
        dangerIndicator->Tint.a = 0;
    }
    if (SpeedMult == 0)
        deathCountDown = -1;
    for (int i = 0; i < SpeedMult; i++) {
        IScene::Update(deltaTime);
        // Check if we should create new enemy.
        ticks += deltaTime;
        if (enemyWaveData.empty()) {
            if (EnemyGroup->GetObjects().empty()) {
                // Free resources.
                /*delete TileMapGroup;
                delete GroundEffectGroup;
                delete DebugIndicatorGroup;
                delete TowerGroup;
                delete EnemyGroup;
                delete BulletGroup;
                delete EffectGroup;
                delete UIGroup;
                delete imgTarget;*/
                WinScene* scene = dynamic_cast<WinScene*>(Engine::GameEngine::GetInstance().GetScene(SceneNames::win));
                scene->MapId = MapId;

                auto* m = reinterpret_cast<MapScene *>(Engine::GameEngine::GetInstance().GetScene(SceneNames::map));

                m->UnlockStage(MapId);

                Engine::GameEngine::GetInstance().ChangeScene(SceneNames::win);
            }
            continue;
        }
        auto current = enemyWaveData.front();
        if (ticks < current.second)
            continue;
        ticks -= current.second;
        enemyWaveData.pop_front();
        const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
        Enemy* enemy;
        switch (current.first) {
            case 1:
                EnemyGroup->AddNewObject(enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                break;
            case 2:
                EnemyGroup->AddNewObject(enemy = new PlaneEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                break;
            case 3:
                EnemyGroup->AddNewObject(enemy = new TankEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                break;
            case 4:
                EnemyGroup->AddNewObject(enemy = new FlameTank(SpawnCoordinate.x, SpawnCoordinate.y));
                break;
            case 5:
                EnemyGroup->AddNewObject(enemy = new BossEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
                break;
            default:
                continue;
        }
        enemy->UpdatePath(mapDistance);
        // Compensate the time lost.
        enemy->Update(ticks);
    }
    if (preview) {
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        // To keep responding when paused.
        preview->Update(deltaTime);
    }
}
void PlayScene::Draw() const {
    //IScene::Draw();

    if (DebugMode) {
        // Draw reverse BFS distance on all reachable blocks.
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                }
            }
        }
    }

    IScene::Draw();

    for (int i = 0; i < lanes; ++i) {   // draw the pressed visual feed-back
        float t = conductor.song_position - last_pressed_time[i];
        if (t < 0) {
            continue;
        }
        if (t <= pressed_fx_attack) {
            al_draw_filled_rectangle(x_shift + (wid/4 * i), 0, x_shift + wid/4*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(t/pressed_fx_attack)));
        } else if (t <= pressed_fx_decay + pressed_fx_attack) {
            al_draw_filled_rectangle(x_shift + wid/4 * i, 0, x_shift + wid/4*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(1 - (t-pressed_fx_attack)/pressed_fx_decay)));
        }
    }

    for (auto note : notes) {
        if (note.is_active) {
            note.render();
        } else {
            break;
        }
    }

    al_draw_line(x_shift, 700, x_shift + wid, 700, white, 10);
    al_draw_line(x_shift + wid/4, 0, x_shift + wid/4, 700, white, 4);
    al_draw_line(x_shift + wid/2, 0, x_shift + wid/2, 700, white, 4);
    al_draw_line(x_shift + 3*wid/4, 0, x_shift + 3*wid/4, 700, white, 4);

    for (int i = 0; i < lanes; ++i) {   // draw the hit visual feed-back
        float t = conductor.song_position - last_hit_time[i];
        if (t < 0.1) {
            al_draw_rectangle(x_shift + wid/4 * i + 20 - (20)*(t*10), 685, x_shift + wid/4 * (i + 1) - 20 + (20)*(t*10), 715,
                              (last_judgement[i] == Judgement::perfect ? cyan : yellow), 40*(t+0.05));
        } else if (last_up_time[i] < last_hit_time[i] && last_judgement[i] != Judgement::missed && last_note_is_hold[i]) {
            al_draw_rectangle(x_shift + wid/4 * i, 685, x_shift + wid/4 * (i + 1), 715,
                              (last_judgement[i] == Judgement::perfect ? cyan : yellow), 40*(0.15));
        }
    }

}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && !imgTarget->Visible && preview) {
        // Cancel turret construct.
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
    IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (!imgTarget->Visible)
        return;
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (button & 1) {
        if (mapState[y][x] != TILE_OCCUPIED) {
            if (!preview)
                return;
            // Check if valid.
            if (!CheckSpaceValid(x, y)) {
                Engine::Sprite* sprite;
                GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
                sprite->Rotation = 0;
                return;
            }
            // Purchase.
            EarnMoney(-preview->GetPrice());
            // Remove Preview.
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            // Construct real turret.
            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
            TowerGroup->AddNewObject(preview);
            // To keep responding when paused.
            preview->Update(0);
            // Remove Preview.
            preview = nullptr;

            mapState[y][x] = TILE_OCCUPIED;
            OnMouseMove(mx, my);
        }
    }
}
void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);

    if (keyCode == ALLEGRO_KEY_Q) {
        // Hotkey for MachineGunTurret.
        UIBtnClicked(0);
    }
    else if (keyCode == ALLEGRO_KEY_W) {
        // Hotkey for LaserTurret.
        UIBtnClicked(1);
    }
    else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for MissileTurret.
        UIBtnClicked(2);
    }
    else if (keyCode == ALLEGRO_KEY_R) {
        UIBtnClicked(3);
    } else if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }

    int lane = -1;
    for (int i = 0; i < lanes; ++i) {
        if (lane_key[i] == keyCode) {
            lane = i;
            last_pressed_time[lane] = conductor.song_position;
        }
    }
    if (lane == -1) {
        return;
    }

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        float t = conductor.song_position - n->start_time * conductor.length_per_beat_in_seconds - 1;
        if (n->lane == lane && n->is_active) {
            if (t > -0.1 && t < 0.1) {
                if(!n->is_hold){
                    n = notes.erase(n);
                    --n;
                    last_note_is_hold[n->lane] = false;
                } else {
                    last_note_is_hold[n->lane] = true;
                }


                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    tmpscore+=100;
                    current_judgement = "Perfect";
                    last_judgement[lane] = Judgement::perfect;
                    EarnMoney(5);
                    if(tmpscore>=1000){
                        tmpscore-=1000;
                        //EarnMoney(200);
                    }
                } else {
                    score += 50;
                    tmpscore+=50;
                    current_judgement = " Good";
                    last_judgement[lane] = Judgement::good;
                    is_all_perfect= false;
                    EarnMoney(2);
                    if(tmpscore>=1000){
                        tmpscore-=1000;
                        //EarnMoney(200);
                    }
                }
                for (auto i : TowerGroup->GetObjects()) {
                    dynamic_cast<Turret *>(i)->TriggerByHit();
                }
                ++combo;
                last_hit_time[lane] = conductor.song_position;
                break;
            }
        }

        if (keyCode == ALLEGRO_KEY_TAB) {
            DebugMode = !DebugMode;
        }
        else {
            keyStrokes.push_back(keyCode);
            if (keyStrokes.size() > code.size())
                keyStrokes.pop_front();
            if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
                auto it = keyStrokes.begin();
                for (int c : code) {
                    if (!((*it == c) ||
                          (c == ALLEGRO_KEYMOD_SHIFT &&
                           (*it == ALLEGRO_KEY_LSHIFT || *it == ALLEGRO_KEY_RSHIFT))))
                        return;
                    ++it;
                }

                EffectGroup->AddNewObject(new Plane());
                EarnMoney(10000);
            }
        }
    }

    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
}
void PlayScene::Hit() {
    lives--;
    UILives->Text = std::string("Life ") + std::to_string(lives);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene(SceneNames::lose);
    }
}
int PlayScene::GetMoney() const {
    return money;
}
void PlayScene::EarnMoney(int money) {
    this->money += money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
}
void PlayScene::ReadMap() {
    std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    // Read map file.
    char c;
    std::vector<bool> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        switch (c) {
            case '0': mapData.push_back(false); break;
            case '1': mapData.push_back(true); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num)
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}
void PlayScene::ReadEnemyWave() {
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
    // Read enemy file.
    float type, wait, repeat;
    enemyWaveData.clear();
    std::ifstream fin(filename);
    while (true) {

        bool ts = (bool)(fin >> type);
        bool ws = (bool)(fin >> wait);
        bool rs = (bool)(fin >> repeat);

        if (!ts && !ws && !rs) {
            break;
        }

        if (!ts) {
            type = 0;
        }
        if (!ws) {
            wait = 1.0;
        }
        if (!rs) {
            repeat = 0;
        }

        for (int i = 0; i < repeat; i++)
            enemyWaveData.emplace_back(type, wait);
    }
    fin.close();
}
void PlayScene::ConstructUI() {
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
    TurretButton* btn;
    // Button 1
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0)
            , 1294, 136, MachineGunTurret::Price);
    // Reference: Class Member Function Pointer and std::bind.
    btn->SetOnClickCallback([this] { UIBtnClicked(0); });
    UIGroup->AddNewControlObject(btn);
    // Button 2
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0)
            , 1370, 136, LaserTurret::Price);
    btn->SetOnClickCallback([this] { UIBtnClicked(1); });
    UIGroup->AddNewControlObject(btn);
    // Button 3
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1446, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/turret-3.png", 1446, 136, 0, 0, 0, 0)
            , 1446, 136, MissileTurret::Price);
    btn->SetOnClickCallback([this] { UIBtnClicked(2); });
    UIGroup->AddNewControlObject(btn);
    // Button 4
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1446+76, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/momoi64x64.png", 1446+76, 136, 0, 0, 0, 0)
            , 1446+76, 136, MomoiTurret::Price);
    btn->SetOnClickCallback([this] { UIBtnClicked(3); });
    UIGroup->AddNewControlObject(btn);

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::UIBtnClicked(int id) {
    if (preview)
        UIGroup->RemoveObject(preview->GetObjectIterator());
    if (id == 0 && money >= MachineGunTurret::Price)
        preview = new MachineGunTurret(0, 0);
    else if (id == 1 && money >= LaserTurret::Price)
        preview = new LaserTurret(0, 0);
    else if (id == 2 && money >= MissileTurret::Price)
        preview = new MissileTurret(0, 0);
    else if (id == 3 && money >= MomoiTurret::Price)
        preview = new MomoiTurret(0, 0);
    if (!preview)
        return;
    preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
    preview->Tint = al_map_rgba(255, 255, 255, 200);
    preview->Enabled = false;
    preview->Preview = true;
    UIGroup->AddNewObject(preview);
    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return false;
    auto map00 = mapState[y][x];
    mapState[y][x] = TILE_OCCUPIED;
    std::vector<std::vector<int>> map = CalculateBFSDistance();
    mapState[y][x] = map00;
    if (map[0][0] == -1)
        return false;
    for (auto& it : EnemyGroup->GetObjects()) {
        Engine::Point pnt;
        pnt.x = std::floor(it->Position.x / BlockSize);
        pnt.y = std::floor(it->Position.y / BlockSize);
        if (pnt.x < 0) pnt.x = 0;
        if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
        if (pnt.y < 0) pnt.y = 0;
        if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
        if (map[pnt.y][pnt.x] == -1) {
            return false;
        }
    }
    // All enemy have path to exit.
    mapState[y][x] = TILE_OCCUPIED;
    mapDistance = map;
    for (auto& it : EnemyGroup->GetObjects())
        dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
    return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> que;
    // Push end point.
    // BFS from end point.
    if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
        return map;
    que.emplace(MapWidth - 1, MapHeight - 1);
    map[MapHeight - 1][MapWidth - 1] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();

        if (p.y < 0 || p.x < 0 || p.y >= MapHeight || p.x >= MapWidth) {
            continue;
        }


        int minbgt0 = -4;

        if (p.y-1 >= 0 && map[p.y-1][p.x] >= 0) {
            minbgt0 = map[p.y-1][p.x] + 1;
        }
        if (p.x-1 >= 0 && map[p.y][p.x-1] >= 0 && map[p.y][p.x-1] > minbgt0) {
            minbgt0 = map[p.y][p.x-1] + 1;
        }
        if (p.y+1 < MapHeight && map[p.y+1][p.x] >= 0 && map[p.y+1][p.x] > minbgt0) {
            minbgt0 = map[p.y+1][p.x] + 1;
        }
        if (p.x+1 < MapWidth && map[p.y][p.x+1] >= 0 && map[p.y][p.x+1] > minbgt0) {
            minbgt0 = map[p.y][p.x+1] + 1;
        }


        if (p.y == MapHeight-1 && p.x == MapWidth-1) {
            map[p.y][p.x] = 0;
        } else {
            map[p.y][p.x] = minbgt0;
        }


        if (p.y-1 >= 0 && mapState[p.y-1][p.x] == TILE_DIRT && map[p.y-1][p.x] == -1) {
            que.emplace(p.x, p.y-1);
            map[p.y-1][p.x] = -2;
        }
        if (p.x-1 >= 0 && mapState[p.y][p.x-1] == TILE_DIRT && map[p.y][p.x-1] == -1) {
            que.emplace(p.x-1, p.y);
            map[p.y][p.x-1] = -2;
        }
        if (p.y+1 < MapHeight && mapState[p.y+1][p.x] == TILE_DIRT && map[p.y+1][p.x] == -1) {
            que.emplace(p.x, p.y+1);
            map[p.y+1][p.x] = -2;
        }
        if (p.x+1 < MapWidth && mapState[p.y][p.x+1] == TILE_DIRT && map[p.y][p.x+1] == -1) {
            que.emplace(p.x+1, p.y);
            map[p.y][p.x+1] = -2;
        }
    }
    return map;
}

void PlayScene::ReadNotes(int songID) {
    std::string filename = std::string("Resource/song") + std::to_string(songID);
    float note_info[lanes];
    float start_time;
    std::ifstream fin(filename);
    while (fin >> note_info[0] >> note_info[1] >> note_info[2] >> note_info[3] >> start_time) {
        for (int i = 0; i < lanes; i++){
            if (note_info[i] > 0.95f && note_info[i] < 1.05f) {
                notes.emplace_back(Note(i, start_time, &red, &blue,false,10));
            }
            else if (note_info[i] != 1.f && note_info[i] != 0.0f) {
                notes.emplace_back(Note(i, start_time, &red, &blue,true,100*(note_info[i] - 1)));
            }
        }
    }
}

void PlayScene::OnKeyUp(int keyCode) {
    int lane = -1;
    for (int i = 0; i < lanes; ++i) {
        if (lane_key[i] == keyCode) {
            lane = i;
            last_up_time[lane] = conductor.song_position;
        }
    }
    if (lane == -1) {
        return;
    }

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        float addtime = (float)n->height_in_pixel / 700.0;
        float t = conductor.song_position - n->start_time * conductor.length_per_beat_in_seconds - 1 - addtime;
        if (n->lane == lane && n->is_hold && n->is_active) {
            if (t > -0.1 && t < 0.1) {
                n = notes.erase(n);
                --n;
                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[lane] = Judgement::perfect;
                    EarnMoney(5);
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[lane] = Judgement::good;
                    is_all_perfect= false;
                    EarnMoney(2);
                }
                for (auto i : TowerGroup->GetObjects()) {
                    dynamic_cast<Turret *>(i)->TriggerByHit();
                }
                ++combo;
                last_up_time[lane] = conductor.song_position;
                break;
            }
        }
    }
}