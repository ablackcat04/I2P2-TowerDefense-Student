#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <sstream>

#include "UI/Component/ImageButton.hpp"
#include "Engine/LOG.hpp"
#include "PlayScene.hpp"
#include "PlotScene.hpp"

void splitLine(const std::string& line, std::vector<std::string>& words) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
}

void PlotScene::Initialize() {
    big_font = al_load_font("Resource/fonts/Cubic11.ttf", 60, 0);
    font = al_load_font("Resource/fonts/Cubic11.ttf", 44, 0);
    name_font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 48, 0);
    default_name_color = new ALLEGRO_COLOR (al_map_rgb(220, 220, 255));
    current_text_color = new ALLEGRO_COLOR (al_map_rgb(255,255,255));

    while (!queue_of_text.empty()) {
        queue_of_text.pop();
    }
    text_sfx = al_load_sample("Resource/audios/slide.ogg");
    text_sfx_id = nullptr;

    history_info.clear();
    history_ptr = 0;

    history = false;
    prev_history = false;

    music_map.clear();
    image_map.clear();
    name_color_map.clear();

    text_target = "";
    name = "";
    middle_text = "";
    partial_text = "";
    partial_middle_text = "";

    Engine::ImageButton* btn;

    btn = new Engine::ImageButton("plot/plot-bg.png", "plot/plot-bg.png", 0, 0, 1600, 832);
    btn->SetOnClickCallback([this]{OnClickCallBack();});
    AddNewControlObject(btn);

    // Read plot from file
    std::string line;
    std::ifstream plot_file_stream(plot_path);

    // Pre Processing
    while (std::getline(plot_file_stream, line)) {
        Engine::LOG(Engine::INFO) << "Plot: " << line;

        if (line == "Plot_Start:") {
            break;
        } else if (line == "") {
            continue;
        }

        std::vector<std::string> words;
        splitLine(line, words);


        // check syntex
        if (words[0] == "image" && words.size() == 6) {
            if (words[2][0] != '"' || words[2][words[2].size()-1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error at 90";
                ChangeScene();
            } else {
                words[2].erase(0,1);
                words[2].erase(words[2].size()-1, 1);
            }
            image_info i = {nullptr, words[2], atoi(words[4].c_str()), atoi(words[5].c_str())};
            i.SetDftImage();
            image_map.emplace(words[1], i);
        } else if (words[0] == "audio" && words.size() == 3) {
            if (words[2][0] != '"' || words[2][words[2].size()-1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error at 101";
                ChangeScene();
            } else {
                words[2].erase(0,1);
                words[2].erase(words[2].size()-1, 1);
            }
            std::string fullpath = "Resource/audios/";
            fullpath += words[2];
            auto sam = al_load_sample(fullpath.c_str());
            if (sam == nullptr) {
                Engine::LOG(Engine::ERROR) << "Plot Audio Load Failed 111";
            }
            audio_info a = {sam, 0};
            music_map.emplace(words[1], a);
        } else if (words[0] == "color" && words.size() == 5) {
            name_color_map.emplace(words[1], new ALLEGRO_COLOR (al_map_rgb(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()))));
        } else {
            Engine::LOG(Engine::ERROR) << "Plot Pre-Processing Syntax Error 116";
            ChangeScene();
        }
    }
    // Reading Script
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
            } else if (words.size() >= 3 && words[2] != "at") {
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

            if (words[0] == "NULL") {
                words[0] = "";
            }
            out.push_back(words[0]);

            if (line[0] != '"' || line[line.size() - 1] != '"') {
                Engine::LOG(Engine::ERROR) << "Plot Script Syntax Error 172";
            } else {
                line.erase(0, 1);
                line.erase(line.size() - 1, 1);
            }
            out.push_back(line);
        }
        queue_of_text.emplace(out);
    }
    OnClickCallBack();

    for (auto i : image_map) {
        AddRefObject(*i.second.img);
    }

    btn = new Engine::ImageButton("stage-select/arrow_left.png", "stage-select/arrow_left_hovered.png", 1500, 9, 32, 32);

    btn->SetOnClickCallback([this] { ChangeScene(); });
    AddNewControlObject(btn);

    bg_history = new Engine::RefImage(transparent, 100, 100, 1400, 632, 0.0, 0.0);
    AddRefObject(*bg_history);

    history_title = "";
    history_label = new Engine::Label(&history_title, "BoutiqueBitmap7x7_1.7.ttf", 48, 120, 120, 255, 255, 255, 255, 0.0, 0.0);
    AddRefObject(*history_label);

    for (int i = 0; i < 8; ++i) {
        history_name[i] = "";
        history_text[i] = "";
        history_name_label[i] = new Engine::Label(&history_name[i], "BoutiqueBitmap7x7_1.7.ttf", 48, 160, 200+65*i, 180, 180, 200, 255, 0.0);
        history_text_label[i] = new Engine::Label(&history_text[i], "Cubic11.ttf", 44, 400, 200+65*i - 2, 180, 180, 180, 255, 0.0);
        AddRefObject(*history_name_label[i]);
        AddRefObject(*history_text_label[i]);
    }

    auto* t = new Engine::ToggledTextButton("auto", &auto_mode, 1400, 9, al_map_rgb(255,255,255), al_map_rgb(180,180,220),
                                            al_map_rgb(200,200,255));
    AddRefControlObject(*t);

    plot_file_stream.close();

    time = 0.0f;
    auto_timer = 0.0f;
}

