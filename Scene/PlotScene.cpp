//
// Created by wuru8 on 2024/6/8.
//

#include "PlotScene.hpp"

#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <sstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "PlayScene.hpp"
#include "Engine/LOG.hpp"

void splitLine(const std::string& line, std::vector<std::string>& words) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
}

void PlotScene::Initialize() {
    history_info.clear();
    history_ptr = 0;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    history = false;
    prev_history = false;

    music_map.clear();
    image_map.clear();

    text_target = "";
    name = "";
    middle_text = "";
    partial_text = "";
    partial_text1 = "";
    partial_text2 = "";
    partial_text3 = "";
    partial_text4 = "";

    auto onClickCallback = [this]{
        if (partial_text != text_target) {
            partial_text = text_target;
            partial_text1 = partial_target1;
            partial_text2 = partial_target2;
            partial_text3 = partial_target3;
            partial_text4 = partial_target4;
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
                al_play_sample(music_map[words[1]].sample, 0.5, 0.5, 1.0, ALLEGRO_PLAYMODE::ALLEGRO_PLAYMODE_ONCE, &music_map[words[1]].id);
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
                partial_text1 = "";
                partial_text2 = "";
                partial_text3 = "";
                partial_text4 = "";
                partial_middle_text = "";

                whole_words.clear();
                splitLine(text_target, whole_words);
                partial_target1 = "";
                partial_target2 = "";
                partial_target3 = "";
                partial_target4 = "";
                int count = 0;
                for (;count < whole_words.size() && partial_target1.size() + whole_words[count].size() <= 50; ++count) {
                    partial_target1 += whole_words[count];
                    partial_target1 += ' ';
                }
                for (;count < whole_words.size() && partial_target2.size() + whole_words[count].size() <= 50; ++count) {
                    partial_target2 += whole_words[count];
                    partial_target2 += ' ';
                }
                for (;count < whole_words.size() && partial_target3.size() + whole_words[count].size() <= 50; ++count) {
                    partial_target3 += whole_words[count];
                    partial_target3 += ' ';
                }
                for (;count < whole_words.size() && partial_target4.size() + whole_words[count].size() <= 50; ++count) {
                    partial_target4 += whole_words[count];
                    partial_target4 += ' ';
                }

                if (count != whole_words.size()) {
                    Engine::LOG(Engine::ERROR) << "Script too long, cannot be fully presented";
                }

                history_info.push_back({name, partial_target1});
                if (partial_target2 != "") {
                    history_info.push_back({"", partial_target2});
                    if (partial_target3 != "") {
                        history_info.push_back({"", partial_target3});
                        if (partial_target4 != "") {
                            history_info.push_back({"", partial_target4});
                        }
                    }
                }
            } else {
                name = "";
                text_target = "";
                middle_text = temp[1];
                partial_text = "";
                partial_text1 = "";
                partial_text2 = "";
                partial_text3 = "";
                partial_text4 = "";
                partial_middle_text = "";

                history_info.push_back({"", middle_text});
            }
            queue_of_text.pop();
        } else {
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
        }
    };

    Engine::ImageButton* btn;

    btn = new Engine::ImageButton("plot/black.png", "plot/black.png", 0, 0, 1600, 832);
    btn->SetOnClickCallback(onClickCallback);
    AddNewControlObject(btn);

    // Read plot from file
    std::string line;
    std::ifstream plot("Resource/plot/test.txt");

    // Pre Processing
    while (std::getline(plot, line)) {
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
                Engine::GameEngine::GetInstance().ChangeScene("stage-select");
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
                Engine::GameEngine::GetInstance().ChangeScene("stage-select");
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
        } else {
            Engine::LOG(Engine::ERROR) << "Plot Pre-Processing Syntax Error 116";
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
        }
    }
    // Reading Script
    while (std::getline(plot, line)) {
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
    onClickCallback();

    for (auto i : image_map) {
        AddRefObject(*i.second.img);
    }

    pText1 = new Engine::Label(&partial_text1, "BoutiqueBitmap7x7_1.7.ttf", 48, 250, 635, 255, 255, 255, 220, 0.5, 0.5);
    AddRefObject(*pText1);
    pText2 = new Engine::Label(&partial_text2, "BoutiqueBitmap7x7_1.7.ttf", 48, 250, 685, 255, 255, 255, 220, 0.5, 0.5);
    AddRefObject(*pText2);
    pText3 = new Engine::Label(&partial_text3, "BoutiqueBitmap7x7_1.7.ttf", 48, 250, 735, 255, 255, 255, 220, 0.5, 0.5);
    AddRefObject(*pText3);
    pText4 = new Engine::Label(&partial_text4, "BoutiqueBitmap7x7_1.7.ttf", 48, 250, 785, 255, 255, 255, 220, 0.5, 0.5);
    AddRefObject(*pText4);

    pName = new Engine::Label(&name, "BoutiqueBitmap7x7_1.7.ttf", 48, 150, 575, 220, 220, 255, 220, 0.5, 0.5);
    AddRefObject(*pName);

    pMiddleText = new Engine::Label(&partial_middle_text, "BoutiqueBitmap7x7_1.7.ttf", 56, 150, 200, 255, 255, 255, 255, 0.5, 0.5);
    AddRefObject(*pMiddleText);

    btn = new Engine::ImageButton("stage-select/arrow_left.png", "stage-select/arrow_left_hovered.png", 1350, 20, 64, 64);
    btn->SetOnClickCallback([] { Engine::GameEngine::GetInstance().ChangeScene("stage-select"); });
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
        history_text_label[i] = new Engine::Label(&history_text[i], "BoutiqueBitmap7x7_1.7.ttf", 48, 400, 200+65*i, 180, 180, 180, 255, 0.0);
        AddRefObject(*history_name_label[i]);
        AddRefObject(*history_text_label[i]);
    }

    plot.close();

    time = 0.0f;

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    //bgmInstance = AudioHelper::PlaySample("BeyondSunshine.ogg", true, AudioHelper::BGMVolume);
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
        if (text_target != partial_text) {
            partial_text += text_target[partial_text.size()];
            while (text_target[partial_text.size()] == ' ') {
                partial_text += ' ';
            }

            if (partial_text1 != partial_target1) {
                partial_text1 += partial_target1[partial_text1.size()];
                while (partial_target1[partial_text1.size()] == ' ') {
                    partial_text1 += ' ';
                }
            } else if (partial_text2 != partial_target2) {
                partial_text2 += partial_target2[partial_text2.size()];
                while (partial_target2[partial_text2.size()] == ' ') {
                    partial_text2 += ' ';
                }
            } else if (partial_text3 != partial_target3) {
                partial_text3 += partial_target3[partial_text3.size()];
                while (partial_target3[partial_text3.size()] == ' ') {
                    partial_text3 += ' ';
                }
            } else {
                partial_text4 += partial_target4[partial_text4.size()];
                while (partial_target4[partial_text4.size()] == ' ') {
                    partial_text4 += ' ';
                }
            }
        } else if (middle_text != partial_middle_text) {
            partial_middle_text += middle_text[partial_middle_text.size()];
            if (middle_text[partial_middle_text.size()] == ' ') {
                partial_middle_text += ' ';
            }
        }
    }
}

void PlotScene::Terminate() {
    //AudioHelper::StopSample(bgmInstance);
    al_stop_samples();
    for (auto i : music_map) {
        al_destroy_sample(i.second.sample);
    }
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
        }
    }
}