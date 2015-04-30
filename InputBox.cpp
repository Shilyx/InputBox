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
                MessageBox(hwndDlg, TEXT("������Ϣ���Ϸ�������������"), NULL, MB_ICONERROR);
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
    t.AddButton(BS_DEFPUSHBUTTON | WS_TABSTOP, 0, L"ȷ��(&O)", 7, 38, 70, 14, IDOK);
    t.AddButton(WS_TABSTOP, 0, L"ȡ��(&C)", 86, 38, 70, 14, IDCANCEL);

    param.hInstance = hInstance;
    param.hIcon = hIcon;
    param.dwLimitText = dwLimitText;
    param.lpResult = lpResult;
    param.dwResultSize = dwResultSize;
    param.checkInputProc = checkInputProc;
    param.lpParam = lpParam;

    return IDOK == DialogBoxIndirectParam(hInstance, t.Lock(), hParentWnd, DialogProc, (LPARAM)&param);
}