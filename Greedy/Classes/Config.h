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
    
    int gold = 0;
    
    bool lock1 = true;
    
    bool lock2 = true;
    
    int selectIndex = 1;

};

#endif /* Config_h */
