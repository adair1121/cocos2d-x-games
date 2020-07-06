//
//  Config.h
//  Puzzle
//
//  Created by 志强 on 2020/5/19.
//

#ifndef Config_h
#define Config_h

#include "cocos2d.h"
#define CF() Config::getInstance()

USING_NS_CC;
class Config {
    
public:
    static Config *getInstance();
    
    int curLevel = 1;
    
    int level = 1;

};

#endif /* Config_h */
