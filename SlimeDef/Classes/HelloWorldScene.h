#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocos2d::ui;

class HelloWorld : public cocos2d::Layer
{
private:
    Sprite*bg;
    Layout*topGroup;
    LoadingBar*energyBar;
    TextBMFont*killLab;
    TextBMFont*timeLab;
    
    Layout*endGroup;
    Button*againBtn;
    TextBMFont*resultTimeLab;
    TextBMFont*resultKillLab;
    
    Layout*startGroup;
    Sprite*logo;
    Button*startBtn;
    
    Layout*gameGroup;
    Layout*topG;
    Layout*bottomG;
    Sprite*arrow;
    Layout*leftG;
    Layout*rightG;
    Layout*centerG;
    EventListenerTouchOneByOne *_listener;
    
    Sprite*heroSp;
    Point beginP;
    int countTime = 0;
    int totalEnergy = 100;
    int curEnerge = 0;
    int killCount = 0;
    Point birthP;
    int birthNum;
    Sprite*lightsp;
    bool battleEnd;
    bool startboo;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void startGame();
    
    void resetDate();
    
    void adapter();
    
    void changeHeroAction(std::string direct ,std::string action);
    
    void updatetime(float dt);
    
    void createEnemy();
    
    void releaseSkill();
};

#endif // __HELLOWORLD_SCENE_H__
