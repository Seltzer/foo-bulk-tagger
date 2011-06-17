#ifndef _MATCH_WITH_RELEASES_DIALOG_H_
#define _MATCH_WITH_RELEASES_DIALOG_H_


#include <QFrame>

class QTreeView;


namespace FBT
{
	class TreeModel;


	class MatchWithReleasesDialog : public QFrame
	{

	public:
		MatchWithReleasesDialog();
		MatchWithReleasesDialog(TreeModel*);

	private:
		metadb_handle_list selectedTracks;
		//vector<MusicBrainzRelease> candidateMatches;

		TreeModel* model;
		QTreeView* view;
	};


}

#endif




