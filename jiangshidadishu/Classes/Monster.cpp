//
//  Monster.cpp
//  jiangshidadishu-mobile
//
//  Created by youxi123 on 2020/6/3.
//

#include "Monster.h"

Monster::Monster()
{
    this->init();
}
bool Monster::init()
{
    this->setContentSize(Size(152, 162));
    this->setAnchorPoint(Vec2(0.5, 0));
    ani = new MovieClip();
    this->addChild(ani);
    icon = Sprite::create();
    this->addChild(icon);
    return true;
}
void Monster::releasePage()
{
    ani->stopAllActions();
    this->stopAllActions();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    this->removeFromParent();
    this->release();
}

void Monster::setAni(int _id, std::string _bg_id)
{
    monster_id = _id;
    bg_id = _bg_id;
    std::string iconName;
    switch (_id) {
        case 1:
            ani->playFile("zombie1.plist", "zombie1_", 1, 4, 1, false);
            iconName = "zombie1_4.png";
            break;
        case 2:
            ani->playFile("zombie2.plist", "zombie2_", 1, 4, 1, false);
            iconName = "zombie2_4.png";
            break;
        case 3:
            ani->playFile("zombie3.plist", "zombie3_", 1, 4, 1, false);
            iconName = "zombie3_4.png";
            break;
        case 4:
            ani->playFile("human.plist", "human_", 1, 4, 1, false);
            iconName = "human_4.png";
            break;
        case 5:
            ani->playFile("corazon.plist", "corazon_", 1, 4, 1, false);
            iconName = "corazon_4.png";
            break;
        default:
            break;
    }
    ani->setAnchorPoint(Vec2(0.5, 0));
    ani->setPosition(0, 0);
    auto delayTime = DelayTime::create(0.23);
    auto func = CallFunc::create([this, iconName]()
    {
        ani->setVisible(false);
        icon->setTexture("game/" + iconName);
        icon->setAnchorPoint(Vec2(0.5, 0));
        icon->setPosition(0, -70);
        auto delayTime0 = DelayTime::create(2.0f);
        auto func0 = CallFunc::create([this]()
        {
            this->monsterAttack();
        });
        auto seq0 = Sequence::create(delayTime0, func0, nullptr);
        this->runAction(seq0);
    });
    auto seq1 = Sequence::create(delayTime, func, nullptr);
    this->runAction(seq1);
}

void Monster::monsterAttack()
{
    ani->setVisible(true);
    icon->setVisible(false);
    switch (monster_id) {
        case 1:
            ani->playFile("zombie1_drop.plist", "zombie1_drop_", 1, 4, 1, true);
            break;
        case 2:
            ani->playFile("zombie2_drop.plist", "zombie2_drop_", 1, 4, 1, true);
            break;
        case 3:
            ani->playFile("zombie3_drop.plist", "zombie3_drop_", 1, 4, 1, true);
            break;
        case 4:
            ani->playFile("human_drop.plist", "human_drop_", 1, 4, 1, true);
            break;
        case 5:
            ani->playFile("corazon_drop.plist", "corazon_drop_", 1, 4, 1, true);
            break;
        default:
            break;
    }
    ani->setAnchorPoint(Vec2(0.5, 0));
    ani->setPosition(0, 0);
    
    auto delayTime = DelayTime::create(0.23);
    auto func = CallFunc::create([this]()
    {
        drop = true;
    });
    auto seq1 = Sequence::create(delayTime, func, nullptr);
    this->runAction(seq1);
}

