#ifndef MATCHING_HEURISTICS_H_
#define MATCHING_HEURISTICS_H_

#include "../common.h"
#include "potentialMatch.h"



namespace FBT
{
	class SelectionToMatch;


	class MatchingHeuristic
	{

	public:
		virtual pfc::chain_list_v2_t<PotentialMatch> FindMatches(const SelectionToMatch&) = 0;

	};


	class MatchingHeuristicImpl : public MatchingHeuristic
	{
	
	public:
		virtual pfc::chain_list_v2_t<PotentialMatch> FindMatches(const SelectionToMatch&);
	};



}









#endif