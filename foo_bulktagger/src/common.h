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



// Wrapper for convert_utf8_to_wide
#define STRING8_TO_LPCTSTR(input, maxLength, output)													\
	wchar_t output[maxLength + 1];																		\
	pfc::stringcvt::convert_utf8_to_wide(output,256, input.get_ptr(), input.length());

