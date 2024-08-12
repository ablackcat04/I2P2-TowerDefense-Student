#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <sstream>

#include "UI/Component/ImageButton.hpp"
#include "Engine/LOG.hpp"
#include "PlayScene.hpp"
#include "PlotScene.hpp"

void PlotScene::Initialize() {
    text_sfx = al_load_sample("Resource/audios/slide.ogg");
    big_font = al_load_font("Resource/fonts/Cubic11.ttf", 60, 0);
    font = al_load_font("Resource/fonts/Cubic11.ttf", 44, 0);
    name_font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 48, 0);

    default_name_color = new ALLEGRO_COLOR (al_map_rgb(220, 220, 255));
    current_name_color = default_name_color;
    current_text_color = new ALLEGRO_COLOR (al_map_rgb(255, 255, 255));

    history_ptr = 0;
    history_mode_is_on = false;

    text_target = "";
    name = "";
    middle_text_target = "";
    text = "";
    middle_text = "";

    history_title = "";
    for (int i = 0; i < 8; ++i) {
        history_name[i] = "";
        history_text[i] = "";
    }
    text_sfx_id = nullptr;

    char_proceed_timer.Initialize(CHAR_PROCEED_TIME);
    auto_timer.Initialize(AUTO_MODE_PROCEED_TIME);

    InitializeUI();         // we do this before initializing the plot engine because we use a black picture to capture
                            // mouse inputs
    InitializePlotEngine();
    InitializeHistoryUI();  // we do this after initializing the plot engine because we want the history UI before image

    AttemptPlotProceed();
}

void PlotScene::InitializeUI() {
    Engine::ImageButton* btn;

    btn = new Engine::ImageButton("plot/plot-bg.png", "plot/plot-bg.png", 0, 0, 1600, 832);
    btn->SetOnClickCallback([this]{ AttemptPlotProceed(); });
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("stage-select/arrow_left.png", "stage-select/arrow_left_hovered.png",
                                  1500, 9, 32, 32);
    btn->SetOnClickCallback([this] { ChangeScene(); });
    AddNewControlObject(btn);

    auto* t = new Engine::ToggledTextButton("auto", &auto_mode_is_on, 1400, 9,
                                            al_map_rgb(255, 255, 255),
                                            al_map_rgb(180, 180, 220),
                                            al_map_rgb(200,200,255));
    AddRefControlObject(*t);
}

void PlotScene::InitializePlotEngine() {
    al_stop_samples();
    for (auto i : music_map) {
        al_destroy_sample(i.second.sample);
    }
    music_map.clear();
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    for (auto i : name_color_map) {
        delete i.second;
    }
    name_color_map.clear();

    while (!queue_of_text.empty()) {
        queue_of_text.pop();
    }

    image_map.clear();
    history_info.clear();

    std::ifstream plot_file_stream(plot_path);

    ProcessScriptAndLoadAssets(plot_file_stream);

    plot_file_stream.close();
}

void PlotScene::InitializeHistoryUI() {
    bg_history = new Engine::RefImage(transparent, 100, 100, 1400, 632, 0.0, 0.0);
    AddRefObject(*bg_history);

    history_label = new Engine::Label(&history_title, "BoutiqueBitmap7x7_1.7.ttf",
                                      48, 120, 120, 255, 255, 255, 255, 0.0, 0.0);
    AddRefObject(*history_label);

    for (int i = 0; i < MAX_LINE_SHOWN_HISTORY_MODE; ++i) {
        history_name_label[i] = new Engine::Label(&history_name[i], "BoutiqueBitmap7x7_1.7.ttf",
                                                  48, 160, 200 + 65 * i, 180, 180, 200, 255, 0.0);
        AddRefObject(*history_name_label[i]);

        history_text_label[i] = new Engine::Label(&history_text[i], "Cubic11.ttf",
                                                  44, 400, 200 + 65 * i - 2, 180, 180, 180, 255, 0.0);
        AddRefObject(*history_text_label[i]);
    }
}

