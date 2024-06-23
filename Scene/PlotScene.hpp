//
// Created by wuru8 on 2024/6/8.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Image.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include "UI/Component/RefImage.hpp"
#include "PlayScene.hpp"
#include "WinScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"

void UpdateText(std::queue<std::list<std::string>>& queue_of_text, std::string& text, std::string& name);

class PlotScene final : public Engine::IScene {
private:
    struct image_info {
        std::shared_ptr<Engine::RefImage> img;
        std::string path;
        int w;
        int h;

        void SetDftImage(){
            img = std::make_shared<Engine::RefImage>("plot/transparent.png",0, 0, w, h, 0, 0);
        }
    };

    struct audio_info {
        ALLEGRO_SAMPLE* sample;
        ALLEGRO_SAMPLE_ID id;
    };

    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    //Engine::Label *pMiddleText = nullptr;
    Engine::Label *pName = nullptr;
    std::string text_target;
    std::string partial_target;
    std::string text1;
    std::string middle_text;
    std::string name;

    std::string partial_text;
    std::string partial_middle_text;

    std::vector<std::string> whole_words;

    std::unordered_map<std::string, image_info> image_map;
    std::unordered_map<std::string, audio_info> music_map;

    std::queue<std::vector<std::string>> queue_of_text;

    const std::string transparent = "plot/transparent.png";
    const std::string gray = "plot/gray.png";

    float time;

    bool history;
    bool prev_history;
    bool auto_mode = false;
    float auto_timer;

    std::string history_title;

    Engine::Label *history_label;
    Engine::RefImage *bg_history;
    std::string history_name[8];
    Engine::Label *history_name_label[8];
    std::string history_text[8];
    Engine::Label *history_text_label[8];

    std::vector<std::pair<std::string, std::string>> history_info;
    int history_ptr;

    std::string plot_path = "Resource/plot/plot3-e.txt";

    std::string next_scene = "stage-select";

    ALLEGRO_SAMPLE* text_sfx;
    ALLEGRO_SAMPLE_ID* text_sfx_id;

    ALLEGRO_FONT* name_font;
    ALLEGRO_COLOR* deafult_name_color;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* big_font;
    ALLEGRO_COLOR* current_text_color;

    std::unordered_map<std::string, ALLEGRO_COLOR*> name_color_map;

public:
    explicit PlotScene() = default;
    void Initialize() override;
    void Terminate() override;
    static void SkipOnClick();
    static void AutoOnClick();
    static void BackOnClick(int stage);
    void Update(float deltaTime) override;
    void OnMouseScroll(int mx, int my, int delta);
    void OnKeyDown(int keyCode) override;
    void Draw() const override;

    void OnClickCallBack();

    void SetPlotPathTo(std::string path);

    void SetNextSceneTo(std::string scene_name);

    bool GoToPlayNext = false;
    int stage;

    void ChangeScene();
};


#endif //INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP
