#include "selectionTreeModel.h"
#include "selectionDivision.h"


namespace FBT
{



//////////////////////////////// Utility functions
bool selectionTreeNodeComparator(const SelectionTreeNode* node1, const SelectionTreeNode* node2)
{
	return node1->GetStringData() < node2->GetStringData();
}






/////////////////////////////// SelectionTreeNode implementation
SelectionTreeNode::SelectionTreeNode(const std::string& data, SelectionTreeNode* parent)
	: parentNode(parent), stringData(data)
{
}

SelectionTreeNode::SelectionTreeNode(SelectionToMatch* selection, const std::string& data, SelectionTreeNode* parent)
	: selectionData(selection), parentNode(parent), stringData(data)
{
}

void SelectionTreeNode::AppendChild(SelectionTreeNode* child)
{
	if (HasChild(child->GetStringData()))
		return;

	child->SetParent(this);
	childNodes.append(child);
}

void SelectionTreeNode::InsertChildInOrder(SelectionTreeNode* child)
{
	if (HasChild(child->GetStringData()))
		return;


	child->SetParent(this);
	childNodes.append(child);
	qStableSort(childNodes.begin(), childNodes.end(), selectionTreeNodeComparator);
}


const std::string& SelectionTreeNode::GetStringData(int role) const
{
	if (role == Qt::DisplayRole)
		return stringData;
	else if (role == Qt::UserRole)
		return stringData;
	else
		return stringData; // lulz
}


bool SelectionTreeNode::HasChild(const std::string& childData) const
{
	foreach(SelectionTreeNode* child, childNodes)
	{
		if (child->GetStringData() == childData)
			return true;
	}

	return false;
}

SelectionTreeNode* SelectionTreeNode::GetChild(const std::string& childData)
{
	foreach(SelectionTreeNode* child, childNodes)
	{
		if (child->GetStringData() == childData)
			return child;
	}

	return NULL;
}

SelectionToMatch* SelectionTreeNode::GetSelectionData() const
{
	return selectionData;
}

SelectionTreeNode* SelectionTreeNode::GetParent()
{
	return parentNode;
}

void SelectionTreeNode::SetParent(SelectionTreeNode* parentNode)
{
	this->parentNode = parentNode;
}

SelectionTreeNode* SelectionTreeNode::ChildAt(int row)
{
	return childNodes.value(row);
}

int SelectionTreeNode::ChildCount() const
{
	return childNodes.count();
}

int SelectionTreeNode::Row() const
{
	if (parentNode)
		return parentNode->childNodes.indexOf(const_cast<SelectionTreeNode*>(this));

	return 0;
}









/////////////////////////////// SelectionTreeModel implementation
SelectionTreeModel::SelectionTreeModel(QObject* parent)
	: QAbstractItemModel(parent)
{
	rootNode = new SelectionTreeNode("Releases to Match");
}

SelectionTreeModel::~SelectionTreeModel()
{
	delete rootNode;
}

void SelectionTreeModel::AppendArtist(SelectionTreeNode* artist)
{
	rootNode->AppendChild(artist);
}

void SelectionTreeModel::InsertArtistInOrder(SelectionTreeNode* artist)
{
	rootNode->InsertChildInOrder(artist);
}

bool SelectionTreeModel::HasArtist(const std::string& artistName) const
{
	return rootNode->HasChild(artistName);
}

SelectionTreeNode* SelectionTreeModel::GetArtist(const std::string& artistName)
{
	return rootNode->GetChild(artistName);
}

SelectionToMatch* SelectionTreeModel::FetchOrCreate(const std::string& artistName, const std::string& albumName)
{
	SelectionTreeNode* artist = GetArtist(artistName);
	SelectionTreeNode* album = NULL;

	if (artist)
	{
		// Artist node exists... let's see if it has the album node we're after
		album = artist->GetChild(albumName);

		if (!album)
		{
			// Nope! Create it
			album = new SelectionTreeNode(new SelectionToMatch(), albumName, artist);
			artist->AppendChild(album);
		}
	}
	else
	{
		// Create an artist node
		artist = new SelectionTreeNode(artistName, rootNode);
		rootNode->AppendChild(artist);
		
		// Create an album node
		album = new SelectionTreeNode(new SelectionToMatch(), albumName, artist);
		artist->AppendChild(album);
	}

	return album->GetSelectionData();
}


QVariant SelectionTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	SelectionTreeNode* item = static_cast<SelectionTreeNode*>(index.internalPointer());
	return QString(item->GetStringData(role).c_str());
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
		return QString(rootNode->GetStringData(role).c_str());

	return QVariant();
}

QModelIndex SelectionTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	SelectionTreeNode* parentNode;

	if (!parent.isValid())
		parentNode = rootNode;
	else
		parentNode = static_cast<SelectionTreeNode*>(parent.internalPointer());

	SelectionTreeNode* childItem = parentNode->ChildAt(row);
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
	SelectionTreeNode* parentNode = childItem->GetParent();

	if (parentNode == rootNode)
		return QModelIndex();

	return createIndex(parentNode->Row(), 0, parentNode);
}

int SelectionTreeModel::rowCount(const QModelIndex& parent) const
{
	SelectionTreeNode* parentNode;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentNode = rootNode;
	else
		parentNode = static_cast<SelectionTreeNode*>(parent.internalPointer());

	return parentNode->ChildCount();
}

int SelectionTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}




































}