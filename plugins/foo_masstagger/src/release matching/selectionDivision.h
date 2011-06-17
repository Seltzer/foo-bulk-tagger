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

		virtual void AddTrackToSelection(metadb_handle_ptr) = 0;
		virtual void AddTracksToSelection(const metadb_handle_list&) = 0;

		// Could use strategy pattern?
		virtual void DivideSelection() = 0;

		std::vector<SelectionToMatch>& GetSelectionDivisions();

	protected:
		metadb_handle_list selection;
		std::vector<SelectionToMatch> divisions;
	};




	/* ISelectionDivisor implementation which divides tracks according to:
			1.) Artist - Album Name (based on tags when Artist + Album Name are both present)
			2.) ? - Album Name (when only Album Name tag is present)
			3.) Artist - Folder Name (when only Artist tag is present)
			4.) Folder Name (when neither Artist, nor Album Name tags are present)
	 */
	class SelectionDivisorByTags : public ISelectionDivisor
	{

	public:
		SelectionDivisorByTags() {}
		SelectionDivisorByTags(const metadb_handle_list&);

		virtual void AddTrackToSelection(metadb_handle_ptr);
		virtual void AddTracksToSelection(const metadb_handle_list&);

		virtual void DivideSelection();
	};


	/* Can be:
			- Artist string (never has SelectionToMatch)
			- Album string  (has SelectionToMatch if non-empty)
			- Folder string (always has SelectionToMatch)
	 */
	class TreeItem
	{

	public:
		// Doesn't have SelectionToMatch
		TreeItem(std::string data, TreeItem* parent = NULL);

		// Has SelectionToMatch
		TreeItem(SelectionToMatch*, std::string data, TreeItem* parent = NULL);

		void appendChild(TreeItem *child);

		TreeItem *child(int row);
		int childCount() const;
		QVariant data(int column) const;
		int row() const;
		TreeItem *parent();

		int TreeItem::columnCount() const;


		void SetParent(TreeItem* parentItem)
		{
			this->parentItem = parentItem;
		}


	private:
		TreeItem* parentItem;
		QList<TreeItem*> childItems;
		
		QList<QVariant> itemData;
		SelectionToMatch* selectionData;
	};


	class TreeModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		TreeModel(QObject *parent = NULL);
		~TreeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		void AddToRoot(TreeItem*);

	private:
		TreeItem *rootItem;
	};



}





#endif