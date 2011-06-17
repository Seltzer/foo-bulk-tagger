#include "selection.h"
#include "matchingHeuristics.h"







namespace FBT
{



///////////////// SelectionToMatch implementation
SelectionToMatch::SelectionToMatch()
{
}
	
SelectionToMatch::SelectionToMatch(const metadb_handle_list& tracks)
	: matchAttempted(false)
{
	AddTracks(tracks);
}

void SelectionToMatch::AddTrack(metadb_handle_ptr track)
{
	tracks.add_item(track);

	file_info_impl info;
	track->get_info(info);
	trackInfo += new file_info_impl(info);
}

void SelectionToMatch::AddTracks(const metadb_handle_list& tracksToAdd)
{
	file_info_impl info;

	for(t_size index = 0; index < tracksToAdd.get_count(); index++)
	{
		tracks.add_item(tracksToAdd[index]);
		tracksToAdd[index]->get_info(info);
		trackInfo += new file_info_impl(info);		
	}
	
	tracks.remove_duplicates();
}

const metadb_handle_list& SelectionToMatch::GetTracks()
{
	return tracks;
}


void SelectionToMatch::BuildMetaTable()
{
	unsigned trackIndex = 0;

	for (t_size t = 0; t < tracks.get_count(); t++)
		metaTableBuilder.ExtractMetadata(tracks[t], *(trackInfo[t]) );
}

const SelectionMetaTableBuilder& SelectionToMatch::GetMetaTableBuilder() const
{
	return metaTableBuilder;
}

void SelectionToMatch::FindMatches(MatchingHeuristic& heuristic)
{
	potentialMatches = heuristic.FindMatches(*this);
	
	matchAttempted = true;
}

void SelectionToMatch::WriteTags(pfc::chain_list_v2_t<ExtendedTagInfo*>& addedTags, HWND dlgHandle)
{
	metaTableBuilder.WriteTags(tracks, trackInfo, addedTags, dlgHandle);
}



















}