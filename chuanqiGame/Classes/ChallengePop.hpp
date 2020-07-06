//
//  ChallengePop.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#ifndef ChallengePop_hpp
#define ChallengePop_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocos2d::ui;

class ChallengePop : public cocos2d::Layer{
    
private:
    Layout*rect;
    Layout*contentGroup;
    Sprite*closeBtn;
    ScrollView*scroller;
    EventListenerTouchOneByOne *_listener;
    Layer*continerLayer;
public:
    ChallengePop();
    ~ChallengePop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ChallengePop_hpp */
