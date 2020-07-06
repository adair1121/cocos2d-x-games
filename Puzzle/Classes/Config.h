//
//  Config.h
//  Puzzle
//
//  Created by 志强 on 2020/5/19.
//

#ifndef Config_h
#define Config_h

#include "cocos2d.h"

USING_NS_CC;
class Config {
    
public:
    static Config *getInstance();
    
    int level;
    
    int row;
    
    int col;
    
    int allNum;
    
    int picIndex;
    
    int curLevel;
    
    int openLevel = 1;

};

#endif /* Config_h */