void PlotScene::ProcessScriptAndLoadAssets(std::ifstream &plot_file_stream) {
    // Pre Processing
    std::string line;

    while (std::getline(plot_file_stream, line)) {
        if (line == "") {
            continue;
        }
        Engine::LOG(Engine::INFO) << "Pre-Process: " << line;

        if (line == "Plot_Start:") {
            for (auto i : image_map) {
                AddRefObject(*i.second.img);
            }
            Engine::LOG(Engine::INFO) << "Plot Pre-Process done";
            break;
        }

        std::vector<std::string> words;
        splitLine(line, words);

        if (words[0] == "image" && words.size() == 6) {
            if (words[2][0] != '"' || words[2][words[2].size()-1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot_Engine Pre-Processing Syntax Error {image}, path not surround with \"\"";
            } else {
                words[2].erase(0,1);
                words[2].erase(words[2].size()-1, 1);
            }
            image_info i = {nullptr, words[2], atoi(words[4].c_str()), atoi(words[5].c_str())};
            i.SetDefaultImage();
            image_map.emplace(words[1], i);
        } else if (words[0] == "audio" && words.size() == 3) {
            if (words[2][0] != '"' || words[2][words[2].size()-1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot_Engine Pre-Processing Syntax Error {audio}, path not surround with \"\"";
            } else {
                words[2].erase(0,1);
                words[2].erase(words[2].size()-1, 1);
            }
            std::string full_path = "Resource/audios/";
            full_path += words[2];
            ALLEGRO_SAMPLE* sample_ptr = al_load_sample(full_path.c_str());
            if (sample_ptr == nullptr) {
                Engine::LOG(Engine::ERROR) << "Plot_Engine: Failed to load audio";
            }
            audio_info a = {sample_ptr, 0};
            music_map.emplace(words[1], a);
        } else if (words[0] == "color" && words.size() == 5) {
            name_color_map.emplace(words[1], new ALLEGRO_COLOR (al_map_rgb(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()))));
        } else {
            Engine::LOG(Engine::ERROR) << "Plot_Engine Pre-Processing: Unsupported command";
        }
    }
    // pre-processing ends

    while (std::getline(plot_file_stream, line)) {
        Engine::LOG(Engine::INFO) << "Plot: " << line;
        if (line == "") {
            continue;
        }

        std::vector<std::string> words;
        splitLine(line, words);

        std::vector<std::string> out;

        // check syntax
        if (words[0] == "show") {
            if (words.size() != 5) {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 135";
            } else if (words[2] != "at") {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 137";
            }
            for (auto i : words) {
                out.push_back(i);
            }
        } else if (words[0] == "hide") {
            if (words.size() != 2) {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 144";
            }
            for (auto i : words) {
                out.push_back(i);
            }
        } else if (words[0] == "play") {
            if (words.size() != 2) {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 151";
            }
            for (auto i : words) {
                out.push_back(i);
            }
        } else if (words[0] == "stop") {
            if (words.size() != 2) {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 158";
            }
            for (auto i : words) {
                out.push_back(i);
            }
        } else {
            line.erase(0, words[0].size() + 1);

            if (line[0] != '"' || line[line.size() - 1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 172";
            } else {
                line.erase(0, 1);
                line.erase(line.size() - 1, 1);
            }
            if (words[0] == "NULL") {
                words[0] = "";
            }
            out.push_back(words[0]);
            out.push_back(line);
        }
        queue_of_text.emplace(out);
    }
}

void PlotScene::splitLine(const std::string& line, std::vector<std::string>& words) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
}

void PlotScene::OnKeyDown(int keyCode) {
    if (keyCode == ALLEGRO_KEY_ENTER) {
        AttemptPlotProceed();
    }
}

void PlotScene::Draw() const {
    IScene::Draw();

    if (!history_mode_is_on) {
        al_draw_text(name_font, *current_name_color, 150, 575, 0.5, name.c_str());

        int ptr = 0;
        int lines = 0;
        while (ptr < middle_text.size()) {
            std::string str = "";
            while (ptr < middle_text.size() && al_get_text_width(big_font, str.c_str()) <= 1250) {
                str += middle_text[ptr++];
            }
            al_draw_text(big_font, *current_text_color, 150, 200 + 64*lines, 0, str.c_str());
            ++lines;
        }

        ptr = 0;
        lines = 0;
        while (ptr < text.size()) {
            std::string str = "";

            while (ptr < text.size() && al_get_text_width(font, str.c_str()) <= 1100) {
                str += text[ptr++];
            }
            al_draw_text(font, *current_text_color, 250, 635 + 50*lines, 0, str.c_str());
            ++lines;
        }
    }
    // we use different draw method between text and history mode, history mode is drawn in IScene::Draw
}

void PlotScene::AttemptPlotProceed() {
    if (history_mode_is_on) {
        return;
    } else if (!LineReachesEnd()) {
        text = text_target;
        middle_text = middle_text_target;
        return;
    }

    while (!queue_of_text.empty()) {
        auto words = queue_of_text.front();
        if (words[0] == "show") {
            image_map[words[1]].img->ChangeImageTo(image_map[words[1]].path, atoi(words[3].c_str()), atoi(words[4].c_str()));
        } else if (words[0] == "hide") {
            image_map[words[1]].img->ChangeImageTo(transparent, 0, 0);
        } else if (words[0] == "play") {
            al_play_sample(music_map[words[1]].sample, 0.5f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, &music_map[words[1]].id);
        } else if (words[0] == "stop") {
            auto temp = music_map[words[1]].id;
            if (&temp != nullptr) {
                al_stop_sample(&temp);
            }
        } else {
            break;
        }
        queue_of_text.pop();
    }

    if (!queue_of_text.empty()) {
        auto words = queue_of_text.front();
        queue_of_text.pop();

        if (words[0] != "middle") {
            name = words[0];
            text_target = words[1];
            middle_text_target = "";
            text = "";
            middle_text = "";
        } else {
            name = "";
            text_target = "";
            middle_text_target = words[1];
            text = "";
            middle_text = "";
        }

        if (!name_color_map.contains(name)) {
            current_name_color = default_name_color;
        } else {
            current_name_color = name_color_map.find(name)->second;
        }

        PushInfoToHistory(text_target != "" ? text_target : middle_text_target, name);
    } else {
        ChangeScene();
    }
}

void PlotScene::PushInfoToHistory(std::string text, std::string name) {
    std::string partial_text = "";
    bool is_first_time = true;
    for (int ptr = 0; ptr < text.size(); is_first_time = false) {
        partial_text = "";
        while (ptr < text.size() && al_get_text_width(font, partial_text.c_str()) <= 1000) {
            partial_text += text[ptr++];
        }
        history_info.push_back({is_first_time ? name : "", partial_text});
    }
}

void PlotScene::SetPlotPathTo(std::string path) {
    plot_path = path;
}

void PlotScene::Update(float deltaTime) {
    if (!LineReachesEnd()) {
        auto_timer.Reset();
        char_proceed_timer.Proceed(deltaTime);
    } else if (auto_mode_is_on) {
        auto_timer.Proceed(deltaTime);
    }

    if (char_proceed_timer.ReachLimit()) {
        char_proceed_timer.Reset();

        if (text_target != text) {
            text += text_target[text.size()];
            while (text_target[text.size()] == ' ') {
                text += ' ';
            }
        } else if (middle_text_target != middle_text) {
            middle_text += middle_text_target[middle_text.size()];
            while (middle_text_target[middle_text.size()] == ' ') {
                middle_text += ' ';
            }
        }

        if (text_sfx_id != nullptr) {
            al_stop_sample(text_sfx_id);
        }
        al_play_sample(text_sfx, 0.3f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, text_sfx_id);
    } else if (auto_timer.ReachLimit()) {
        auto_timer.Reset();
        AttemptPlotProceed();
    }
}

void PlotScene::Terminate() {
    al_destroy_sample(text_sfx);

    al_destroy_font(big_font);
    al_destroy_font(font);
    al_destroy_font(name_font);

    delete default_name_color;
    delete current_text_color;
    IScene::Terminate();
}

void PlotScene::OnMouseScroll(int mx, int my, int delta) {
    if (delta > 0) {
        if (!history_mode_is_on) {
            history_mode_is_on = true;
            history_ptr = history_info.size() - MAX_LINE_SHOWN_HISTORY_MODE;

            bg_history->ChangeImageTo(gray, 100, 100);
            history_title = "History:";
        } else {
            --history_ptr;
        }

        if (history_ptr < 0) {
            history_ptr = 0;
        }
    } else if (delta < 0) {
        if (!history_mode_is_on) {
            AttemptPlotProceed();
        } else {
            if (history_ptr + MAX_LINE_SHOWN_HISTORY_MODE >= history_info.size()) {
                history_mode_is_on = false;

                bg_history->ChangeImageTo(transparent, 100, 100);
                history_title = "";
            } else {
                ++history_ptr;
            }
        }
    }
    UpdateHistoryInfo();
}

void PlotScene::UpdateHistoryInfo() {
    if (!history_mode_is_on) {
        for (int i = 0; i < MAX_LINE_SHOWN_HISTORY_MODE; ++i) {
            history_name[i] = "";
            history_text[i] = "";
        }
    } else {
        int count = 0;
        for (; history_ptr + count < history_info.size() && count < MAX_LINE_SHOWN_HISTORY_MODE; ++count) {
            history_name[count] = history_info[history_ptr + count].first;
            history_text[count] = history_info[history_ptr + count].second;
        }
        for (; count < MAX_LINE_SHOWN_HISTORY_MODE; ++count) {
            history_name[count] = "";
            history_text[count] = "";
        }
    }
}

void PlotScene::SetNextSceneTo(std::string scene_name) {
    next_scene = scene_name;
}

void PlotScene::ChangeScene() {
    Engine::GameEngine::GetInstance().ChangeScene(next_scene);
}

bool PlotScene::LineReachesEnd() const { return text_target == text && middle_text_target == middle_text; }
