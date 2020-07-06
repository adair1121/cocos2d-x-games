//
//  CreateLayer.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#include "CreateLayer.hpp"
#include "Config.h"
#include "CommonUtil.h"
#include "HomeLayer.hpp"
Scene* CreateLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CreateLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CreateLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("CreateLayer.csb");

    addChild(rootNode);
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    manImg = static_cast<Sprite*>(rootNode->getChildByName("manImg"));
    womanImg = static_cast<Sprite*>(rootNode->getChildByName("womanImg"));
    startBtn = static_cast<Sprite*>(rootNode->getChildByName("startBtn"));
    title = static_cast<Sprite*>(rootNode->getChildByName("title"));
    nameGroup = static_cast<Layout*>(rootNode->getChildByName("nameGroup"));
    nameLab = static_cast<Text*>(nameGroup->getChildByName("nameLab"));
    randomBtn = static_cast<Sprite*>(nameGroup->getChildByName("randomBtn"));
    womanGroup = static_cast<Layout*>(rootNode->getChildByName("womanGroup"));
    manGroup = static_cast<Layout*>(rootNode->getChildByName("manGroup"));
    
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(CreateLayer::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(CreateLayer::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    this->adapter();
    this->changeRole(2);
    namestr = "Arafat_Arago_Arbuckle_Arbuthnot_Archer_Archimedes_Prescott_Presley_Price_Prince_Priscilla_Quintin_Quinton_Fanny_Farley_Farmer_Farrah_Farrell_Faulkner_Farris_Lacey_Lamb_Lambert_Lamont_Lancaster_Lane_Lange_Oakes_Octavia_Odette_Odom_Olga_Olive_Tabitha_Tabor_Tammie_Tanner_Tatum_Yates_Young_Yvonne_Baade_Babbie_Babington_Babcock_Babs_Nadine_Nance_Napier_Naomi_Natalia_Nathalie_Nathan_Kaley_Kane";
    
    nameLab->setString(randomName());
    
    return true;
}
void CreateLayer::changeRole(int sex)
{
    CF()->sex = sex;
    womanImg->setVisible(sex == 2);
    manImg->setVisible(sex == 1);
    Sprite*manIconbg = static_cast<Sprite*>(manGroup->getChildByName("bg2"));
    Sprite*womanIconbg = static_cast<Sprite*>(womanGroup->getChildByName("bg2"));
    manIconbg->setVisible(sex == 1);
    womanIconbg->setVisible(sex == 2);
}
void CreateLayer::adapter()
{
    int bgW = 750;
    int bgH = 1334;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX);
    bg->setPosition(Vec2(bg->getPositionX()*scaleX,bg->getPositionY()*scaleY));
    startBtn->setPosition(Vec2(startBtn->getPositionX()*scaleX,startBtn->getPositionY()*scaleY));
    manImg->setPosition(Vec2(manImg->getPositionX()*scaleX,manImg->getPositionY()*scaleY));
    womanImg->setPosition(Vec2(womanImg->getPositionX()*scaleX,womanImg->getPositionY()*scaleY));
    title->setPosition(Vec2(title->getPositionX()*scaleX,title->getPositionY()*scaleY));
    nameGroup->setPosition(Vec2(nameGroup->getPositionX()*scaleX,nameGroup->getPositionY()*scaleY));
    womanGroup->setPosition(Vec2(womanGroup->getPositionX()*scaleX,womanGroup->getPositionY()*scaleY));
    manGroup->setPosition(Vec2(manGroup->getPositionX()*scaleX,manGroup->getPositionY()*scaleY));
}
bool CreateLayer::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p))
    {
        startBtn->setScale(0.9f);
        return true;
    }else if(manGroup->getBoundingBox().containsPoint(p))
    {
        return true;
    }else if(womanGroup->getBoundingBox().containsPoint(p))
    {
        return true;
    }else if(nameGroup->getBoundingBox().containsPoint(p))
    {
        return true;
    }
    return false;
}
std::string CreateLayer::randomName()
{
    std::vector<std::string> names = CommonUtil::split(namestr, "_");
    srand(int(time(0))+ rand());//置随机数种子
    int num=CCRANDOM_0_1()*names.size();
    return names.at(num);
}
void CreateLayer::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point p = this->convertToNodeSpace(tTouch->getLocation());
    if(startBtn->getBoundingBox().containsPoint(p))
    {
        startBtn->setScale(1.0f);
        CF()->namestr = nameLab->getString();
        auto scene = HomeLayer::createScene();
        Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
    }else if(manGroup->getBoundingBox().containsPoint(p))
    {
        if(CF()->sex != 1)
        {
            this->changeRole(1);
        }
    }else if(womanGroup->getBoundingBox().containsPoint(p))
    {
        if(CF()->sex != 2)
        {
            this->changeRole(2);
        }
    }else if(nameGroup->getBoundingBox().containsPoint(p))
    {
        Point nameP = nameGroup->convertToNodeSpace(tTouch->getLocation());
        if(randomBtn->getBoundingBox().containsPoint(nameP))
        {
            nameLab->setString(randomName());
        }
    }
}
