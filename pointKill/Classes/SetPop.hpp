//
//  SetPop.hpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/29.
//

#ifndef SetPop_hpp
#define SetPop_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class SetPop : public cocos2d::Layer {
public:
    std::function<void()> callback;
private:
    Layout*bg;
    Layout*contentGroup;
    Layout*mainBtn;
    Layout*audioBtn;
    Sprite*closeBtn;
    EventListenerTouchOneByOne *_listener;
public:
    SetPop(int state);
    ~SetPop();
private:
    void adapter();
    
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
};

#endif /* SetPop_hpp */
