//
//  ResultPop.h
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/18.
//

#ifndef ResultPop_h
#define ResultPop_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;
class ResultPop : public cocos2d::Layer{
private:
    EventListenerTouchOneByOne *_listener;
    Sprite*mainBtn;
    Sprite*resetBtn;
    Layout*failGroup;
    
    Layout*winGroup;
    Sprite*mainBtn2;
    Sprite*nextBtn;
    
    Layout*bg;
public:
    std::function<void()> resetFun;
public:
    ResultPop(int state);
    ~ResultPop();
private:
    void adapter();
    
    void floatFont(std::string cnt);
    
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
};

#endif /* ResultPop_h */
