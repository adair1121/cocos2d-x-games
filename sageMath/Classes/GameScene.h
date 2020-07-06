//
//  GameScene.h
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MovieClip.h"
#include "MonEntity.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    MovieClip*roleMc;
    MonEntity*enemy;
    int gameCount = 1;
    int hpcount = 3;
    bool battleEnd = false;
private:
    Sprite*bg;
    Button*hp1;
    Button*hp2;
    Button*hp3;
    Button*setBtn;
    Sprite*hpBg;
    Sprite*bottomBg;
    Layout*gameGroup;
    TextBMFont*lab1;
    TextBMFont*lab2;
    Layout*topPanel;
    TextBMFont*levelLab;
    Button*backBtn;
    
    Layout*operGrop;
    Sprite*add;
    Sprite*sub;
    Sprite*mul;
    Sprite*div;
    Layout*addRect;
    Layout*subRect;
    Layout*mulRect;
    Layout*divRect;
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
   
    void resetData();
   
    void adapter();
    
    void calculNum();
    
    void judgeCorrect(int correntNum,int endNum);
    
    void changeRoleAction(std::string actionstr);
    
    void gameWin();
    void gameFail();
    
    void exitGameScene();
};

#endif /* GameScene_h */
