#pragma once
#include <string>

struct UserInput
{
    bool backSpace;
    bool enter;
    bool verticle;
    std::string keyPressed;
    float scroll;

    UserInput(bool backspace = false, bool ent = false, bool vert = true,
        std::string key = "", float scrolled = 0.0f) :
        backSpace{ backspace },
        enter{ ent },
        verticle{ vert },
        keyPressed{ key },
        scroll{scrolled}
    {};
};

