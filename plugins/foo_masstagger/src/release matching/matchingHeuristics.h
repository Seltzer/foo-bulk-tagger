#ifndef MATCHING_HEURISTICS_H_
#define MATCHING_HEURISTICS_H_

#include <vector>

#include "potentialMatch.h"



namespace FBT
{
	class SelectionToMatch;


	class MatchingHeuristic
	{

	public:
		virtual std::vector<PotentialMatch> FindMatches(const SelectionToMatch&) = 0;

	};


	class MatchingHeuristicImpl : public MatchingHeuristic
	{
	
	public:
		virtual std::vector<PotentialMatch> FindMatches(const SelectionToMatch&);
	};



}









#endif