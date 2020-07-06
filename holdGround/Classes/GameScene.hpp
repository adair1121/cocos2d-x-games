//
//  GameScene.hpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer {
private:
    Sprite*bg;
    Node*roleNode;
    Sprite*grass;
    Sprite*homeImg;
    Sprite*returnBtn;
    Sprite*skillIcon;
    Sprite*scoreIcon;
    TextBMFont*scoreLab;
    TextBMFont*timeLab;
    TextBMFont*cdLab;
    Button*soundBtn;
    LoadingBar*proBar;
    Sprite*proBg;
    
    EventListenerTouchOneByOne *_listener;
    Node*rootNode;
    int allTime = 600;
    int score = 0;
    int cdTime = 10;
    //创建实体的时间
    float createTime = 0.1;
    float curTime = 0.0;
    bool touchDown = false;
    float hp = 100.0f;
    float angle;
    Point posP;
    Size visibleSize;
    cocostudio::timeline::ActionTimeline * roleAction;
public:
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    void menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void refreshTimeShow();
    
    void gameEnd();
    
    void countDownTime(float dt);
    
    void archery(float dt);
    
    void createEnemy(float dt);
    
};

#endif /* GameScene_hpp */
