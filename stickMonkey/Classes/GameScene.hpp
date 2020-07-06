//
//  GameScene.hpp
//  stickMonkey-mobile
//
//  Created by 志强 on 2020/6/5.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Node*roleNode;
    cocostudio::timeline::ActionTimeline * roleAction;
    Sprite*bg;
    Sprite*scoreBg;
    TextBMFont*scoreLab;
    Layout*guideGroup;
    Size visibleSize;
    int bgNum = 1;
    Vector<Sprite*> pillars;
    int score = 0;
    Point normalPillarP;
    bool gameStart = false;
    Sprite*operSp;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void randomPillar();
    
    void ticktime(float dt);
    
    void gameWin();
};

#endif /* GameScene_hpp */
