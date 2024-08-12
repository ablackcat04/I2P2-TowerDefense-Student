#ifndef INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP

#include <memory>
#include <queue>
#include <vector>
#include <unordered_map>

#include <allegro5/allegro_audio.h>

#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/RefImage.hpp"
#include "PlayScene.hpp"
#include "WinScene.hpp"
#include "Utility/UsefulConstants.hpp"
#include "Utility/Timer.hpp"

#define MAX_LINE_SHOWN_HISTORY_MODE 8
#define CHAR_PROCEED_TIME 0.025
#define AUTO_MODE_PROCEED_TIME 2.0

class PlotScene final : public Engine::IScene {
private:
    struct image_info {
        std::shared_ptr<Engine::RefImage> img;
        std::string path;
        int w;
        int h;

        void SetDefaultImage() {
            img = std::make_shared<Engine::RefImage>("plot/transparent.png",0, 0, w, h, 0, 0);
        }
    };

    struct audio_info {
        ALLEGRO_SAMPLE* sample;
        ALLEGRO_SAMPLE_ID id;
    };

    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

    std::string text_target;
    std::string middle_text_target;
    std::string name;

    std::string text;
    std::string middle_text;

    std::unordered_map<std::string, image_info> image_map;
    std::unordered_map<std::string, audio_info> music_map;

    std::queue<std::vector<std::string>> queue_of_text;

    const std::string transparent = "plot/transparent.png";
    const std::string gray = "plot/gray.png";

    bool history_mode_is_on;
    bool auto_mode_is_on;

    Timer auto_timer;
    Timer char_proceed_timer;

    std::string history_title;

    Engine::Label *history_label;
    Engine::RefImage *history_background;
    std::string history_name[MAX_LINE_SHOWN_HISTORY_MODE];
    Engine::Label *history_name_label[MAX_LINE_SHOWN_HISTORY_MODE];
    std::string history_text[MAX_LINE_SHOWN_HISTORY_MODE];
    Engine::Label *history_text_label[MAX_LINE_SHOWN_HISTORY_MODE];

    std::vector<std::pair<std::string, std::string>> history_info;
    int history_ptr;

    std::string plot_path = "Resource/plot/Please-Set-Path-Before-Changing-To-Plot-Scene";

    std::string next_scene = SceneNames::select;

    ALLEGRO_SAMPLE* text_sfx;
    ALLEGRO_SAMPLE_ID* text_sfx_id;

    ALLEGRO_FONT* name_font;
    ALLEGRO_COLOR* default_name_color;
    ALLEGRO_COLOR* current_name_color;

    ALLEGRO_FONT* font;
    ALLEGRO_FONT* big_font;
    ALLEGRO_COLOR* current_text_color;

    std::unordered_map<std::string, ALLEGRO_COLOR*> name_color_map;

    void splitLine(const std::string& line, std::vector<std::string>& words);

    void InitializeUI();
    void InitializePlotEngine();
    void InitializeHistoryUI();

    void AttemptPlotProceed();

    bool LineReachesEnd() const;
    void PushInfoToHistory(std::string text, std::string name);

    void UpdateHistoryInfo();

    void ChangeScene();
public:
    explicit PlotScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnMouseScroll(int mx, int my, int delta);
    void OnKeyDown(int keyCode) override;
    void Draw() const override;

    void SetPlotPathTo(std::string path);
    void SetNextSceneTo(std::string scene_name);

    void CleanPlotEngine();
};

#endif //INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP
