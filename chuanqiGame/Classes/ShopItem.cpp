//
//  ShopItem.cpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#include "ShopItem.hpp"

ShopItem::ShopItem()
{
    auto rootNode = CSLoader::createNode("ShopItem.csb");

    addChild(rootNode);
    this->setContentSize(Size(602,148));
    skillNameLab = static_cast<Text*>(rootNode->getChildByName("skillNameLab"));
    costLab = static_cast<Text*>(rootNode->getChildByName("costLab"));
    skillDescLab = static_cast<Text*>(rootNode->getChildByName("skillDescLab"));
    buyBtn = static_cast<Sprite*>(rootNode->getChildByName("buyBtn"));
    skillIcon = static_cast<Sprite*>(rootNode->getChildByName("skillIcon"));
}
void ShopItem::refreshInfo(SkillCfg*cfg)
{
    skillCfg = cfg;
    if(!cfg->isUnlock)
    {
        buyBtn->setVisible(false);
    }
    Texture2D*texture;
    if(cfg->isUnlock)
    {
        texture = Director::getInstance()->getTextureCache()->addImage("shop/shop_lock.png");
    }else{
        std::string skillass = StringUtils::format("battle/battle_skill_%d.png",cfg->skillIndex);
        texture = Director::getInstance()->getTextureCache()->addImage(skillass);
    }
    skillIcon->setTexture(texture);
    skillNameLab->setString(cfg->skillName);
    costLab->setString(StringUtils::format("%d",cfg->lockNum));
    skillDescLab->setString(cfg->skillDesc);
}
ShopItem::~ShopItem()
{
    
}
