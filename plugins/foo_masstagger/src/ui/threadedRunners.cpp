
#include "../selectionDivision.h"
#include "matchWithReleasesDialog.h"
#include "threadedRunners.h"




namespace FBT
{




void ThreadedRunner::run()
{
	run_hook();

	// Start event loop
	exec();
}




MatchWithReleasesDialogRunner::MatchWithReleasesDialogRunner(SelectionTreeModel* treeModel)
	: treeModel(treeModel)
{
}


void MatchWithReleasesDialogRunner::run_hook()
{
	/*
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



	SelectionTreeModel* treeModel = new SelectionTreeModel();
	treeModel->AppendToRoot(topNode1);
	treeModel->AppendToRoot(topNode2);
*/


	MatchWithReleasesDialog* newDialog = new MatchWithReleasesDialog(treeModel);
	newDialog->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	newDialog->show();
}





}
