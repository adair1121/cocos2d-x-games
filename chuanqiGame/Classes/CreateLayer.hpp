//
//  CreateLayer.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef CreateLayer_hpp
#define CreateLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class CreateLayer : public cocos2d::Layer {
private:
    Sprite*bg;
    Sprite*manImg;
    Sprite*womanImg;
    Sprite*startBtn;
    Sprite*title;
    Layout*nameGroup;
    Text*nameLab;
    Sprite*randomBtn;
    Layout*womanGroup;
    Layout*manGroup;
    EventListenerTouchOneByOne *_listener;
    std::string namestr;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(CreateLayer);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void changeRole(int sex);
    
    std::string randomName();
};

#endif /* CreateLayer_hpp */
