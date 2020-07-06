//
//  ResultScene.hpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#ifndef ResultScene_hpp
#define ResultScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ResultScene : public cocos2d::Layer{
    
private:
    Sprite*bg;
    Sprite*scoreIcon;
    TextBMFont*scoreLab;
    Sprite*returnBtn;
    Sprite*playBtn;
    Sprite*tip1;
    Sprite*tip2;
    Sprite*tip3;
    Sprite*dmgBtn;
    Sprite*towerBtn;
    Sprite*hpBtn;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(ResultScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
          
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void adapter();
    
    void floatFont(std::string cnt);
};
#endif /* ResultScene_hpp */
