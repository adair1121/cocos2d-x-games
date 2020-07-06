//
//  GameScene.hpp
//  shotGame-mobile
//
//  Created by 志强 on 2020/5/20.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DeviceUtils.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    TextBMFont*timeLab;
    TextBMFont*scoreLab;
    Layout*gameCon;
    Sprite*role;
    Sprite*timeBg;
    Sprite*scoreBg;
    Sprite*bg;
    EventListenerTouchOneByOne *_listener;
    int countTime = 30;
 
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void updateTime(float dt);
    
    void ticktime(float dt);
    void adapter();
};

#endif /* GameScene_h */
