#ifndef MATCHING_HEURISTICS_H_
#define MATCHING_HEURISTICS_H_

#include <vector>
#include "../common.h"
#include "potentialMatch.h"



namespace FBT
{
	class SelectionToMatch;
	class ScoringHeuristic;
	class MusicBrainz::Release;

	typedef std::vector<std::vector<MusicBrainz::Release*> > ReleaseCombinations;


	/* A MatchingHeuristic:
	 *		- Examines a SelectionToMatch in terms of its audio data, metadata and file data
	 *		- Uses this data to match a SelectionToMatch with candidate Release/ReleaseGroups in MusicBrainz
	 *		- Does this using any number of sub-heuristics
	 *		- Evalutes candidate matches using scoring heuristics which can score at various
	 *           levels of granularity including tracks, releases
	 *		- Can split a SelectionToMatch according to client or server data
	 */
	class MatchingHeuristic
	{

	public:
		virtual pfc::chain_list_v2_t<PotentialMatch> FindMatches(const SelectionToMatch&, ScoringHeuristic&) = 0;

	private:
		pfc::array_t<MatchingHeuristic*> subHeuristics;
	};





	class MatchingHeuristicImpl : public MatchingHeuristic
	{
	
	public:
		virtual pfc::chain_list_v2_t<PotentialMatch> FindMatches(const SelectionToMatch&, ScoringHeuristic&);

	private:
		/* Generally a Selection is matched against a ReleaseGroup containing multiple Releases. This method will take that 
		 * set of releases and generate { x | x ∈ P(Releases) ^ #tracks(x) <= #tracks(Selection) ^ #x > 0} 
		 * That is, all possible non-empty combinations of Releases such that the total number of tracks in the combinations do not exceed the 
		 * number of tracks in the Selection. In this way, we obtain the set of valid allocations of selection tracks to releases
		 */
		//ReleaseCombinations GenerateCandidateCombinations(const SelectionToMatch&, MusicBrainz::ReleaseGroup* releaseGroup);

	};



	class ScoringHeuristic
	{
	public:
		ScoringHeuristic();

		virtual int CalculateScore(const SelectionToMatch&, MusicBrainz::Release*) = 0;
		int GetLastScore();

		const pfc::string8& GetScoreReport();


	protected:
		int score;

		pfc::string8 scoreReport;
	};


	class ScoringHeuristicImpl : public ScoringHeuristic
	{
	public:
		ScoringHeuristicImpl();

		virtual int CalculateScore(const SelectionToMatch&, MusicBrainz::Release*);
	};







}









#endif