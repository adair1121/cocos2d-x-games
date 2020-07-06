#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SoldierEntity.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class HelloWorld : public cocos2d::Layer
{
private:
    int hp = 3;
    int energe = 100;
    int tenerge = 100;
    int atkNum = 3;
    int shieldNum = 3;
    int headNum = 3;
    int score = 0;
    SoldierEntity*soldier;
    Size visiblity;
    Point circleP;
    bool touchCircle = false;
    int _normalRadius = 60;
    Sprite*startBg;
    
    int direct = 0;
    bool execAction;
    bool wudi = false;
    int otherSpeed = 0;
    bool battleEnd = false;
private:
    Layout*bgGroup;
    Layout*gameGroup;
    Layout*bg;
    Layout*entityGroup;
    Sprite*startBtn;
    Sprite*atkIcon;
    Sprite*circle;
    Sprite*weaponIcon;
    Sprite*headIcon;
    Sprite*shiedIcon;
    TextBMFont*scoreImg;
    Sprite*resultBg;
    Sprite*roleimg;
    TextBMFont*scoreLab;
    Sprite*againBtn;
    EventListenerTouchOneByOne *_listener;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void updateEnergy(float dt);
    
    void actiontick(float dt);
    
    void createMon(float dt);
    
    void adapter();
    
    void refreshPageData();
    
    void resetData();
    
    void shakeNode(cocos2d::Node *node);
};

#endif // __HELLOWORLD_SCENE_H__
