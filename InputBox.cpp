#include "InputBox.h"
#include "DlgTemplate.hxx"

struct DialogParam
{
    HINSTANCE hInstance;
    HICON hIcon;
    DWORD dwLimitText;
    LPTSTR lpResult;
    DWORD dwResultSize;
    PCheckInput checkInputProc;
    LPVOID lpParam;
};

static enum
{
    IDC_TEXT = 801,
};

static void OnOK(HWND hwndDlg)
{
    DialogParam *pParam = (DialogParam *)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

    if (pParam != NULL)
    {
        INT_PTR nTextLength = SendDlgItemMessage(hwndDlg, IDC_TEXT, WM_GETTEXTLENGTH, 0, 0) + 100;
        TCHAR *pText = (TCHAR *)malloc(nTextLength * sizeof(TCHAR));

        if (pText != NULL)
        {
            ZeroMemory(pText, nTextLength * sizeof(TCHAR));
            SendDlgItemMessage(hwndDlg, IDC_TEXT, WM_GETTEXT, nTextLength, (LPARAM)pText);

            if (pParam->checkInputProc == NULL || pParam->checkInputProc(pText, pParam->lpParam))
            {
                lstrcpyn(pParam->lpResult, pText, pParam->dwResultSize);
                EndDialog(hwndDlg, IDOK);
                return;
            }
            else
            {
                MessageBox(hwndDlg, TEXT("输入信息不合法，请重新输入"), NULL, MB_ICONERROR);
            }

            free(pText);
        }
    }

    SendDlgItemMessage(hwndDlg, IDC_TEXT, EM_SETSEL, 0, -1);
    SetFocus(GetDlgItem(hwndDlg, IDC_TEXT));
}

static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        if (lParam != 0)
        {
            DialogParam *pParam = (DialogParam *)lParam;

            if (pParam->hIcon != NULL)
            {
                SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)pParam->hIcon);
                SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)pParam->hIcon);
            }

            if (pParam->dwLimitText != 0)
            {
                SendDlgItemMessage(hwndDlg, IDC_TEXT, EM_SETLIMITTEXT, pParam->dwLimitText, 0);
            }

            SetDlgItemText(hwndDlg, IDC_TEXT, pParam->lpResult);
        }
        SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);
        break;

    case WM_SYSCOMMAND:
        if (SC_CLOSE == LOWORD(wParam))
        {
            EndDialog(hwndDlg, IDCANCEL);
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            OnOK(hwndDlg);
            break;

        case IDCANCEL:
            EndDialog(hwndDlg, IDCANCEL);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return FALSE;
}

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
    )
{
    if (dwResultSize == 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    else
    {
        lpResult[dwResultSize - 1] = TEXT('\0');
    }

    DialogParam param;
    DlgTemplate t(
        WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_SETFONT | DS_CENTER,
        0,
        L"MS SHELL DLG",
        8,
        L"Input Box",
        0,
        0,
        163,
        59
        );

    t.AddStatic(SS_LEFT, 0, lpMessage, 7, 7, 149, 8, 0xffff);

    DWORD dwEditStyle = ES_LEFT | WS_TABSTOP | ES_AUTOHSCROLL;

    if (bPassword)
    {
        dwEditStyle |= ES_PASSWORD;
    }

    if (bNumber)
    {
        bNumber |= ES_NUMBER;
    }

    t.AddEdit(dwEditStyle, WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY, NULL, 7, 17, 149, 14, IDC_TEXT);
    t.AddButton(BS_DEFPUSHBUTTON | WS_TABSTOP, 0, L"确定(&O)", 7, 38, 70, 14, IDOK);
    t.AddButton(WS_TABSTOP, 0, L"取消(&C)", 86, 38, 70, 14, IDCANCEL);

    param.hInstance = hInstance;
    param.hIcon = hIcon;
    param.dwLimitText = dwLimitText;
    param.lpResult = lpResult;
    param.dwResultSize = dwResultSize;
    param.checkInputProc = checkInputProc;
    param.lpParam = lpParam;

    return IDOK == DialogBoxIndirectParam(hInstance, t.Lock(), hParentWnd, DialogProc, (LPARAM)&param);
}