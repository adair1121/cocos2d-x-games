//
//  GameScene.h
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/18.
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
    int totalHp = 30;
    int totalMp = 30;
    int curHp = 30;
    int curMp = 30;
    int gold = 0;
    int hpnum = 0;
    int mpnum = 0;
    int gemnum = 0;
    int meganum = 0;
    int shieldnum = 0;
    int timecount = 0;
    int levelCount = 3;
    int promptatk = 0;
    int promptdef = 0;
    MovieClip*roleMc;
    bool battleEnd = false;
    Vector<MovieClip*> _enemys;
    bool topShow;
    int curExp = 0;
private:
    Text*goldNum;
    Sprite*setBtn;
    Sprite*bottombg;
    LoadingBar*hpBar;
    LoadingBar*mpBar;
    Sprite*useHp;
    Sprite*useMp;
    Sprite*useGem;
    Sprite*uesMega;
    Sprite*useShield;
    Sprite*buyHp;
    Sprite*buyMp;
    Sprite*buyGem;
    Sprite*buyMega;
    Sprite*buySheld;
    Text*hpNum;
    Text*mpNum;
    Text*gemNum;
    Text*megaNum;
    Text*shieldNum;
    Layout*topPanel;
    Layout*bottomGroup;
    Layout*gameGroup;
    Layout*otherGroup;
    Sprite*map;
    Sprite*expBg;
    LoadingBar*expBar;
    EventListenerTouchOneByOne *_listener;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    void onExit();
    
    void adapter();
    
    void refreshPageData();
    
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void floatFont(std::string cnt);
    
    void promptAtk();
    
    void promptDef();
    
    void atkAll();
    
    void createEnemy();
    
    void updateEnemy(float dt);
    
    void reduceHp();
    
    void resetData();
    
    void shakeNode(cocos2d::Node *node);
    
    void topcallback(int state);
    
    void setCall();
    CREATE_FUNC(GameScene);
};

#endif /* GameScene_h */
