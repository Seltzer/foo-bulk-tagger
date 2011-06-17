#ifndef __PROPERTYGRID__H
#define __PROPERTYGRID__H

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPropertyGrid - A simple grid control
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2002-2003 Bjarke Viksoe.
// Thanks to Ludvig A. Norin for the PGS_EX_ADDITEMATEND feature.
//
// Add the following macro to the parent's message map:
//   REFLECT_NOTIFICATIONS()
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>


#if !((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
  #include <zmouse.h>
#endif //!((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))


// Include property base class
#include "PropertyItem.h"

// Include property implementations
#include "PropertyItemEditors.h"
#include "PropertyItemImpl.h"



/////////////////////////////////////////////////////////////////////////////
// CPropertyGrid control

// Extended Grid styles
#define PGS_EX_SINGLECLICKEDIT   0x00000001
#define PGS_EX_NOGRID            0x00000002
#define PGS_EX_TABNAVIGATION     0x00000004
#define PGS_EX_NOSHEETNAVIGATION 0x00000008
#define PGS_EX_FULLROWSELECT     0x00000010
#define PGS_EX_INVERTSELECTION   0x00000020
#define PGS_EX_ADDITEMATEND      0x00000040



/////////////////////////////////////////////////////////////////////////////
// The "AppendAction" property (PGS_EX_ADDITEMATEND support)

class CPropertyAppendActionItem : public CProperty
{

public:
     
	CPropertyAppendActionItem(LPCTSTR pstrName, LPARAM lParam); 
	BYTE GetKind() const; 
	void DrawValue(PROPERTYDRAWINFO& di);
	BOOL Activate(UINT action, LPARAM /*lParam*/) ;
};

