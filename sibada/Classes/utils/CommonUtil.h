//
//  CommonUtil.h
//  wuxiaCocosProject
//
//  Created by 志强 on 2020/4/1.
//

#ifndef CommonUtil_h
#define CommonUtil_h
#include "cocos2d.h"
#include <sstream>

using namespace std;

template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

USING_NS_CC;
class CommonUtil {
    
    
public:
    
    static  vector<string>  split(const string& src, const string& separator)
    {
        vector<string>dest;

        string str = src;

        string substring;

        string::size_type start = 0, index;



        do

        {

            index = str.find_first_of(separator,start);

            if (index != string::npos)

            {

                substring = str.substr(start,index-start);

                dest.push_back(substring);

                start = str.find_first_not_of(separator,index);

                if (start == string::npos) return dest;

            }

        }while(index != string::npos);



        //the last token

        substring = str.substr(start);

        dest.push_back(substring);



        return dest;

    }
    
    static string returnAttr(const string & attr)
    {
        if(attr == "hp")
        {
            return "生命";
        }else if(attr == "atk")
        {
            return "攻击";
        }else if(attr == "sp")
        {
            return "内力";
        }else if(attr == "def")
        {
            return "防御";
        }else if(attr == "luk")
        {
            return "暴击";
        }else if(attr == "kai")
        {
            return "抗暴";
        }else if(attr == "hit")
        {
            return "命中";
        }else if(attr == "evd")
        {
            return "命中";
        }else if(attr == "none" || attr == "")
        {
            return "";
        }
    }
    static EaseInOut* createParabola(float t, Point startPoint, Point endPoint, float height /* = 0 */, float angle /* = 60 */){
     
        Size visiblity = Director::getInstance()->getVisibleSize();
        int dic = (startPoint.x > visiblity.width/2)?-1:1;
        // 把角度转换为弧度
        float radian = angle*3.14159/180.0;
        // 第一个控制点为抛物线左半弧的中点
        float q1x = startPoint.x+(abs(endPoint.x - startPoint.x)/4.0)*dic;
        Point q1 = Point(q1x, height);
        // 第二个控制点为整个抛物线的中点
        float q2x = startPoint.x + (abs(endPoint.x - startPoint.x)/2.0)*dic;
        Point q2 = Point(q2x, height);
        
        //曲线配置
        ccBezierConfig cfg;
        cfg.controlPoint_1 = q1;
        cfg.controlPoint_2 = q2;
        cfg.endPosition = endPoint;
        //使用EaseInOut让曲线运动有一个由慢到快的变化，显得更自然
        return EaseInOut::create(BezierTo::create(t,cfg),0.5);
    }
    
    static std::string longTransString(long value)
    {
        ostringstream os;
        os<<value;
        string result;
        istringstream is(os.str());
        is>>result;
        return result;
    }

};
#endif /* CommonUtil_h */
