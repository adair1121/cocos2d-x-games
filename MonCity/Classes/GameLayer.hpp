//
//  GameLayer.hpp
//  MonCity-mobile
//
//  Created by 志强 on 2020/6/16.
//

#ifndef GameLayer_hpp
#define GameLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameLayer : public cocos2d::Layer{
private:
    Sprite*map;
    Layout*topGroup;
    LoadingBar*loadBar;
    TextBMFont*levelLab;
    
    Layout*bottomGroup;
    Button*roleBtn;
    Button*partnerBtn;
    Button*skillBtn;
    Text*dmgLab;
    Text*dpsLab;
    Text*goldLab;
    ScrollView*scroller;
    Layer*continerLayer;
    Sprite*returnBtn;
    
    Node*enemyNode;
    Node*mainGeneral;
    cocostudio::timeline::ActionTimeline *mainAct;
    Node*general2;
    cocostudio::timeline::ActionTimeline *general2Act;
    Node*general3;
    cocostudio::timeline::ActionTimeline *general3Act;
    Node*general4;
    cocostudio::timeline::ActionTimeline *general4Act;
    Node*general5;
    cocostudio::timeline::ActionTimeline *general5Act;
    EventListenerTouchOneByOne *_listener;
    bool mainAtk;
    int tabIndex = 1;
    int curLevel = 1;
    int totalLevel = 1;
    int curHp;
    int totalHp;
    int skillTime = 0;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void changeTab();
    
    void calculAttr();
    
    void createSkill(int skillIndex);
    
    void showDamage(int dmg);
    
    void nextLevel();
    
    void secondAtk(float dt);
};

#endif /* GameLayer_hpp */
