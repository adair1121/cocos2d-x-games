//
//  ScoreEffect.cpp
//  jiangshidadishu-mobile
//
//  Created by youxi123 on 2020/6/3.
//

#include "ScoreEffect.h"

ScoreEffect::ScoreEffect()
{
    this->init();
}
bool ScoreEffect::init()
{
    auto rootNode = CSLoader::createNode("ScoreEffect.csb");
    this->setContentSize(Size(230, 100));
    scoreText = static_cast<TextBMFont*>(rootNode->getChildByName("scoreText"));
    return true;
}
void ScoreEffect::releasePage()
{
    this->stopAllActions();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    this->removeFromParent();
    this->release();
}

void ScoreEffect::moveScore(int num)
{
    scoreText->setString(std::to_string(num));
    std::string str = scoreText->getString();
//    auto moveTo = MoveTo::create(20.0f, Vec2(this->getPositionX(), this->getPositionY()+50));
//    auto func = CallFunc::create([this]()
//    {
//        this->releasePage();
//    });
//    auto seq = Sequence::create(moveTo, func, nullptr);
//    this->runAction(seq);
}
