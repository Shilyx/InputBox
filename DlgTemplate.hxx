#ifndef _DlgTemplate_HXX
#define _DlgTemplate_HXX

#include <Windows.h>

class DlgTemplate
{
public:
    DlgTemplate(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpFontName, WORD usFontSize, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy);
    ~DlgTemplate();

public:
    bool AddEdit     (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddStatic   (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddButton   (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddListBox  (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddComboBox (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddScrollBar(DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id);
    bool AddControl  (DWORD dwStyle, DWORD dwExStyle, LPCWSTR lpTitle, SHORT x, SHORT y, SHORT cx, SHORT cy, WORD id, WORD type);

public:
    DLGTEMPLATE *Lock();
    void Unlock();

private:
    void Align(size_t size);
    void AlignWord();
    void AlignDoubleWord();
    void AddData(const void *buffer, DWORD dwSize);
    void AddString(LPCWSTR lpString);
    bool AddCount();

private:
    IStream *m_pStream;
    DLGTEMPLATE *m_pTemplate;
};

#endif /* _CUSTOMDLG_HXX */