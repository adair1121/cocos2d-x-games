//
//  SetPop.cpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/29.
//

#include "SetPop.hpp"
#include "SoundCtl.h"
#include "HelloWorldScene.h"

SetPop::SetPop(int state)
{
    auto rootNode = CSLoader::createNode("SetPop.csb");
    addChild(rootNode);
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    mainBtn = static_cast<Layout*>(contentGroup->getChildByName("mainBtn"));
    audioBtn = static_cast<Layout*>(contentGroup->getChildByName("audioBtn"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SetPop::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SetPop::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, contentGroup);
    std::string audioass = "ui/option_bt_music01.png";
    if(!SoundCtl::getInstance()->musicBool)
    {
        audioass = "ui/option_bt_music02.png";
        SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    }
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(audioass);
    static_cast<Sprite*>(audioBtn->getChildByName("btn"))->setTexture(texture);
    if(state == 1)
    {
        mainBtn->setVisible(false);
        audioBtn->setPositionX(contentGroup->getContentSize().width/2);
    }
    
    this->adapter();
}
bool SetPop::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(mainBtn->getBoundingBox().containsPoint(localP) && mainBtn->isVisible()){
       mainBtn->setScale(0.9f);
       return true;
    }else if(audioBtn->getBoundingBox().containsPoint(localP) && audioBtn->isVisible())
    {
        audioBtn->setScale(0.9f);
        return true;
    }else if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
void SetPop::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    
    if(mainBtn->getBoundingBox().containsPoint(localP) && mainBtn->isVisible()){
        mainBtn->setScale(1.0f);
        this->removeFromParent();
        if(callback)
        {
            callback();
        }
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
        return;
    }else if(audioBtn->getBoundingBox().containsPoint(localP) && audioBtn->isVisible())
    {
        audioBtn->setScale(1.0f);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        std::string audioass = "ui/option_bt_music01.png";
        if(SoundCtl::getInstance()->musicBool)
        {
            SoundCtl::getInstance()->stopBg();
            audioass = "ui/option_bt_music02.png";
        }else{
            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
        }
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(audioass);
        static_cast<Sprite*>(audioBtn->getChildByName("btn"))->setTexture(texture);
    }else if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        if(callback)
        {
            callback();
        }
        this->removeFromParent();
    }
}
void SetPop::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    contentGroup->setScale(scaleX);
    contentGroup->setPosition(Vec2(contentGroup->getPositionX()*scaleX,contentGroup->getPositionY()*scaleY));
//    bg->setScale(scaleX, scaleY);
}
SetPop::~SetPop()
{
    _eventDispatcher->removeEventListener(_listener);
}
