#ifndef _MATCH_WITH_RELEASES_DIALOG_H_
#define _MATCH_WITH_RELEASES_DIALOG_H_

#include <QFrame>


class QTreeView;




namespace FBT
{

	class SelectionTreeModel;
	

	class MatchWithReleasesDialog : public QFrame
	{

	public:
		MatchWithReleasesDialog(SelectionTreeModel*);

	private:
		metadb_handle_list selectedTracks;
		SelectionTreeModel* model;

		QTreeView* view;
	};



}








#endif




