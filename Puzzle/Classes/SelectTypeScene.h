//
//  SelectTypeScene.h
//  Puzzle-mobile
//  //选择图片界面
//  Created by 志强 on 2020/5/19.
//

#ifndef SelectTypeScene_h
#define SelectTypeScene_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DeviceUtils.h"

USING_NS_CC;

using namespace cocos2d::ui;

class SelectTypeScene : public cocos2d::Layer{
private:
    Sprite*closeBtn;
    Sprite*level1;
    Sprite*level2;
    Sprite*level3;
    Sprite*level4;
    Sprite*bg;
    Layout*contentGroup;
    Layout*piecesGroup;
    Sprite*difficult1;
    Sprite*difficult2;
    Sprite*difficult3;
  
    EventListenerTouchOneByOne *_listener;
public:
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(SelectTypeScene);
private:
    bool menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
       
    void menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
    
    void selectPicitem(int itemIndex);
    
    void transToGameScene();
    
    void adapter();
    
    void floatFont(std::string cnt);
};

#endif /* SelectTypeScene_h */
