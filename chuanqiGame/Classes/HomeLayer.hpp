//
//  HomeLayer.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef HomeLayer_hpp
#define HomeLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "RoleTitleLayer.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class HomeLayer : public cocos2d::Layer{
    
private:
    Sprite*bg;
    Layout*navGroup;
    Sprite*challengeBtn;
    Sprite*shopBtn;
    Layout*popLayout;
    Layout*contentGroup;
    EventListenerTouchOneByOne *_listener;
    RoleTitleLayer*roleTitle;
    Size visibleSize;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HomeLayer);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* HomeLayer_hpp */
