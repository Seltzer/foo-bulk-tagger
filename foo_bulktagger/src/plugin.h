#ifndef PLUGIN_H_
#define PLUGIN_H_


#include "common.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>


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