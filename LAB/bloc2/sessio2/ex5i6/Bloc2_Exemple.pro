TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm \
                ../../models/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp \
        ../../models/Model/model.cpp

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert
