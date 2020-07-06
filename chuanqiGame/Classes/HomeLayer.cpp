//
//  HomeLayer.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#include "HomeLayer.hpp"
#include "ShopPop.hpp"
#include "ChallengePop.hpp"

Scene* HomeLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HomeLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HomeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("HomeLayer.csb");

    addChild(rootNode);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    navGroup = static_cast<Layout*>(rootNode->getChildByName("navGroup"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    popLayout = static_cast<Layout*>(rootNode->getChildByName("popLayout"));
    challengeBtn = static_cast<Sprite*>(navGroup->getChildByName("challengeBtn"));
    shopBtn = static_cast<Sprite*>(navGroup->getChildByName("shopBtn"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HomeLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HomeLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    
    roleTitle = new RoleTitleLayer();
    contentGroup->addChild(roleTitle);
    roleTitle->setPosition(Vec2(0,visibleSize.height - 140));
    return true;
}
void HomeLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    popLayout->setContentSize(Size(visibleSize.width,visibleSize.height));
    popLayout->setPosition(Vec2(0,0));
    contentGroup->setContentSize(Size(visibleSize.width,visibleSize.height));
    contentGroup->setPosition(Vec2(0,0));
    bg->setPosition(Vec2(bg->getPositionX()*scaleX,bg->getPositionY()*scaleY));
    navGroup->setPosition(Vec2(navGroup->getPositionX()*scaleX,navGroup->getPositionY()*scaleY));
}
bool HomeLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(navGroup->getBoundingBox().containsPoint(p) && popLayout->getChildren().size() <= 0)
    {
        Point groupP = navGroup->convertToNodeSpace(tTouch->getLocation());
        if(challengeBtn->getBoundingBox().containsPoint(groupP))
        {
            challengeBtn->setScale(0.9f);
            return true;
        }else if(shopBtn->getBoundingBox().containsPoint(groupP))
        {
            shopBtn->setScale(0.9f);
            return true;
        }
        
    }
    return false;
}

void HomeLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(navGroup->getBoundingBox().containsPoint(p) && popLayout->getChildren().size() <= 0)
    {
        Point groupP = navGroup->convertToNodeSpace(tTouch->getLocation());
        if(challengeBtn->getBoundingBox().containsPoint(groupP))
        {
            challengeBtn->setScale(1.0f);
            ChallengePop*challengePop = new ChallengePop();
            popLayout->addChild(challengePop);
            
        }else if(shopBtn->getBoundingBox().containsPoint(groupP))
        {
            shopBtn->setScale(1.0f);
            ShopPop*shop = new ShopPop();
            popLayout->addChild(shop);
        }
    }
}
