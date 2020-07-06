//
//  ResultPop.h
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/25.
//

#ifndef ResultPop_h
#define ResultPop_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MovieClip.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ResultPop : public cocos2d::Layer{
private:
    Layout*winGroup;
    Text*pointLab;
    Sprite*resetBtn;
    Sprite*nextBtn;
    Layout*failGroup;
    Sprite*againBtn;
    Sprite*homeBtn;
    Layout*bg;
    EventListenerTouchOneByOne *_listener;
public:
    std::function<void(int oper)>  callFunc;
public:
    ResultPop(int state);
    ~ResultPop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ResultPop_h */
