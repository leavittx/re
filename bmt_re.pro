SOURCES += \
    main.cpp \
    core/glwindowsdl.cpp \
    core/system.cpp \
    demo/demo.cpp \
    demo/frametimer.cpp \
    core/config.cpp \
    demo/scene.cpp \
    math/math.cpp \
    util/templateinstantiations.cpp \
    core/configsimple.cpp \
    fx/sphere.cpp \
    fx/test.cpp \
    core/inputmanager.cpp \
    render/glhelper.cpp

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
    math/rect.h

LIBS += \
    -lSDL \
    -lGL \
    -lGLU \
    -lGLEW

INCLUDEPATH += \
    /usr/include/ \
    /usr/include/c++/4.6.1/ \
    /usr/lib/gcc/x86_64-pc-linux-gnu/4.5.3/include/g++-v4/























































