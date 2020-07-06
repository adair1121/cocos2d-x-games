//
//  SelectLevel.hpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/27.
//

#ifndef SelectLevel_hpp
#define SelectLevel_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class SelectLevel : public cocos2d::Layer{
private:
    Sprite*bg;
    Layout*levelGroup;
    Sprite*shopBtn;
    Layout*goldPanel;
    TextBMFont*goldLab;
    Sprite*line;
    EventListenerTouchOneByOne *_listener;
    bool topPanel;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(SelectLevel);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void popCall();
};

#endif /* SelectLevel_hpp */