void PlotScene::OnKeyDown(int keyCode) {
    if (keyCode == ALLEGRO_KEY_ENTER) {
        OnClickCallBack();
    }
}


void PlotScene::Draw() const {
    IScene::Draw();
    if (!history) {
        if (partial_middle_text != "") {

            int ptr = 0;
            std::string str = "";

            while (ptr < partial_middle_text.size() && al_get_text_width(big_font, str.c_str()) <= 1250) {
                str += partial_middle_text[ptr++];
            }
            al_draw_text(big_font, *current_text_color, 150, 200, 0, str.c_str());

            int lines = 0;

            while (ptr < partial_middle_text.size()) {
                str = "";
                ++lines;
                while (ptr < partial_middle_text.size() && al_get_text_width(big_font, str.c_str()) <= 1250) {
                    str += partial_middle_text[ptr++];
                }
                al_draw_text(big_font, *current_text_color, 150, 200+64*lines, 0, str.c_str());
            }
        } else {
            if (!name_color_map.contains(name)) {
                al_draw_text(name_font, *default_name_color, 150, 575, 0.5, name.c_str());
            } else {
                al_draw_text(name_font, *(name_color_map.find(name)->second), 150, 575, 0.5, name.c_str());
            }

            int ptr = 0;
            std::string str = "";

            while (ptr < partial_text.size() && al_get_text_width(font, str.c_str()) <= 1100) {
                str += partial_text[ptr++];
            }
            al_draw_text(font, *current_text_color, 250, 635, 0, str.c_str());

            int lines = 0;

            while (ptr < partial_text.size()) {
                str = "";
                ++lines;
                while (ptr < partial_text.size() && al_get_text_width(font, str.c_str()) <= 1100) {
                    str += partial_text[ptr++];
                }
                al_draw_text(font, *current_text_color, 250, 635+50*lines, 0, str.c_str());
            }
        }
    }
}

