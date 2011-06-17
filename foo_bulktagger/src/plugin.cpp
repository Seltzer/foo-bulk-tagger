#include "common.h"
#include "config.h"

//#include "ui/threadedRunners.h"
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
	// previously the src of all evil
}


Plugin::~Plugin()
{
	//appFrame->deleteLater();
	//app->quit();
}


void Plugin::SpawnMatchWithReleasesDialog(SelectionTreeModel* model)
{
	MatchWithReleasesDialog* dialog = new MatchWithReleasesDialog(model, core_api::get_main_window());

	if (!dialog->Create(NULL))
		console::printf("Dialog creation failed");

	dialog->ShowWindow(SW_SHOW);

	dialog->SendMessageW(dialog->operator HWND(), WM_TESTMSGLOOP, 0U, 0U);
	dialog->PostMessageW(WM_TESTMSGLOOP, 0U, 0U);
}






}


