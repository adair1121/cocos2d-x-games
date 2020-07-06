//
//  GameScene.hpp
//  Castleguard-mobile
//
//  Created by 志强 on 2020/6/3.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ModelEntity.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Sprite*bg;
    Layout*gameGroup;
    Layout*topGroup;
    LoadingBar*bar;
    Text*timeLab;
    Sprite*topwer;
    Layout*operGroup;
    Sprite*skill1;
    Sprite*skill2;
    Sprite*skill3;
    Sprite*focus;
    Layout*hpGroup;
    Layout*resultGroup;
    Sprite*resetBtn;
    Text*ownScoreLab;
    Text*bestScoreLab;
    EventListenerTouchOneByOne *_listener;
    
    int hp = 100;
    int thp = 100;
    int timecount = 0;
    int bestcount = 0;
    bool guideBoo = true;
    bool hitShow = false;
    int skillIndex = -1;
    ModelEntity*guideMc;
    MovieClip*handMc;
    int guideCount = 1;
    int createCount = 5;
    float tickTimeCon = 0.0f;
    Vector<ModelEntity*> entitys;
public:
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void resetData();
    
    void createEntity(float dt);
    
    void ticktime(float dt);
    
    void gameEnd();
};

#endif /* GameScene_hpp */
