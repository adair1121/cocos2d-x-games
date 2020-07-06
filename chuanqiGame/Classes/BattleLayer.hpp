//
//  BattleLayer.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#ifndef BattleLayer_hpp
#define BattleLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "RoleTitleLayer.hpp"


USING_NS_CC;

using namespace cocos2d::ui;

class BattleLayer : public cocos2d::Layer{
private:
    Sprite*bg;
    Layout*gameCon;
    Node*roleNode;
    cocostudio::timeline::ActionTimeline * roleAction;
    Sprite*leftArrow;
    Sprite*rightArrow;
    TextBMFont*killLab;
    Sprite*blood;
    RoleTitleLayer*roleTitle;
    bool rolePlay;
    Size visibleSize;
    EventListenerTouchOneByOne *_listener;
    int killnum = 0;
    bool gameState;
    int releaseIndex = 1;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(BattleLayer);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void createEneity(float dt);
    
    void createSkillIcon(float dt);
    
    void judgeAtk(bool skillAtk);
    
    void shakeNode(cocos2d::Node *node);
        
    void gameEnd();
};

#endif /* BattleLayer_hpp */
