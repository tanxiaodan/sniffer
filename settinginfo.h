// =====================================================================================
// 
//       Filename:  settinginfo.h
//
//    Description:  程序设置信息类的头文件
//
//        Version:  1.0
//        Created:  2013年01月27日 14时15分49秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#ifndef SETTINGINFO_H_
#define SETTINGINFO_H_

// 程序配置信息类
struct SettingInfo
{
	int  iOpenDevNum;			// 要打开的适配器编号
	bool bPromiscuous;			// 是否以混杂模式打开适配器
    bool bAutoBegin;			// 选择适配器后自动捕获

    SettingInfo()
    {
    	iOpenDevNum  = 0;
		bPromiscuous = true;
    	bAutoBegin   = false;
    }
};

#endif	// SETTINGINFO_H_
