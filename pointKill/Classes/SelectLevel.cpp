//
//  SelectLevel.cpp
//  pointKill-mobile
//
//  Created by 志强 on 2020/5/30.
//

#include "SelectLevel.hpp"
#include "Config.h"

SelectLevel::SelectLevel(){
    
    auto rootNode = CSLoader::createNode("SelectLevel.csb");
    addChild(rootNode);
    bg = static_cast<Layout*>(rootNode->getChildByName("bg"));
    contentGroup = static_cast<Layout*>(rootNode->getChildByName("contentGroup"));
    closeBtn = static_cast<Sprite*>(contentGroup->getChildByName("closeBtn"));
    rightArrow = static_cast<Sprite*>(contentGroup->getChildByName("rightArrow"));
    leftArrow = static_cast<Sprite*>(contentGroup->getChildByName("leftArrow"));
    scroller = static_cast<ScrollView*>(contentGroup->getChildByName("scroller"));
    scroller->setSwallowTouches(false);
    con = static_cast<Layout*>(scroller->getChildByName("con"));
    _listener=EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(SelectLevel::menuBegin,this);
    _listener->onTouchEnded = CC_CALLBACK_2(SelectLevel::menuEndCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, contentGroup);
    
    for(int i = 1;i<=5;i++)
    {
        std::string levelkey = StringUtils::format("level%d",i);
        Sprite*levelItem = static_cast<Sprite*>(con->getChildByName(levelkey));
        levelItem->setName("item");
        if(i > Config::getInstance()->level)
        {
            Sprite*lock = Sprite::create("ui/level_lock.png");
            con->addChild(lock);
            
            lock->setPosition(levelItem->getPosition());
            levelItem->setTag(0);
        }
    }
    
    this->adapter();
}
bool SelectLevel::menuBegin(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        closeBtn->setScale(0.9f);
        return true;
    }else if(scroller->getBoundingBox().containsPoint(localP))
    {
        beginP = localP;
        return true;
    }else if(leftArrow->getBoundingBox().containsPoint(localP))
    {
        leftArrow->setScale(0.9f);
        return true;
    }else if(rightArrow->getBoundingBox().containsPoint(localP))
    {
        rightArrow->setScale(0.9f);
        return true;
    }
    
    return false;
}
void SelectLevel::menuEndCallback(cocos2d::Touch* tTouch,cocos2d::Event* eEvent)
{
    Point localP = contentGroup->convertToNodeSpace(tTouch->getLocation());
    
    if(closeBtn->getBoundingBox().containsPoint(localP))
    {
        closeBtn->setScale(1.0f);
        if(callback)
        {
            callback(0);
        }
        this->removeFromParent();
    }else if(scroller->getBoundingBox().containsPoint(localP))
    {
        if(beginP.x == localP.x && beginP.y == localP.y)
        {
            Point scrollP = con->convertToNodeSpace(tTouch->getLocation());
            Vector<Node*> children = con->getChildren();
            int index= 0;
            for(auto c : children)
            {
                index += 1;
                if(c->getName() == "item" && c->getBoundingBox().containsPoint(scrollP))
                {
                    if(c->getTag() == 0)
                    {
                        this->floatFont("Please clear the first level");
                    }else{
                        Config::getInstance()->curLevel = index;
                        this->removeFromParent();
                        if(callback)
                        {
                            callback(index);
                        }
                    }
                }
            }
        }
    }else if(rightArrow->getBoundingBox().containsPoint(localP))
    {
        rightArrow->setScale(1.0f);
        scroller->scrollToRight(0.5f, true);
    }else if(leftArrow->getBoundingBox().containsPoint(localP))
    {
        leftArrow->setScale(1.0f);
        scroller->scrollToLeft(0.5f, true);
    }
}
void SelectLevel::adapter()
{
    int bgW = bg->getContentSize().width;
    int bgH = bg->getContentSize().height;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float scaleX = visibleSize.width/bgW;
    float scaleY = visibleSize.height/bgH;
    bg->setScale(scaleX, scaleY);
    contentGroup->setScale(scaleX);
    contentGroup->setPosition(Vec2(contentGroup->getPositionX()*scaleX,contentGroup->getPositionY()*scaleY));
//    bg->setScale(scaleX, scaleY);
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
SelectLevel::~SelectLevel()
{
    _eventDispatcher->removeEventListener(_listener);
}
