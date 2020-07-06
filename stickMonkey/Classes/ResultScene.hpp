//
//  ResultScene.hpp
//  stickMonkey-mobile
//
//  Created by 志强 on 2020/6/5.
//

#ifndef ResultScene_hpp
#define ResultScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class ResultScene : public cocos2d::Layer {
private:
    Sprite*bg;
    Sprite*img;
    Sprite*light;
    TextBMFont*scoreLab;
    Sprite*scoreBg;
    TextBMFont*bestScore;
    Button*resetBtn;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(ResultScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
};

#endif /* ResultScene_hpp */
