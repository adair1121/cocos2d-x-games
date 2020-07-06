#include "HelloWorldScene.h"


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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    bg1 = static_cast<Sprite*>(rootNode->getChildByName("bg1"));
    bg2 = static_cast<Sprite*>(rootNode->getChildByName("bg2"));
    bg3 = static_cast<Sprite*>(rootNode->getChildByName("bg3"));
    roleImg = static_cast<Sprite*>(rootNode->getChildByName("roleImg"));
    bottomImg = static_cast<Sprite*>(rootNode->getChildByName("bottomImg"));
    startGroup = static_cast<Layout*>(rootNode->getChildByName("startGroup"));
    scoreLab = static_cast<TextBMFont*>(rootNode->getChildByName("scoreLab"));
    startBtn = static_cast<Sprite*>(startGroup->getChildByName("startBtn"));
    arrow = static_cast<Sprite*>(rootNode->getChildByName("arrow"));
    rope = static_cast<Layout*>(rootNode->getChildByName("rope"));
    gameCon = static_cast<Layout*>(rootNode->getChildByName("gameCon"));
    
    resultGroup = static_cast<Layout*>(rootNode->getChildByName("resultGroup"));
    scoreLab2 = static_cast<TextBMFont*>(resultGroup->getChildByName("scoreLab2"));
    titleImg = static_cast<Sprite*>(resultGroup->getChildByName("titleImg"));
    againBtn = static_cast<Sprite*>(resultGroup->getChildByName("againBtn"));
    
    resultGroup->setVisible(false);
    
    scoreLab->setVisible(false);
    arrow->setVisible(false);
    rope->setVisible(false);
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    this->adapter();
    this->resetData();
    return true;
}
void HelloWorld::adapter()
{
    int bgW = bg1->getContentSize().width;
    int bgH = bg1->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg1->setScale(scaleX, scaleY);
    gameCon->setScale(scaleX,scaleY);
    bg2->setScale(scaleX, scaleY);
    bg2->setPosition(Vec2(bg2->getPositionX()*scaleX,bg2->getPositionY()*scaleY));
    bg3->setScale(scaleX,scaleY);
    bg3->setPosition(Vec2(bg3->getPositionX()*scaleX,bg3->getPositionY()*scaleY));
    startGroup->setScale(scaleY);
    startGroup->setPosition(Vec2(visibleSize.width/2,startGroup->getPositionY()*scaleY));
    scoreLab->setPosition(Vec2(scoreLab->getPositionX()*scaleX,scoreLab->getPositionY()*scaleY));
    bottomImg->setScale(scaleY);
    bottomImg->setPosition(Vec2(bottomImg->getPositionX()*scaleX,bottomImg->getPositionY()*scaleY));
    roleImg->setScale(scaleY);
    roleImg->setPosition(Vec2(roleImg->getPositionX()*scaleX,roleImg->getPositionY()*scaleY));
    arrow->setPosition(Vec2(arrow->getPositionX()*scaleX,arrow->getPositionY()*scaleY));
    rope->setPosition(Vec2(rope->getPositionX()*scaleX,rope->getPositionY()*scaleY));
    
}
bool HelloWorld::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(startGroup->getBoundingBox().containsPoint(localP) && startGroup->isVisible() && !gameEnd)
    {
        Point groupP = startGroup->convertToNodeSpace(tTouch->getLocation());
        if(startBtn->getBoundingBox().containsPoint(groupP))
        {
            startBtn->setScale(0.9f);
            return true;
        }
    }else if(gameCon->getBoundingBox().containsPoint(localP) && !startGroup->isVisible() && !gameEnd && !treeMove)
    {
        return true;
    }
    else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        return true;
    }
    return false;
}
void HelloWorld::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(startGroup->getBoundingBox().containsPoint(localP) && startGroup->isVisible() && !gameEnd)
    {
        Point groupP = startGroup->convertToNodeSpace(tTouch->getLocation());
        if(startBtn->getBoundingBox().containsPoint(groupP))
        {
            startBtn->setScale(1.0f);
            auto moveto = MoveTo::create(1.0f, Vec2(startGroup->getPositionX(),-startGroup->getContentSize().height));
            auto easein = EaseBackIn::create(moveto->clone());
            auto callback = CallFunc::create([&](){
                startGroup->setVisible(false);
                scoreLab->setVisible(true);
                Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("assets/texture_13.png");
                roleImg->setTexture(texture);
                roleImg->setContentSize(Size(117,159));
                arrow->setVisible(true);
                rope->setVisible(true);
                createTreeItem("assets/tree1.png");
            });
            startGroup->runAction(Sequence::create(easein,callback, NULL));
        }
    }
    else if(gameCon->getBoundingBox().containsPoint(localP) && !startGroup->isVisible() && !gameEnd && !treeMove)
    {
        if(startSpread)
        {
            //正在伸展
            
            this->unschedule(schedule_selector(HelloWorld::tickTime));
            startSpread = false;
            //结束伸展 判断是否扎到了树枝
            Point hitP = Point(arrow->getPositionX(),arrow->getPositionY());
            Sprite*treeItem = trees.size() > 1?trees.at(1):trees.at(0);
            Rect rect = treeItem->getBoundingBox();
            rect.origin.x *= gameCon->getScaleX();
            rect.origin.y *= gameCon->getScaleY();
            rect.size.width *= gameCon->getScaleX();
            rect.size.height *= gameCon->getScaleY();
            if(rect.containsPoint(hitP))
            {
                //已经碰撞到了
                scoreLab->setString(StringUtils::format("%d",(atoi(scoreLab->getString().c_str()))+1));
                float offy = abs(treeItem->getPositionY() - roleImg->getPositionY());
                auto callback = CallFunc::create([&](){
                    Size visibleSize = Director::getInstance()->getVisibleSize();
                    if(bg1->getPositionY() <= -visibleSize.height)
                    {
                        bg1->setPositionY(bg3->getPositionY() + bg3->getContentSize().height*bg3->getScaleY());
                    }
                    if(bg2->getPositionY() <= -visibleSize.height)
                    {
                        bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height*bg1->getScaleY());
                    }
                    if(bg3->getPositionY() <= -visibleSize.height)
                    {
                        bg3->setPositionY(bg2->getPositionY() + bg2->getContentSize().height*bg2->getScaleY());
                    }
                });
                auto moveTo = MoveTo::create(0.5f, Vec2(bg1->getPositionX(),bg1->getPositionY() - offy));

                auto moveTo2 = MoveTo::create(0.5f, Vec2(bg2->getPositionX(),bg2->getPositionY() - offy));
                
                auto moveTo3 = MoveTo::create(0.5f, Vec2(bg3->getPositionX(),bg3->getPositionY() - offy));
                srand(int(time(0))+ rand());//置随机数种子
                int num=CCRANDOM_0_1()*4+1;
                createTreeItem(StringUtils::format("assets/tree%d.png",num));
                bg1->runAction(moveTo);
                bg2->runAction(moveTo2);
                bg3->runAction(Sequence::create(moveTo3,callback, NULL));
            }else{
                //没有碰撞 显示失败界面
                scoreLab->setVisible(false);
                gameEnd = true;
                resultGroup->setVisible(true);
                scoreLab2->setString(scoreLab->getString());
                int num = atoi(scoreLab->getString().c_str());
                std::string titleass = num <= 10?"assets/title1.png":num <= 20?"assets/title2.png":"assets/title3.png";
                Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(titleass);
                titleImg->setTexture(texture);
            }
        }else{
            //目前没有伸展;
            this->schedule(schedule_selector(HelloWorld::tickTime), 0.016f);
            startSpread = true;
        }
    }else if(resultGroup->getBoundingBox().containsPoint(localP) && resultGroup->isVisible())
    {
        Point groupP = resultGroup->convertToNodeSpace(tTouch->getLocation());
        if(againBtn->getBoundingBox().containsPoint(groupP))
        {
            resultGroup->setVisible(false);
            this->resetData();
        }
    }
}
void HelloWorld::tickTime(float dt)
{
    float y = arrow->getPositionY() + 5;
    arrow->setPosition(Vec2(arrow->getPositionX(),y));
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    float h = rope->getContentSize().height + 5;
    rope->setScaleY(rope->getScaleY() + 0.25);
    if(arrow->getPositionY() >= visibleSize.height)
    {
        this->unschedule(schedule_selector(HelloWorld::tickTime));
        startSpread = false;
        scoreLab->setVisible(false);
        gameEnd = true;
        resultGroup->setVisible(true);
        scoreLab2->setString(scoreLab->getString());
        int num = atoi(scoreLab->getString().c_str());
        std::string titleass = num <= 10?"assets/title1.png":num <= 20?"assets/title2.png":"assets/title3.png";
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(titleass);
        titleImg->setTexture(texture);
    }
}
Sprite * HelloWorld::createTreeItem(std::string treeass)
{
    Sprite*treeItem = Sprite::create(treeass);
    gameCon->addChild(treeItem);
    srand(int(time(0))+ rand());//置随机数种子
    int num=CCRANDOM_0_1()*100;
    int dic = num > 50?1:-1;
    treeItem->setScale(gameCon->getScaleY());
    treeItem->setScaleX(dic);
    treeItem->setAnchorPoint(Vec2(0,0.5));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    trees.pushBack(treeItem);
    treeItem->setPosition(Vec2((dic == 1?0:gameCon->getContentSize().width*gameCon->Node::getScaleX()),gameCon->getContentSize().height*gameCon->getScaleY() + treeItem->getContentSize().height));
    treeMove = true;
    float arrowoffy = 0;
    if(trees.size() == 1)
    {
        arrowoffy = abs(treeItem->getPositionY() - roleImg->getPositionY());
        
    }else if(trees.size() > 1){
        arrowoffy = abs(trees.at(trees.size()-2)->getPositionY() - roleImg->getPositionY());
    }
    if(trees.size() > 1)
    {
        auto arrowmove = MoveTo::create(0.5f, Vec2(arrow->getPositionX(),arrow->getPositionY() - arrowoffy));
        auto scaleTo = ScaleTo::create(0.35f, 1.0f);
        auto arrowCall = CallFunc::create([&](){
            rope->stopAllActions();
            rope->setScale(1.0f);
            arrow->setPosition(Vec2(400*gameCon->getScaleX(),368*gameCon->getScaleY()));
            this->schedule(schedule_selector(HelloWorld::tickTime), 0.016f);
            startSpread = true;
        });
        arrow->runAction(Sequence::create(arrowmove,arrowCall, NULL));
        rope->runAction(scaleTo);
    }
    
    for( auto c : trees)
    {

        if(c == treeItem)
        {
            float moveY = gameCon->getContentSize().height/2;
            if(trees.size() > 1)
            {
                float firstY = trees.at(0)->getPositionY();
                float randomY = CCRANDOM_0_1()*100 + 200;
                moveY += randomY;
            }
            auto callback = CallFunc::create([&](){
                treeMove = false;
            });
            auto moveTo = MoveTo::create(0.5f, Vec2(treeItem->getPositionX(),moveY));
            c->runAction(Sequence::create(moveTo,callback, NULL));
        }else{
            float offy = abs(c->getPositionY() - roleImg->getPositionY());
            if(offy <= 0.0001){offy = c->getPositionY() + c->getContentSize().height;}
            auto moveTo = MoveTo::create(0.5f, Vec2(c->getPositionX(),c->getPositionY() - offy));
            
            auto callback = CallFunc::create([&](){
                treeMove = false;
                bottomImg->setVisible(false);
                for(auto t : trees)
                {
                    if(t->getPositionY() < 0)
                    {
                        t->removeFromParent();
                        trees.eraseObject(t);
                    }
                }
            });
            c->runAction(Sequence::create(moveTo,callback, NULL));
            if(bottomImg->isVisible())
            {
                bottomImg->runAction(MoveTo::create(0.5f, Vec2(bottomImg->getPositionX(),-bottomImg->getContentSize().height)));
            }
        }
    }
    return treeItem;
}
void HelloWorld::resetData()
{
    startGroup->setVisible(true);
    scoreLab->setString("0");
    scoreLab->setVisible(false);
    int bgW = bg1->getContentSize().width;
    int bgH = bg1->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg1->setPosition(Vec2(0,0));
    gameCon->setPosition(Vec2(0,0));
    bottomImg->setPosition(Vec2(375*scaleX,0));
    bg2->setPosition(Vec2(0,1334*scaleY));
    bg3->setPosition(Vec2(0,2668*scaleY));
    scoreLab->setPosition(Vec2(375*scaleX,1205*scaleY));
    startGroup->setPosition(Vec2(visibleSize.width/2,830*scaleY));
    roleImg->setPosition(Vec2(388*scaleX,184*scaleY));
    arrow->setPosition(Vec2(400*scaleX,368*scaleY));
    rope->setScaleY(1.0f);
    rope->setPosition(Vec2(400*scaleX,264*scaleY));
    bottomImg->setVisible(true);
    bottomImg->setPosition(Vec2(375*scaleX,0));
    
    gameEnd = false;
    for(auto c : trees)
    {
        c->removeFromParent();
    }
    trees.clear();
}
