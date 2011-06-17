#include "selectionDivision.h"
#include "matchingHeuristics.h"

using namespace std;




namespace FBT
{



///////////////// SelectionToMatch implementation
SelectionToMatch::SelectionToMatch(const metadb_handle_list& tracks)
	: tracks(tracks), matchAttempted(false)
{
}

void SelectionToMatch::AddTrack(metadb_handle_ptr track)
{
	tracks.add_item(track);
}

void SelectionToMatch::AddTracks(const metadb_handle_list& tracksToAdd)
{
	tracks += tracksToAdd;
	tracks.remove_duplicates();
}

const metadb_handle_list& SelectionToMatch::GetTracks()
{
	return tracks;
}

void SelectionToMatch::FindMatches(MatchingHeuristic& heuristic)
{
	potentialMatches = heuristic.FindMatches(*this);
	
	matchAttempted = true;
}





///////////////// ISelectionDivisor implementation
ISelectionDivisor::ISelectionDivisor(const metadb_handle_list& initSelection)
	: selection(initSelection)
{
}


vector<SelectionToMatch>& ISelectionDivisor::GetSelectionDivisions()
{
	return divisions;
}



///////////////// SelectionDivisorByTags implementation
SelectionDivisorByTags::SelectionDivisorByTags(const metadb_handle_list& initSelection)
	: ISelectionDivisor(initSelection)
{
}

void SelectionDivisorByTags::AddTrackToSelection(metadb_handle_ptr track)
{
	selection.add_item(track);	
}

void SelectionDivisorByTags::AddTracksToSelection(const metadb_handle_list& tracks)
{
	selection += tracks;
	selection.remove_duplicates();
}

void SelectionDivisorByTags::DivideSelection()
{
	// For now, chuck everything into one SelectionDivision
	divisions.push_back(SelectionToMatch());
	divisions[0].AddTracks(selection);
}






TreeItem::TreeItem(std::string data, TreeItem* parent)
	: parentItem(parent)
{
	itemData.append(QVariant(data.c_str()));
}

TreeItem::TreeItem(SelectionToMatch* selection, std::string data, TreeItem* parent)
	: selectionData(selection), parentItem(parent)
{
	itemData.append(QVariant(data.c_str()));
}


/*
void TreeItem::appendChild(TreeItem* child)
{
	childItems.append(child);
}

TreeItem* TreeItem::child(int row)
{
	return childItems.value(row);
}

int TreeItem::childCount() const
{
	return childItems.count();
}

QVariant TreeItem::data(int column) const
{
	return itemData.value(column);
}


int TreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

TreeItem* TreeItem::parent()
{
	return parentItem;
}

int TreeItem::columnCount() const
{
	return itemData.count();
}
*/


 void TreeItem::appendChild(TreeItem *item)
 {
	 item->SetParent(this);
     childItems.append(item);
 }

 TreeItem *TreeItem::child(int row)
 {
     return childItems.value(row);
 }

 int TreeItem::childCount() const
 {
     return childItems.count();
 }

 int TreeItem::columnCount() const
 {
     return itemData.count();
 }

 QVariant TreeItem::data(int column) const
 {
     return itemData.value(column);
 }

 TreeItem *TreeItem::parent()
 {
     return parentItem;
 }

 int TreeItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

     return 0;
 }



/*
TreeModel::TreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	rootItem = new TreeItem("Root baby yeah!");
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}
	
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}



QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}



void TreeModel::AddToRoot(TreeItem* item)
{
	rootItem->appendChild(item);
}
	

int TreeModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}
*/



 TreeModel::TreeModel(QObject *parent)
     : QAbstractItemModel(parent)
 {
     //QList<QVariant> rootData;
     //rootData << "Title" << "Summary";
     rootItem = new TreeItem("hai");
     //setupModelData(data.split(QString("\n")), rootItem);
 }

 TreeModel::~TreeModel()
 {
     delete rootItem;
 }

 int TreeModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant TreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     TreeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     TreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex TreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
     TreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int TreeModel::rowCount(const QModelIndex &parent) const
 {
     TreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }


 void TreeModel::AddToRoot(TreeItem* item)
{
	rootItem->appendChild(item);
}
	











}