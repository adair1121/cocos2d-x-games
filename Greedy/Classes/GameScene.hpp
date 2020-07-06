//
//  GameScene.hpp
//  Greedy-mobile
//
//  Created by 志强 on 2020/6/1.
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
    Layout*leftGroup;
    Layout*rightGroup;
    Sprite*leftIcon;
    Sprite*rightIcon;
    Text*ownScoreLab;
    Text*comScoreLab;
    Layout*gameGroup;
    Layout*greyLayer;
    Sprite*guidebg;
    Sprite*returnBtn;
    Layout*resultGroup;
    Layout*goldGroup;
    Text*rewardNum;
    Sprite*winIcon;
    Button*mainBtn;
    Button*resetBtn;
    int battleCount = 0;
    Sprite*virtualSp;
    std::string newDropstr;
    std::string bottomNamestr;
    EventListenerTouchOneByOne *_listener;
    std::string totalstr;
    int showIndex = 0;
    bool battleState;
public:
    static cocos2d::Scene* createScene();

   // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
   virtual bool init();
   void onExit();
   // implement the "static create()" method manually
   CREATE_FUNC(GameScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void menuMove(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void adapter();
    
    void resetData();
    
    void showVirtualSpPos(float x,float y);
    
    Point getBottomPos(int col);
    
    void execAiPosJudge();
    
    void changePlayer();
    
    void resultCalCul();
    
    bool calculLine(int x,int y,Vector<Sprite*> & vecs);
    
    void showBlock();
};

#endif /* GameScene_hpp */
