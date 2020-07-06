//
//  SelectLevel.cpp
//  chuanqiStory-mobile
//
//  Created by 志强 on 2020/5/22.
//

#include "SelectLevel.h"
#include "HelloWorldScene.h"
#include "ShopScene.h"
#include "Config.h"
#include "SoundCtl.h"
#include "GameScene.h"

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
    bottomGroup = static_cast<Layout*>(rootNode->getChildByName("bottomGroup"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SelectLevel::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SelectLevel::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    static_cast<Text*>(bottomGroup->getChildByName("pointLab"))->setString(StringUtils::format("%d",CF()->point));
    
    for(int i = 1;i<=7;i++)
    {
        std::string itemkey = StringUtils::format("level%d",i);
        Button*item = static_cast<Button*>(levelGroup->getChildByName(itemkey));
        if(CF()->level >= i)
        {
            item->setBrightStyle(BRIGHT_HIGHLIGHT);
            item->setTag(1);
        }else{
            item->setTag(0);
        }
    }
    this->adapter();
    return true;
}
void SelectLevel::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    levelGroup->setScale(scaleX);
    levelGroup->setPosition(Vec2(levelGroup->getPositionX()*scaleX,levelGroup->getPositionY()*scaleY));
    bottomGroup->setScale(scaleX);
    bottomGroup->setPosition(Vec2(bottomGroup->getPositionX()*scaleX,bottomGroup->getPositionY()*scaleY));
}
void SelectLevel::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool SelectLevel::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(bottomGroup->getBoundingBox().containsPoint(localP))
    {
        return true;
    }if(levelGroup->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
void SelectLevel::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(bottomGroup->getBoundingBox().containsPoint(localP))
    {
        Point bottomP = bottomGroup->convertToNodeSpace(tTouch->getLocation());
        Sprite*homeBtn = static_cast<Sprite*>(bottomGroup->getChildByName("homeBtn"));
        Sprite*upBtn = static_cast<Sprite*>(bottomGroup->getChildByName("upBtn"));
        if(homeBtn->getBoundingBox().containsPoint(bottomP))
        {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
            SoundCtl::getInstance()->playEffect("sound/click.m4a");
        }else if(upBtn->getBoundingBox().containsPoint(bottomP))
        {
            auto scene = ShopScene::createScene();
            Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
            SoundCtl::getInstance()->playEffect("sound/click.m4a");
        }
    }else if(levelGroup->getBoundingBox().containsPoint(localP))
    {
        Point itemP = levelGroup->convertToNodeSpace(tTouch->getLocation());
        for(int i = 1;i<=7;i++)
        {
            std::string itemkey = StringUtils::format("level%d",i);
            Button*item = static_cast<Button*>(levelGroup->getChildByName(itemkey));
            if(item->getBoundingBox().containsPoint(itemP))
            {
                if(item->getTag() == 1)
                {
                    //切换到战斗场景
                    CF()->challengeLev = i;
                    SoundCtl::getInstance()->playEffect("sound/click.m4a");
                    auto scene = GameScene::createScene();
                    Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
                }else{
                    this->floatFont("Please clear the first level");
                }
                return;
            }
        }
    }
}
void SelectLevel::floatFont(std::string cnt)
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
