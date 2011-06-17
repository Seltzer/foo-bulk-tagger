#ifndef MWR_DIALOG_H_
#define MWR_DIALOG_H_

#include "../common.h"




namespace FBT
{

	class MWRDialog : public dialog_helper::dialog_modeless_v2
	{


	public:
		MWRDialog(unsigned p_id, HWND p_parent);

		void show_window();
		void enable(bool is_enabled);
		void hide();
	};

}













#endif