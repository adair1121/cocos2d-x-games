//
//  SelectLevel.hpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/30.
//

#ifndef SelectLevel_hpp
#define SelectLevel_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class SelectLevel : public cocos2d::Layer {
public:
    std::function<void(int state)> callback;
private:
    Layout*bg;
    Layout*contentGroup;
    Sprite*closeBtn;
    Sprite*rightArrow;
    Sprite*leftArrow;
    ScrollView*scroller;
    Layout*con;
    EventListenerTouchOneByOne *_listener;
    Point beginP;
public:
    SelectLevel();
    ~SelectLevel();
private:
    void adapter();
    
    void floatFont(std::string cnt);
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
};

#endif /* SelectLevel_hpp */