inline HPROPERTY PropCreateAppendActionItem(LPCTSTR pstrName, LPARAM lParam = 0)
{
   return new CPropertyAppendActionItem(pstrName, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// Block property - looks like a header (or button)

class CPropertyBlockItem : public CProperty
{
public:
   CPropertyBlockItem(LPCTSTR pstrName, LPARAM lParam);

   BYTE GetKind() const; 
   void DrawValue(PROPERTYDRAWINFO& di);
   BOOL Activate(UINT action, LPARAM /*lParam*/); 
};

inline HPROPERTY PropCreateBlockItem(LPCTSTR pstrName, LPARAM lParam = 0)
{
   return new CPropertyBlockItem(pstrName, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// The Property Grid control

template< class T, class TBase = CListViewCtrl, class TWinTraits = CWinTraitsOR<LVS_SINGLESEL|LVS_SHOWSELALWAYS> >
class ATL_NO_VTABLE CPropertyGridImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public CCustomDraw< T >
{

public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	CHeaderCtrl m_ctrlHeader;
	PROPERTYDRAWINFO m_di;
	CFont m_TextFont, m_CategoryFont;
	CPen m_BorderPen;
	HWND m_hwndInplace;
	int m_iInplaceRow, m_iInplaceCol, m_nColumns, m_iSelectedRow, m_iSelectedCol;

	CPropertyGridImpl();

	// Operations


	BOOL SubclassWindow(HWND hWnd);
   	void SetExtendedGridStyle(DWORD dwExtStyle);
	DWORD GetExtendedGridStyle() const;
	BOOL SelectItem(int iRow, int iCol = 0);
	int GetItemCount() const;
   

	int InsertItem(int nItem, HPROPERTY hProp);  
	BOOL SetSubItem(int nItem, int nSubItem, HPROPERTY hProp);
	BOOL GetItemText(int iItem, int iSubItem, LPTSTR pstrText, UINT cchMax) const;
	BOOL GetItemText(HPROPERTY hProp, LPTSTR pstrText, UINT cchMax) const;
	BOOL GetItemValue(HPROPERTY hProp, VARIANT* pValue) const;
	BOOL SetItemValue(HPROPERTY hProp, VARIANT* pValue);
	int GetSelectedColumn() const;
	BOOL DeleteColumn(int nCol);
    BOOL GetColumnCount() const;
	BOOL FindProperty(IProperty* prop, int& iItem, int& iSubItem) const;
	IProperty* GetProperty(int iRow, int iCol) const;
	LPARAM GetItemData(HPROPERTY hProp) const;
	void SetItemData(HPROPERTY hProp, LPARAM lParam);
	BOOL GetItemEnabled(HPROPERTY hProp) const;
	void SetItemEnabled(HPROPERTY hProp, BOOL bEnable);
	void Navigate(UINT wCode);

	// Unsupported methods

	BOOL SetItem(int /*nItem*/, int /*nSubItem*/, UINT /*nMask*/, LPCTSTR /*lpszItem*/,
		int /*nImage*/, UINT /*nState*/, UINT /*nStateMask*/, LPARAM /*lParam*/);
	BOOL SetItemText(int /*nItem*/, int /*nSubItem*/, LPCTSTR /*lpszText*/);
	DWORD SetViewType(DWORD /*dwType*/);
	CEdit EditLabel(int /*nItem*/);


	// Implementation
	void _Init();
	BOOL _SpawnInplaceWindow(IProperty* prop, int iItem, int iSubItem);
	void _DestroyInplaceWindow();
	void _GetSubItemRect(int iItem, int iSubItem, RECT* pRect) const;

	void _InvalidateItem(int iItem, int iSubItem);

	bool _IsValidSelection() const;

	bool _IsAppendActionItem(int iItem) const;

	// Message map and handlers
	BEGIN_MSG_MAP(CPropertyGridImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_SIZE, OnScroll)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnScroll)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnScroll)      
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
		MESSAGE_HANDLER(WM_USER_PROP_NAVIGATE, OnNavigate);
		MESSAGE_HANDLER(WM_USER_PROP_UPDATEPROPERTY, OnUpdateProperty);
		MESSAGE_HANDLER(WM_USER_PROP_CANCELPROPERTY, OnCancelProperty);
		MESSAGE_HANDLER(WM_USER_PROP_CHANGEDPROPERTY, OnChangedProperty);
		NOTIFY_CODE_HANDLER(HDN_ITEMCHANGEDA, OnHeaderChanging)
		NOTIFY_CODE_HANDLER(HDN_ITEMCHANGEDW, OnHeaderChanging)
		NOTIFY_CODE_HANDLER(HDN_ITEMCHANGINGA, OnHeaderChanging) // See Q183258 why we all need these
		NOTIFY_CODE_HANDLER(HDN_ITEMCHANGINGW, OnHeaderChanging)
		NOTIFY_CODE_HANDLER(HDN_TRACKA, OnHeaderChanging)
		NOTIFY_CODE_HANDLER(HDN_TRACKW, OnHeaderChanging)
		NOTIFY_CODE_HANDLER(HDN_DIVIDERDBLCLICKA, OnHeaderDblClick)
		NOTIFY_CODE_HANDLER(HDN_DIVIDERDBLCLICKW, OnHeaderDblClick)
		REFLECTED_NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnSelChanged)
		REFLECTED_NOTIFY_CODE_HANDLER(LVN_DELETEITEM, OnDeleteItem)
		CHAIN_MSG_MAP_ALT(CCustomDraw<T>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnGetDlgCode(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDeleteItem(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnNavigate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateProperty(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnCancelProperty(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnChangedProperty(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnHeaderChanging(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& bHandled);
	LRESULT OnHeaderDblClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
	DWORD OnSubItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw);
	void DrawItem(CDCHandle dc, int iItem, int iSubItem, UINT iState);
};

#include "PropertyGrid.inl"


class CPropertyGridCtrl : public CPropertyGridImpl<CPropertyGridCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_PropertyGrid"), GetWndClassName())
};


#endif // __PROPERTYGRID__H
