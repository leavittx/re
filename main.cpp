#include "globals.h"

#include "core/system.h"
#include "demo/demo.h"
#include "render/texturemanager.h"
#include "render/shadermanager.h"

#include "fx/test.h"
#include "fx/noisy.h"
#include "fx/sphere.h"

using namespace recore;
using namespace redemo;
using namespace render;
using namespace std;

reutil::Debug reutil::g_debug;

int main(void)
{
	System::inst().init();

	Demo *demo = new Demo();

	if (demo->initOk())
	{
		TextureManager::inst().init();
		TextureManager::inst().loadImages();
		ShaderManager::inst().init();

		demo->addScene("test", new TestScene());
		demo->addScene("noisy", new NoisyScene());
//		demo->addScene("sphere", new SphereScene());
		demo->addSceneToTimeline("noisy",      0,  100000, 0);
//		demo->addSceneToTimeline("sphere",     0,  100000, 1);
//		demo->addSceneToTimeline("test",      0, 1000000, 0);

		TextureManager::inst().uploadImages();
		ShaderManager::inst().loadShaders();
		demo->initEffects();

		demo->start();

		System::inst().setWindowTitle("BMT::re engine test");

		bool demorunning = true;
        while (demorunning)
        {
			System::inst().update();

			//too often title updating kills gnome-shell
			static int lastUpdate = System::inst().getTime();
			if (System::inst().getTime() - lastUpdate > 20)
			{
				lastUpdate = System::inst().getTime();

				stringstream ss;
				ss << "time = " << System::inst().getTime() << " fps = " << System::inst().getFPS();
				System::inst().setWindowTitle(ss.str().c_str());
			}

			demorunning = System::inst().pollEvents() &&
						  System::inst().isRunning();

			demo->update();
            demo->draw();

			System::inst().swapBuffers();
		}

		demo->stop();
	}

	// Clean up everything
	delete demo;
	// Exit program
	return 0;
}
