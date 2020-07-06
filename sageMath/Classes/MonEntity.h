//
//  MonEntity.h
//  sageMath-mobile
//
//  Created by 志强 on 2020/5/21.
//

#ifndef MonEntity_h
#define MonEntity_h
#include "MovieClip.h"

#include "cocos2d.h"
USING_NS_CC;
class MonEntity : public cocos2d::Node{
private:
    MovieClip*mc;
    int oper;
    std::string ass;
    int showNum;
public:
    MonEntity(int num);
    ~MonEntity();
public:
    void changeAction(std::string actionstr);
    
    int getRemainNum();
};
#endif /* MonEntity_h */
