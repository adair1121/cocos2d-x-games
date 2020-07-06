//
//  ResultScene.cpp
//  holdGround-mobile
//
//  Created by 志强 on 2020/6/4.
//

#include "ResultScene.hpp"
#include "SoundCtl.h"
#include "GameScene.hpp"
#include "HelloWorldScene.h"
#include "Config.h"
Scene* ResultScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ResultScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ResultScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("ResultScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    scoreIcon = static_cast<Sprite*>(rootNode->getChildByName("scoreIcon"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    returnBtn = static_cast<Sprite*>(rootNode->getChildByName("returnBtn"));
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
    tip1 = static_cast<Sprite*>(rootNode->getChildByName("tip1"));
    tip2 = static_cast<Sprite*>(rootNode->getChildByName("tip2"));
    tip3 = static_cast<Sprite*>(rootNode->getChildByName("tip3"));
    dmgBtn = static_cast<Sprite*>(rootNode->getChildByName("dmgBtn"));
    towerBtn = static_cast<Sprite*>(rootNode->getChildByName("towerBtn"));
    hpBtn = static_cast<Sprite*>(rootNode->getChildByName("hpBtn"));
    
    auto scaleTo = ScaleTo::create(1.0f, 1.1f);
    auto scaleFrom = ScaleTo::create(1.0f, 1.0f);
    auto seque = Sequence::create(scaleTo,scaleFrom, NULL);
    playBtn->runAction(RepeatForever::create(seque));
    
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    
    auto moveTo1 = MoveTo::create(2.0f, Vec2(scoreIcon->getPositionX()*scaleX,scoreIcon->getPositionY()*scaleY + 10));
    auto moveTo2 = MoveTo::create(2.0f, Vec2(scoreIcon->getPositionX()*scaleX,scoreIcon->getPositionY()*scaleY - 10));
    scoreIcon->runAction(RepeatForever::create(Sequence::create(moveTo1,moveTo2, NULL)));
    
    auto moveTo3 = MoveTo::create(2.0f, Vec2(tip1->getPositionX()*scaleX,tip1->getPositionY()*scaleY + 10));
    auto moveTo4 = MoveTo::create(2.0f, Vec2(tip1->getPositionX()*scaleX,tip1->getPositionY()*scaleY - 10));
    tip1->runAction(RepeatForever::create(Sequence::create(moveTo3,moveTo4, NULL)));
    
    auto moveTo5 = MoveTo::create(2.0f, Vec2(tip2->getPositionX()*scaleX,tip2->getPositionY()*scaleY + 10));
    auto moveTo6 = MoveTo::create(2.0f, Vec2(tip2->getPositionX()*scaleX,tip2->getPositionY()*scaleY - 10));
    tip2->runAction(RepeatForever::create(Sequence::create(moveTo5,moveTo6, NULL)));
    
    auto moveTo7 = MoveTo::create(2.0f, Vec2(tip3->getPositionX()*scaleX,tip3->getPositionY()*scaleY + 10));
    auto moveTo8 = MoveTo::create(2.0f, Vec2(tip3->getPositionX()*scaleX,tip3->getPositionY()*scaleY - 10));
    tip3->runAction(RepeatForever::create(Sequence::create(moveTo7,moveTo8, NULL)));
    
    scoreLab->setString(StringUtils::format("%d",CF()->score));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ResultScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    return true;
}
void ResultScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    playBtn->setScale(playBtn->getScaleX()*scaleX);
    tip1->setScale(tip1->getScaleX()*scaleX);
    tip2->setScale(tip2->getScaleX()*scaleX);
    tip3->setScale(tip3->getScaleX()*scaleX);

    playBtn->setPosition(Vec2(playBtn->getPositionX()*scaleX,playBtn->getPositionY()*scaleY));
    scoreIcon->setPosition(Vec2(scoreIcon->getPositionX()*scaleX,scoreIcon->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    returnBtn->setPosition(Vec2(returnBtn->getPositionX()*scaleX,returnBtn->getPositionY()*scaleY));
    tip1->setPosition(Vec2(tip1->getPositionX()*scaleX,tip1->getPositionY()*scaleY));
    tip2->setPosition(Vec2(tip2->getPositionX()*scaleX,tip2->getPositionY()*scaleY));
    tip3->setPosition(Vec2(tip3->getPositionX()*scaleX,tip3->getPositionY()*scaleY));
    dmgBtn->setPosition(Vec2(dmgBtn->getPositionX()*scaleX,dmgBtn->getPositionY()*scaleY));
    towerBtn->setPosition(Vec2(towerBtn->getPositionX()*scaleX,towerBtn->getPositionY()*scaleY));
    hpBtn->setPosition(Vec2(hpBtn->getPositionX()*scaleX,hpBtn->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
bool ResultScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)){
       playBtn->setScale(0.9f);
       return true;
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.4f);
        return true;
    }else if(dmgBtn->getBoundingBox().containsPoint(localP))
    {
        dmgBtn->setScale(0.6f);
        return true;
    }else if(towerBtn->getBoundingBox().containsPoint(localP))
    {
        towerBtn->setScale(0.6f);
        return true;
    }else if(hpBtn->getBoundingBox().containsPoint(localP))
    {
        hpBtn->setScaleX(0.6f);
        return true;
    }
    return false;
}
void ResultScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setScale(1.0f);
        //打开布阵弹窗
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }else if(returnBtn->getBoundingBox().containsPoint(localP))
    {
        returnBtn->setScale(0.5f);
        auto scene = HelloWorld::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(dmgBtn->getBoundingBox().containsPoint(localP))
    {
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        dmgBtn->setScale(0.7f);
        if(CF()->score >= 200)
        {
            CF()->score -= 200;
            scoreLab->setString(StringUtils::format("%d",CF()->score));
        }else{
            floatFont("Not enough score");
        }
    }else if(towerBtn->getBoundingBox().containsPoint(localP))
    {
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        towerBtn->setScale(0.7f);
        if(CF()->score >= 200)
        {
            CF()->score -= 200;
            scoreLab->setString(StringUtils::format("%d",CF()->score));
        }else{
            floatFont("Not enough score");
        }
    }else if(hpBtn->getBoundingBox().containsPoint(localP))
    {
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        hpBtn->setScaleX(0.7f);
        if(CF()->score >= 200)
        {
            CF()->score -= 200;
            scoreLab->setString(StringUtils::format("%d",CF()->score));
            CF()->hp += 50;
        }else{
            floatFont("Not enough score");
        }
    }

}
void ResultScene::floatFont(std::string cnt)
{
    Text*content = Text::create();
    this->addChild(content);
    content->setString(cnt);
    content->setFontSize(30);
    content->setColor(Color3B(255,255,255));
    Size visible = Director::getInstance()->getVisibleSize();
    content->setPosition(Vec2(visible.width/2,visible.height/2));
    auto move = MoveTo::create(1.0f, Vec2(content->getPositionX(),content->getPositionY() + 100));
    auto call = CallFunc::create([=](){
        content->removeFromParent();
    });
    auto seque = Sequence::create(move,call, NULL);
    content->runAction(seque);
}
