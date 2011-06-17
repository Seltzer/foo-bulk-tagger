#ifndef PLUGIN_H_
#define PLUGIN_H_


#include "common.h"
#include <QThread>

class QApplication;



namespace FBT
{
	class SelectionTreeModel;
	class MatchWithReleasesDialog;


	class Plugin : public QThread
	{
		Q_OBJECT

	public:

		static Plugin* GetInstance();
		
		static void ShowWindow();
		static void HideWindow();
		
		// QThread override
		virtual void run();

		void SpawnMatchWithReleasesDialog(SelectionTreeModel*);
		
	private:
		static Plugin* pluginInstance;

		// Initialisation/Destruction
		Plugin();
		~Plugin();

		QApplication* app;

		bool initialised;	
	};

}





#endif