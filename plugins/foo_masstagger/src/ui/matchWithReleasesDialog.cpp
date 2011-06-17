#include "../common.h"
#include "matchWithReleasesDialog.h"
#include "../selectionDivision.h"
#include <QTreeView>


namespace FBT
{




MatchWithReleasesDialog::MatchWithReleasesDialog()
{
	setWindowTitle("Match Dialog");

	// TODO wasteful
	//this->palette().setColor(QPalette::Background, QColor(0, 0, 0));
	//setPalette(palette);
	setAutoFillBackground(true);

	setFixedSize(400,300);
}

MatchWithReleasesDialog::MatchWithReleasesDialog(TreeModel* model)
	: model(model)
{
	setWindowTitle("Match Dialog");

	// TODO wasteful
	//this->palette().setColor(QPalette::Background, QColor(0, 0, 0));
	//setPalette(palette);
	setAutoFillBackground(true);

	setFixedSize(400,300);

	
	view = new QTreeView(this);

	QAbstractItemModel* modelCast = (QAbstractItemModel*) model;
	console::printf("successful cast");
	view->setModel(modelCast);
		
	view->move(10,10);
	view->resize(200,200);
	

}




}



/*
HWND MatchWithReleasesDialog::Create(HWND p_hWndParent) 
{
	return super::Create(core_api::get_main_window(),
		TEXT(COMPONENT_NAME),
		WS_POPUP | WS_THICKFRAME | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		WS_EX_TOOLWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500);
}

void MatchWithReleasesDialog::Draw(HDC hdc, CRect rcPaint) {
	// We will paint the background in the default window color.
	HBRUSH hBrush = GetSysColorBrush(COLOR_WINDOW);
	FillRect(hdc, rcPaint, hBrush);

	HICON hIcon = static_api_ptr_t<ui_control>()->get_main_icon();
	if (hIcon != NULL) {
		DrawIconEx(hdc, 2, 2, hIcon, 32, 32, 0, hBrush, DI_NORMAL);
	}

	try
	{
		static_api_ptr_t<play_control> pc;
		metadb_handle_ptr handle;;
		if (pc->get_now_playing(handle)) {
			pfc::string8 format;
			g_advconfig_string_format.get_static_instance().get_state(format);
			service_ptr_t<titleformat_object> script;
			static_api_ptr_t<titleformat_compiler>()->compile_safe(script, format);

			pfc::string_formatter text;
			pc->playback_format_title_ex(handle, NULL, text, script, NULL, play_control::display_level_titles);
		}
	}
	catch (const std::exception & exc) {
		console::formatter() << "Exception occurred while drawing " COMPONENT_NAME " window:\n" << exc;
	}
}
*/