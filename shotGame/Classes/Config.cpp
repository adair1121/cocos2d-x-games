//
//  Config.cpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/19.
//

#include "Config.h"

static Config *s_pInstance = NULL; // pointer to singleton

Config* Config::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new Config();
    }

    return s_pInstance;
}
