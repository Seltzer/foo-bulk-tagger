#include "mwrDialog.h"
#include "atlmisc.h"
#include "../release matching/selectionTreeModel.h"

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
		console::printf("Dialog initiated");

		return TRUE;
	}

	// TODO make root non-collapsible and take away its button accordingly
	void MatchWithReleasesDialog::SetupTreeView()
	{
		HWND treeViewHandle = GetDlgItem(IDC_SELECTIONTREE);
		selectionTreeView.Attach(treeViewHandle);

		// Populate tree view according to selection model
		CTreeItem rootNode = selectionTreeView.InsertItem(_T("Selection"), TVI_ROOT, TVI_ROOT);
		
		for (unsigned i = 0; i < model->ArtistCount(); i++)
		{
			SelectionTreeNode* artist = model->ArtistAt(i);
						
			// Extract artist string and convert it to a wchar_t[] (compatible with LPCTSTR since we're using UNICODE)
			const std::string& artistString = artist->GetStringData();
			wchar_t convertedArtistString[256];
			int NbChars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, artistString.c_str(), static_cast<int>(artistString.size()), convertedArtistString, sizeof(convertedArtistString) / sizeof(*convertedArtistString));
			convertedArtistString[NbChars] = L'\0';

			// Create artist node
			CTreeItem artistNode = rootNode.AddTail(convertedArtistString,0);
						
			for(int j = 0; j < artist->ChildCount(); j++)
			{
				SelectionTreeNode* album = artist->ChildAt(j);

				const std::string& albumString = album->GetStringData();
				wchar_t convertedAlbumString[256];
				int NbChars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, albumString.c_str(), static_cast<int>(albumString.size()), convertedAlbumString, sizeof(convertedAlbumString) / sizeof(*convertedAlbumString));
				convertedAlbumString[NbChars] = L'\0';
			
				CTreeItem albumNode = artistNode.AddTail(convertedAlbumString, 0);
			}
		}
	}

	LRESULT MatchWithReleasesDialog::OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}


	bool cvtLPW2stdstring(std::string& s, const LPWSTR pw,
                      UINT codepage = CP_ACP)
{
    bool res = false;
    char* p = 0;
    int bsz;

    bsz = WideCharToMultiByte(codepage,
        0,
        pw,-1,
        0,0,
        0,0);

	console::printf("length = %d", bsz);


    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw,-1,
            p,bsz,
            0,0);
        if (rc != 0) {
            p[bsz-1] = 0;
            s = p;
            res = true;
        }
    }
    delete [] p;
    return res;
}



	LRESULT MatchWithReleasesDialog::OnTreeSelectionChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		LPNMTREEVIEW newSelection = (LPNMTREEVIEW) pNMHDR;
		LPTVITEM lptvid = &newSelection->itemNew;
		LPWSTR orig = lptvid->pszText;

		std::string str;

		if (cvtLPW2stdstring(str,orig))
		{
			console::printf("selection = %s", str.c_str());
		}
	
		

		return 0L;
	}
		
	LRESULT MatchWithReleasesDialog::TestMsgLoop(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		console::print("Message loop is working");
		return 0;
	}
}