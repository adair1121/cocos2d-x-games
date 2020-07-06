//
//  MessageCenter.h
//  wuxiaCocosProject
//
//  Created by 志强 on 2020/3/25.
//

#ifndef MessageCenter_h
#define MessageCenter_h

#define MC() MessageCenter::getInstance()

#include "cocos2d.h"

USING_NS_CC;

class  MessageCenter
{
public:
    typedef std::function<void(EventCustom*)> EventFun;
    virtual ~MessageCenter();
public:
    static MessageCenter *getInstance();
    void release();

public:

    void dispatchEvent(const std::string &eventName, void *object);
    
    void dispatchEvent(const std::string &eventName, std::vector<std::string> *datavecs);
    
    void dispatchEvent(const std::string &eventName);
    
    EventListenerCustom* addListener(const std::string &eventName,const EventFun& callback);
    /* 移除侦听 */
    void removeListener(const std::string& eventName);
    /* 移除所有侦听 */
    void removeAllListeners();
    
    
private:
    void init();
    MessageCenter();
private:
    EventDispatcher* _eventDispatcher;
    std::map<std::string,EventListenerCustom*> m_hasAddEvtList;//已经侦听的事件
};

#endif
