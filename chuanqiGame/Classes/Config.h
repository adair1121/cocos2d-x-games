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
#include "SkillCfg.hpp"

USING_NS_CC;
class Config {
    
public:
    static Config *getInstance();
    
    int c_chapter = 1;
    
    int t_chapter = 1;
    
    int level = 1;
    
    int sex = 2;
    
    std::string namestr;
    
    int gold = 200;
    
    int totalHp = 100;
    
    int curHp = 100;
    
    Vector<SkillCfg*> skillCfgs;

};

#endif /* Config_h */
