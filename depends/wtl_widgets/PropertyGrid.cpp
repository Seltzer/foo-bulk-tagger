#include "PropertyGrid.h"


CPropertyBlockItem::CPropertyBlockItem(LPCTSTR pstrName, LPARAM lParam)
	: CProperty(pstrName, lParam)
{
}

BYTE CPropertyBlockItem::GetKind() const
{
	return PROPKIND_SIMPLE;
}
 

void CPropertyBlockItem::DrawValue(PROPERTYDRAWINFO& di)
{
	RECT rc = di.rcItem;
	rc.bottom--;
	::DrawFrameControl(di.hDC, &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
}

BOOL CPropertyBlockItem::Activate(UINT action, LPARAM /*lParam*/) 
{ 
	switch(action) 
	{
		case PACT_SPACE:
		case PACT_CLICK:
		case PACT_DBLCLICK:
			// Send AddItem notification to parent of control
			NMPROPERTYITEM nmh = { m_hWndOwner, ::GetDlgCtrlID(m_hWndOwner), PIN_BROWSE, NULL };
			::SendMessage(::GetParent(m_hWndOwner), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM) &nmh);
			break;
	}

	return TRUE;
}






CPropertyAppendActionItem::CPropertyAppendActionItem(LPCTSTR pstrName, LPARAM lParam) 
	: CProperty(pstrName, lParam)
{
}

BYTE CPropertyAppendActionItem::GetKind() const 
{ 
	return PROPKIND_SIMPLE;
}

void CPropertyAppendActionItem::DrawValue(PROPERTYDRAWINFO& di)
{
	#ifdef IDS_LASTVALUE
		TCHAR szText[128] = { 0 };
		::LoadString(_Module.GetResourceInstance(), IDS_LASTVALUE, szText, (sizeof(szText)/sizeof(TCHAR))-1);
		LPCTSTR pstrText = szText;
	#else
		LPCTSTR pstrText = _T("<< Click here to add a new item >>");
	#endif  // IDS_LASTVALUE

	CDCHandle dc(di.hDC);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor((di.state & ODS_DISABLED) != 0 ? di.clrDisabled : di.clrText);
	dc.SetBkColor(di.clrBack);
	RECT rcText = di.rcItem;
	rcText.left += PROP_TEXT_INDENT;
	dc.DrawText(pstrText, -1, &rcText, DT_LEFT | DT_SINGLELINE | DT_EDITCONTROL | DT_NOPREFIX | DT_END_ELLIPSIS | DT_VCENTER);
}

BOOL CPropertyAppendActionItem::Activate(UINT action, LPARAM /*lParam*/) 
{ 
	switch(action) 
	{
		case PACT_SPACE:
		case PACT_CLICK:
		case PACT_DBLCLICK:
			// Send AddItem notification to parent of control
			NMPROPERTYITEM nmh = { m_hWndOwner, ::GetDlgCtrlID(m_hWndOwner), PIN_ADDITEM, NULL };
			::SendMessage(::GetParent(m_hWndOwner), WM_NOTIFY, nmh.hdr.idFrom, (LPARAM) &nmh);
			break;
	}
	
	return TRUE;
}



