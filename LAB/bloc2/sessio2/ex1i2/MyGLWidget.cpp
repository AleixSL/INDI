#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::calculaCapsaCont() {
    float minx, miny, minz, maxx, maxy, maxz;
    minx = maxx = m.vertices()[0];
    miny = maxy = m.vertices()[1];
    minz = maxz = m.vertices()[2];
    for (unsigned int i = 3; i < m.vertices().size(); i+=3) {
        if (m.vertices()[i+0] < minx)
            minx = m.vertices()[i+0];
        if (m.vertices()[i+0] > maxx)
            maxx = m.vertices()[i+0];
        if (m.vertices()[i+1] < miny)
            miny = m.vertices()[i+1];
        if (m.vertices()[i+1] > maxy)
            maxy = m.vertices()[i+1];
        if (m.vertices()[i+2] < minz)
            minz = m.vertices()[i+2];
        if (m.vertices()[i+2] > maxz)
            maxz = m.vertices()[i+2];
        }
    pmin= glm::vec3(minx,miny,minz);
    pmax= glm::vec3(maxx,maxy,maxz);
}

void MyGLWidget::calculaCiR() {
    centre = (pmax+pmin)/2.0f;
    radi = glm::distance(centre, pmax);
}

void MyGLWidget::iniCamera() {
    calculaCapsaCont();
    calculaCiR();
    angle = asin(radi/2*radi);
    FOV = 2.0f * angle;
    ra = radi/radi;
    zNear = radi;
    zFar = radi*3;

    left = -radi;
    right = radi;
    top = radi;
    bot = -radi;

    OBS = float(2)*radi*glm::vec3(0,0,1);
    VRP = glm::vec3(0,0,0);
    UP = glm::vec3(0,1,0);

    projectTransform();
    viewTransform();
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  this->angleDaHoma = 0;
  iniCamera();
}

void MyGLWidget::paintGL () 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);

  glBindVertexArray (VAO_terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Carreguem la transformació de model

//  projectTransform();
//  viewTransform();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_dahoma);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

  modelTransform ();

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, this->angleDaHoma, glm::vec3(0.0, 1.0, 0.0));
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
//    glm::mat4 Proj = glm::perspective((float)M_PI/2.0f, 1.0f, 0.4f, 3.0f);
    glm::mat4 Proj = glm::perspective (FOV,ra,zNear,zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    // glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    calculaCapsaCont();
    calculaCiR();
    glm::mat4 View = glm::lookAt (centre + ((2 * radi) * glm::vec3(0,0,1)), centre, glm::vec3(0,1,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R :
      this->angleDaHoma += M_PI/4;
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers () 
{
    //Terra
    glm::vec3 Verticee[4];
    Verticee[0] = glm::vec3(-1., -1., -1.);
    Verticee[1] = glm::vec3(-1., -1., 1.);
    Verticee[2] = glm::vec3(1., -1., -1.);
    Verticee[3] = glm::vec3(1., -1., 1.);
    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_terra);
    glBindVertexArray(VAO_terra);
    // Creació del buffer amb les dades dels vèrtexs

    GLuint VBO3;
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticee), Verticee, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glm::vec3 Colors[4];
    Colors[0] = glm::vec3(1, 1, 0);
    Colors[1] = glm::vec3(1, 1, 0);
    Colors[2] = glm::vec3(1, 1, 0);
    Colors[3] = glm::vec3(1, 1, 0);

    GLuint VBOterraColor;
    glGenBuffers(1, &VBOterraColor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOterraColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    m.load("../../models/HomerProves.obj");

    // m.VBO_vertices(); posició
    // m.VBO_matdiff();  color

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_dahoma);
    glBindVertexArray(VAO_dahoma);

    GLuint VBO_dahoma[2];
    glGenBuffers(2, VBO_dahoma);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dahoma[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_dahoma[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

