#include <musicbrainz3/webservice.h>
#include <musicbrainz3/query.h>
#include <musicbrainz3/model.h>

#include "matchingHeuristics.h"
#include "selection.h"
#include "../common.h"

using namespace std;




namespace FBT
{

	


ReleaseCombinations GenerateCandidateCombinations(const SelectionToMatch& selection, MusicBrainz::ReleaseGroup* releaseGroup)
{
	ReleaseCombinations result;

	/*
	// Iterate over releases in release group
	for (int i = 0; i < releaseGroup->getNumReleases(); i++)
	{
		MusicBrainz::Release* release = releaseGroup->getRelease(i);

		// For each combination in the current power set
		for (int j = 0; j < result.size(); j++)
		{
			// Clone the combination and append release to it
			std::vector<MusicBrainz::Release*> newCombination = result[j];
			newCombination.push_back(release);

			// Add this new combination to the results
			result.push_back(newCombination);
		}

		// Create a singleton combination which has only the release, and add it to the results
		std::vector<MusicBrainz::Release*> singletonCombination(1, release);
		result.push_back(singletonCombination);
	}*/

	// Sort combinations according by ascending length

	return result;
}


pfc::chain_list_v2_t<PotentialMatch> MatchingHeuristicImpl::FindMatches(const SelectionToMatch& tracks, ScoringHeuristic& scoringHeuristic)
{
	pfc::chain_list_v2_t<PotentialMatch> matches;

	// If artist and album are both present
	pfc::chain_list_v2_t<pfc::string8> artistValues = tracks.GetArtistValues();
	pfc::chain_list_v2_t<pfc::string8> albumValues = tracks.GetAlbumValues();
	
	if (artistValues.get_count() == 1 && albumValues.get_count() == 1)
	{
		const char* artist = (*artistValues.first()).get_ptr();
		const char* album = (*albumValues.first()).get_ptr();

		console::printf("Searching for match with artist = %s and album = %s", artist, album);
				
		MusicBrainz::Query q;

		MusicBrainz::ReleaseGroupFilter filter = MusicBrainz::ReleaseGroupFilter().artistName(artist).title(album);
		MusicBrainz::ReleaseGroupResultList results = q.getReleaseGroups(&filter);
		console::printf("Search yields %d results", results.size());

		for(MusicBrainz::ReleaseGroupResultList::iterator it = results.begin(); it < results.end(); it++)
		{
			MusicBrainz::ReleaseGroupResult* releaseGroupResult = *it;
			MusicBrainz::ReleaseGroup* releaseGroup = releaseGroupResult->getReleaseGroup();
			
			
			//ReleaseCombinations releaseCombinations = GenerateCandidateCombinations(tracks, releaseGroup);
			//ReleaseCombinations releaseCombinations;

			/*
			for (ReleaseCombinations::iterator it = releaseCombinations.begin(); it < releaseCombinations.end(); it++)
			{
				// For now, just single disc releases
				if ((*it).size() == 1)
				{
					int score = scoringHeuristic.CalculateScore(tracks, (*it)[0]);

					console::printf("\tRelease result has score = %d... summary below:", score);
					console::printf(scoringHeuristic.GetScoreReport().get_ptr());
					console::printf("\n\n");
				}
			}*/
		}

		/*
		MusicBrainz::ReleaseFilter filter = MusicBrainz::ReleaseFilter().artistName(artist).title(album);
		MusicBrainz::ReleaseResultList results = q.getReleases(&filter);

		console::printf("Got %d results", results.size());

		// TODO I assume results are arranged according to MB score
		for(MusicBrainz::ReleaseResultList::iterator it = results.begin(); it < results.end(); it++)
		{
			MusicBrainz::ReleaseResult* releaseResult = *it;
			MusicBrainz::Release* release = releaseResult->getRelease();
			
			int score = scoringHeuristic.CalculateScore(tracks, release);

			console::printf("\tRelease result has score = %d... summary below:", score);
			console::printf(scoringHeuristic.GetScoreReport().get_ptr());
			console::printf("\n\n");
		}*/
	}
	else
	{
		console::printf("***************unimplemented*******************");
	}


	


	return matches;
}





ScoringHeuristic::ScoringHeuristic() : score(INVALID)
{
}

int ScoringHeuristic::GetLastScore()
{
	return score;
}

const pfc::string8& ScoringHeuristic::GetScoreReport()
{
	return scoreReport;
}



ScoringHeuristicImpl::ScoringHeuristicImpl()
{
}

int ScoringHeuristicImpl::CalculateScore(const SelectionToMatch& selection, MusicBrainz::Release* release)
{
	score = 0;

	// 76% of score for release specific things
	// ... Artist
	pfc::chain_list_v2_t<pfc::string8> artistValues = selection.GetArtistValues();
	pfc::dynamic_assert(artistValues.get_count() == 1);
	pfc::string8 releaseArtist(release->getArtist()->getName().c_str());
	
	// TODO fuzzy matching
	if (releaseArtist == *artistValues.first())
		score += 19;

	pfc::chain_list_v2_t<pfc::string8> albumValues = selection.GetAlbumValues();
	pfc::dynamic_assert(albumValues.get_count() == 1);
	pfc::string8 releaseTitle(release->getTitle().c_str());

	// TODO fuzzy matching
	if (releaseTitle == *albumValues.first())
		score += 19;

	if (selection.GetTrackCount() == release->getTracksCount())
		score += 19;
	

	// TODO date matching for 19%
	
	// and 24% for track specific
	


	#ifdef DEV_MODE
		scoreReport = pfc::string8("Report for potential match:\n");
		scoreReport += "Artist = ";
		scoreReport += release->getArtist()->getName().c_str();
		scoreReport += "\n";
		scoreReport += "Release = ";
		scoreReport += release->getTitle().c_str();
	#endif
			

	return score;
}



}