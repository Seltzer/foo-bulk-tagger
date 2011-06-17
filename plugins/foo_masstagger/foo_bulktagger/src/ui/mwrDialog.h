#ifndef MWR_DIALOG_H_
#define MWR_DIALOG_H_

#include "../common.h"

#include <atlbase.h>
#include <atlapp.h>
//extern CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>

//#include "ATLHelpers.h"
//#include <atlbase.h>    // Base ATL classes
//include <atlwin.h> 

namespace FBT
{

	class MWRDialog
		: public ATL::CDialogImpl<MWRDialog>
		// : public dialog_helper::dialog_modeless
		//: public dialog_helper::dialog_modeless_v2
	{


	public:
		//MWRDialog(HWND p_parent);

		// preferences_page
		HWND create(HWND parent) { return Create(parent); }
		const char * get_name() { return "Some Name"; };

		// CAxDialogImpl
		enum { IDD = IDD_ABOUTBOX }; // ID of your dialog.

		//COMMAND_HANDLER_EX(IDC_SOME_CHECKBOX, BN_CLICKED, OnSomeCheckBoxClicked)
		BEGIN_MSG_MAP_EX(MWRDialog)
			MSG_WM_INITDIALOG(OnInitDialog)
	        COMMAND_ID_HANDLER(IDOK, OnOKCancel)
	        COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
		END_MSG_MAP()

		BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
		{
			console::printf("oninitdialog");
			CenterWindow();
			// Initialize control states.
			//CheckDlgButton(IDC_SOME_CHECKBOX, g_some_setting ? BST_CHECKED : BST_UNCHECKED);
			// Other controls can be accessed with GetDlgItem(ID).
			// You can also use WTL helpers like CListViewCtrl(GetDlgItem(ID)).
			// To pass UTF-8 strings to controls, use uSetDlgItemText() etc. from shared.h.
			return TRUE;
		}

		LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			EndDialog(wID);
			return 0;
		}

	};


	//////////////////////////////////////////////////////////////////////////
// Mix-in class that handles WM_ERASEBKGND
	/*
template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd
{
public:
    CPaintBkgnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
    ~CPaintBkgnd() { DeleteObject ( m_hbrBkgnd ); }
 
    BEGIN_MSG_MAP(CPaintBkgnd)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()
 
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
    T* pT = static_cast<T*>(this);
    HDC dc = (HDC) wParam;
    RECT rcClient;
 
        pT->GetClientRect ( &rcClient );
        FillRect ( dc, &rcClient, m_hbrBkgnd );
        return 1;    // we painted the background
    }
 
protected:
    HBRUSH m_hbrBkgnd;
};*/


//////////////////////////////////////////////////////////////////////////
// Our main window

class CMyWindow : public CWindowImpl<CMyWindow, CWindow, CFrameWinTraits>//,
    //              public CPaintBkgnd<CMyWindow, RGB(0,0,255)>
{
private:
    //typedef CPaintBkgnd<CMyWindow, RGB(0,0,255)> CPaintBkgndBase;

public:
    DECLARE_WND_CLASS(_T("My Window Class"))

    BEGIN_MSG_MAP(CMyWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        //CHAIN_MSG_MAP(CPaintBkgndBase)
    END_MSG_MAP()

	        //COMMAND_ID_HANDLER(IDC_ABOUT, OnAbout)

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
		//HMENU hm = LoadMenu ( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU1) );

        //SetMenu ( hm );
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        DestroyWindow();
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PostQuitMessage(0);
        return 0;
    }

    LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
	 MWRDialog dlg;
	
        dlg.DoModal();
        return 0;
    }
};
}













#endif