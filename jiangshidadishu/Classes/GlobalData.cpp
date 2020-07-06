//
//  GlobalData.cpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/30.
//

#include "GlobalData.h"

USING_NS_CC;

using namespace std;
//using json = nlohmann::json;
static GlobalData *s_pInstance = NULL; // pointer to singleton
GlobalData* GlobalData::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new GlobalData();
    }
    return s_pInstance;
}
