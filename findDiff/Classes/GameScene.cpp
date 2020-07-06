//
//  GameScene.cpp
//  FindDiff-mobile
//
//  Created by 志强 on 2020/5/15.
//

#include "GameScene.h"
#include "HelpInfoTip.h"
#include "SoundCtl.h"
#include "PausePop.h"
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
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("GameScene.csb");

    addChild(rootNode);
    
    auto delay = DelayTime::create(1.0f);
    auto callBack = CallFunc::create([&](){
        HelpInfoTip*infoTip = new HelpInfoTip();
//        infoTip->setAnchorPoint(Vec2(0.5,0.5));
        Size visibleSize = Director::getInstance()->getVisibleSize();
        infoTip->callFunc = CC_CALLBACK_0(GameScene::startGame, this);
//        infoTip->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
        this->addChild(infoTip);
    });
    auto seque = Sequence::create(delay,callBack, NULL);
    this->runAction(seque);
    
    hitCountLab = static_cast<Text*>(rootNode->getChildByName("hitCountLab"));
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    soundIcon = static_cast<Sprite*>(rootNode->getChildByName("soundIcon"));
    group = static_cast<Layout*>(rootNode->getChildByName("group"));
    goodCon = static_cast<Layout*>(rootNode->getChildByName("goodCon"));
    timeIcon = static_cast<Sprite*>(rootNode->getChildByName("timeIcon"));
    gameOverBtn =  static_cast<Sprite*>(rootNode->getChildByName("gameOverBtn"));
    nextLevelBtn = static_cast<Sprite*>(rootNode->getChildByName("nextLevelBtn"));
    soundGroup = static_cast<Layout*>(rootNode->getChildByName("soundGroup"));
    passGroup = static_cast<Layout*>(rootNode->getChildByName("passGroup"));
    gameOverBtn ->setVisible(false);
    nextLevelBtn->setVisible(false);
    
    float num = curCount/60*100;
    
    Sprite * mask = Sprite::create("assets/vrijemePuno.png");
    
    Sprite *cal=Sprite::create("assets/vrijemePuno.png");
    
    maskSp = maskedSpriteWithSprite(cal, mask,Vec2(0,0));
    
    maskSp->setPosition(timeIcon->getPositionX(),timeIcon->getPositionY());
    
    addChild(maskSp);
    
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(GameScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(GameScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    hitCountLab->setString("3");
    this->adapter();
    this->resetData();
    
    return true;
}
void GameScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    hitCountLab->setPositionX(hitCountLab->getPositionX()*scaleX);
    soundIcon->setPositionX(soundIcon->getPositionX()*scaleX);
    passGroup-> setPositionX(passGroup->getPositionX()*scaleX);
    soundGroup->setPositionX(soundGroup->getPositionX()*scaleX - 20);
    gameOverBtn->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    nextLevelBtn->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    goodCon->setPositionX(goodCon->getPositionX()*scaleX - 20);
    goodCon->setContentSize(Size(goodCon->getContentSize().width*scaleX,goodCon->getContentSize().height*scaleY));
}
bool GameScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    
    return true;
}
   
void GameScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    if(hitCountLab->getBoundingBox().containsPoint(localP) && !resetState && !nextState)
    {
        if(hintBoo == false && hitCount > 0)
        {
            hintBoo = true;
            srand(int(time(0))+ rand());//置随机数种子
            Vector<Node*> children = goodCon->getChildren();
            int ran=CCRANDOM_0_1()*children.size();
            hintItem = static_cast<Sprite*>(children.at(ran));
            auto sprite = Sprite::create("assets/hint2.png");
            sprite->setName("focus");
            goodCon->addChild(sprite);
            sprite->setOpacity(0.0f);
            hitCount -= 1;
            hitCountLab->setString(StringUtils::format("%d",hitCount));
            auto fadeIn = FadeIn::create(1.0f);
            auto fadeout = FadeOut::create(1.0f);
            auto seque = Sequence::create(fadeIn,fadeout, NULL);
            sprite->runAction(RepeatForever::create(seque));
            sprite->setPosition(Vec2(hintItem->getPosition().x,hintItem->getPosition().y));
        }
        SC()->playEffect("sound/click.m4a");
        return;
    }else if(soundGroup->getBoundingBox().containsPoint(localP)  && !resetState && !nextState)
    {
        std::string iconass = "";
        if(SC()->musicBool)
        {
            iconass = "assets/sound-off.png";
            SC()->stopBg();
        }else{
            iconass = "assets/sound-on.png";
            SC()->playBg("sound/Wizard.m4a", true);
        }
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(iconass);
        soundIcon->setTexture(texture);
        SC()->playEffect("sound/click.m4a");
        return;
    }else if(passGroup->getBoundingBox().containsPoint(localP) && !resetState && !nextState)
    {
        PausePop*pause = new PausePop();
        pause->callFunc = CC_CALLBACK_0(GameScene::startGame, this);
        this->addChild(pause);
        //暂停
        this->stopGame();
        return;
    }else if(nextLevelBtn->getBoundingBox().containsPoint(localP) && nextState)
    {
        nextState = false;
        levelIndex += 1;
        if(levelIndex >= 6){levelIndex = 1;}
        nextLevelBtn->setVisible(false);
        this->resetData();
        return;
    }else if(gameOverBtn->getBoundingBox().containsPoint(localP) && resetState)
    {
        resetState = false;
        gameOverBtn->setVisible(false);
        this->resetData();
        this->startGame();
        return;
    }
    if(resetState || nextState)
    {
        return;
    }
    Point groupP = goodCon->convertToNodeSpace(tTouch->getLocation());
    Vector<Node*> children = goodCon->getChildren();
    bool correct = false;
    for(auto c : children)
    {
        Sprite*item = static_cast<Sprite*>(c);
        if(c->getBoundingBox().containsPoint(groupP))
        {
            if(c->getName() == "focus")
            {
                hintBoo = false;
                hintItem->removeFromParent();
                hintItem = NULL;
            }
            if(hintItem && c->getPosition().x == hintItem->getPosition().x && c->getPosition().y == hintItem->getPosition().y)
            {
                continue;
            }
            c->removeFromParent();
            this->reduceStar();
            correct = true;
            SC()->playEffect("sound/click.m4a");
            break;
        }
    }
    std::string assets = correct?"assets/objPogodio.png":"assets/objFalio.png";
    Sprite*floatSprite = Sprite::create(assets);
    this->addChild(floatSprite);
    floatSprite->setScale(0.0f);
    floatSprite->setPosition(localP.x,localP.y);
    auto moveTo = MoveTo::create(1.0f, Vec2(localP.x,localP.y + 50));
    auto scaleTo = ScaleTo::create(1.0f, 1.0f);
    auto fadeOut = FadeOut::create(1.0f);
    auto swapn = Spawn::create(moveTo,scaleTo,fadeOut, NULL);
    auto callBack = CallFunc::create([=](){
        floatSprite->removeFromParent();
    });
    auto seque = Sequence::create(swapn,callBack, NULL);
    floatSprite->runAction(seque);
}
void GameScene::reduceStar()
{
    //减少星星
    ansterNum +=1;
    if(ansterNum >= 10)
    {
        //游戏胜利
        this->nextLevelBtn->setVisible(true);
        auto scaleTo = ScaleTo::create(1.0f, 1.5f);
        auto scaleFrom = ScaleTo::create(1.0f, 1.0f);
        auto seque = Sequence::create(scaleTo,scaleFrom, NULL);
        nextLevelBtn->runAction(RepeatForever::create(seque));
        nextState = true;
    }
    for(int i = 1;i<=10;i++)
    {
        std::string starKey = StringUtils::format("star%d",i);
        Sprite*item = static_cast<Sprite*>(group->getChildByName(starKey));
        if(i <= ansterNum)
        {
            item->setCascadeOpacityEnabled(true);
            item->setOpacity(0.0f);
        }
    }
}
void GameScene::resetData()
{
    goodCon->removeAllChildren();
    for(int i = 1;i<=goodNum;i++)
    {
        std::string itemass = i == 10?"10.png":StringUtils::format("0%d.png",i);
        std::string assets = StringUtils::format("assets/obj0%d-",levelIndex)+itemass;
        Sprite*item = Sprite::create(assets);
        goodCon->addChild(item);
        item->setTag(i);
        srand(int(time(0))+ rand());//置随机数种子
        int ranx=CCRANDOM_0_1()*goodCon->getContentSize().width;
        int rany = CCRANDOM_0_1()*goodCon->getContentSize().height;
        item->setPosition(Vec2(ranx,rany));
    }
    ansterNum = 0;
    Vector<Node*> starChildren = group->getChildren();
    curCount = 0;
    nextState = false;
    resetState = false;
    hintBoo = false;
    for(auto c : starChildren)
    {
        Sprite*item = static_cast<Sprite*>(c);
        item->setOpacity(255);
    }
    gameOverBtn ->setVisible(false);
    nextLevelBtn->setVisible(false);
    gameOverBtn->stopAllActions();
    nextLevelBtn->stopAllActions();
    if(maskSp)
    {
        maskSp->removeFromParent();
        maskSp = NULL;
    }
    
    float num = curCount/60*100;
    
    Sprite * mask = Sprite::create("assets/vrijemePuno.png");
    
    Sprite *cal=Sprite::create("assets/vrijemePuno.png");
    
    maskSp = maskedSpriteWithSprite(cal, mask,Vec2(0,0));
    
    maskSp->setPosition(timeIcon->getPositionX(),timeIcon->getPositionY());
    
    addChild(maskSp);
    std::string bgstr = StringUtils::format("assets/bck0%d.jpg",levelIndex);
    Texture2D*bgtexture = Director::getInstance()->getTextureCache()->addImage(bgstr);
    bg->setTexture(bgtexture);
}
void GameScene::startGame()
{
   this->schedule(schedule_selector(GameScene::updateTime), 1.0f);
}
void GameScene::updateTime(float dt)
{
    curCount += 1;
    if(maskSp)
    {
        maskSp->removeFromParent();
        maskSp = NULL;
    }
    float cur = float(curCount);
    float num = cur/60.0f*100.0f;
    
    Sprite * mask = Sprite::create("assets/vrijemePuno.png");
    
    Sprite *cal=Sprite::create("assets/vrijemePuno.png");
    
    maskSp = maskedSpriteWithSprite(cal, mask,Vec2(0,-num));
    
    maskSp->setPosition(timeIcon->getPositionX(),timeIcon->getPositionY());
    
    addChild(maskSp);
    if(curCount >= 45)
    {
        this->unschedule(schedule_selector(GameScene::updateTime));
        gameOverBtn->setVisible(true);
        auto scaleTo = ScaleTo::create(1.0f, 1.5f);
        auto scaleFrom = ScaleTo::create(1.0f, 1.0f);
        auto seque = Sequence::create(scaleTo,scaleFrom, NULL);
        gameOverBtn->runAction(RepeatForever::create(seque));
        resetState = true;
    }
}
void GameScene::stopGame()
{
    this->unschedule(schedule_selector(GameScene::updateTime));
}
void GameScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
    levelIndex = 1;
    hitCount = 3;
}
Sprite* GameScene::maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite,Vec2 maskP)
{
    RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,

    maskSprite->getContentSize().height );
    
    float x = maskSprite->getContentSize().width/2 + maskP.x;
    float y = maskSprite->getContentSize().height/2 + maskP.y;
    
    maskSprite->setPosition(x,y);
    

    textureSprite->setPosition(textureSprite->getContentSize().width/2,textureSprite->getContentSize().height/2);
    

    maskSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );

    textureSprite->setBlendFunc( BlendFunc{GL_DST_ALPHA, GL_ZERO} );

    rt->begin();

    maskSprite->visit();

    textureSprite->visit();

    rt->end();

    Sprite *retval = Sprite::createWithTexture(rt->getSprite()->getTexture());

    retval->setFlippedY(true);

    return retval;
}
