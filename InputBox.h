#ifndef _INPUTBOX_H
#define _INPUTBOX_H

#include <Windows.h>

// 回调函数格式，参数为字符串和用户数据
typedef BOOL (CALLBACK *PCheckInput)(LPCTSTR lpString, LPVOID lpParam);

BOOL InputBox(
    IN HINSTANCE hInstance,                     // 窗口过程所在的模块句柄
    IN HWND hParentWnd,                         // 父窗口句柄
    IN HICON hIcon,                             // 标题栏图标句柄
    IN LPCWSTR lpMessage,                       // 输入框之上的提示文字
    IN BOOL bNumber,                            // 是否是数字输入框
    IN BOOL bPassword,                          // 是否是密码输入框
    IN DWORD dwLimitText,                       // 输入长度限制
    IN OUT LPTSTR lpResult,                     // 返回值。初始值将作为输入框的默认值
    IN DWORD dwResultSize,                      // 返回值缓冲区总大小
    IN OPTIONAL PCheckInput checkInputProc,     // 检查合法性回调函数
    IN LPVOID lpParam                           // 用户数据，回调函数使用
    );

#endif /* _INPUTBOX_H */