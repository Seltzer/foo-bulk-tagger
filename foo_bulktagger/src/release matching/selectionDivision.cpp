#include "selectionDivision.h"
#include "selectionTreeModel.h"
#include "selection.h"

using namespace std;






namespace FBT
{




///////////////// ISelectionDivisor implementation
ISelectionDivisor::ISelectionDivisor(const metadb_handle_list& initSelection)
	: selection(initSelection)
{
}

void ISelectionDivisor::AddTrackToSelection(metadb_handle_ptr track)
{
	selection.add_item(track);	
}

void ISelectionDivisor::AddTracksToSelection(const metadb_handle_list& tracks)
{
	selection += tracks;
	selection.remove_duplicates();
}


///////////////// SelectionDivisorByTags implementation
SelectionDivisorByTags::SelectionDivisorByTags(const metadb_handle_list& initSelection)
	: ISelectionDivisor(initSelection), treeModel(NULL)
{
}

void SelectionDivisorByTags::DivideSelection()
{
	treeModel = new SelectionTreeModel();

		
	file_info_impl trackInfo;

	for (unsigned i = 0; i < selection.get_count(); i++)
	{
		metadb_handle_ptr trackPtr = selection.get_item(i);

		if (trackPtr->get_info(trackInfo))
		{
			const char* artist = trackInfo.meta_get("Artist",0);
			const char* album = trackInfo.meta_get("Album",0);

			if (!artist || !album)
			{
				console::printf("couldn't get track info");
				continue;
			}
				
			SelectionToMatch* match = treeModel->FetchOrCreate(artist, album);
			match->AddTrack(trackPtr);
		}
	}

	treeModel->SortTreeAlphabetically();
	
}

SelectionTreeModel* SelectionDivisorByTags::GetTreeModel()
{
	return treeModel;
}


























}