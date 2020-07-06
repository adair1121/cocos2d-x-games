//
//  GameScene.hpp
//  chuanqiStory-mobile
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

class GameScene : public cocos2d::Layer {
private:
    int curMp = 0;
    int curHp = 0;
    int totalMp = 0;
    int totalHp = 0;
    int challengeCout = 0;
    int curChallengeCout = 0;
    Vector<Sprite*> energys;
    int skillIndex = 1;

    MovieClip*roleMc;
    MovieClip*collectMc;
    Size visiblity;
    bool battleEnd = false;
private:
    Sprite*bg;
    Layout*mpGroup;
    Layout*hpGroup;
    Sprite*tower;
    Sprite*pauseBtn;
    Sprite*collectBtn;
    Button*skill1;
    Button*skill2;
    Button*skill3;
    Button*skill4;
    Sprite*guide1;
    Sprite*guide2;
    Sprite*guide3;
    Sprite*guide4;
    Layout*pauseGroup;
    Layout*bottomGroup;
    Layout*gameGroup;
    Text*iceNumLab;
    Text*lightNumLab;
    EventListenerTouchOneByOne *_listener;
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
    
    void adapter();
    
    void resetData();
    
    void createEnergy(Vec2 pos);
    
    void startGame();
    
    void stopGame();
    
    void updateTime(float dt);
    void operMp(int value);
    void operHp(int value);
    void ticktime(float dt);
    void createMon(float dt);
    void changeSkillTab();
    
    void floatFont(std::string cnt);
    
    void resultCallBack(int oper);
};

#endif /* GameScene_h */
