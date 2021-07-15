#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void canviaFOV(int f);
    void canviaFocusCam();
    void canviaFocusEsc();
    void canviaCamPat1();
    void canviaColorR(int color);
    void canviaColorG(int color);
    void canviaColorB(int color);

  signals:
    void setFOV(int);
    void setFocCam(bool);
    void setFocEsc(bool);
    void setCamPat1();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffersModels (Model &model, const char *filename, GLuint *VAO_Models, float &escala, glm::vec3 &centreBase);
    void createBuffersPatricio ();
    void createBuffersTerraIParet ();
    void carregaShaders ();
    void iniEscena ();
    void iniCamera ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio ();
    void modelTransformPatricio2 ();
    void calculaCapsaModel (Model &model, float &escala, glm::vec3 &centreBase);

    // VAO names
    GLuint VAO_Vaca, VAO_Patr;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc, matvaca, franges, tipusFocus, posXpat1Loc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // model
    Model vaca, patr;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr, centreVaca, diff = glm::vec3(0.0, 0.0, 0.0);
    float escalaPatr, escalaVaca;
    // radi de l'escena
    float radiEsc;

    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick, teclaX, teclaL;
    float angleX, angleY, escala2, posXpat1;
    bool perspectiva, camPat1;
    glm::vec3 Pmin, Pmax, VRP, OBS, UP;
    float alfaIni, FOV, rav, d, znear, zfar, right, left, top, bottom, angleModels;
};

