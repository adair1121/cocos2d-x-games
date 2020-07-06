//
//  RecordLayer.hpp
//  warrior-mobile
//
//  Created by 志强 on 2020/6/12.
//

#ifndef RecordLayer_hpp
#define RecordLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class RecordLayer : public cocos2d::Node{
private:
    Layout*rect;
    Layout*contentGroup;
    Text*time1lab;
    Text*time2lab;
    Text*time3lab;
    Sprite*closeBtn;
    EventListenerTouchOneByOne *_listener;
public:
    RecordLayer();
    ~RecordLayer();
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* RecordLayer_hpp */
