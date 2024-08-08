//
// Created by wuru8 on 2024/5/11.
//
#include <allegro5/allegro.h>
#include "Engine/IObject.hpp"
#include "UI/Component/Label.hpp"
#include "Scoreboard.hpp"
#include "Utility/TimeHelper.hpp"

int upper;

namespace Engine {

    void Scoreboard::Draw() const {
        if (!show) {
            return;
        }
        //draw score

        if (num_of_mem < max_line - 1) {
            upper = num_of_mem;
        } else {
            upper = max_line-1;
        }

        while (upper + ptr >= num_of_mem) {
            upper--;
        }

        for (int j = 0; j < upper + 1; j++) {
            char temp[100];

            if (j + ptr + 1 >= 10) {
                sprintf_s(temp, 100, "%d", j + ptr + 1);
            } else {
                sprintf_s(temp, 100, " %d", j + ptr + 1);
            }

            al_draw_text(
                    menuFont,
                    al_map_rgb(r, g, b),
                    x,
                    y + 50 * j,
                    ALLEGRO_ALIGN_LEFT,
                    temp
            );

            al_draw_text(
                    menuFont,
                    al_map_rgb(r, g, b),
                    x + 75,
                    y + 50 * j,
                    ALLEGRO_ALIGN_LEFT,
                    scb[j + ptr].name
            );

            sprintf_s(temp, 100, "%d", scb[j + ptr].score);

            al_draw_text(
                    menuFont,
                    al_map_rgb(r, g, b),
                    x+700,
                    y + 50 * j,
                    ALLEGRO_ALIGN_RIGHT,
                    temp
            );

            al_draw_text(
                    menuFont,
                    al_map_rgb(r, g, b),
                    x+800,
                    y + 50 * j,
                    ALLEGRO_ALIGN_LEFT,
                    scb[j + ptr].time
            );
        }
    }

    void Scoreboard::AddNew(const std::string& username, int points) {
        FILE* score;

        if (remove(path.c_str()) == 0)
        {
            Engine::LOG(Engine::INFO) << path << (" deleted successfully");
        }
        else
        {
            Engine::LOG(Engine::ERROR) << ("Fail to delete ") << path;
        }


        if (fopen_s(&score, path.c_str(), "w+") != 0)
        {
            Engine::LOG(Engine::ERROR) << ("Fail to open new ScoreBoard");
        }
        else
        {
            Engine::LOG(Engine::INFO) << ("ScoreBoard File create Successfully");
        }

        bool flag = true;

        for (int j = 0; j < num_of_mem; j++)
        {
            if (points > scb[j].score && flag)
            {
                fprintf_s(score, "%s %d %s\n", username.c_str(), points, GetCurrentTime());
                Engine::LOG(Engine::INFO) << username << points;
                flag = false;
                fprintf_s(score, "%s %d %s\n", scb[j].name, scb[j].score, scb[j].time);
                Engine::LOG(Engine::INFO) << j;
                continue;
            }

            fprintf_s(score, "%s %d %s\n", scb[j].name, scb[j].score, scb[j].time);
        }

        if (flag)
        {
            fprintf_s(score, "%s %d %s\n", username.c_str(), points, GetCurrentTime());
        }

        ++num_of_mem;

        fclose(score);
    }

    void Scoreboard::OnMouseScroll(int mx, int my, int delta) {
        if (!show) {
            return;
        } else if (delta > 0) {
            PtrDec();
        } else if (delta < 0) {
            PtrInc();
        }
    }

    void Scoreboard::PtrInc() {
        ++ptr;
        if (ptr >= num_of_mem - 1) {
            ptr = num_of_mem - 1;
        }
    }

    void Scoreboard::PtrDec() {
        --ptr;
        if (ptr < 0) {
            ptr = 0;
        }
    }

    void Scoreboard::NextPage() {
        if (ptr + max_line < num_of_mem) {
            ptr += max_line;
        }
    }

    void Scoreboard::PrevPage() {
        ptr -= max_line;
        if (ptr < 0) {
            ptr = 0;
        }
    }

    void Scoreboard::MakeShow() {
        show = true;
    }

    void Scoreboard::MakeNotShow() {
        show = false;
    }
}