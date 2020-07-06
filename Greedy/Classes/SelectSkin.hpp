//
//  SelectSkin.hpp
//  Greedy-mobile
//
//  Created by 志强 on 2020/6/12.
//

#ifndef SelectSkin_hpp
#define SelectSkin_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class SelectSkin : public cocos2d::Layer{
private:
    Sprite*bg;
    Sprite*icon1;
    Sprite*icon1_focus;
    Layout*goldGroup;
    Text*goldLab;
    Layout*skin2Group;
    Layout*skin3Group;
    Sprite*playBtn;
    int selectIndex = 1;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(SelectSkin);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void refreshPage();
    
    void changeTab();
};

#endif /* SelectSkin_hpp */
