#ifndef PLUGIN_H_
#define PLUGIN_H_


#include "common.h"




namespace FBT
{
	class SelectionTreeModel;
	class MatchWithReleasesDialog;


	class Plugin
	{
	public:

		static Plugin* GetInstance();
		
		static void ShowWindow();
		static void HideWindow();
		
		virtual void run();

		void SpawnMatchWithReleasesDialog(SelectionTreeModel*);
		
	private:
		static Plugin* pluginInstance;

		// Initialisation/Destruction
		Plugin();
		~Plugin();


		bool initialised;	
	};

}





#endif