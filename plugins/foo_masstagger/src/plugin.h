#ifndef PLUGIN_H_
#define PLUGIN_H_



#include "common.h"

#include <vector>
#include "release matching/selectionDivision.h"
#include <QFont>
#include <QThread>

class QApplication;



namespace FBT
{
	class MatchWithReleasesDialog;


	class Plugin : public QThread
	{

	public:

		static Plugin* GetInstance();
		
		static void ShowWindow();
		static void HideWindow();
		
		// QThread override
		virtual void run();

		void SpawnMatchWithReleasesDialog(SelectionTreeModel*);
			
		
		//static const QFont& GetDefaultFont();
		//static QFont* defaultFont;

	private:
		static Plugin* pluginInstance;

		// Initialisation/Destruction
		Plugin();
		~Plugin();

		QApplication* app;

		bool initialised;	

		std::vector<MatchWithReleasesDialog*> matchDialogs;

	};

}





#endif