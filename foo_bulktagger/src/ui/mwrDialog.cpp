#include "mwrDialog.h"
#include "atlmisc.h"
#include "../release matching/selectionTreeModel.h"
#include "../release matching/selection.h"

using namespace WTL;




namespace FBT
{
	

	
	MatchWithReleasesDialog::MatchWithReleasesDialog(SelectionTreeModel* model, HWND parent)
		: model(model), parentWindow(parent)
	{
	}


	BOOL MatchWithReleasesDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
	{
		CenterWindow();
		SetupTreeView();
		SetupPropertyGrids();

		HWND listBoxHandle = GetDlgItem(IDC_SELECTIONTRACKS);
		selectionTracksListBox.Attach(listBoxHandle);

		console::printf("Dialog initiated");
		return TRUE;
	}
		
	// Populates tree view according to selection model
	// TODO make root non-collapsible and take away its button accordingly
	void MatchWithReleasesDialog::SetupTreeView()
	{
		HWND treeViewHandle = GetDlgItem(IDC_SELECTIONTREE);
		selectionTreeView.Attach(treeViewHandle);

		// Create root node
		STRING8_TO_LPCTSTR(model->GetRoot()->GetStringData(), 32, convertedRootString);
		CTreeItem rootNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedRootString, 
															0, 0, 0, 0, (LPARAM) model->GetRoot(),TVI_ROOT, TVI_ROOT);
		HTREEITEM rootHandle = rootNode.operator HTREEITEM();

		for (unsigned i = 0; i < model->ArtistCount(); i++)
		{
			SelectionTreeNode* artist = model->ArtistAt(i);

			// Create artist node and append it to the root
			STRING8_TO_LPCTSTR(artist->GetStringData(), 256, convertedArtistString);

			CTreeItem artistNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedArtistString, 
															0, 0, 0, 0, (LPARAM) artist, rootHandle, TVI_LAST);
									
			for(int j = 0; j < artist->ChildCount(); j++)
			{
				SelectionTreeNode* album = artist->ChildAt(j);

				// Create album node and append it to the artist node
				STRING8_TO_LPCTSTR(album->GetStringData(), 256, convertedAlbumString);
				CTreeItem rootNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedAlbumString, 
										0, 0, 0, 0, (LPARAM) album, artistNode.operator HTREEITEM(), TVI_LAST);

				// While we're here, build the metadata table for the selection - we're gonna need it later
				album->GetSelectionData()->BuildMetaTable();
			}
		}
	}

	void MatchWithReleasesDialog::SetupPropertyGrids()
	{
		selectionDataGrid.SubclassWindow(GetDlgItem(IDC_SELECTIONDATA));
		selectionDataGrid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT | PGS_EX_ADDITEMATEND);
	}


	LRESULT MatchWithReleasesDialog::OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}



	LRESULT MatchWithReleasesDialog::OnTreeSelectionChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		LPNMTREEVIEW newSelection = (LPNMTREEVIEW) pNMHDR;
		LPTVITEM lptvid = &newSelection->itemNew;
		
		SelectionTreeNode* node = (SelectionTreeNode*) lptvid->lParam;
		
		if (node->HasSelectionData())
		{
			selectionTracksListBox.ResetContent();
						
			SelectionToMatch* selection = node->GetSelectionData();
			const metadb_handle_list& tracks = selection->GetTracks();

			file_info_impl trackInfo;
			for (t_size i = 0; i < tracks.get_count(); i++)
			{
				tracks[i]->get_info(trackInfo);
								
				pfc::string8 displayString;

				const char* track = trackInfo.meta_get("tracknumber", 0);
				if (track)
				{
					displayString.add_string(track);
					displayString.add_string(" - ", 3);
				}
				
				const char* title = trackInfo.meta_get("title", 0);
				displayString.add_string(title);
				
				STRING8_TO_LPCTSTR(displayString, 256, convertedDisplayString);
				selectionTracksListBox.AddString(convertedDisplayString);
			}

			selection->GetMetaTableBuilder().UpdatePropertyGrid(selectionDataGrid);
		}
		
		return 0L;
	}
		
	LRESULT MatchWithReleasesDialog::OnAddItem(int idCtrl, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		ATLTRACE(_T("OnAddItem - Ctrl: %d\n"), idCtrl); idCtrl;

		int i = selectionDataGrid.InsertItem(-1, PropCreateReadOnlyItem(_T(""), _T("Dolly")));
		selectionDataGrid.SetSubItem(i, 1, PropCreateSimple(_T(""), true));
		selectionDataGrid.SetSubItem(i, 2, PropCreateCheckButton(_T(""), false));
		selectionDataGrid.SetSubItem(i, 3, PropCreateSimple(_T(""), _T("")));
		selectionDataGrid.SelectItem(i);
		return 0;
	}

	LRESULT MatchWithReleasesDialog::OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMPROPERTYITEM pnpi = (LPNMPROPERTYITEM) pnmh;
		if( pnpi->prop==NULL ) 
			return 0;
		
		TCHAR szValue[100] = { 0 };      
		pnpi->prop->GetDisplayValue(szValue, sizeof(szValue)/sizeof(TCHAR));
		CComVariant vValue;
		pnpi->prop->GetValue(&vValue);
		vValue.ChangeType(VT_BSTR);
		
		ATLTRACE(_T("OnSelChanged - Ctrl: %d, Name: '%s', DispValue: '%s', Value: '%ls'\n"),
					idCtrl, pnpi->prop->GetName(), szValue, vValue.bstrVal); idCtrl;
		return 0;
	}

	LRESULT MatchWithReleasesDialog::OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMPROPERTYITEM pnpi = (LPNMPROPERTYITEM) pnmh;
		TCHAR szValue[100] = { 0 };
		pnpi->prop->GetDisplayValue(szValue, sizeof(szValue)/sizeof(TCHAR));
		CComVariant vValue;
		pnpi->prop->GetValue(&vValue);
		vValue.ChangeType(VT_BSTR);
		
		ATLTRACE(_T("OnItemChanged - Ctrl: %d, Name: '%s', DispValue: '%s', Value: '%ls'\n"),
						idCtrl, pnpi->prop->GetName(), szValue, vValue.bstrVal); idCtrl;
		return 0;
	}


	LRESULT MatchWithReleasesDialog::TestMsgLoop(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		console::print("Message loop is working");
		return 0;
	}
}