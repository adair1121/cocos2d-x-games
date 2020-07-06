//
//  HelpInfoTip.hpp
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#ifndef HelpInfoTip_h
#define HelpInfoTip_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class HelpInfoTip : public cocos2d::Layer{
public:
    std::function<void()>  callFunc;
private:
    EventListenerTouchOneByOne *_listener;
    Sprite*infoPop;
    
public:
    HelpInfoTip();
    ~HelpInfoTip();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
};
#endif /* HelpInfoTip_h */
