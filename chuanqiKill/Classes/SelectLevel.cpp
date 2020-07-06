//
//  SelectLevel.cpp
//  ChuanqiKill-mobile
//
//  Created by 志强 on 2020/5/27.
//

#include "SelectLevel.hpp"
#include "Date.h"
#include "GameScene.h"
#include "ShopPop.hpp"

Scene* SelectLevel::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectLevel::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectLevel::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("SelectLevel.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    levelGroup = static_cast<Layout*>(rootNode->getChildByName("levelGroup"));
    shopBtn = static_cast<Sprite*>(rootNode->getChildByName("shopBtn"));
    goldPanel = static_cast<Layout*>(rootNode->getChildByName("goldPanel"));
    goldLab = static_cast<TextBMFont*>(goldPanel->getChildByName("goldLab"));
    line = static_cast<Sprite*>(rootNode->getChildByName("line"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SelectLevel::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SelectLevel::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
   
    goldLab->setString(StringUtils::format("%d",DT()->gold));
    for(int i = 1;i<=9;i++)
    {
        std::string levelkey = StringUtils::format("level%d",i);
        Layout*itemLayout = static_cast<Layout*>(levelGroup->getChildByName(levelkey));
        Button*button = static_cast<Button*>(itemLayout->getChildByName("btn"));
        if(DT()->level > i)
        {
            button->setBright(true);
            button->setTag(1);
            button->setBrightStyle(BRIGHT_NORMAL);
        }else if(DT()->level == i){
            button->setBright(true);
            button->setTag(1);
            button->setBrightStyle(BRIGHT_HIGHLIGHT);
        }else{
            button->setBright(false);
            button->setTag(0);
        }
    }
    this->adapter();
    return true;
}
bool SelectLevel::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point groupP = levelGroup->convertToNodeSpace(tTouch->getLocation());
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(shopBtn->getBoundingBox().containsPoint(localP)  && !topPanel)
    {
        shopBtn->setScale(0.9f);
        return true;
    }
    Vector<Node*> childrens = levelGroup->getChildren();
    for(auto c : childrens)
    {
        Layout*item = static_cast<Layout*>(c);
        Button*button = static_cast<Button*>(item->getChildByName("btn"));
        if(item->getBoundingBox().containsPoint(groupP)  && !topPanel && button->getTag() == 1)
        {
            return true;
        }
    }
    return false;
}
void SelectLevel::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point groupP = levelGroup->convertToNodeSpace(tTouch->getLocation());
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(shopBtn->getBoundingBox().containsPoint(localP)  && !topPanel)
    {
        shopBtn->setScale(1.0f);
        topPanel = true;
        ShopPop*shop = new ShopPop();
        shop->touchBuyCall = CC_CALLBACK_0(SelectLevel::popCall, this);
        this->addChild(shop);
    }
    
    Vector<Node*> childrens = levelGroup->getChildren();
    int index = 0;
    for(auto c : childrens)
    {
        index += 1;
        Layout*item = static_cast<Layout*>(c);
        Button*button = static_cast<Button*>(item->getChildByName("btn"));
        if(item->getBoundingBox().containsPoint(groupP) && !topPanel && button->getTag() == 1)
        {
            DT()->curLevel = index;
            auto scene = GameScene::createScene();
            
            Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(1, scene));
            break;
        }
    }
}
void SelectLevel::popCall()
{
    topPanel = false;
}
void SelectLevel::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    levelGroup->setScale(scaleX, scaleY);
    line->setScale(scaleX, scaleY);
    line->setPosition(Vec2(line->getPositionX()*scaleX,line->getPositionY()*scaleY));
}
void SelectLevel::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
