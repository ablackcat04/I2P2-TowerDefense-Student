#ifndef INC_2024_I2P2_TOWERDEFENSE_SCOREBOARD_HPP
#define INC_2024_I2P2_TOWERDEFENSE_SCOREBOARD_HPP

#include <memory>
#include <string>
#include <utility>
#include <allegro5/bitmap.h>

#include "Engine/IObject.hpp"
#include "Engine/LOG.hpp"
#include "Engine/IControl.hpp"


struct ScoreBoardInstance {
    int score;
    char name[100];
    char time[30];
};

extern int upper;

namespace Engine {
    class Scoreboard : public IObject, public IControl {
    private:
        static const int max_line = 10;
        std::basic_string<char> path;

        int x;
        int y;

        int num_of_mem;

        int ptr;

        int n;

        int r;
        int g;
        int b;

        ALLEGRO_FONT* menuFont;

        bool show = true;

    public:
        ScoreBoardInstance* scb;

        Scoreboard(std::string _path, int _x, int _y, int _r, int _g, int _b) : path(std::move(_path)), x(_x), y(_y), num_of_mem(0), scb(), ptr(0), n(99), r(_r), g(_g), b(_b)
        {
            menuFont = al_load_font("Resource/fonts/pirulen.ttf", 30, 0);

            //load string
            FILE* score;

            scb = static_cast<ScoreBoardInstance *>(malloc(sizeof(ScoreBoardInstance) * n));

            while (n > 1 && scb == nullptr)
            {
                scb = static_cast<ScoreBoardInstance *>(malloc(sizeof(ScoreBoardInstance) * --n));
            }

            if (scb == nullptr)
            {
                Engine::LOG(Engine::ERROR) << "scoreboard memory alloc failed";
            }
            else
            {
                Engine::LOG(Engine::INFO) << "Scoreboard memory successfully alloc! " << n;
            }


            if (fopen_s(&score, path.c_str(), "r") != 0)
            {
                Engine::LOG(Engine::ERROR) << "Fail to open ScoreBoard: " << path;
            }
            else
            {
                Engine::LOG(Engine::INFO) << "ScoreBoard File Load Successfully";
            }


            for (; num_of_mem < n; num_of_mem++)
            {
                if (fscanf_s(score, "%s%d%s", scb[num_of_mem].name, sizeof(char) * 100, &scb[num_of_mem].score, scb[num_of_mem].time, sizeof(char) * 30) == 3)
                {
                    //Engine::LOG(Engine::INFO) << "scoreboard read:" << scb[num_of_mem].name << " " << scb[num_of_mem].score << " " << scb[num_of_mem].time;
                }
                else
                {
                    Engine::LOG(Engine::INFO) << "scoreboard ends";
                    break;
                }
            }

            fclose(score);
        }

        Scoreboard(std::string _path, int _x, int _y) : path(std::move(_path)), x(_x), y(_y), num_of_mem(0), scb(), ptr(0), n(99), r(255), g(255), b(255)
        {
            menuFont = al_load_font("Resource/fonts/pirulen.ttf", 30, 0);

            //load string
            FILE* score;

            scb = static_cast<ScoreBoardInstance *>(malloc(sizeof(ScoreBoardInstance) * n));

            while (n > 1 && scb == nullptr)
            {
                scb = static_cast<ScoreBoardInstance *>(malloc(sizeof(ScoreBoardInstance) * --n));
            }

            if (scb == nullptr)
            {
                Engine::LOG(Engine::ERROR) << "scoreboard memory alloc failed";
            }
            else
            {
                Engine::LOG(Engine::INFO) << "Scoreboard memory successfully alloc! " << n;
            }


            if (fopen_s(&score, path.c_str(), "r") != 0)
            {
                Engine::LOG(Engine::ERROR) << "Fail to open ScoreBoard: " << path;
            }
            else
            {
                Engine::LOG(Engine::INFO) << "ScoreBoard File Load Successfully";
            }


            for (; num_of_mem < n; num_of_mem++)
            {
                if (fscanf_s(score, "%s%d%s", scb[num_of_mem].name, sizeof(char) * 100, &scb[num_of_mem].score, scb[num_of_mem].time, sizeof(char) * 30) == 3)
                {
                    Engine::LOG(Engine::INFO) << "scoreboard read:" << scb[num_of_mem].name << " " << scb[num_of_mem].score << " " << scb[num_of_mem].time;
                }
                else
                {
                    Engine::LOG(Engine::INFO) << "scoreboard ends";
                    break;
                }
            }

            fclose(score);
        }

        ~Scoreboard() override
        {
            free(scb);
        }

        void Draw() const override;

        void AddNew(const std::string& username, int points);

        void PtrInc();
        void NextPage();
        void PtrDec();
        void PrevPage();

        void OnMouseScroll(int mx, int my, int delta) override;

        int get_num() const;

        void MakeShow();
        void MakeNotShow();
    };
}


#endif //INC_2024_I2P2_TOWERDEFENSE_SCOREBOARD_HPP