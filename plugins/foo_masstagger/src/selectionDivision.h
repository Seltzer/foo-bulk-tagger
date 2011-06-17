#ifndef SELECTION_DIVISOR_H_
#define SELECTION_DIVISOR_H_

#include <vector>
#include <musicbrainz3/webservice.h>
#include <musicbrainz3/query.h>
#include <musicbrainz3/model.h>
#include "common.h"



namespace FBT
{

	class MatchingHeuristic;


	class SelectionToMatch
	{

	public:
		SelectionToMatch() {}
		SelectionToMatch(const metadb_handle_list&);

		void AddTrack(metadb_handle_ptr);
		void AddTracks(const metadb_handle_list&);

		// Change return type to bool when we know the semantics
		void AttemptMatch(MatchingHeuristic&);

	private:
		metadb_handle_list tracks;
		

		bool matchAttempted;
		std::vector<MusicBrainz::Release> matchedReleases;
	};




	class ISelectionDivisor
	{

	public:
		ISelectionDivisor() {}
		ISelectionDivisor(const metadb_handle_list&);

		virtual void AddTrackToSelection(metadb_handle_ptr) = 0;
		virtual void AddTracksToSelection(const metadb_handle_list&) = 0;

		// Could use strategy pattern?
		virtual void DivideSelection() = 0;

	protected:
		metadb_handle_list selection;
		std::vector<SelectionToMatch> divisions;
	};





	class SelectionDivisorByTags : public ISelectionDivisor
	{

	public:
		SelectionDivisorByTags() {}
		SelectionDivisorByTags(const metadb_handle_list&);

		virtual void AddTrackToSelection(metadb_handle_ptr);
		virtual void AddTracksToSelection(const metadb_handle_list&);

		virtual void DivideSelection();
	};



}





#endif