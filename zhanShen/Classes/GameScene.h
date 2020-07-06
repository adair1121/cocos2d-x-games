//
//  GameScene.hpp
//  zhanShen-mobile
//
//  Created by 志强 on 2020/5/22.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MovieClip.h"
USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    MovieClip*roleMc;
    int timecount = 5*60;
    int level = 1;
    int hp = 5;
    int score = 0;
    Size visiblity;
private:
    Sprite*bg;
    Layout*gameGroup;
    Sprite*topbg;
    Button*audioBtn;
    TextBMFont*scoreLab;
    Layout*bottombg;
    
    Layout*bottomPanel;
    TextBMFont*timeLab;
    TextBMFont*waveLab;
    TextBMFont*hpLab;
    TextBMFont*goldLab;
    Sprite*boneIcon;
    
    Layout*overBg;
    Layout*contentGroup;
    Sprite*returnBtn;
    Layout*continueBtn;
    EventListenerTouchOneByOne *_listener;
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
    
    void startGame(float dt);
    void ticktime(float dt);
    void showLevelAdd();
    void adapter();
    void stopGame();
    void floatFont(std::string cnt);
};

#endif /* GameScene_hpp */
