//
//  ShopPop.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef ShopPop_hpp
#define ShopPop_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SkillCfg.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class ShopPop : public cocos2d::Layer{
    
private:
    Layout*rect;
    Layout*contentGroup;
    Sprite*closeBtn;
    ScrollView*scroller;
    EventListenerTouchOneByOne *_listener;
    Vector<SkillCfg*> skillCfgs;
    Layer*continerLayer;
public:
    ShopPop();
    ~ShopPop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ShopPop_hpp */
