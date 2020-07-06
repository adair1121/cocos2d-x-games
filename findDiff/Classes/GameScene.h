//
//  GameScene.h
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
class GameScene : public cocos2d::Layer
{
private:
    int hitCount = 3;
    int goodNum = 10;
    int levelIndex = 1;
    Text*hitCountLab;
    Sprite*bg;
    Sprite*soundIcon;
    Layout*group;
    Layout*goodCon;
    Layout*soundGroup;
    Layout*passGroup;
    Sprite*timeIcon;
    Sprite*nextLevelBtn;
    Sprite*gameOverBtn;
    EventListenerTouchOneByOne *_listener;
    bool hintBoo;
    Sprite*hintItem;
    int ansterNum = 0;
    bool nextState = false;
    bool resetState = false;
    int endTime = 60;
    int curCount = 0;
    Sprite*maskSp;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void startGame();
    
    void stopGame();
    
    void resetData();
    
    void reduceStar();
    
    void updateTime(float dt);
    
    void adapter();
    
    Sprite* maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite,Vec2 maskP);
};

#endif /* GameScene_h */
