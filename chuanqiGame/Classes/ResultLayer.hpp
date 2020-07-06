//
//  ResultLayer.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#ifndef ResultLayer_hpp
#define ResultLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocos2d::ui;

class ResultLayer : public cocos2d::Layer{
private:
    Layout*winGroup;
    Layout*failGroup;
    Layout*rect;
    Sprite*returnBtn;
    Sprite*sureBtn;
    Text*goldNumLab;
    EventListenerTouchOneByOne *_listener;
public:
    ResultLayer(int state);
    ~ResultLayer();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ResultLayer_hpp */
