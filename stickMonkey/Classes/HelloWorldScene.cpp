#include "HelloWorldScene.h"
#include "SoundCtl.h"
#include "GameScene.hpp"

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
    logo = static_cast<Sprite*>(rootNode->getChildByName("logo"));
    playBtn = static_cast<Button*>(rootNode->getChildByName("playBtn"));
    model = static_cast<Sprite*>(rootNode->getChildByName("model"));
    audioBtn = static_cast<Button*>(rootNode->getChildByName("audioBtn"));
    view_layout = static_cast<Layout*>(rootNode->getChildByName("view_layout"));
   
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    SoundCtl::getInstance()->playBg("sound/music.m4a", true);
    audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
    this->adapter();
    
    auto moveTo1 = MoveTo::create(2.0f, Vec2(logo->getPositionX(),logo->getPositionY() + 10));
    auto moveTo2 = MoveTo::create(2.0f, Vec2(logo->getPositionX(),logo->getPositionY() - 10));
    logo->runAction(RepeatForever::create(Sequence::create(moveTo1,moveTo2, NULL)));
    
//    this->initCloud(0.0f);
//    schedule(schedule_selector(HelloWorld::initCloud), 10.0f);
    return true;
}
void HelloWorld::initCloud(float dt)
{
    for(int i = 0;i<8;i++)
    {
        srand(int(time(0))+ rand());//置随机数种子
        int ran=CCRANDOM_0_1()*3 + 1;
        std::string itemass = "assets/clouds_1-sheet0_0"+StringUtils::format("%d",ran)+".png";
        Sprite*item = Sprite::create(itemass);
        view_layout->addChild(item, 0);
        float x = CCRANDOM_0_1()*view_layout->getContentSize().width;
        float y = CCRANDOM_0_1()*view_layout->getContentSize().height;
        item->setScale(3.0f);
        item->setPosition(Vec2(x,y));
        item->setOpacity(0.0f);
        
        auto fadein = FadeIn::create(1.0f);
        
        Vec2 xy1 = Point(x+50,y);
        Vec2 xy2 = Point(x+100,y);
        Vec2 xy3 = Point(x+150,y);
        auto moveto1 = MoveTo::create(8, xy1);
        auto fadeto1 = FadeTo::create(8, 0.8f);
        auto mySpawn = Spawn::createWithTwoActions(moveto1,fadeto1);

        auto moveto2 = MoveTo::create(8, xy2);
        auto fadeto2 = FadeTo::create(8, 0.8f);
        auto mySpawn2 = Spawn::createWithTwoActions(moveto2,fadeto2);

        auto moveto3 = MoveTo::create(8, xy3);
        auto fadeto3 = FadeTo::create(8, 0);
        auto mySpawn3 = Spawn::createWithTwoActions(moveto3,fadeto3);

        auto callback = CallFunc::create([=](){
            item->removeFromParent();
        });
        auto seq = Sequence::create(fadein,mySpawn, mySpawn2,mySpawn3,callback,nullptr);
        item->setScale(0.7, 0.7);
        item->runAction(seq);
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
    playBtn->setPosition(Vec2(playBtn->getPositionX()*scaleX,playBtn->getPositionY()*scaleY));
    logo->setPosition(Vec2(logo->getPositionX()*scaleX,logo->getPositionY()*scaleY));
    model->setPosition(Vec2(model->getPositionX()*scaleX,model->getPositionY()*scaleY));
    audioBtn->setPosition(Vec2(audioBtn->getPositionX()*scaleX,audioBtn->getPositionY()*scaleY));
    view_layout->setPosition(Vec2(view_layout->getPositionX()*scaleX,view_layout->getPositionY()*scaleY));
//    audioBtn->setPositionX(audioBtn->getPositionX()*scaleX);
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
       return true;
    }else if(audioBtn->getBoundingBox().containsPoint(localP))
    {
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(playBtn->getBoundingBox().containsPoint(localP)){
        playBtn->setBrightStyle(BRIGHT_NORMAL);
        //打开布阵弹窗
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        return;
    }else if(audioBtn->getBoundingBox().containsPoint(localP))
    {
        if(SoundCtl::getInstance()->musicBool)
        {
            audioBtn->setBrightStyle(BRIGHT_NORMAL);
            SoundCtl::getInstance()->stopBg();
        }else{
            SoundCtl::getInstance()->playBg("sound/music.m4a", true);
            audioBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
        }
    }

}
