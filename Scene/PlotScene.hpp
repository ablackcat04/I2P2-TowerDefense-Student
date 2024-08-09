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

    Timer char_proceed_timer;

    bool history_mode_is_on;
    bool was_history_mode_on;
    bool is_auto_mode_on = false;

    Timer auto_timer;

    std::string history_title;

    Engine::Label *history_label;
    Engine::RefImage *bg_history;
    std::string history_name[MAX_LINE_SHOWN_HISTORY_MODE];
    Engine::Label *history_name_label[MAX_LINE_SHOWN_HISTORY_MODE];
    std::string history_text[MAX_LINE_SHOWN_HISTORY_MODE];
    Engine::Label *history_text_label[MAX_LINE_SHOWN_HISTORY_MODE];

    std::vector<std::pair<std::string, std::string>> history_info;
    int history_ptr;

    std::string plot_path = "Resource/plot/plot3-e.txt";

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

public:
    explicit PlotScene() = default;
    void Initialize() override;
    void Terminate() override;
    static void BackOnClick(int stage);
    void Update(float deltaTime) override;
    void OnMouseScroll(int mx, int my, int delta);
    void OnKeyDown(int keyCode) override;
    void Draw() const override;
    void AttemptPlotProceed();
    void SetPlotPathTo(std::string path);
    void SetNextSceneTo(std::string scene_name);
    void ChangeScene();

    void CleanAudio();
    void CleanColor();
    void CleanPlotInQue();

    void OnMouseScrollUp();
    void OnMouseScrollDown();
    void UpdateHistoryInfo();

    void CleanPlotEngine();

    void ResetVariables();

    void LoadResources();
    void FreeResources();

    void PreProcessScriptAndLoadAssets(std::ifstream &plot_file_stream);

    void InitPartOfUI();

    void ProcessScript(std::ifstream &plot_file_stream);

    void splitLine(const std::string& line, std::vector<std::string>& words);

    void UpdateHistoryBackground();

    bool LineReachesEnd() const;

    void AttemptCharProceed();

    void ReplayTextSFX() const;

    void UpdateTimer(float deltaTime);

    void ImageAudioPlotProceed();

    void UpdateCurrentNameColor();

    void GoToEndOfLine();

    void LoadNextText();

    void PushCurrentTextToHistory();
};

#endif //INC_2024_I2P2_TOWERDEFENSE_PLOTSCENE_HPP
