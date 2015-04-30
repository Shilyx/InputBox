#ifndef _INPUTBOX_H
#define _INPUTBOX_H

#include <Windows.h>

// �ص�������ʽ������Ϊ�ַ������û�����
typedef BOOL (CALLBACK *PCheckInput)(LPCTSTR lpString, LPVOID lpParam);

BOOL InputBox(
    IN HINSTANCE hInstance,                     // ���ڹ������ڵ�ģ����
    IN HWND hParentWnd,                         // �����ھ��
    IN HICON hIcon,                             // ������ͼ����
    IN LPCWSTR lpMessage,                       // �����֮�ϵ���ʾ����
    IN BOOL bNumber,                            // �Ƿ������������
    IN BOOL bPassword,                          // �Ƿ������������
    IN DWORD dwLimitText,                       // ���볤������
    IN OUT LPTSTR lpResult,                     // ����ֵ����ʼֵ����Ϊ������Ĭ��ֵ
    IN DWORD dwResultSize,                      // ����ֵ�������ܴ�С
    IN OPTIONAL PCheckInput checkInputProc,     // ���Ϸ��Իص�����
    IN LPVOID lpParam                           // �û����ݣ��ص�����ʹ��
    );

#endif /* _INPUTBOX_H */