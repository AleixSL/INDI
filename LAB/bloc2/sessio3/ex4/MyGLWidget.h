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
    protected:
        virtual void initializeGL ( );	// Aqui incluim les inicialitzacions del contexte grafic.
        virtual void paintGL ( );  // Mètode cridat cada cop que cal refrescar la finestra.
        virtual void resizeGL (int width, int height); // És cridat quan canvia la mida del widget
        virtual void keyPressEvent (QKeyEvent *event); //Es cridat quan es prem una tecla
        virtual void mouseMoveEvent(QMouseEvent *event);
    private:
        /* -----------------------------------------*/
        /* --------------- Functions ---------------*/
        /* -----------------------------------------*/
        // initializeGL
        void carregaShaders ();
        void createBuffers ();
        void carregaPatricio ();
        void carregaTerra ();
        void ini_camera ();
        void calculaBaseCapsaPatricio ();
        void viewTransform ();
        void projectTransform ();
        // paintGL
        void modelTransformPatricio1 ();
        void modelTransformPatricio2 ();
        void modelTransformPatricio3 ();
        void modelTransformTerra ();

        /* -----------------------------------------*/
        /* -------------- Locations  ---------------*/
        /* -----------------------------------------*/
        GLuint vertexLoc, colorLoc;									// attribute locations
        GLuint transLoc, projLoc, viewLoc; 							// uniform locations

        /* -----------------------------------------*/
        /* --------------- VAO, VBO ----------------*/
        /* -----------------------------------------*/
        GLuint VAO_Patricio, VBO_PatricioPos, VBO_PatricioCol;		// Patricio
        GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;				// Terra
        QOpenGLShaderProgram *program;								// Program

        /* -----------------------------------------*/
        /* ------------- Internal vars -------------*/
        /* -----------------------------------------*/
        Model patricio;
        float scale;
        glm::vec3 pos;
        GLfloat angle = 0.0;


        /* -----------------------------------------*/
        /* -------------- Added vars  --------------*/
        /* -----------------------------------------*/
        glm::vec3 Pmin = glm::vec3(-2.5, 0.0,-2.5);
        glm::vec3 Pmax = glm::vec3( 2.5, 4.0, 2.5);
        glm::vec3 centreBaseCapsaPatricio;
        GLfloat altura;
        GLfloat r, d;
        glm::vec3 VRP, OBS, UP;
        GLfloat FOV, ra, znear, zfar, left, right, bottom, top, alfaIni;
        GLuint cam = 0;
        float anglePhy=0., angleTheta=0., anglePsi=0., antX, antY;
};
