//
//  ChallengeItem.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#ifndef ChallengeItem_hpp
#define ChallengeItem_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocos2d::ui;

class ChallengeItem : public cocos2d::Layer{
    
public:
    Layout*noActive;
    TextBMFont*levelLab;
    Sprite*challengeBtn;
    Sprite*levelbg;
public:
    ChallengeItem(int levelIndex);
    ~ChallengeItem();
    
};



#endif /* ChallengeItem_hpp */
