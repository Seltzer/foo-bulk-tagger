#include "selectionDivision.h"
#include "matchingHeuristics.h"
#include "selectionTreeModel.h"

using namespace std;




namespace FBT
{




///////////////// SelectionToMatch implementation
SelectionToMatch::SelectionToMatch()
{
}
	
SelectionToMatch::SelectionToMatch(const metadb_handle_list& tracks)
	: tracks(tracks), matchAttempted(false)
{
}

void SelectionToMatch::AddTrack(metadb_handle_ptr track)
{
	tracks.add_item(track);
}

void SelectionToMatch::AddTracks(const metadb_handle_list& tracksToAdd)
{
	tracks += tracksToAdd;
	tracks.remove_duplicates();
}

const metadb_handle_list& SelectionToMatch::GetTracks()
{
	return tracks;
}

void SelectionToMatch::FindMatches(MatchingHeuristic& heuristic)
{
	potentialMatches = heuristic.FindMatches(*this);
	
	matchAttempted = true;
}





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
				
			treeModel->FetchOrCreate(artist, album);
		}
	}

	treeModel->SortTreeAlphabetically();
	
}

SelectionTreeModel* SelectionDivisorByTags::GetTreeModel()
{
	return treeModel;
}


























}