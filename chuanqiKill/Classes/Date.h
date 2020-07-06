//
//  Date.h
//  ChuanqiKill
//
//  Created by 志强 on 2020/5/16.
//

#ifndef Date_h
#define Date_h

#include "cocos2d.h"
#define DT() Date::getInstance()

USING_NS_CC;
class Date {
    
public:
    static Date *getInstance();
    
    int gameScore = 0;
    
    int gold = 0;
    
    int level = 1;
    
    int curLevel = 1;
    
    int rewardGold = 0;
    
    bool battleState = false;
    
    int skill1num = 1;
    int skill2num = 1;
    int skill3num = 1;

};
#endif /* Date_h */
