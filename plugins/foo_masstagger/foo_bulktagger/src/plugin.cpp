#include "common.h"
#include "config.h"

//#include "ui/threadedRunners.h"
#include "plugin.h"

#include "ui/mwrDialog.h"



namespace FBT
{






Plugin* Plugin::pluginInstance (NULL);

CAppModule _Module;


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


void Plugin::SpawnMatchWithReleasesDialog()
{
	MSG msg;
	
	//MWRDialog* dialog = new MWRDialog(core_api::get_main_window());

	/*
	CMyWindow window;

	

    // Create our main window
    //if ( NULL == dialog->Create ( core_api::get_main_window(), CWindow::rcDefault, _T("My First ATL Window") ))
	



	window.Create(core_api::get_main_window(), CWindow::rcDefault, NULL);

	window.ShowWindow(SW_SHOW);
	window.EnableWindow();
	window.UpdateWindow();*/
	


	MWRDialog* dialog = new MWRDialog();
	
	if ( NULL == dialog->Create ( NULL, CWindow::rcDefault, NULL))
		console::printf("fuck you");

    dialog->ShowWindow(SW_SHOW);
	dialog->SetFocus();
	dialog->EnableWindow();
	dialog->UpdateWindow();
	
	//wndMain.operator HWND

    // Your standard Win32 message loop:
    while ( GetMessage ( &msg, NULL, 0, 0 ) > 0 )
        {
        TranslateMessage ( &msg );
        DispatchMessage ( &msg );
        }

	
	/* CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

	MWRDialog* dialog = new MWRDialog(core_api::get_main_window());
	dialog->create(core_api::get_main_window());
    //wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();*/
 
//_Module.Term();
	//return msg.wParam;

	/*
	if (dialog->create(IDD_MWRDIALOG, core_api::get_main_window()))
	{
		console::printf("success");
	}
	else
	{
		console::printf("massive fail");
	}*/



	//MatchWithReleasesDialogRunner* runner = new MatchWithReleasesDialogRunner(model);
	//runner->start();
}

void Plugin::SpawnMatchWithReleasesDialog(SelectionTreeModel* model)
{
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


