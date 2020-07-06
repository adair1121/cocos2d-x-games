//
//  SkillCfg.hpp
//  chuanqiGame-mobile
//
//  Created by 志强 on 2020/6/10.
//

#ifndef SkillCfg_hpp
#define SkillCfg_hpp

USING_NS_CC;

using namespace cocos2d::ui;

class SkillCfg : public cocos2d::Node {
    
public:
    SkillCfg();
    
    std::string skillName;
    
    int skillIndex;
    
    int lockNum;
    
    bool isUnlock;
    
    std::string skillDesc;
};
#endif /* SkillCfg_hpp */
