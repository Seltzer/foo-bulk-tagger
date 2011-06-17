#include "mwrDialog.h"
#include "atlmisc.h"
#include "../release matching/selectionTreeModel.h"
#include "../release matching/selectionDivision.h"

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
			}
		}
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

			for (unsigned  i = 0; i < tracks.get_count(); i++)
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
		}
		
		return 0L;
	}
		
	LRESULT MatchWithReleasesDialog::TestMsgLoop(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		console::print("Message loop is working");
		return 0;
	}
}