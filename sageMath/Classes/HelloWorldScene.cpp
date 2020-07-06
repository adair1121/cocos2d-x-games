#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SoundCtl.h"
#include "Config.h"
#include "GameScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    audioBtn = static_cast<Button*>(rootNode->getChildByName("audioBtn"));
    playBtn = static_cast<Button*>(rootNode->getChildByName("playBtn"));
    
    selectMapCon = static_cast<Layout*>(rootNode->getChildByName("selectMapCon"));
    map1 = static_cast<Sprite*>(selectMapCon->getChildByName("map1"));
    map2 = static_cast<Sprite*>(selectMapCon->getChildByName("map2"));
    map3 = static_cast<Sprite*>(selectMapCon->getChildByName("map3"));
    backBtn = static_cast<Button*>(selectMapCon->getChildByName("backBtn"));
    
    selectLevCon = static_cast<Layout*>(rootNode->getChildByName("selectLevCon"));
    backBtn2 = static_cast<Button*>(selectLevCon->getChildByName("backBtn2"));
    for(int i = 1 ;i <= CF()->totalLev;i++)
    {
        std::string keystr = StringUtils::format("level%d",i);
        Layout*iteml = static_cast<Layout*>(selectLevCon->getChildByName(keystr));
        levels.pushBack(iteml);
    }
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    selectMapCon->setVisible(false);
    selectLevCon->setVisible(false);
    this->adapter();
    if(CF()->showMapView == true)
    {
        CF()->showMapView = false;
        audioBtn->setVisible(false);
        playBtn->setVisible(false);
        selectMapCon->setVisible(true);
        if(CF()->map1Level >= CF()->totalLev)
        {
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/select_stage_sand_01.png");
            map2->setTexture(texture);
            CF()->curMapLev = 2;
        }
        if(CF()->map2Level >= CF()->totalLev)
        {
            CF()->curMapLev = 3;
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/select_stage_ice_01.png");
            map3->setTexture(texture);
        }
    }
    if(CF()->stars1.size() <= 0)
    {
        for(int i = 0;i<10;i++)
        {
            CF()->stars1.push_back("3");
        }
    }
    if(CF()->stars2.size() <= 0)
    {
        for(int i = 0;i<10;i++)
        {
            CF()->stars2.push_back("3");
        }
    }
    if(CF()->stars3.size() <= 0)
    {
        for(int i = 0;i<10;i++)
        {
            CF()->stars3.push_back("3");
        }
    }
    this->refreshStar();
    return true;
}
void HelloWorld::refreshStar()
{
    std::vector<std::string> stars;
    if(CF()->enterMap == 1){stars = CF()->stars1;}
    else if(CF()->enterMap == 2){stars = CF()->stars2;}
    else if(CF()->enterMap == 3){stars = CF()->stars3;}
    
    for(int i = 1;i<=stars.size();i++)
    {
        int num = atoi(stars.at(i-1).c_str());
        Layout*level = static_cast<Layout*>(selectLevCon->getChildByName(StringUtils::format("level%d",i)));
        for(int j = 1;j<=3;j++)
        {
            Sprite*star = static_cast<Sprite*>(level->getChildByName(StringUtils::format("star%d",j)));
            star->setVisible(false);
            if(j<=num)
            {
                star->setVisible(true);
            }
        }
    }
}
void HelloWorld::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    playBtn->setPosition(Vec2(visibleSize.width/2,150));
    bg->setScale(scaleX, scaleY);
    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
    backBtn->setPositionX(backBtn->getPositionX()*scaleX);
    backBtn2->setPositionX(backBtn->getPositionX()*scaleX);
    map1->setScale(map1->getScaleX()*scaleX);
    map1->setPositionX(map1->getPositionX()*scaleX);
    map2->setPositionX(map2->getPositionX()*scaleX);
    map2->setScale(map2->getScaleX()*scaleX);
    map3->setPositionX(map3->getPositionX()*scaleX);
    map3->setScale(map3->getScaleX()*scaleX);
    for(auto c : levels)
    {
        c->setScale(c->getScaleX()*scaleX);
        c->setPosition(Vec2(c->getPositionX()*scaleX,c->getPositionY()*scaleY));
    }
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point mapP = selectMapCon->convertToNodeSpace(tTouch->getLocation());
    Point levelP = selectLevCon->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP) && playBtn->isVisible()){
       playBtn->setScale(0.9f);
       return true;
    }else if(audioBtn->getBoundingBox().containsPoint(localP) && audioBtn->isVisible())
    {
        return true;
    }else if(backBtn->getBoundingBox().containsPoint(mapP) && selectMapCon->isVisible())
    {
        backBtn->setScale(0.9f);
        return true;
    }else if(backBtn2->getBoundingBox().containsPoint(levelP) && selectLevCon->isVisible())
    {
        backBtn2->setScale(0.9f);
        return  true;
    }else if((map1->getBoundingBox().containsPoint(mapP) || map2->getBoundingBox().containsPoint(mapP) || map3->getBoundingBox().containsPoint(mapP)) && selectMapCon->isVisible())
    {
        return true;
    }
    for(auto c : levels)
    {
        if(c->getBoundingBox().containsPoint(levelP) && selectLevCon->isVisible())
        {
            return true;
        }
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point mapP = selectMapCon->convertToNodeSpace(tTouch->getLocation());
    Point levelP = selectLevCon->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP) && playBtn->isVisible()){
        playBtn->setScale(1.0f);
        //打开布阵弹窗
        audioBtn->setVisible(false);
        playBtn->setVisible(false);
        selectMapCon->setVisible(true);
        if(CF()->map1Level >= CF()->totalLev)
        {
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/select_stage_sand_01.png");
            map2->setTexture(texture);
            CF()->curMapLev = 2;
        }
        if(CF()->map2Level >= CF()->totalLev)
        {
            CF()->curMapLev = 3;
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/select_stage_ice_01.png");
            map3->setTexture(texture);
        }
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        return;
    }else if(audioBtn->getBoundingBox().containsPoint(localP) && audioBtn->isVisible())
    {
        if(SoundCtl::getInstance()->musicBool)
        {
            SoundCtl::getInstance()->stopBg();
            audioBtn->setBrightStyle(BRIGHT_NORMAL);
        }else{
            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
            audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        }
        return;
    }else if(backBtn->getBoundingBox().containsPoint(mapP) && selectMapCon->isVisible())
    {
        backBtn->setScale(1.0f);
        selectMapCon->setVisible(false);
        audioBtn->setVisible(true);
        playBtn->setVisible(true);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        return;
    }else if(backBtn2->getBoundingBox().containsPoint(levelP) && selectLevCon->isVisible())
    {
        backBtn2->setScale(1.0f);
        selectLevCon->setVisible(false);
        selectMapCon->setVisible(true);
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/bg_title_a.png");
        bg->setTexture(texture);
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        return;
    }else if(map1->getBoundingBox().containsPoint(mapP) && selectMapCon->isVisible())
    {
        this->showMapLevel(1);
        return;
    }
    else if(map2->getBoundingBox().containsPoint(mapP) && selectMapCon->isVisible())
    {
        this->showMapLevel(2);
        return;
    }else if(map3->getBoundingBox().containsPoint(mapP) && selectMapCon->isVisible())
    {
        this->showMapLevel(3);
        return;
    }
    int index = 0;
    for(auto c : levels)
    {
        index += 1;
        bool boo = CF()->enterMap == 1?(index <= CF()->map1Level):CF()->enterMap == 2?(index <= CF()->map2Level):(index <= CF()->map3Level);
        if(c->getBoundingBox().containsPoint(levelP) && selectLevCon->isVisible() && boo)
        {
            SoundCtl::getInstance()->playEffect("sound/click.m4a");
            CF()->curChallengeLev = index;
            //进入战斗场景
            auto scene = GameScene::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
            return;
        }
    }
}
void HelloWorld::showMapLevel(int maplevel)
{
    if(CF()->curMapLev >= maplevel)
    {
//        CF()->curMapLev = maplevel;
        CF()->enterMap = maplevel;
        this->selectMapCon->setVisible(false);
        this->selectLevCon->setVisible(true);
        std::string mapkey = StringUtils::format("assets/map0%d.png",CF()->enterMap);
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(mapkey);
        bg->setTexture(texture);
        int num = maplevel == 1?CF()->map1Level:maplevel == 2?CF()->map2Level:CF()->map3Level;
        for(int i = 0;i<levels.size();i++)
        {
            levels.at(i)->getChildByName("lock")->setVisible(true);
            if(i <= num-1)
            {
                levels.at(i)->getChildByName("lock")->setVisible(false);
            }
        }
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }
}
