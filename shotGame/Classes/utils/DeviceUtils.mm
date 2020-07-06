//
//  DeviceUtils.cpp
//  wuxiaCocosProject-mobile
//
//  Created by 志强 on 2020/4/7.
//

#import "DeviceUtils.h"
  
const char* getModel() {
    NSString *strModel = [[UIDevice currentDevice] model];
    return [strModel UTF8String];
}
  
int getSdkVersion() {
    return 0;
}
  
const char* getOsVersion() {
    NSString *strSysVersion = [[UIDevice currentDevice] systemVersion];
    return [strSysVersion UTF8String];
}
  
const char* getPhoneNumber() {
    return "";
}
