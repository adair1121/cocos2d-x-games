//
//  GameScene.hpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/19.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DeviceUtils.h"

USING_NS_CC;

using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer{
private:
    Sprite*bg;
    Sprite*closeBtn;
    Sprite*resetBtn;
    TextBMFont*timeLab;
    Button*previewBtn;
    Layout*frameGroup;
    Layout*frameGroup2;
    Layout*frameGroup3;
    
    Sprite*tipPic;
    Sprite*showImg;
    Layout*levelmap1;
    EventListenerTouchOneByOne *_listener;
    Vector<Layout*> items;
    bool startDrag = false;
    bool startClick = false;
    int timeCount = 0;
    Layout* dragItem;
    int orderIndex = 10;
    std::vector<std::string> connectBlock;
    bool battleEnd;
public:
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();
    
    void onEnter();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
public:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void startGame(float dt);
    
    void updateTime(float dt);
    
    void resetData();
    
    void refreshTimeShow();
    
    void enterBlock(std::string enterstr , std::string dragstr);
    
    void lineBlockMove(Point draItemP);
    
    void resetBlockPos(float offx,float offy);

    Sprite* maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite,Vec2 maskP);
};

#endif /* GameScene_h */
