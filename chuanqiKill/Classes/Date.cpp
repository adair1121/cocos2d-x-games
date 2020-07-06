//
//  Date.cpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/18.
//

#include "Date.h"

static Date *s_pInstance = NULL; // pointer to singleton



Date* Date::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new Date();
    }

    return s_pInstance;
}
