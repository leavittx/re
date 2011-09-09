#include <core/system.h>
#include <util/debug.h>

using namespace recore;

reutil::Debug reutil::g_debug;

int main(void)
{
	System<>::inst().init();


	return 0;
}
