#include "common.h"
#include "windows.h"
#include "ui/contextmenu_item_fbt.h"

DECLARE_COMPONENT_VERSION(
	COMPONENT_NAME,
	"0.1",
	"Foo Bulk Tagger\n"
	"for foobar2000 vx.x.x.x\n"
	"\n"
	"Developed by::\n"
	"Nathan Pitman"
);



class initquit_fbt : public initquit 
{
	virtual void on_init() {}
	virtual void on_quit() {}
};

static initquit_factory_t<initquit_fbt> foo_initquit;



BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	/*
	if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH)
        if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
            return(FALSE);

    if (fdwReason == DLL_PROCESS_DETACH || fdwReason == DLL_THREAD_DETACH)
        if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
            return(FALSE);
    return(TRUE);*/

	return TRUE;
}

