#ifndef SELECTION_H_
#define SELECTION_H_

#include "../common.h"
#include "potentialMatch.h"
#include "selectionMetadata.h"

namespace FBT
{
	class MatchingHeuristic;
	struct ExtendedTagInfo;
	

	// A selection of tracks to be matched with one release in the MusicBrainz database
	class SelectionToMatch
	{

	public:
		SelectionToMatch();
		SelectionToMatch(const metadb_handle_list&);

		void AddTrack(metadb_handle_ptr);
		void AddTracks(const metadb_handle_list&);
		const metadb_handle_list& GetTracks();

		void BuildMetaTable();
		const SelectionMetaTableBuilder& GetMetaTableBuilder() const;
		
		// This writes any dirty data for existing tags as well as the tags specified by addedTags to tracks
		void WriteTags(pfc::chain_list_v2_t<ExtendedTagInfo*>& addedTags, HWND dlgHandle);
		
		void FindMatches(MatchingHeuristic&);
		
	private:
		metadb_handle_list tracks;
		pfc::list_t<const file_info*> trackInfo;	// currently unused
		
		SelectionMetaTableBuilder metaTableBuilder;
		
		bool matchAttempted;
		pfc::chain_list_v2_t<PotentialMatch> potentialMatches;
	};


}


#endif

