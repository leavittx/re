#include "globals.h"

#include "core/system.h"
#include "demo/demo.h"
#include "render/texturemanager.h"
#include "render/shadermanager.h"

#include "fx/testcoreprofile.h"
#include "fx/terrain.h"
#include "fx/heightmap.h"
#include "fx/warping.h"

using namespace recore;
using namespace redemo;
using namespace render;
using namespace std;

reutil::Debug reutil::g_debug;

int main()
{
	System::inst().init();

	Demo *demo = new Demo();

	if (demo->initOk())
	{
		TextureManager::inst().init();
		TextureManager::inst().loadImages();
		ShaderManager::inst().init();

//		demo->addScene("core", new TestCoreProfileScene());
//		demo->addSceneToTimeline("core", 0, 1000000, 0);

//		demo->addScene("terrain", new TerrainScene());
//		demo->addSceneToTimeline("terrain", 0, 1000000, 0);

		demo->addScene("heightmap", new HeightMapScene());
		demo->addSceneToTimeline("heightmap", 0, 1000000, 0);

//		demo->addScene("warping", new WarpingScene());
//		demo->addSceneToTimeline("warping", 0, 1000000, 0);

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
			if (System::inst().getTime() - lastUpdate > 200)
			{
				stringstream ss;
//				ss << "time = " << System::inst().getTime() << " fps = " << System::inst().getFPS();
				ss << System::inst().getFPS() << " fps";
				System::inst().setWindowTitle(ss.str().c_str());
				lastUpdate = System::inst().getTime();
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
