//
//  GameScene.hpp
//  warrior-mobile
//
//  Created by 志强 on 2020/6/9.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Layout*guidePanel;
    Sprite*hp1;
    Sprite*hp2;
    Sprite*hp3;
    Node*roleNode;
    Sprite*leftArrow;
    Sprite*rightArrow;
    Layout*gameCon;
    Sprite*bg;
    Sprite*bloodPic;
    Layout*resultPanel;
    Layout*topGroup;
    Text*timeLab;
    EventListenerTouchOneByOne *_listener;
    cocostudio::timeline::ActionTimeline * roleAction;
    bool gameStart;
    Size visibleSize;
    bool atkState;
    int hp = 3;
    Node*rootNode;
    int killnum = 0;
    int timecount = 0;
    float count = 0.0f;
    Point birthP;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void createEneity(float dt);
    
    void judgeAtk();
    
    void shakeNode(cocos2d::Node *node);
    
    void refreshHp();
    
    void resetData();
};

#endif /* GameScene_hpp */
