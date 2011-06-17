#ifndef MWR_DIALOG_H_
#define MWR_DIALOG_H_

#include "../common.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>

#include "wtl_widgets\PropertyGrid.h"


namespace FBT
{
	class SelectionTreeModel;

	class MatchWithReleasesDialog : public ATL::CDialogImpl<MatchWithReleasesDialog>
	{
		BEGIN_MSG_MAP_EX(MatchWithReleasesDialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			MESSAGE_HANDLER(WM_TESTMSGLOOP, TestMsgLoop)
	        COMMAND_ID_HANDLER(IDOK, OnOKCancel)
	        COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
			NOTIFY_HANDLER(IDC_SELECTIONTREE, TVN_SELCHANGED, OnTreeSelectionChanged)
	      
			NOTIFY_CODE_HANDLER(PIN_SELCHANGED, OnSelChanged);
			NOTIFY_CODE_HANDLER(PIN_ITEMCHANGED, OnItemChanged);
			NOTIFY_CODE_HANDLER(PIN_ADDITEM, OnAddItem);

			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()


	public:
		enum { IDD = IDD_MWRDIALOG };
				
		MatchWithReleasesDialog(SelectionTreeModel*, HWND);
		

	private: // Methods
		void SetupTreeView();
		void SetupPropertyGrids();
	
	private: // Message handlers
		BOOL OnInitDialog(CWindow, LPARAM);
		LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnTreeSelectionChanged(int, LPNMHDR, BOOL&);
		LRESULT TestMsgLoop(UINT, WPARAM, LPARAM, BOOL&);

		LRESULT OnAddItem(int idCtrl, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	    LRESULT OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);
	    LRESULT OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);

	private: // Fields
		// Model stuff
		metadb_handle_list selectedTracks;
		SelectionTreeModel* model;

		// View stuff
		HWND parentWindow;
		WTL::CTreeViewCtrlEx selectionTreeView;
		WTL::CListBox selectionTracksListBox;
					
		CPropertyGridCtrl selectionDataGrid;



	};



}













#endif