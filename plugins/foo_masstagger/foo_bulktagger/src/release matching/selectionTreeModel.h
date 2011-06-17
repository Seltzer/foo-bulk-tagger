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
		SelectionTreeNode(const std::string& data, SelectionTreeNode* parent = NULL);
		// Doesn't own a SelectionToMatch
		SelectionTreeNode(SelectionToMatch*, const std::string& data, SelectionTreeNode* parent = NULL);

		void AppendChild(SelectionTreeNode*);
		// TODO improve efficiency by caching

		bool HasChild(const std::string& childData) const;
		SelectionTreeNode* GetChild(const std::string& childData);
		SelectionTreeNode* ChildAt(int row);
		int ChildCount() const;

		void SortChildrenAlphabetically(bool recurse);

		SelectionTreeNode* GetParent();
		void SetParent(SelectionTreeNode*);

		const std::string& GetStringData() const;
		SelectionToMatch* GetSelectionData() const;


	private:
		SelectionTreeNode* parentNode;
		std::vector<SelectionTreeNode*> childNodes;
		
		std::string stringData;
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
		bool HasArtist(const std::string& artistName) const;
		SelectionTreeNode* ArtistAt(unsigned);
		unsigned ArtistCount() const;



		// TODO provide overload for folder name
		// TODO doesn't handle blank artist/album tags yet
		SelectionToMatch* FetchOrCreate(const std::string& artist, const std::string& album);

		void SortTreeAlphabetically();


	private:
		SelectionTreeNode* GetArtist(const std::string& artistName);
		
		SelectionTreeNode* rootNode;

	};




}









#endif