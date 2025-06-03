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

    UserInput(bool backspace = false, bool ent = false, bool vert = true,
        bool del = false, std::string key = "", float scrolled = 0.0f) :
        backSpace{ backspace },
        enter{ ent },
        verticle{ vert },
        del{del},
        keyPressed{ key },
        scroll{scrolled}
    {};
};

