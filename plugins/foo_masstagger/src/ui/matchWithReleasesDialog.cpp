/*
#include "../common.h"
#include "matchWithReleasesDialog.h"
#include "../selectionDivision.h"





namespace FBT
{




MatchWithReleasesDialog::MatchWithReleasesDialog(SelectionTreeModel* model)
	: model(model)
{
	setWindowTitle("Match Dialog");

	// TODO wasteful
	//this->palette().setColor(QPalette::Background, QColor(0, 0, 0));
	//setPalette(palette);
	setAutoFillBackground(true);

	setFixedSize(400,300);
	


	view = new QTreeView(this);
	view->setModel((QAbstractItemModel*) model);
	view->move(10,10);
	view->resize(200,200);
	view->setAnimated(true);
	view->setVisible(true);
}




}


*/