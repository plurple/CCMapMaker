#pragma once
#include <string>

struct UserInput
{
    bool backSpace;
    bool enter;
    bool verticle;
    bool del;
    std::string keyPressed;
    float scroll;
    bool right, left, up, down;
    bool shift, tab;

    UserInput() :
        backSpace{ false },
        enter{ false },
        verticle{ false },
        del{false},
        keyPressed{ "" },
        scroll{0.0f},
        right{false},
        left{false},
        up{false},
        down{false},
        shift{false},
        tab{false}
    {};
};

