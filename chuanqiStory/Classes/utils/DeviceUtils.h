//
//  DeviceUtils.h
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/4/7.
//

#ifndef DeviceUtils_h
#define DeviceUtils_h

struct DeviceScaleInfo
{
    float scaleX;
    float scaleY;
    const char*model;
    int sdkVersion;
    const char*osVersion;
    const char*phoneNumber;
};
#include <iostream>


const char* getModel();
  
int getSdkVersion();
  
const char* getOsVersion();
  
const char* getPhoneNumber();

#endif /* DeviceUtils_h */
