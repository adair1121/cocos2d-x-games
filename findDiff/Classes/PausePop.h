//
//  PausePop.hpp
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#ifndef PausePop_h
#define PausePop_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PausePop :public cocos2d::Layer{
public:
    std::function<void()>  callFunc;
private:
    EventListenerTouchOneByOne *_listener;
    Sprite*playBtn;
public:
    PausePop();
    ~PausePop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
};
#endif /* PausePop_h */
