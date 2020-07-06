//
//  SelectTypeScene.cpp
//  Puzzle-mobile
//
//  Created by 志强 on 2020/5/19.
//

#include "SelectTypeScene.h"
#include "SoundCtl.h"
#include "HelloWorldScene.h"
#include "Config.h"
#include "GameScene.h";
Scene* SelectTypeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectTypeScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectTypeScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("SelectTypeScene.csb");

    addChild(rootNode);
    closeBtn = static_cast<Sprite*>(rootNode->getChildByName("closeBtn"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    level1 = static_cast<Sprite*>(contentGroup->getChildByName("level1"));
    level2 = static_cast<Sprite*>(contentGroup->getChildByName("level2"));
    level3 = static_cast<Sprite*>(contentGroup->getChildByName("level3"));
    level4 = static_cast<Sprite*>(contentGroup->getChildByName("level4"));
    bg = static_cast<Sprite*>(rootNode->getChildByName("bg"));
    
    piecesGroup = static_cast<Layout*>(rootNode->getChildByName("piecesGroup"));
    difficult1 = static_cast<Sprite*>(piecesGroup->getChildByName("difficult1"));
    difficult2 = static_cast<Sprite*>(piecesGroup->getChildByName("difficult2"));
    difficult3 = static_cast<Sprite*>(piecesGroup->getChildByName("difficult3"));
    piecesGroup->setVisible(false);
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SelectTypeScene::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SelectTypeScene::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    for(int i = 1 ; i<=4;i++)
    {
        std::string ass = StringUtils::format("sprites/level%d_lock.png",i);
        std::string levelkey = StringUtils::format("level%d",i);
        if(i <= Config::getInstance()->openLevel )
        {
            ass = StringUtils::format("sprites/level%d.png",i);
        }
        Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(ass);
        static_cast<Sprite*>(contentGroup->getChildByName(levelkey))->setTexture(texture);
    }
    this->adapter();
    return true;
}
void SelectTypeScene::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    contentGroup->setScale(scaleX);
    contentGroup->setPositionX(contentGroup->getPositionX()*scaleX);
    closeBtn->setPositionX(closeBtn->getPositionX()*scaleX);
}
void SelectTypeScene::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_listener);
}
bool SelectTypeScene::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    
    if(closeBtn->getBoundingBox().containsPoint(localP)){
       closeBtn->setScale(0.9f);
       return true;
    }else if((level1->getBoundingBox().containsPoint(groupP) || level2->getBoundingBox().containsPoint(groupP) || level3->getBoundingBox().containsPoint(groupP) || level4->getBoundingBox().containsPoint(groupP) ) && contentGroup->isVisible())
    {
        return true;
        
    }else if(piecesGroup->getBoundingBox().containsPoint(localP) && piecesGroup->isVisible()){
        Point piecesP = piecesGroup->convertToNodeSpace(tTouch->getLocation());
        if(difficult1->getBoundingBox().containsPoint(piecesP) || difficult2->getBoundingBox().containsPoint(piecesP) || difficult3->getBoundingBox().containsPoint(piecesP) )
        {
            if(piecesGroup->isVisible())
            {
                return true;
            }else{
                return false;
            }
        }
    }
    return false;
}
void SelectTypeScene::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = this->convertToNodeSpace(tTouch->getLocation());
    Point groupP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    Point piecesP = piecesGroup->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(localP)){
        closeBtn->setScale(1.0f);
        //打开布阵弹窗
        SoundCtl::getInstance()->playEffect("sound/click.m4a");
        if(contentGroup->isVisible())
        {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
        }else{
            piecesGroup->setVisible(false);
            contentGroup->setVisible(true);
            Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("sprites/bg_game1.png");
            bg->setTexture(texture);
        }
        return;
    }else if(level1->getBoundingBox().containsPoint(groupP) && contentGroup->isVisible())
    {
        this->selectPicitem(1);
    }else if(level2->getBoundingBox().containsPoint(groupP) && contentGroup->isVisible())
    {
        this->selectPicitem(2);
    }else if(level3->getBoundingBox().containsPoint(groupP) && contentGroup->isVisible())
    {
        this->selectPicitem(3);
    }else if(level4->getBoundingBox().containsPoint(groupP) && contentGroup->isVisible())
    {
        this->selectPicitem(4);
    }
    else if(difficult1->getBoundingBox().containsPoint(piecesP) && piecesGroup->isVisible())
    {
        Config::getInstance()->level = 1;
        Config::getInstance()->row = 4;
        Config::getInstance()->col = 4;
        Config::getInstance()->allNum = 16;
        this->transToGameScene();
    }else if(difficult2->getBoundingBox().containsPoint(piecesP) && piecesGroup->isVisible())
    {
        Config::getInstance()->level = 2;
        Config::getInstance()->row = 6;
        Config::getInstance()->col = 6;
        Config::getInstance()->allNum = 36;
        this->transToGameScene();
    }else if(difficult3->getBoundingBox().containsPoint(piecesP) && piecesGroup->isVisible())
    {
        Config::getInstance()->level = 3;
        Config::getInstance()->row = 8;
        Config::getInstance()->col = 8;
        Config::getInstance()->allNum = 64;
        this->transToGameScene();
    }
}
void SelectTypeScene::transToGameScene()
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}
void SelectTypeScene::selectPicitem(int itemIndex)
{
    if(itemIndex > Config::getInstance()->openLevel)
    {
        this->floatFont("Please clear the first level");
        return;
    }
//    Config::getInstance()->level = itemIndex;
//    Config::getInstance()->row = 4;
//    Config::getInstance()->col = 4;
//    this->transToGameScene();
    Config::getInstance()->picIndex = itemIndex;
    contentGroup->setVisible(false);
////    picAss = StringUtils::format("sprites/image_%d.jpg",itemIndex);
    piecesGroup->setVisible(true);
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage("sprites/difficultbg.png");
    bg->setTexture(texture);
}
void SelectTypeScene::floatFont(std::string cnt)
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
