#ifndef SELECTION_TREE_H_
#define SELECTION_TREE_H_

#include "../common.h"
#include <QAbstractItemModel>
#include <QModelIndex>


namespace FBT
{
	class SelectionToMatch;
	


	/* A SelectionTreeNode can be:
			- Artist string (never has SelectionToMatch)
			- Album string  (has SelectionToMatch if non-empty)
			- Folder string (always has SelectionToMatch)
	 */
	class SelectionTreeNode
	{

	public:
		// Owns a SelectionToMatch
		SelectionTreeNode(const std::string& data, SelectionTreeNode* parent = NULL);
		// Doesn't own a SelectionToMatch
		SelectionTreeNode(SelectionToMatch*, const std::string& data, SelectionTreeNode* parent = NULL);

		void AppendChild(SelectionTreeNode*);
		// Insert child and maintain alphabetical order - TODO improve efficiency, this sorts list every time
		void InsertChildInOrder(SelectionTreeNode*);
		// TODO improve efficiency by caching

		bool HasChild(const std::string& childData) const;
		SelectionTreeNode* GetChild(const std::string& childData);
		SelectionTreeNode* ChildAt(int row);
		int ChildCount() const;
				
		int Row() const;

		SelectionTreeNode* GetParent();
		void SetParent(SelectionTreeNode*);

		const std::string& GetStringData(int role = Qt::UserRole) const;
		SelectionToMatch* GetSelectionData() const;


	private:
		SelectionTreeNode* parentNode;
		QList<SelectionTreeNode*> childNodes;
		
		std::string stringData;
		SelectionToMatch* selectionData;
	};



	/* Contains SelectionTreeNodes
	 */
	class SelectionTreeModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		SelectionTreeModel(QObject *parent = NULL);
		~SelectionTreeModel();

		void AppendArtist(SelectionTreeNode*);
		void InsertArtistInOrder(SelectionTreeNode*);
		
		bool HasArtist(const std::string& artistName) const;

		// TODO provide overload for folder name
		// TODO doesn't handle blank artist/album tags yet
		SelectionToMatch* FetchOrCreate(const std::string& artist, const std::string& album);

		// QT Overrides
		QVariant data(const QModelIndex& index, int role) const;
		Qt::ItemFlags flags(const QModelIndex& index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex& index) const;
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
	

	private:
		SelectionTreeNode* GetArtist(const std::string& artistName);
		
		SelectionTreeNode* rootNode;

	};




}








#endif