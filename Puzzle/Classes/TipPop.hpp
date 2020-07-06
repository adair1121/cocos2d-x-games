//
//  TipPop.hpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/28.
//

#ifndef TipPop_hpp
#define TipPop_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class TipPop : public cocos2d::Layer{
private:
    Layout*bg;
    Layout*content;
    Sprite*sureBtn;
    EventListenerTouchOneByOne *_listener;
public:
    TipPop();
    ~TipPop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};
#endif /* TipPop_hpp */
