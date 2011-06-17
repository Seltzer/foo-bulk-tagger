#ifndef SELECTION_DIVISOR_H_
#define SELECTION_DIVISOR_H_

#include <vector>
#include "../common.h"
#include "potentialMatch.h"
#include <QAbstractItemModel>
#include <QModelIndex>



namespace FBT
{
	class MatchingHeuristic;
	class SelectionTreeModel;



	// A selection of tracks to be matched with one release in the MusicBrainz database
	class SelectionToMatch
	{

	public:
		SelectionToMatch() {}
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

	   And generates a tree
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


	
	/* A SelectionTreeNode can be:
			- Artist string (never has SelectionToMatch)
			- Album string  (has SelectionToMatch if non-empty)
			- Folder string (always has SelectionToMatch)
	 */
	class SelectionTreeNode
	{

	public:
		// Owns a SelectionToMatch
		SelectionTreeNode(std::string data, SelectionTreeNode* parent = NULL);
		// Doesn't own a SelectionToMatch
		SelectionTreeNode(SelectionToMatch*, std::string data, SelectionTreeNode* parent = NULL);

		void AppendChild(SelectionTreeNode*);

		SelectionTreeNode* ChildAt(int row);
		int ChildCount() const;
		QVariant data(int column) const;
		int Row() const;
		int SelectionTreeNode::ColumnCount() const;

		SelectionTreeNode* GetParent();
		void SetParent(SelectionTreeNode*);


		// Convenience methods
		//void AppendChild(SelectionTreeNode*);
		// Insert child and maintain alphabetical order - TODO improve efficiency, this sorts list every time
		//void InsertChildInOrder(SelectionTreeNode*);
		// TODO improve efficiency by caching
		//bool HasChild(const std::string& childData) const;
		//const std::string& GetStringData(int role = Qt::UserRole) const;
		//SelectionToMatch* GetSelectionData() const;

	private:
		SelectionTreeNode* parentItem;
		QList<SelectionTreeNode*> childItems;
		
		QList<QVariant> itemData;
		SelectionToMatch* selectionData;
	};


	class SelectionTreeModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		SelectionTreeModel(QObject *parent = NULL);
		~SelectionTreeModel();

		QVariant data(const QModelIndex& index, int role) const;
		Qt::ItemFlags flags(const QModelIndex& index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex& index) const;
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;

		void AppendToRoot(SelectionTreeNode*);

	private:
		SelectionTreeNode* rootItem;
	};



}





#endif