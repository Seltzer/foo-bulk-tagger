#ifndef POTENTIAL_MATCH_H_
#define POTENTIAL_MATCH_H_

#include <musicbrainz3/model.h>


namespace FBT
{

	class SelectionDivision;
	class MusicBrainz::Release;



	struct PotentialMatch
	{
		PotentialMatch()
			: tracks(NULL), match(NULL), score(0)
		{
		}

		SelectionDivision* tracks;
		MusicBrainz::Release* match;
		unsigned score;

	};


}




#endif