void PlotScene::OnClickCallBack() {
    if (history) {
        return;
    }

    if (partial_text != text_target) {
        partial_text = text_target;
        return;
    } else if (partial_middle_text != middle_text) {
        partial_middle_text = middle_text;
        return;
    }

    while (!queue_of_text.empty()) {
        auto words = queue_of_text.front();
        if (words[0] == "show") {
            image_map[words[1]].img->ChangeImageTo(image_map[words[1]].path, atoi(words[3].c_str()), atoi(words[4].c_str()));
        } else if (words[0] == "hide") {
            image_map[words[1]].img->ChangeImageTo(transparent, 0, 0);
        } else if (words[0] == "play") {
            al_play_sample(music_map[words[1]].sample, 0.5f, 0.0f, 1.0f, ALLEGRO_PLAYMODE::ALLEGRO_PLAYMODE_ONCE, &music_map[words[1]].id);
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
        auto temp = queue_of_text.front();
        if (temp[0] != "middle") {
            name = temp[0];
            text_target = temp[1];
            middle_text = "";
            partial_text = "";
            partial_middle_text = "";
            partial_target = "";

            int ptr = 0;

            while (ptr < text_target.size() && al_get_text_width(font, partial_target.c_str()) <= 1000) {
                partial_target += text_target[ptr++];
            }
            history_info.push_back({name, partial_target});

            int lines = 1;

            while (ptr < text_target.size()) {
                partial_target = "";
                ++lines;
                while (ptr < text_target.size() && al_get_text_width(font, partial_target.c_str()) <= 1000) {
                    partial_target += text_target[ptr++];
                }
                history_info.push_back({"", partial_target});
                if (lines == 4) {
                    Engine::LOG(Engine::ERROR) << "Script maybe too long, cannot be fully presented";
                }
            }
        } else {
            name = "";
            text_target = "";
            middle_text = temp[1];
            partial_text = "";
            partial_middle_text = "";
            partial_target = "";

            int ptr = 0;

            while (ptr < middle_text.size() && al_get_text_width(font, partial_target.c_str()) <= 1000) {
                partial_target += middle_text[ptr++];
            }
            history_info.push_back({"", partial_target});

            int lines = 1;

            while (ptr < middle_text.size()) {
                partial_target = "";
                ++lines;
                while (ptr < middle_text.size() && al_get_text_width(font, partial_target.c_str()) <= 1000) {
                    partial_target += middle_text[ptr++];
                }
                history_info.push_back({"", partial_target});
                if (lines == 8) {
                    Engine::LOG(Engine::ERROR) << "Script maybe too long, cannot be fully presented";
                }
            }
        }
        queue_of_text.pop();
    } else {
        ChangeScene();
    }
}

void PlotScene::SetPlotPathTo(std::string path) {
    plot_path = path;
}

void PlotScene::Update(float deltaTime) {
    if (history != prev_history) {
        if (!history) {
            bg_history->ChangeImageTo(transparent, 100, 100);
            history_title = "";
        } else {
            bg_history->ChangeImageTo(gray, 100, 100);
            history_title = "history:";
        }
        prev_history = history;
    }
    time += deltaTime;
    if (time > 0.04) {
        time -= 0.04;
        if (text_target == "" && middle_text == "") {
            OnClickCallBack();
            return;
        }
        if (text_target != partial_text) {
            if (text_sfx_id != nullptr) {
                al_stop_sample(text_sfx_id);
            }
            al_play_sample(text_sfx, 0.3f, 0.0f, 1.0f, ALLEGRO_PLAYMODE::ALLEGRO_PLAYMODE_ONCE, text_sfx_id);
            partial_text += text_target[partial_text.size()];
            while (text_target[partial_text.size()] == ' ') {
                partial_text += ' ';
            }
        } else if (middle_text != partial_middle_text) {
            if (text_sfx_id != nullptr) {
                al_stop_sample(text_sfx_id);
            }
            al_play_sample(text_sfx, 0.3f, 0.0f, 1.0f, ALLEGRO_PLAYMODE::ALLEGRO_PLAYMODE_ONCE, text_sfx_id);
            partial_middle_text += middle_text[partial_middle_text.size()];
            if (middle_text[partial_middle_text.size()] == ' ') {
                partial_middle_text += ' ';
            }
        } else {
            if (auto_mode) {
                auto_timer += deltaTime;
                if (auto_timer >= 1.0f) {
                    OnClickCallBack();
                    auto_timer = 0.0f;
                }
            }
        }
    }
}

void PlotScene::Terminate() {
    al_stop_samples();
    al_destroy_sample(text_sfx);
    for (auto i : music_map) {
        al_destroy_sample(i.second.sample);
    }

    music_map.clear();

    for (auto i : name_color_map) {
         delete i.second;
    }

    name_color_map.clear();

    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();

    while (!queue_of_text.empty()) {
        queue_of_text.pop();
    }
}

void PlotScene::OnMouseScroll(int mx, int my, int delta) {
    if (delta > 0) {
        Engine::LOG(Engine::INFO) << "mouse scroll up, history_ptr=" << history_ptr;
        if (!history) {
            history = true;
            history_ptr = history_info.size() - 8;
            if (history_ptr < 0) {
                history_ptr = 0;
            }
        } else {
            if (history_ptr > 0) {
                --history_ptr;
            }
        }
        int count = 0;
        for (; history_ptr + count < history_info.size() && count < 8; ++count) {
            history_name[count] = history_info[history_ptr + count].first;
            history_text[count] = history_info[history_ptr + count].second;
        }
        for (; count < 8; ++count) {
            history_name[count] = "";
            history_text[count] = "";
        }
    } else if (delta < 0) {
        Engine::LOG(Engine::INFO) << "mouse scroll down, history_ptr=" << history_ptr;
        if (history) {
            if (history_ptr + 8 >= history_info.size()) {
                history = false;
                for (int i = 0; i < 8; ++i) {
                    history_name[i] = "";
                    history_text[i] = "";
                }
            } else {
                if (history_ptr < history_info.size()-1) {
                    ++history_ptr;
                }
                int count = 0;
                for (; history_ptr + count < history_info.size() && count < 8; ++count) {
                    history_name[count] = history_info[history_ptr + count].first;
                    history_text[count] = history_info[history_ptr + count].second;
                }
                for (; count < 8; ++count) {
                    history_name[count] = "";
                    history_text[count] = "";
                }
            }
        } else {
            OnClickCallBack();
        }
    }
}

void PlotScene::SetNextSceneTo(std::string scene_name) {
    next_scene = scene_name;
}

void PlotScene::ChangeScene() {
    if (next_scene == "play-scene") {
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play-scene"));
        scene->MapId = stage;
        SetLastStage(stage);
        next_scene = "map-scene";
        Engine::GameEngine::GetInstance().ChangeScene("play-scene");
    } else {
        Engine::GameEngine::GetInstance().ChangeScene(next_scene);
    }
}