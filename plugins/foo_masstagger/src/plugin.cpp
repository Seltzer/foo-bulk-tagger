#include "common.h"
#include "plugin.h"

#include <Qt/qapplication.h>
#include "ui/matchWithReleasesDialog.h"


#include "config.h"





// Static Displayer fields
Plugin* Plugin::pluginInstance (NULL);
//QFont* Displayer::defaultFont(NULL);






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
	start();
	while(!initialised);
}


Plugin::~Plugin()
{
//	appFrame->deleteLater();
	app->quit();
	

	// defaultFont should be nullified before being deleted to prevent a potential race condition
//	QFont* font = Displayer::defaultFont;
//	Displayer::defaultFont = NULL;
//	delete font;
	
//	delete qtAppThread;
}


void Plugin::SpawnMatchWithReleasesDialog()
{
	MatchWithReleasesDialog* newDialog = new MatchWithReleasesDialog;
	matchDialogs.push_back(newDialog);
	newDialog->show();
}


/*
const QFont& Displayer::GetDefaultFont()
{
	if (defaultFont == NULL)
		defaultFont = new QFont("helvetica",16,QFont::Bold);

	return *defaultFont;
}

*/

/*
void Displayer::userClosedWindow()
{
	Registry::GetInstance()->DisplayerIsShuttingDown();
	delete this;
}


void Displayer::userResizedWindow(QSize* size)
{
	controlFrame->setGeometry(0,size->height() - controlFrame->height(), size->width(), controlFrame->height());

	QSize dim(size->width(), size->height() - controlFrame->height());

	worldScrollArea->setGeometry(0,0,dim.width(), dim.height());

	dim -= QSize(worldScrollArea->horizontalScrollBar()->width(), 
				worldScrollArea->verticalScrollBar()->height());

	world->setGeometry(0,0, dim.width(), dim.height());
	actionAgent->setGeometry(0,0, dim.width(), dim.height());
	world->repaint();
}*/


void Plugin::run()
{
	WHERES_WALLY

	int argc(0);
	app = new QApplication(argc,NULL);
	
	/*
    // Create the main frame
	appFrame = new QFrame;
	appFrame->setWindowTitle("Algovis Viewer");

	QPalette palette = appFrame->palette();
	palette.setColor(QPalette::Background, QColor(0, 0, 0));
	appFrame->setPalette(palette);
	appFrame->setAutoFillBackground(true);*/
	

	initialised = true;

	app->exec();
}



