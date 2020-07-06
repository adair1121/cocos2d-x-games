//
//  SelectLevel.h
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/22.
//

#ifndef SelectLevel_h
#define SelectLevel_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
class SelectLevel : public cocos2d::Layer {
private:
    Sprite*bg;
    Layout*levelGroup;
    Layout*bottomGroup;
    EventListenerTouchOneByOne *_listener;
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
    
    void floatFont(std::string cnt);
};

#endif /* SelectLevel_h */
