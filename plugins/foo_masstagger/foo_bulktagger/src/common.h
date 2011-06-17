#include "foobar2000.h"
#include "helpers.h"
#include "../res/resource.h"

#include <windowsx.h>

#define COMPONENT_NAME "Foo Bulk Tagger"
#define COMPONENT_NAME_MINIMAL "Bulk Tagger"


#define WM_TESTMSGLOOP WM_USER + 1



// Keystroke friendly print macro
#define prt(x) console::print(x);

#define WHERES_WALLY console::printf("Inside %s at %d", __FUNCTION__, __LINE__);

