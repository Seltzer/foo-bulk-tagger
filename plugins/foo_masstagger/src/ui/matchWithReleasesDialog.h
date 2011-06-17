#ifndef _MATCH_WITH_RELEASES_DIALOG_H_
#define _MATCH_WITH_RELEASES_DIALOG_H_


#include <QFrame>



class MatchWithReleasesDialog : public QFrame
{

public:
	MatchWithReleasesDialog();

private:
	metadb_handle_list selectedTracks;
	//vector<MusicBrainzRelease> candidateMatches;
};




#endif




