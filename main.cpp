#include "globals.h"

#include "core/system.h"
#include "demo/demo.h"

#include "fx/test.h"
#include "fx/sphere.h"

using namespace recore;
using namespace redemo;
using namespace std;

reutil::Debug reutil::g_debug;

int main(void)
{
	System::inst().init();

	string scriptfile("data/script.txt");
	Demo *demo = new Demo(scriptfile);

	if (demo->initOk())
	{
		demo->addScene("test", new TestScene());
		demo->addSceneToTimeline("test", 0, 20000, 0);

		demo->start();

		System::inst().setWindowTitle("BMT::re engine test");

//		for (int i = 0; i < 500; i++)
//		{
//			System::inst().update();

//			stringstream ss;
//			ss << "time = " << System::inst().getTime() + i << " fps = " << System::inst().getFPS();
//			System::inst().setWindowTitle(ss.str().c_str());

//			System::inst().swapBuffers();
//		}

		bool demorunning = true;
        while (demorunning)
        {
			System::inst().update();

			//TODO: kills gnome-shell
//			stringstream ss;
//			ss << "time = " << System::inst().getTime() << " fps = " << System::inst().getFPS();
//			System::inst().setWindowTitle(ss.str().c_str());

			demorunning = System::inst().pollEvents() &&
						  System::inst().isRunning();

			demo->update();
            demo->draw();

			System::inst().swapBuffers();
		}

		demo->stop();
	}

	delete demo;
	return 0;
}
