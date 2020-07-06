//
//  GameScene.hpp
//  CastleguardV2-mobile
//
//  Created by 志强 on 2020/6/15.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ModelEntity.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Sprite*bg;
    Layout*gameCon;
    Sprite*enemyRect;
    Sprite*ownRect;
    Sprite*enemy_tower;
    Sprite*own_tower;
    Sprite*returnBtn;
    Layout*topGroup;
    Text*ownScore;
    Text*enemyScore;
    Sprite*hpbg;
    LoadingBar*hpBar;
    Sprite*hero1;
    Sprite*hero2;
    Sprite*hero3;
    EventListenerTouchOneByOne *_listener;
    Sprite*helpImg;
    Sprite*hpShow;
    Layout*resultPanel;
    Sprite*continueBtn;
    Text*scoreLab;
    Sprite*scoreFont;
    Sprite*loseicon;
    
    int hp = 100;
    bool firstboo = true;
    int createTime = 5;
    int singleCount = 0;
    Size visibleSize;
    bool clickCard;
    ModelEntity*dragEntity;
    int score = 0;
    float timeCount = 0.0f;
    int emenyscore = 0;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
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
    
    void createEntity(float dt);
    
    void update(float dt);
    
    void clickCardFunc(int type);
    
    void createSkill(int type,Vec2 pos);

};

#endif /* GameScene_hpp */
