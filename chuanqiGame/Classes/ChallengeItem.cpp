//
//  ChallengeItem.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/11.
//

#include "ChallengeItem.hpp"
#include "Config.h"

ChallengeItem::ChallengeItem(int levelIndex)
{
    auto rootNode = CSLoader::createNode("ChallengeItem.csb");

    addChild(rootNode);
    this->setContentSize(Size(606,160));
    noActive = static_cast<Layout*>(rootNode->getChildByName("noActive"));
    levelLab = static_cast<TextBMFont*>(rootNode->getChildByName("levelLab"));
    challengeBtn = static_cast<Sprite*>(rootNode->getChildByName("challengeBtn"));
    levelbg = static_cast<Sprite*>(rootNode->getChildByName("levelbg"));
    
    levelLab->setString(StringUtils::format("%d",levelIndex));
    std::string bgass = StringUtils::format("challenge/level_%d.png",levelIndex);
    Texture2D*texture = Director::getInstance()->getTextureCache()->addImage(bgass);
    levelbg->setTexture(texture);
    noActive->setVisible(CF()->t_chapter < levelIndex);
    challengeBtn->setVisible(CF()->t_chapter >= levelIndex);
}
ChallengeItem::~ChallengeItem()
{
    
}
