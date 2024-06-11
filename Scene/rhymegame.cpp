//
// Created by yuwei on 2024/6/11.
//
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "rhymegame.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 初始化 Allegro 库和所需的模块
bool init() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return false;
    }

    if (!al_install_audio()) {
        std::cerr << "Failed to initialize audio!" << std::endl;
        return false;
    }

    if (!al_init_acodec_addon()) {
        std::cerr << "Failed to initialize audio codecs!" << std::endl;
        return false;
    }

    if (!al_reserve_samples(1)) {
        std::cerr << "Failed to reserve samples!" << std::endl;
        return false;
    }

    if (!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return false;
    }

    if (!al_install_keyboard()) {
        std::cerr << "Failed to initialize keyboard!" << std::endl;
        return false;
    }

    return true;
}

void rhymescene::Initialize() {
    if (!init()) {
        return ;
    }

    // 创建窗口
    ALLEGRO_DISPLAY* display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return ;
    }

    // 创建计时器，设置更新频率为每秒 60 次
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    if (!timer) {
        std::cerr << "Failed to create timer!" << std::endl;
        return ;
    }

    // 创建事件队列
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "Failed to create event queue!" << std::endl;
        return ;
    }

    // 加载音频文件
    ALLEGRO_SAMPLE* sample = al_load_sample("your_music_file.ogg");
    if (!sample) {
        std::cerr << "Failed to load audio sample!" << std::endl;
        return ;
    }

    // 注册事件源
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // 创建 Conductor 对象
    Conductor conductor(120.0f, 0.1f);
    std::vector<Note> notes;  // 用于存储音符的向量
    std::srand(std::time(nullptr));  // 初始化随机数种子

    // 播放音乐
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);
    al_start_timer(timer);

    bool quit = false;
    while (!quit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            quit = true;
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            conductor.update();  // 更新 Conductor

            // 随机生成音符
            if (notes.size() < 10 && std::rand() % 100 < 2) {
                float startX = std::rand() % (WINDOW_WIDTH - 50);
                notes.emplace_back(startX);
            }

            // 更新音符位置
            for (auto& note : notes) {
                note.update();
            }

            // 清空屏幕
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // 绘制音符
            for (auto& note : notes) {
                note.render();
            }

            // 更新显示
            al_flip_display();
        }
    }

    // 销毁资源
    al_destroy_sample(sample);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return ;
}

void rhymescene::Terminate(){}

void rhymescene::Update(float deltaTime){}
void rhymescene::Draw() const{}