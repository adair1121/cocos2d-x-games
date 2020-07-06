//
//  GlobalData.h
//  wuxiaCocosProject
//
//  Created by 志强 on 2020/3/30.
//

#ifndef GlobalData_h
#define GlobalData_h
#define GD() GlobalData::getInstance()

#include "cocos2d.h"

USING_NS_CC;

class GlobalData {
private:
    const int bagmaxnum = 300;
public:
    static GlobalData* getInstance();
    bool soundBool = true;
    bool musicBool = true;
    bool gamePause = false;
    bool gameTouch = true;
};



#endif /* GlobalData_h */
