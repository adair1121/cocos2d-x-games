//
//  ShopScene.cpp
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/22.
//

#include "ShopScene.h"
#include "Config.h"
#include "SelectLevel.h"
#include "SoundCtl.h"
Scene* ShopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShopScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ShopScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("ShopScene.csb");

    addChild(rootNode);
    
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    
    item1 = static_cast<Layout*>(rootNode->getChildByName("item1"));
    item2 = static_cast<Layout*>(rootNode->getChildByName("item2"));
    item3 = static_cast<Layout*>(rootNode->getChildByName("item3"));
    item4 = static_cast<Layout*>(rootNode->getChildByName("item4"));
    item5 = static_cast<Layout*>(rootNode->getChildByName("item5"));
    item6 = static_cast<Layout*>(rootNode->getChildByName("item6"));
    homeBtn = static_cast<Sprite*>(rootNode->getChildByName("homeBtn"));
    pointBg = static_cast<Sprite*>(rootNode->getChildByName("pointBg"));
    pointLab = static_cast<Text*>(rootNode->getChildByName("pointLab"));
    shopbg = static_cast<Sprite*>(rootNode->getChildByName("shopbg"));
    
    pointLab->setString(StringUtils::format("%d",CF()->point));
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(ShopScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(ShopScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    static_cast<Text*>(item1->getChildByName("num"))->setString(StringUtils::format("%d",CF()->way1Level*400));
    static_cast<Text*>(item2->getChildByName("num"))->setString(StringUtils::format("%d",(CF()->way2Level+1)*1000));
    static_cast<Text*>(item3->getChildByName("num"))->setString(StringUtils::format("%d",CF()->mpLevel*25));
    static_cast<Text*>(item1->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->way1Level));
    static_cast<Text*>(item2->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->way2Level));
    static_cast<Text*>(item3->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->mpLevel));
    static_cast<Text*>(item4->getChildByName("txt"))->setString(StringUtils::format("Lightnings:%d",CF()->lightNum));
    static_cast<Text*>(item5->getChildByName("txt"))->setString(StringUtils::format("ice:%d",CF()->iceNum));
    static_cast<Text*>(item6->getChildByName("txt"))->setString(StringUtils::format("Health:%d",CF()->hpNum));
    this->adapter();
    return true;
}
void ShopScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    item1->setScale(scaleX);
    item1->setPosition(Vec2(item1->getPositionX()*scaleX,item1->getPositionY()*scaleY));
    item2->setScale(scaleX);
    item2->setPosition(Vec2(item2->getPositionX()*scaleX,item2->getPositionY()*scaleY));
    item3->setScale(scaleX);
    item3->setPosition(Vec2(item3->getPositionX()*scaleX,item3->getPositionY()*scaleY));
    item4->setScale(scaleX);
    item4->setPosition(Vec2(item4->getPositionX()*scaleX,item4->getPositionY()*scaleY));
    item5->setScale(scaleX);
    item5->setPosition(Vec2(item5->getPositionX()*scaleX,item5->getPositionY()*scaleY));
    item6->setScale(scaleX);
    item6->setPosition(Vec2(item6->getPositionX()*scaleX,item6->getPositionY()*scaleY));
    homeBtn->setPosition(Vec2(homeBtn->getPositionX()*scaleX,homeBtn->getPositionY()*scaleY));
    pointBg->setPosition(Vec2(pointBg->getPositionX()*scaleX,pointBg->getPositionY()*scaleY));
    pointLab->setPosition(Vec2(pointLab->getPositionX()*scaleX,pointLab->getPositionY()*scaleY));
    
    shopbg->setScale(scaleX);
    shopbg->setPosition(Vec2(shopbg->getPositionX()*scaleX,shopbg->getPositionY()*scaleY));
}
void ShopScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool ShopScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(item1->getBoundingBox().containsPoint(localP) || item2->getBoundingBox().containsPoint(localP) || item3->getBoundingBox().containsPoint(localP) || item4->getBoundingBox().containsPoint(localP) || item5->getBoundingBox().containsPoint(localP) || item6->getBoundingBox().containsPoint(localP))
    {
        return true;
    }else if(homeBtn->getBoundingBox().containsPoint(localP))
    {
        homeBtn->setScale(0.9f);
        return true;
    }
    return false;
}
void ShopScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(homeBtn->getBoundingBox().containsPoint(localP))
    {
        homeBtn->setScale(1.0f);
        auto scene = SelectLevel::createScene();
        Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item1->getBoundingBox().containsPoint(localP))
    {
        int cost = CF()->way1Level*400;
        if(CF()->point < cost){this->floatFont("not enough badge");return;}
        CF()->point -= cost;
        CF()->way1Level += 1;
        static_cast<Text*>(item1->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->way1Level));
        static_cast<Text*>(item1->getChildByName("num"))->setString(StringUtils::format("%d",CF()->way1Level*400));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item2->getBoundingBox().containsPoint(localP))
    {
        int cost = (CF()->way2Level+1)*1000;
        if(CF()->point < cost){this->floatFont("not enough badge");return;}
        CF()->point -= cost;
        CF()->way2Level += 1;
        static_cast<Text*>(item2->getChildByName("num"))->setString(StringUtils::format("%d",(CF()->way2Level+1)*1000));
        static_cast<Text*>(item2->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->way2Level));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item3->getBoundingBox().containsPoint(localP))
    {
        int cost = CF()->mpLevel*25;
        if(CF()->point < cost){this->floatFont("not enough badge");return;}
        CF()->point -= cost;
        CF()->mpLevel += 1;
        static_cast<Text*>(item3->getChildByName("num"))->setString(StringUtils::format("%d",CF()->mpLevel*25));
        static_cast<Text*>(item3->getChildByName("txt"))->setString(StringUtils::format("level:%d",CF()->mpLevel));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item4->getBoundingBox().containsPoint(localP))
    {
        if(CF()->point < 1000)
        {
            this->floatFont("not enough badge");
            return;
        }
        CF()->point -= 1000;
        CF()->lightNum += 1;
        static_cast<Text*>(item4->getChildByName("txt"))->setString(StringUtils::format("Lightnings:%d",CF()->lightNum));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item5->getBoundingBox().containsPoint(localP))
    {
        if(CF()->point < 2000)
        {
            this->floatFont("not enough badge");
            return;
        }
        CF()->point -= 2000;
        CF()->iceNum += 1;
        static_cast<Text*>(item6->getChildByName("txt"))->setString(StringUtils::format("ice:%d",CF()->lightNum));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }else if(item6->getBoundingBox().containsPoint(localP))
    {
        if(CF()->point < 30)
        {
            this->floatFont("not enough badge");
            return;
        }
        CF()->point -= 30;
        CF()->hpNum += 1;
        static_cast<Text*>(item6->getChildByName("txt"))->setString(StringUtils::format("Health:%d",CF()->hpNum));
        pointLab->setString(StringUtils::format("%d",CF()->point));
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
    }
}

void ShopScene::floatFont(std::string cnt)
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
