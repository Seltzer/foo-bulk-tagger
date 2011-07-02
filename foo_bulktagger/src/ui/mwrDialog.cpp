#include "mwrDialog.h"
#include "atlmisc.h"
#include "../release matching/selectionTreeModel.h"
#include "../release matching/selection.h"

using namespace WTL;




namespace FBT
{
	

	
	MatchWithReleasesDialog::MatchWithReleasesDialog(SelectionTreeModel* model, HWND parent)
		: model(model), parentWindow(parent), changesMadeToTags(false), currentSelection(NULL)
	{
	}


	BOOL MatchWithReleasesDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
	{
		CenterWindow();
		SetupTreeView();
		SetupPropertyGrids();

		HWND listBoxHandle = GetDlgItem(IDC_SELECTIONTRACKS);
		selectionTracksListBox.Attach(listBoxHandle);

		HWND applyButtonHandle = GetDlgItem(IDC_APPLYTAGS);
		applyTagsButton.Attach(applyButtonHandle);

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

			// Create artist node in view and append it to the root
			STRING8_TO_LPCTSTR(artist->GetStringData(), 256, convertedArtistString);

			CTreeItem artistNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedArtistString, 
															0, 0, 0, 0, (LPARAM) artist, rootHandle, TVI_LAST);
									
			for(int j = 0; j < artist->ChildCount(); j++)
			{
				SelectionTreeNode* album = artist->ChildAt(j);

				// Create album node in view and append it to the artist node
				STRING8_TO_LPCTSTR(album->GetStringData(), 256, convertedAlbumString);
				CTreeItem albumNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedAlbumString, 
										0, 0, 0, 0, (LPARAM) album, artistNode.operator HTREEITEM(), TVI_LAST);
								
				if (album->HasSelectionData())
				{
					// While we're here, build the metadata table for the selection - we're gonna need it later
					album->GetSelectionData()->BuildMetaTable();
				}
				else
				{
					for(int k = 0; k < album->ChildCount(); k++)
					{
						SelectionTreeNode* directory = album->ChildAt(k);

						// Create directory node in view and append it to the album node
						STRING8_TO_LPCTSTR(directory->GetStringData(), 256, convertedDirectoryString);
						CTreeItem dirNode = selectionTreeView.InsertItem(TVIF_TEXT | TVIF_PARAM, convertedDirectoryString, 
										0, 0, 0, 0, (LPARAM) directory, albumNode.operator HTREEITEM(), TVI_LAST);

						// While we're here, build the metadata table for the selection - we're gonna need it later
						directory->GetSelectionData()->BuildMetaTable();
					}
				}
			}
		}
	}

	void MatchWithReleasesDialog::SetupPropertyGrids()
	{
		selectionDataGrid.SubclassWindow(GetDlgItem(IDC_SELECTIONDATA));
		selectionDataGrid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT);

		selectionDataGrid.InsertColumn(0, _T("Tag Name"), LVCFMT_LEFT, 100, 0);
		selectionDataGrid.InsertColumn(1, _T("Tag Values"), LVCFMT_LEFT, 180, 0);
	}


	LRESULT MatchWithReleasesDialog::OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT MatchWithReleasesDialog::FetchTags(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		pfc::dynamic_assert(currentSelection != NULL);

		currentSelection->DoPuidStuff();
			
		//currentSelection->FindMatches(heuristic, scoringHeuristic);
		

		//currentSelection->WriteTags(addedModifiedTags, this->operator HWND());

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
						
			currentSelection = node->GetSelectionData();
			const metadb_handle_list& tracks = currentSelection->GetTracks();

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

			currentSelection->GetMetaTableBuilder().UpdatePropertyGrid(selectionDataGrid);
			selectionDataGrid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT | PGS_EX_ADDITEMATEND);
		}
		else
		{
			currentSelection = NULL;

			selectionDataGrid.SetExtendedGridStyle(PGS_EX_SINGLECLICKEDIT);

			while(selectionDataGrid.GetItemCount())
				selectionDataGrid.DeleteItem(0);
		}

		changesMadeToTags = false;
		applyTagsButton.EnableWindow(false);

		
		return 0L;
	}
		
	LRESULT MatchWithReleasesDialog::OnAddItem(int idCtrl, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		// Build tag info for new item
		ExtendedTagInfo* tagInfo = new ExtendedTagInfo();
		tagInfo->realTagName = pfc::string8("tag name");
		tagInfo->realTagValues.insert_last(pfc::string8("<tag value>"));
		tagInfo->isStandardTag = true;
		tagInfo->hasMultipleValuesProperty = false;
		tagInfo->isDirty = true;
		tagInfo->GenerateDecoratedValues();
		
		// Create tag name property	
		HPROPERTY tagNameProperty = PropCreateSimple(_T("key"), _T("<tag name>"));
		tagNameProperty->SetItemData((LPARAM) tagInfo);
		int row = selectionDataGrid.InsertItem(-1, tagNameProperty);
		tagInfo->index = row;
		
		HPROPERTY tagValuesProperty = PropCreateSimple(_T("value"), _T("<tag value>"));
		tagValuesProperty->SetItemData((LPARAM) tagInfo);
		selectionDataGrid.SetSubItem(row, 1, tagValuesProperty);
		selectionDataGrid.SelectItem(row);

		// Don't inform the SelectionToMatch of the added tag until as late as possible
		addedTags.insert_last(tagInfo);

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
		return 0;
	}

	LRESULT MatchWithReleasesDialog::OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMPROPERTYITEM pnpi = (LPNMPROPERTYITEM) pnmh;
		
		// Grab value and convert it to UTF-8
		TCHAR szValue[100] = { 0 };
		pnpi->prop->GetDisplayValue(szValue, sizeof(szValue)/sizeof(TCHAR));
		char convertedValue[256];
		pfc::stringcvt::convert_wide_to_utf8(convertedValue, 256, szValue, 256);

				
		ExtendedTagInfo* tagInfo = (ExtendedTagInfo*) pnpi->prop->GetItemData();
		
		// Update tag info
		// First, work out whether the changed property is a tag name or tag values
		if (::lstrcmpiW(pnpi->prop->GetName(), _T("key")) == 0)
		{
			tagInfo->realTagName = pfc::string8(convertedValue);
		}
		else if (::lstrcmpiW(pnpi->prop->GetName(), _T("value")) == 0)
		{
			tagInfo->realTagValues.remove_all();
			pfc::splitStringByChar<pfc::chain_list_v2_t<pfc::string8> >(tagInfo->realTagValues, convertedValue, ';');
		}
		else
		{
			pfc::dynamic_assert(false);
		}

		tagInfo->isStandardTag = true;
		tagInfo->hasMultipleValuesProperty = false;
		tagInfo->isDirty = true;
		tagInfo->GenerateDecoratedValues();
		


		// If this is an added tag
		if (addedTags.find_item(tagInfo).is_valid())
		{
			// And it hasn't been marked as modified
			if (!addedModifiedTags.find_item(tagInfo).is_valid())
			{
				// Mark it as modified
				addedModifiedTags.insert_last(tagInfo);
			}
		}


		// Update Apply Tags button
		changesMadeToTags = true;
		applyTagsButton.EnableWindow();


		CComVariant vValue;
		pnpi->prop->GetValue(&vValue);
		vValue.ChangeType(VT_BSTR);
		return 0;
	}

	LRESULT MatchWithReleasesDialog::OnApplyTags(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		pfc::dynamic_assert(currentSelection != NULL);

		currentSelection->WriteTags(addedModifiedTags, this->operator HWND());
		
		return 0;
	}
}