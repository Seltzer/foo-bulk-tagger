#include <algorithm>

#include "selectionTreeModel.h"
#include "selection.h"

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
	pfc::dynamic_assert(row < childNodes.size()); 

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


SelectionToMatch* SelectionTreeModel::FetchOrCreate(const pfc::string8& artistNameInput, const pfc::string8& albumNameInput)
{
	pfc::string8 artistName = artistNameInput;
	if (artistName.is_empty())
		artistName = "?";
	pfc::string8 albumName = albumNameInput;
	if (albumName.is_empty())
		pfc::dynamic_assert(false);

	
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

	pfc::dynamic_assert(album->HasSelectionData());
	return album->GetSelectionData();
}

SelectionToMatch* SelectionTreeModel::FetchOrCreate(const pfc::string8& artistNameInput, 
										 		    const pfc::string8& albumNameInput, 
													const pfc::string8& directoryNameInput)
{
	pfc::string8 artistName = artistNameInput;
	if (artistName.is_empty())
		artistName = "?";

	pfc::string8 albumName = albumNameInput;
	if (albumName.is_empty())
		albumName = "?";
	
	pfc::string8 directoryName("./");
	directoryName.add_string(directoryNameInput);
	
	

	
	SelectionTreeNode* artist = GetArtist(artistName);
	SelectionTreeNode* album = NULL;
	SelectionTreeNode* directory = NULL;

	// Fetch/create artist/album/directory nodes
	if (!artist)
	{
		artist = new SelectionTreeNode(artistName, rootNode);
		rootNode->AppendChild(artist);
		album = new SelectionTreeNode(albumName, artist);
		artist->AppendChild(album);
		directory = new SelectionTreeNode(new SelectionToMatch(), directoryName, album);
		album->AppendChild(directory);
	}
	else
	{
		album = artist->GetChild(albumName);

		if (!album)
		{
			album = new SelectionTreeNode(albumName, artist);
			artist->AppendChild(album);
			directory = new SelectionTreeNode(new SelectionToMatch(), directoryName, album);
			album->AppendChild(directory);
		}
		else
		{
			directory = album->GetChild(directoryName);

			if (!directory)
			{
				directory = new SelectionTreeNode(new SelectionToMatch(), directoryName, album);
				album->AppendChild(directory);
			}
		}
	}

	pfc::dynamic_assert(directory->HasSelectionData());
	return directory->GetSelectionData();
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

