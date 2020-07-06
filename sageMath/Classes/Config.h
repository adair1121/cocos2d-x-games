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
    
    int curChallengeLev = 1;
    
    int curMapLev = 1;
    
    int map1Level = 1;
    int map2Level = 1;
    int map3Level = 1;
    
    int enterMap = 1;
    
    bool showMapView;
    
    int totalLev = 10;
    
    std::vector<std::string> stars1;
    std::vector<std::string> stars2;
    std::vector<std::string> stars3;
};

#endif /* Config_h */
