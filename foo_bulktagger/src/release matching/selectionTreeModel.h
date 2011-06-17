#ifndef SELECTION_TREE_MODEL_H_
#define SELECTION_TREE_MODEL_H_

#include <vector>
#include "../common.h"


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
		SelectionTreeNode(const pfc::string8& data, SelectionTreeNode* parent = NULL);
		// Doesn't own a SelectionToMatch
		SelectionTreeNode(SelectionToMatch*, const pfc::string8& data, SelectionTreeNode* parent = NULL);

		void AppendChild(SelectionTreeNode*);
		// TODO improve efficiency by caching

		bool HasChild(const pfc::string8& childData) const;
		SelectionTreeNode* GetChild(const pfc::string8& childData);
		SelectionTreeNode* ChildAt(int row);
		int ChildCount() const;

		void SortChildrenAlphabetically(bool recurse);

		SelectionTreeNode* GetParent();
		void SetParent(SelectionTreeNode*);

		const pfc::string8& GetStringData() const;
		bool HasSelectionData() const;
		SelectionToMatch* GetSelectionData() const;


	private:
		SelectionTreeNode* parentNode;
		std::vector<SelectionTreeNode*> childNodes;
		
		pfc::string8 stringData;
		SelectionToMatch* selectionData;
	};
	


	/* Contains SelectionTreeNodes
	 */
	class SelectionTreeModel
	{

	public:
		SelectionTreeModel();
		~SelectionTreeModel();

		void AppendArtist(SelectionTreeNode*);
		bool HasArtist(const pfc::string8& artistName) const;
		SelectionTreeNode* ArtistAt(unsigned);
		unsigned ArtistCount() const;

		// TODO provide overload for folder name
		// TODO doesn't handle blank artist/album tags yet
		SelectionToMatch* FetchOrCreate(const pfc::string8& artist, const pfc::string8& album);

		SelectionTreeNode* GetRoot();
		
		void SortTreeAlphabetically();


	private:
		SelectionTreeNode* GetArtist(const pfc::string8& artistName);
		
		SelectionTreeNode* rootNode;

	};




}









#endif