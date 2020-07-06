//
//  MessageCenter.cpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/3/25.
//

#include "MessageCenter.h"
#include <algorithm>

USING_NS_CC;

MessageCenter::~MessageCenter()
{
    removeAllListeners();
    _eventDispatcher->release();
}

MessageCenter::MessageCenter()
{

}

void MessageCenter::init()
{
    Director *director = Director::getInstance();
    _eventDispatcher = director->getEventDispatcher();
    _eventDispatcher->retain();
}


static MessageCenter *s_pInstance = NULL; // pointer to singleton

MessageCenter* MessageCenter::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new MessageCenter();
        if (s_pInstance)
        {
            s_pInstance->init();
        }
    }

    return s_pInstance;
}


//发送事件,name：事件名字

void MessageCenter::dispatchEvent(const std::string &eventName, void *object)
{
    _eventDispatcher->dispatchCustomEvent(eventName,object);
}
 void MessageCenter::dispatchEvent(const std::string &eventName, std::vector<std::string> *datavecs)
{
    _eventDispatcher->dispatchCustomEvent(eventName,datavecs);
}
 void MessageCenter::dispatchEvent(const std::string &eventName)
{
    _eventDispatcher->dispatchCustomEvent(eventName);
}

EventListenerCustom* MessageCenter::addListener(const std::string &eventName,const std::function<void(EventCustom*)>& callback)
{
    removeListener(eventName);
    EventListenerCustom* evt = _eventDispatcher->addCustomEventListener(eventName,callback);
    m_hasAddEvtList[eventName] = evt;
    return evt;
}


//删除侦听

void  MessageCenter::release()
{
    s_pInstance = nullptr;
    delete this;
}
void MessageCenter::removeListener(const std::string& eventName)
{
    if(m_hasAddEvtList.find(eventName) != m_hasAddEvtList.end())
    {

        EventListenerCustom* pListener = m_hasAddEvtList[eventName];
        if(pListener == nullptr)
       {
           return;
       }
       for(std::map<std::string,EventListenerCustom*>::iterator iter = m_hasAddEvtList.begin();iter != m_hasAddEvtList.end();++iter)
       {
           if(iter->second == pListener)
           {
               m_hasAddEvtList.erase(iter);
               break;
           }
       }
       _eventDispatcher->removeEventListener(pListener);
    }
}
void MessageCenter::removeAllListeners()
{
    for(auto& iter:m_hasAddEvtList)
    {
        _eventDispatcher->removeEventListener(iter.second);
    }
    m_hasAddEvtList.clear();
}
