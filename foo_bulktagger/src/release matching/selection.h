#ifndef SELECTION_H_
#define SELECTION_H_

#include <vector>
#include "../common.h"
#include "potentialMatch.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include "wtl_widgets\PropertyGrid.h"

class CPropertyGridCtrl;

namespace FBT
{
	class MatchingHeuristic;
	
	
	// For building up a table of metadata representing a selection (multiple tracks)
	class SelectionMetaTableBuilder : private meta_table_builder
	{

	public:
		SelectionMetaTableBuilder();
		SelectionMetaTableBuilder(const metadb_handle_list& selection);

		void AddTrackToSelection(const metadb_handle_ptr);
		void ClearSelection();

		// Refers to <<multiple values>>
		bool TagHasMultipleValuesProperty(const pfc::string8& tag) const;
		
		const meta_table_builder::t_content& GetMetaTable() const;
		
		void UpdatePropertyGrid(CPropertyGridCtrl&) const;
		void DebugToConsole() const;
		
	private:
		// Pre-Conditions: 
		//		- Tag doesn't already exist as a key
		//		- Tag is valid
		// Returns true if added successfully, false otherwise.
		bool AddTagAsKey(const pfc::string8& tag);
		bool HasTagAsKey(const pfc::string8& tag) const;

		void AddTagValues(const file_info& track, const pfc::string8& tag);
		pfc::string8 GetDecoratedTagValues(const pfc::string8& tag) const;


		pfc::map_t<pfc::string8, bool, file_info::field_name_comparator> tagHasMultipleValuesProperty;
		bool processedATrack;
	};




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
		
void FindMatches(MatchingHeuristic&);

	private:
		metadb_handle_list tracks;
		SelectionMetaTableBuilder metaTableBuilder;
		
		bool matchAttempted;
		std::vector<PotentialMatch> potentialMatches;
	};


}


#endif

//meta_table_builder