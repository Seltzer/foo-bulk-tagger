#include "selectionDivision.h"
#include "selectionTreeModel.h"
#include "selection.h"
#include "../stringConstants.h"

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
			const char* artist = trackInfo.meta_get(ARTIST_TAG,0);
			const char* album = trackInfo.meta_get(ALBUM_TAG,0);
							
			SelectionToMatch* match = NULL;
			
			if (!artist)
				artist = "";
			
			if (album)
			{
				match = treeModel->FetchOrCreate(artist, album);
			}
			else
			{
				album = "";
				pfc::string8 trackLocation;

				
				static_api_ptr_t<library_manager> libraryMgr;
				// Grab the track's filepath, relative to the base music folder
				// TODO I'm ignoring the library_manager::get_relative_path() warning
				if (libraryMgr->get_relative_path(trackPtr, trackLocation))
				{
					// Extract the folder (non-filename) part of the filepath
					pfc::string_directory trackFolder(trackLocation.get_ptr());
					char trackFolderAsCharArr[256];
					// yeah i know i shouldn't be using strcpy - TODO
					::strcpy(trackFolderAsCharArr, trackFolder.operator const char *());

					match = treeModel->FetchOrCreate(artist, album, trackFolderAsCharArr);
				}
				else
				{
					pfc::dynamic_assert(false);
					//match = treeModel->FetchOrCreate(artist, "", trackPtr->get_path());
				}
			}

			match->AddTrack(trackPtr);
		}
		else
		{
			pfc::dynamic_assert(false);
		}
	}

	treeModel->SortTreeAlphabetically();
	
}

SelectionTreeModel* SelectionDivisorByTags::GetTreeModel()
{
	return treeModel;
}


























}