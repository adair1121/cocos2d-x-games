//
//  ResultPop.h
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#ifndef ResultPop_h
#define ResultPop_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ResultPop : public cocos2d::Node{
public:
    std::function<void()>  backFunc;
    std::function<void()>  resetFunc;
private:
    Sprite*bg;
    Layout*failGroup;
    Button*backBtn;
    Button*retryBtn;
    
    Layout*winGroup;
    Button*retryBtn2;
    Button*nextBtn;
    EventListenerTouchOneByOne *_listener;
public:
    ResultPop(int state);
    ~ResultPop();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
           
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};
#endif /* ResultPop_h */
