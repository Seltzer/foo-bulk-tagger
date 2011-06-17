/*
#ifndef THREADED_RUNNERS_H_
#define THREADED_RUNNERS_H_


#include <QThread>




namespace FBT
{

	class SelectionTreeModel;




	class ThreadedRunner : public QThread
	{

	public:
		// QThread override (template method)
		virtual void run();


	protected:
		// Hook component of run() template method
		virtual void run_hook() = 0;
	};



	

	class MatchWithReleasesDialogRunner : public ThreadedRunner
	{

	public:
		MatchWithReleasesDialogRunner(SelectionTreeModel*);

	protected:
		virtual void run_hook();


	private:
		SelectionTreeModel* treeModel;

	};







}










#endif
*/