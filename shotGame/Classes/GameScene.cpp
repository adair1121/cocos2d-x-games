//
//  GameScene.cpp
//  shotGame-mobile
//
//  Created by 志强 on 2020/5/20.
//

#include "GameScene.h"
#include "Config.h"
#include "ResultScene.h"
#include "MovieClip.h"
#include "SoundCtl.h"
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("GameScene.csb");

    addChild(rootNode);
    Config::getInstance()->score = 0;
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    timeLab = static_cast<TextBMFont*>(rootNode->getChildByName("timeLab"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    gameCon = static_cast<Layout*>(rootNode->getChildByName("gameCon"));
    role = static_cast<Sprite*>(rootNode->getChildByName("role"));
    timeBg = static_cast<Sprite*>(rootNode->getChildByName("timeBg"));
    scoreBg = static_cast<Sprite*>(rootNode->getChildByName("scoreBg"));
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->schedule(schedule_selector(GameScene::updateTime),1.0f);
    this->schedule(schedule_selector(GameScene::ticktime),0.1f);
    this->adapter();
    return true;
}
void GameScene::ticktime(float dt)
{
    Vector<Sprite*> bones;
    Vector<Sprite*> entitys;
    Vector<Node*> childrens = gameCon->getChildren();
    for(auto c : childrens)
    {
        if(c->getName() == "bone")
        {
            bones.pushBack(static_cast<Sprite*>(c));
        }
        if(c->getName() == "entity")
        {
            entitys.pushBack(static_cast<Sprite*>(c));
        }
    }
    for(auto c : bones)
    {
        for(auto e:entitys)
        {
            float distance=sqrt(pow(c->getPositionX()-e->getPositionX(),2)+pow(c->getPositionY()-e->getPositionY(),2));
            if(distance <= 50)
            {
                MovieClip*mc = new MovieClip();
                mc->playFile("boom.plist", "frame", 1, 4, 1, true);
                gameCon->addChild(mc);
                mc->setPosition(Vec2(e->getPositionX(),e->getPositionY()));
                Config::getInstance()->score += 10;
                scoreLab->setString(StringUtils::format("%d",Config::getInstance()->score));
                c->removeFromParent();
                e->removeFromParent();
                break;
            }
        }
    }
}
void GameScene::updateTime(float dt)
{
    countTime -= 1;
    if(countTime <= 0)
    {
        this->unschedule(schedule_selector(GameScene::updateTime));
        this->unschedule(schedule_selector(GameScene::ticktime));
        auto scene = ResultScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    }
    std::string timestr = countTime < 10?StringUtils::format("0:0%d",countTime):StringUtils::format("0:%d",countTime);
    timeLab->setString(timestr);
    srand(int(time(0))+ rand());//置随机数种子
    int num=CCRANDOM_0_1()*2+2;
    for(int i = 0;i<num;i++)
    {
        int x = CCRANDOM_0_1()*gameCon->getContentSize().width;
        int y = gameCon->getContentSize().height + 50;
        srand(int(time(0))+ rand());//置随机数种子
        int monnum = CCRANDOM_0_1()*5+1;
        Node*entity = CSLoader::createNode(StringUtils::format("Enemy%d.csb",monnum));
        cocostudio::timeline::ActionTimeline *entityAction = CSLoader::createTimeline(StringUtils::format("Enemy%d.csb",monnum));
//        entity->setScale(0.6f);
        entity->setName("entity");
        entity->runAction(entityAction);
        int endnum = (monnum == 2 || monnum == 4)?35:monnum == 1?50:75;
        entityAction->gotoFrameAndPlay(0,endnum,true);
        gameCon->addChild(entity);
        entity->setPosition(Vec2(x,y));
        
        int endpx = (CCRANDOM_0_1()*2 == 0?1:-1)*CCRANDOM_0_1()*300 + gameCon->getContentSize().width/2;
        auto moveto = MoveTo::create(3.0f, Vec2(endpx,0));
        
        auto callback = CallFunc::create([=](){
            if(entity)
            {
                MovieClip*mc = new MovieClip();
                mc->playFile("boom.plist", "frame", 1, 4, 1, true);
                gameCon->addChild(mc);
                SoundCtl::getInstance()->playEffect("sound/enemy.m4a");
                mc->setPosition(Vec2(entity->getPositionX(),entity->getPositionY()));
                entity->removeFromParent();
            }
        });
        entity->runAction(Sequence::create(moveto,callback, NULL));
    }
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    gameCon->setScale(scaleX,scaleY);
    timeLab->setPositionX(timeLab->getPositionX()*scaleX);
    scoreLab->setPositionX(scoreLab->getPositionX()*scaleX);
    role->setPositionX(role->getPositionX()*scaleX);
    timeBg->setPositionX(timeBg->getPositionX()*scaleX);
    scoreBg->setPositionX(scoreBg->getPositionX()*scaleX);
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(gameCon->getBoundingBox().containsPoint(localP)){
       return true;
    }
    return false;
}
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    
    if(gameCon->getBoundingBox().containsPoint(localP)){
        
        Size size = Director::getInstance()->getVisibleSize();
        
        Point centerP = Point(gameCon->getContentSize().width/2,0);
        float angle = CC_RADIANS_TO_DEGREES(atan2(localP.y - centerP.y, localP.x - centerP.x));
        Sprite*bone = Sprite::create("assets/missile.png");
        gameCon->addChild(bone);
        bone->setPosition(Vec2(centerP.x,centerP.y));
        bone->setRotation(-angle);
        bone->setName("bone");
        //射击方向向量
        Point shootVector = localP - centerP;
        //向量标准化(即向量长度为1)
        Point normalizedVector = ccpNormalize(shootVector);
        //移动长度向量
        Point overShootVector = normalizedVector * 900;
        //超出屏幕的点
        Point offScreenPoint = centerP + overShootVector;
        //假设速度为500（pix/s）
        float moveSpeed = 500;
        //移动时间
        float distance=sqrt(pow(centerP.x-offScreenPoint.x,2)+pow(centerP.y-offScreenPoint.y,2));
        float moveDuration = distance / moveSpeed;
        
        auto move = MoveTo::create(moveDuration,offScreenPoint);
        
        SoundCtl::getInstance()->playEffect("sound/shoot.m4a");
        
        CallFunc* moveDone = CallFunc::create([=](){
            if(bone)
            {
                bone->removeFromParent();
            }
            
        });
        bone->runAction(Sequence::create(move,moveDone, NULL));
    }
}
