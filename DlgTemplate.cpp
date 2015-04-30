#include "DlgTemplate.hxx"

DlgTemplate::DlgTemplate(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpFontName, WORD usFontSize, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy)
{
    m_pTemplate = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);

    DWORD zero = 0;
    DLGTEMPLATE temp;

    temp.style = dwStyle; // WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_SETFONT | DS_CENTER;
    temp.dwExtendedStyle = dwExStyle;
    temp.cdit = 0;
    temp.x = x;
    temp.y = y;
    temp.cx = cx;
    temp.cy = cy;

    temp.style |= DS_SETFONT;

    AddData(&temp, sizeof(temp));

    AlignWord();
    AddData(&zero, 2); // menu
    AddData(&zero, 2); // class

    AlignWord();
    AddString(lpTitle); // title

    AlignWord();
    AddData(&usFontSize, 2); // font size

    AlignWord();
    AddString(lpFontName); // font name

    AlignDoubleWord();
}

DlgTemplate::~DlgTemplate()
{
    m_pStream->Release();
}

bool DlgTemplate::AddControl(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id, WORD type)
{
    if (!AddCount())
    {
        return false;
    }

    DLGITEMTEMPLATE temp;
    WORD mark = 0xffff;
    WORD creation_data = 0;

    temp.style = dwStyle;
    temp.dwExtendedStyle = dwExStyle;
    temp.x = x;
    temp.y = y;
    temp.cx = cx;
    temp.cy = cy;
    temp.id = id;

    temp.style |= (WS_CHILD | WS_VISIBLE);

    AlignDoubleWord();
    AddData(&temp, sizeof(temp));

    AlignWord();
    AddData(&mark, sizeof(mark));

    AlignWord();
    AddData(&type, sizeof(type));

    AlignWord();
    AddString(lpTitle); // title

    AlignWord();
    AddData(&creation_data, sizeof(creation_data));

    AlignDoubleWord();

    return true;
}

bool DlgTemplate::AddStatic(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0082);
}

bool DlgTemplate::AddEdit(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0081);
}

bool DlgTemplate::AddButton(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0080);
}

bool DlgTemplate::AddListBox(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0083);
}

bool DlgTemplate::AddScrollBar(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0084);
}

bool DlgTemplate::AddComboBox(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id)
{
    return AddControl(dwStyle, dwExStyle, lpTitle, x, y, cx, cy, id, 0x0085);
}

DLGTEMPLATE *DlgTemplate::Lock()
{
    if (m_pTemplate == NULL)
    {
        HGLOBAL hGlobal = NULL;

        GetHGlobalFromStream(m_pStream, &hGlobal);
        m_pTemplate = (DLGTEMPLATE *)GlobalLock(hGlobal);
    }

    return m_pTemplate;
}

void DlgTemplate::Unlock()
{
    if (m_pTemplate != NULL)
    {
        GlobalUnlock(m_pTemplate);
        m_pTemplate = NULL;
    }
}

void DlgTemplate::Align(size_t size)
{
    unsigned zero = 0;
    STATSTG stat;

    if (SUCCEEDED(m_pStream->Stat(&stat, STATFLAG_NONAME)))
    {
        while (stat.cbSize.QuadPart % size != 0)
        {
            stat.cbSize.QuadPart += 1;
            m_pStream->Write(&zero, 1, NULL);
        }
    }
}

void DlgTemplate::AlignWord()
{
    Align(sizeof(WORD));
}

void DlgTemplate::AlignDoubleWord()
{
    Align(sizeof(DWORD));
}

void DlgTemplate::AddData(const void *buffer, DWORD dwSize)
{
    m_pStream->Write(buffer, dwSize, NULL);
}

void DlgTemplate::AddString(LPCWSTR lpString)
{
    if (lpString == NULL)
    {
        lpString = L"";
    }

    AddData(lpString, sizeof(*lpString) * (1 + lstrlenW(lpString)));
}

bool DlgTemplate::AddCount()
{
    bool result = false;
    HGLOBAL hGlobal = NULL;

    if (SUCCEEDED(GetHGlobalFromStream(m_pStream, &hGlobal)))
    {
        DLGTEMPLATE *pTemplate = (DLGTEMPLATE *)GlobalLock(hGlobal);

        if (pTemplate != NULL)
        {
            pTemplate->cdit += 1;
            result = true;

            GlobalUnlock(pTemplate);
        }
    }

    return result;
}
