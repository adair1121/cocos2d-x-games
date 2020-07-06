//
//  ResultScene.h
//  shotGame-mobile
//
//  Created by 志强 on 2020/5/20.
//

#ifndef ResultScene_h
#define ResultScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DeviceUtils.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ResultScene : public cocos2d::Layer{
private:
    Button*resetBtn;
    TextBMFont*scoreLab;
    Sprite*bg;
    Sprite*scorebg;
    EventListenerTouchOneByOne *_listener;
public:
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();

    // implement the "static create()" method manually
    CREATE_FUNC(ResultScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);

    void adapter();
};

#endif /* ResultScene_h */
