//
//  ResultScene.h
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/16.
//

#ifndef ResultScene_h
#define ResultScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ResultScene : public cocos2d::Layer{
private:
    TextBMFont*goldLab;
    Sprite*returnBtn;
    Sprite*bg;
    Layout*contentGroup;
    Sprite*resultBg;
    EventListenerTouchOneByOne *_listener;
public:
    ResultScene();
    ~ResultScene();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ResultScene_h */
