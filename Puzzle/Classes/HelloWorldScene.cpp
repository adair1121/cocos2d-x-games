#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SoundCtl.h"
#include "SelectTypeScene.h"
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
    playBtn = static_cast<Sprite*>(rootNode->getChildByName("playBtn"));
//    audioBtn = static_cast<Button*>(rootNode->getChildByName("audioBtn"));
    auto scaleTo = ScaleTo::create(1.0f, 1.1f);
    auto scaleFrom = ScaleTo::create(1.0f, 1.0f);
    auto seque = Sequence::create(scaleTo,scaleFrom, NULL);
    playBtn->runAction(RepeatForever::create(seque));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    this->adapter();
    return true;
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
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
void HelloWorld::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)){
       playBtn->setScale(0.9f);
       return true;
    }
//    else if(audioBtn->getBoundingBox().containsPoint(localP))
//    {
//        return true;
//    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setScale(1.0f);
        //打开布阵弹窗
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = SelectTypeScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }
//    else if(audioBtn->getBoundingBox().containsPoint(localP))
//    {
//        if(SoundCtl::getInstance()->musicBool)
//        {
//            SoundCtl::getInstance()->stopBg();
//            audioBtn->setBrightStyle(BRIGHT_NORMAL);
//        }else{
//            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
//            audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
//        }
//    }
}
