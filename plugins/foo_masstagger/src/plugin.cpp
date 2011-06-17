#include "common.h"
#include "config.h"

#include "ui/threadedRunners.h"
#include "plugin.h"

#include "ui/mwrDialog.h"



namespace FBT
{




Plugin* Plugin::pluginInstance (NULL);


Plugin* Plugin::GetInstance()
{
	if (!pluginInstance)
		pluginInstance = new Plugin();

	return pluginInstance;
}

void Plugin::ShowWindow() {
	
	cfg_enabled = true;

	/*
	if (!g_instance.IsWindow()) {
		cfg_enabled = (g_instance.Create(core_api::get_main_window()) != NULL);
	}*/
}

void Plugin::HideWindow() {
	
	// Set window state to disabled.
	cfg_enabled = false;

	// Destroy the window.
	//g_instance.Destroy();
//	delete displayer;
}



Plugin::Plugin()
	: initialised(false)
{
	//console::printf("hello - initialised resides at %d", &initialised);

	//console::printf("before start");
	//start();
	//console::printf("before while loop");
	
	//while(!initialised);
	//console::printf("after while loop");
}


Plugin::~Plugin()
{
	//appFrame->deleteLater();
	//app->quit();
}


void Plugin::SpawnMatchWithReleasesDialog(SelectionTreeModel* model)
{
	MWRDialog* dialog = new MWRDialog(IDD_MWRDIALOG,core_api::get_main_window());

	//new find_release_dialog(IDD_DIALOG_FIND_RELEASE, core_api::get_main_window(), p_data);

	//MatchWithReleasesDialogRunner* runner = new MatchWithReleasesDialogRunner(model);
	//runner->start();
	//
}

void Plugin::run()
{
	int argc(0);
	//app = new QApplication(argc,NULL);
	
	initialised = true;

	//console::printf("hello - initialised resides at %d", &initialised);

	//app->exec();
}





}


