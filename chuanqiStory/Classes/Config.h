//
//  Config.h
//  Puzzle
//
//  Created by 志强 on 2020/5/19.
//

#ifndef Config_h
#define Config_h

#define CF() Config::getInstance()

#include "cocos2d.h"

USING_NS_CC;
class Config {
    
public:
    static Config *getInstance();
    //拥有的点数
    int point = 0;
    //当前过的关数
    int level = 1;
    //当前挑战的关卡
    int challengeLev = 0;
    //当前雷电书数量
    int lightNum = 0;
    //当前冰冻书数量
    int iceNum = 0;
    //当前血包数量
    int hpNum = 1;
    //当前攻击方式1的等级
    int way1Level = 1;
    //当前攻击方式2等级
    int way2Level = 0;
    //当前蓝量等级
    int mpLevel = 1;

};

#endif /* Config_h */
