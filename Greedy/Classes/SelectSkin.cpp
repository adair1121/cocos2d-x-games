//
//  SelectSkin.cpp
//  Greedy-mobile
//
//  Created by 志强 on 2020/6/12.
//

#include "SelectSkin.hpp"
#include "GameScene.hpp"
#include "Config.h"
#include "CommonUtil.h"
Scene* SelectSkin::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectSkin::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectSkin::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("SelectSkin.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    icon1 = static_cast<Sprite*>(rootNode->getChildByName("icon1"));
    icon1_focus = static_cast<Sprite*>(rootNode->getChildByName("icon1_focus"));
    goldGroup = static_cast<Layout*>(rootNode->getChildByName("goldGroup"));
    goldLab = static_cast<Text*>(goldGroup->getChildByName("goldLab"));
    skin2Group = static_cast<Layout*>(rootNode->getChildByName("skin2Group"));
    skin3Group = static_cast<Layout*>(rootNode->getChildByName("skin3Group"));
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SelectSkin::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SelectSkin::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    goldLab->setString(StringUtils::format("%d",CF()->gold));
    
    this->refreshPage();
    this->adapter();
    this->changeTab();
    return true;
}
void SelectSkin::refreshPage()
{
    static_cast<Sprite*>(skin2Group->getChildByName("lock"))->setVisible(CF()->lock1);
    static_cast<Sprite*>(skin2Group->getChildByName("btn"))->setVisible(CF()->lock1);
    static_cast<Sprite*>(skin2Group->getChildByName("gold"))->setVisible(CF()->lock1);
    static_cast<Sprite*>(skin2Group->getChildByName("goldnum"))->setVisible(CF()->lock1);
    static_cast<Sprite*>(skin3Group->getChildByName("lock"))->setVisible(CF()->lock2);
    static_cast<Sprite*>(skin3Group->getChildByName("btn"))->setVisible(CF()->lock2);
    static_cast<Sprite*>(skin3Group->getChildByName("gold"))->setVisible(CF()->lock2);
    static_cast<Sprite*>(skin3Group->getChildByName("goldnum"))->setVisible(CF()->lock2);
}
void SelectSkin::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    skin2Group->setScale(scaleX,scaleY);
    skin3Group->setScale(scaleX,scaleY);
    icon1->setScale(scaleX,scaleY);
    playBtn->setPosition(Vec2(playBtn->getPositionX()*scaleX,playBtn->getPositionY()*scaleY));
    goldGroup->setPosition(Vec2(goldGroup->getPositionX()*scaleX,goldGroup->getPositionY()*scaleY));
    icon1->setPosition(Vec2(icon1->getPositionX()*scaleX,icon1->getPositionY()*scaleY));
    skin2Group->setPosition(Vec2(skin2Group->getPositionX()*scaleX,skin2Group->getPositionY()*scaleY));
    skin3Group->setPosition(Vec2(skin3Group->getPositionX()*scaleX,skin3Group->getPositionY()*scaleY));
}
void SelectSkin::changeTab()
{
    CF()->selectIndex = selectIndex;
    if(selectIndex == 1)
    {
        icon1_focus->setVisible(true);
        static_cast<Sprite*>(skin2Group->getChildByName("icon_focus"))->setVisible(false);
        static_cast<Sprite*>(skin3Group->getChildByName("icon_focus"))->setVisible(false);
    }else if(selectIndex == 2)
    {
        icon1_focus->setVisible(false);
        static_cast<Sprite*>(skin2Group->getChildByName("icon_focus"))->setVisible(true);
        static_cast<Sprite*>(skin3Group->getChildByName("icon_focus"))->setVisible(false);
    }else if(selectIndex == 3)
    {
        icon1_focus->setVisible(false);
        static_cast<Sprite*>(skin2Group->getChildByName("icon_focus"))->setVisible(false);
        static_cast<Sprite*>(skin3Group->getChildByName("icon_focus"))->setVisible(true);
    }
    
}
bool SelectSkin::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)){
       playBtn->setScale(0.9f);
       return true;
    }else if(skin2Group->getBoundingBox().containsPoint(localP))
    {
        Point skinP = skin2Group->convertToNodeSpace(tTouch->getLocation());
        Sprite*btn = static_cast<Sprite*>(skin2Group->getChildByName("btn"));
        Sprite*icon = static_cast<Sprite*>(skin2Group->getChildByName("icon"));
        if(btn->getBoundingBox().containsPoint(skinP))
        {
            btn->setScale(0.9f);
            return true;
        }else if(icon->getBoundingBox().containsPoint(skinP))
        {
            return true;
        }
    }else if(skin3Group->getBoundingBox().containsPoint(localP))
    {
        Point skinP = skin3Group->convertToNodeSpace(tTouch->getLocation());
        Sprite*btn = static_cast<Sprite*>(skin3Group->getChildByName("btn"));
        Sprite*icon = static_cast<Sprite*>(skin3Group->getChildByName("icon"));
        if(btn->getBoundingBox().containsPoint(skinP))
        {
            btn->setScale(0.9f);
            return true;
        }else if(icon->getBoundingBox().containsPoint(skinP))
        {
            return true;
        }
    }else if(icon1->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
void SelectSkin::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setScale(1.0f);
        //打开布阵弹窗
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        return;
    }else if(skin2Group->getBoundingBox().containsPoint(localP))
    {
        Point skinP = skin2Group->convertToNodeSpace(tTouch->getLocation());
        Sprite*btn = static_cast<Sprite*>(skin2Group->getChildByName("btn"));
        Sprite*icon = static_cast<Sprite*>(skin2Group->getChildByName("icon"));
        if(btn->getBoundingBox().containsPoint(skinP))
        {
            btn->setScale(1.0f);
            if(CF()->gold < 215)
            {
                CommonUtil::floatFont("Not enough gold", this);
            }else{
                CF()->lock1 = false;
                CommonUtil::floatFont("Unlocked successfully", this);
                CF()->gold -= 215;
                goldLab->setString(StringUtils::format("%d",CF()->gold));
                this->refreshPage();
            }
        }else if(icon->getBoundingBox().containsPoint(skinP))
        {
            if(CF()->lock1)
            {
                CommonUtil::floatFont("Not unlocked", this);
            }else{
                selectIndex = 2;
                this->changeTab();
            }
        }
    }else if(skin3Group->getBoundingBox().containsPoint(localP))
    {
        Point skinP = skin3Group->convertToNodeSpace(tTouch->getLocation());
        Sprite*btn = static_cast<Sprite*>(skin3Group->getChildByName("btn"));
        Sprite*icon = static_cast<Sprite*>(skin3Group->getChildByName("icon"));
        if(btn->getBoundingBox().containsPoint(skinP))
        {
            btn->setScale(1.0f);
            if(CF()->gold < 535)
            {
                CommonUtil::floatFont("Not enough gold", this);
            }else{
                CF()->lock2 = false;
                CommonUtil::floatFont("Unlocked successfully", this);
                CF()->gold -= 535;
                goldLab->setString(StringUtils::format("%d",CF()->gold));
                this->refreshPage();
            }
        }else if(icon->getBoundingBox().containsPoint(skinP))
        {
            if(CF()->lock2)
            {
                CommonUtil::floatFont("Not unlocked", this);
            }else{
                selectIndex = 3;
                this->changeTab();
            }
        }
    }else if(icon1->getBoundingBox().containsPoint(localP))
    {
        selectIndex = 1;
        this->changeTab();
    }
}
