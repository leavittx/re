#include "globals.h"

using namespace std;
using namespace recore;
using namespace redemo;

reutil::Debug reutil::g_debug;

int main(void)
{
	System<>::inst().init();

	string scriptfile("data/script.txt");
	Demo<> *demo = new Demo<>(scriptfile);

	if (demo->initOk())
	{
		demo->start();

		System<>::inst().setWindowTitle("BMT::re engine test");

		bool demorunning = true;
        while (demorunning)
        {
			System<>::inst().update();

			demo->update();
            demo->draw();

			System<>::inst().swapBuffers();
		}

		demo->stop();
	}

	delete demo;

	return 0;
}
