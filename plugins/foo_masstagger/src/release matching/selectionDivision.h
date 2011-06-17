#ifndef SELECTION_DIVISOR_H_
#define SELECTION_DIVISOR_H_

#include <vector>
#include "../common.h"
#include "potentialMatch.h"




namespace FBT
{
	class MatchingHeuristic;
	class SelectionTreeModel;



	// A selection of tracks to be matched with one release in the MusicBrainz database
	class SelectionToMatch
	{

	public:
		SelectionToMatch();
		SelectionToMatch(const metadb_handle_list&);

		void AddTrack(metadb_handle_ptr);
		void AddTracks(const metadb_handle_list&);
		const metadb_handle_list& GetTracks();

		void FindMatches(MatchingHeuristic&);

	private:
		metadb_handle_list tracks;
		
		bool matchAttempted;
		std::vector<PotentialMatch> potentialMatches;
	};




	// Interface for a class which divides a selection of tracks into SelectionToMatch divisions 
	// according to some criteria
	class ISelectionDivisor
	{

	public:
		ISelectionDivisor() {}
		ISelectionDivisor(const metadb_handle_list&);

		virtual void AddTrackToSelection(metadb_handle_ptr);
		virtual void AddTracksToSelection(const metadb_handle_list&);

		virtual void DivideSelection() = 0;

	protected:
		metadb_handle_list selection;
	};




	/* ISelectionDivisor implementation which divides tracks according to:
			1.) Artist - Album Name (based on tags when Artist + Album Name are both present)
			2.) ? - Album Name (when only Album Name tag is present)
			3.) Artist - Folder Name (when only Artist tag is present)
			4.) Folder Name (when neither Artist, nor Album Name tags are present)

	   And generates a SelectionTreeModel
	 */
	class SelectionDivisorByTags : public ISelectionDivisor
	{

	public:
		SelectionDivisorByTags() {}
		SelectionDivisorByTags(const metadb_handle_list&);

		virtual void DivideSelection();
		SelectionTreeModel* GetTreeModel();

	private:
		SelectionTreeModel* treeModel;
	};





}





#endif