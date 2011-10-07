SOURCES += \
    main.cpp \
    core/glwindowsdl.cpp \
    core/system.cpp \
    demo/demo.cpp \
    demo/frametimer.cpp \
    core/config.cpp \
    demo/scene.cpp \
    util/templateinstantiations.cpp \
    core/configsimple.cpp \
    fx/sphere.cpp \
    fx/test.cpp \
    core/inputmanager.cpp \
    render/glhelper.cpp \
    render/shader.cpp \
    render/shadermanager.cpp \
    util/stringutils.cpp \
    util/imagefactory.cpp \
    render/texture.cpp \
    render/texturemanager.cpp \
    render/fbo.cpp \
    fx/noisy.cpp \
    core/glwindowx.cpp \
    fx/testcoreprofile.cpp \
    fx/terrain.cpp \
    render/batch/trianglebatch.cpp \
    render/batch/batch.cpp \
    render/GLTools/math3d.cpp \
    fx/heightmap.cpp

HEADERS += \
    core/glwindow.h \
    core/glwindowsdl.h \
    core/system.h \
    core/timer.h \
    core/timersdl.h \
    util/singleton.h \
    demo/demo.h \
    demo/frametimer.h \
    core/config.h \
    demo/scene.h \
    util/debug.h \
    math/math.h \
    globals.h \
    core/configsimple.h \
    fx/test.h \
    fx/sphere.h \
    core/keyboard.h \
    core/keyboardsdl.h \
    core/inputmanager.h \
    math/matrix4.h \
    math/vector3.h \
    math/vector2.h \
    render/glhelper.h \
    math/rect.h \
    math/vector4.h \
    render/shader.h \
    render/shadermanager.h \
    util/stringutils.h \
    util/imagefactory.h \
    render/texture.h \
    render/texturemanager.h \
    render/fbo.h \
    fx/noisy.h \
    core/time.h \
    core/glwindowx.h \
    core/keyboardx.h \
    core/timerlinux.h \
	fx/testcoreprofile.h \
    fx/terrain.h \
    render/stockshaders.h \
    render/batch/batch.h \
    render/batch/batchbase.h \
    render/batch/trianglebatch.h \
    render/frustum.h \
    fx/heightmap.h \
    math/aabbox3d.h \
    render/objects.h

#    render/GLTools/math3d.cpp \
#    render/GLTools/GLTriangleBatch.cpp \
#    render/GLTools/GLTools.cpp \
#    render/GLTools/GLShaderManager.cpp \
#    render/GLTools/GLBatch.cpp \

#    render/GLTools/StopWatch.h \
#    render/GLTools/math3d.h \
#    render/GLTools/GLTriangleBatch.h \
#    render/GLTools/GLTools.h \
#    render/GLTools/GLShaderManager.h \
#    render/GLTools/GLMatrixStack.h \
#    render/GLTools/GLGeometryTransform.h \
#    render/GLTools/GLFrustum.h \
#    render/GLTools/GLFrame.h \
#    render/GLTools/GLBatchBase.h \
#    render/GLTools/GLBatch.h \

LIBS += \
    -lSDL \
	-lGL \
    -lGLEW \
    -lIL -lILU -lILUT

#	-lGLU \

INCLUDEPATH += \
    /usr/include/ \
    /usr/include/c++/4.6.1/ \
    /usr/lib/gcc/x86_64-pc-linux-gnu/4.5.3/include/g++-v4/ \
    render/GLTools

































































































