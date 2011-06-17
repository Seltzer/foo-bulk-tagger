#include <algorithm>

#include "selectionTreeModel.h"
#include "selectionDivision.h"

using namespace std;


namespace FBT
{



//////////////////////////////// Utility functions
bool selectionTreeNodeComparator(const SelectionTreeNode* node1, const SelectionTreeNode* node2)
{
	return node1->GetStringData() < node2->GetStringData();
}






/////////////////////////////// SelectionTreeNode implementation
SelectionTreeNode::SelectionTreeNode(const pfc::string8& data, SelectionTreeNode* parent)
	: parentNode(parent), stringData(data)
{
}

SelectionTreeNode::SelectionTreeNode(SelectionToMatch* selection, const pfc::string8& data, SelectionTreeNode* parent)
	: selectionData(selection), parentNode(parent), stringData(data)
{
}

void SelectionTreeNode::AppendChild(SelectionTreeNode* child)
{
	if (HasChild(child->GetStringData()))
		return;

	child->SetParent(this);
	childNodes.push_back(child);
}


const pfc::string8& SelectionTreeNode::GetStringData() const
{
	return stringData;
}


bool SelectionTreeNode::HasChild(const pfc::string8& childData) const
{
	for(vector<SelectionTreeNode*>::const_iterator it = childNodes.begin(); it < childNodes.end(); it++)
	{
		if ((*it)->GetStringData() == childData)
			return true;
	}

	return false;
}

SelectionTreeNode* SelectionTreeNode::GetChild(const pfc::string8& childData)
{
	for(vector<SelectionTreeNode*>::const_iterator it = childNodes.begin(); it < childNodes.end(); it++)
	{
		if ((*it)->GetStringData() == childData)
			return *it;
	}

	return NULL;
}

void SelectionTreeNode::SortChildrenAlphabetically(bool recurse)
{
	std::sort(childNodes.begin(), childNodes.end(), selectionTreeNodeComparator);

	if (recurse)
	{
		for(vector<SelectionTreeNode*>::const_iterator it = childNodes.begin(); it < childNodes.end(); it++)
			(*it)->SortChildrenAlphabetically(true);
	}
}

bool SelectionTreeNode::HasSelectionData() const
{
	return selectionData != NULL;
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
	return childNodes[row];
}

int SelectionTreeNode::ChildCount() const
{
	return childNodes.size();
}









/////////////////////////////// SelectionTreeModel implementation
SelectionTreeModel::SelectionTreeModel()
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

bool SelectionTreeModel::HasArtist(const pfc::string8& artistName) const
{
	return rootNode->HasChild(artistName);
}

SelectionTreeNode* SelectionTreeModel::GetArtist(const pfc::string8& artistName)
{
	return rootNode->GetChild(artistName);
}

SelectionTreeNode* SelectionTreeModel::ArtistAt(unsigned index)
{
	return rootNode->ChildAt(index);
}

unsigned SelectionTreeModel::ArtistCount() const
{
	return rootNode->ChildCount();
}


SelectionToMatch* SelectionTreeModel::FetchOrCreate(const pfc::string8& artistName, const pfc::string8& albumName)
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

void SelectionTreeModel::SortTreeAlphabetically()
{
	rootNode->SortChildrenAlphabetically(true);
}


SelectionTreeNode* SelectionTreeModel::GetRoot()
{
	return rootNode;
}






}

