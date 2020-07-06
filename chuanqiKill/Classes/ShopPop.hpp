//
//  ShopPop.hpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/28.
//

#ifndef ShopPop_hpp
#define ShopPop_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ShopPop : public cocos2d::Layer {
    
private:
    Sprite*bg;
    Layout*contentGroup;
    Sprite*closeBtn;
    Sprite*buy1;
    Sprite*buy2;
    Sprite*buy3;
    EventListenerTouchOneByOne *_listener;
public:
    std::function<void()> touchBuyCall;
public:
    ShopPop();
    ~ShopPop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void floatFont(std::string cnt);
    
    void touchBuy(int index);
};

#endif /* ShopPop_hpp */
