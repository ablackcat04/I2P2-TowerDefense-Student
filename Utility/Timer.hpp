//
// Created by wuru8 on 2024/8/9.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_TIMER_HPP
#define INC_2024_I2P2_TOWERDEFENSE_TIMER_HPP


class Timer {
private:
    float time;
    float limit;
public:
    void Proceed(float delta_time);
    bool ReachLimit();
    void Reset();
    void Initialize(float limit_time);
};


#endif //INC_2024_I2P2_TOWERDEFENSE_TIMER_HPP
