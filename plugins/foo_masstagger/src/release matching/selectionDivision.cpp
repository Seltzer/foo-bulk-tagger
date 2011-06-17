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

void ISelectionDivisor::AddTrackToSelection(metadb_handle_ptr track)
{
	selection.add_item(track);	
}

void ISelectionDivisor::AddTracksToSelection(const metadb_handle_list& tracks)
{
	selection += tracks;
	selection.remove_duplicates();
}


///////////////// SelectionDivisorByTags implementation
SelectionDivisorByTags::SelectionDivisorByTags(const metadb_handle_list& initSelection)
	: ISelectionDivisor(initSelection), treeModel(NULL)
{
}

void SelectionDivisorByTags::DivideSelection()
{
	SelectionTreeNode* topNode1 = new SelectionTreeNode("tn1");
	SelectionTreeNode* topNode2 = new SelectionTreeNode("tn2");
	SelectionTreeNode* aNode = new SelectionTreeNode("aaa", topNode1);
	topNode1->AppendChild(aNode);
	SelectionTreeNode* bNode = new SelectionTreeNode("bbb", topNode1);
	topNode1->AppendChild(bNode);
	SelectionTreeNode* cNode = new SelectionTreeNode("ccc", topNode2);
	topNode2->AppendChild(cNode);

	SelectionTreeNode* dNode = new SelectionTreeNode("ddd", aNode);
	aNode->AppendChild(dNode);
	SelectionTreeNode* eNode = new SelectionTreeNode("eee", aNode);
	aNode->AppendChild(eNode);
	SelectionTreeNode* fNode = new SelectionTreeNode("fff", aNode);
	aNode->AppendChild(fNode);

	SelectionTreeNode* gNode = new SelectionTreeNode("ggg", dNode);
	dNode->AppendChild(gNode);



	treeModel = new SelectionTreeModel();
	treeModel->AppendToRoot(topNode1);
	treeModel->AppendToRoot(topNode2);

}

SelectionTreeModel* SelectionDivisorByTags::GetTreeModel()
{
	return treeModel;
}





SelectionTreeNode::SelectionTreeNode(std::string data, SelectionTreeNode* parent)
	: parentItem(parent)
{
	itemData.append(QVariant(data.c_str()));
}

SelectionTreeNode::SelectionTreeNode(SelectionToMatch* selection, std::string data, SelectionTreeNode* parent)
	: selectionData(selection), parentItem(parent)
{
	itemData.append(QVariant(data.c_str()));
}


void SelectionTreeNode::SetParent(SelectionTreeNode* parentItem)
{
	this->parentItem = parentItem;
}


void SelectionTreeNode::AppendChild(SelectionTreeNode* item)
{
	item->SetParent(this);
	childItems.append(item);
}

SelectionTreeNode* SelectionTreeNode::ChildAt(int row)
{
	return childItems.value(row);
}

int SelectionTreeNode::ChildCount() const
{
	return childItems.count();
}

int SelectionTreeNode::ColumnCount() const
{
	return itemData.count();
}

QVariant SelectionTreeNode::data(int column) const
{
	return itemData.value(column);
}

SelectionTreeNode* SelectionTreeNode::GetParent()
{
	return parentItem;
}

int SelectionTreeNode::Row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<SelectionTreeNode*>(this));

	return 0;
}



/*
const std::string& SelectionTreeNode::GetStringData(int role) const
{
	console::printf("data %s obtained from node @ %d", stringData.c_str(),this);
	console::printf("\tmy # children = %d", childCount());

	QString myData = QString(stringData.c_str());
	console::printf("my data as qstring = %s", myData.toLocal8Bit().data());
	

	if (role == Qt::DisplayRole)
	{
		return stringData;
	}
	else if (role == Qt::UserRole)
	{
		return stringData;
	}
	else
	{
		return std::string("lll");
	}
}

SelectionToMatch* SelectionTreeNode::GetSelectionData() const
{
	return selectionData;
}

void SelectionTreeNode::AppendChild(SelectionTreeNode* item)
{
	pfc::dynamic_assert(item);

	item->SetParent(this);
	childItems.append(item);
}


bool selectionTreeItemComparator(const SelectionTreeNode* item1, const SelectionTreeNode* item2)
{
	return item1->GetStringData() < item2->GetStringData();
}


void SelectionTreeNode::InsertChildInOrder(SelectionTreeNode* item)
{
	item->SetParent(this);

	childItems.append(item);
	qStableSort(childItems.begin(), childItems.end(), selectionTreeItemComparator);
}

bool SelectionTreeNode::HasChild(const std::string& childData) const
{
	foreach(SelectionTreeNode* child, childItems)
	{
		if (child->GetStringData() == childData)
			return true;
	}

	return false;
}
*/








 SelectionTreeModel::SelectionTreeModel(QObject* parent)
     : QAbstractItemModel(parent)
 {
     //QList<QVariant> rootData;
     //rootData << "Title" << "Summary";
     rootItem = new SelectionTreeNode("hai");
     //setupModelData(data.split(QString("\n")), rootItem);
 }

SelectionTreeModel::~SelectionTreeModel()
{
	delete rootItem;
}

int SelectionTreeModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return static_cast<SelectionTreeNode*>(parent.internalPointer())->ColumnCount();
	else
		return rootItem->ColumnCount();
}

QVariant SelectionTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	SelectionTreeNode* item = static_cast<SelectionTreeNode*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags SelectionTreeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SelectionTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex SelectionTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	SelectionTreeNode* parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<SelectionTreeNode*>(parent.internalPointer());

	SelectionTreeNode* childItem = parentItem->ChildAt(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex SelectionTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	SelectionTreeNode* childItem = static_cast<SelectionTreeNode*>(index.internalPointer());
	SelectionTreeNode* parentItem = childItem->GetParent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->Row(), 0, parentItem);
}

int SelectionTreeModel::rowCount(const QModelIndex& parent) const
{
	SelectionTreeNode* parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<SelectionTreeNode*>(parent.internalPointer());

	return parentItem->ChildCount();
}


void SelectionTreeModel::AppendToRoot(SelectionTreeNode* item)
{
	rootItem->AppendChild(item);
}
	











}