#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable (GL_DEPTH_TEST);
  carregaShaders();
  createBuffersVaca();
  createBuffersPatricio();
  createBuffersTerraIParet();

  iniEscena();
  iniCamera();
}

void MyGLWidget::iniEscena ()
{
  radiEsc = sqrt(5);
  angleModels = 0;
  teclaX = 0;
  glUniform1i(franges, teclaX);
  teclaL = 0;
  glUniform1i(tipusFocus, teclaL);
}

void MyGLWidget::iniCamera ()
{
  angleX = 0.0;
  angleY = 0.0;

  VRP[0] = (Pmin[0] + Pmax[0]) / 2.0;
  VRP[1] = (Pmin[1] + Pmax[1]) / 2.0;
  VRP[2] = (Pmin[2] + Pmax[2]) / 2.0;
  radiEsc = sqrt(
    (Pmin[0]-VRP[0])*(Pmin[0]-VRP[0]) +
    (Pmin[1]-VRP[1])*(Pmin[1]-VRP[1]) +
    (Pmin[2]-VRP[2])*(Pmin[2]-VRP[2])
  );

  d= 2*radiEsc;
  OBS = VRP + d*glm::vec3(0.0, 0.0, 1.0);
  UP = glm::vec3(0.0, 1.0, 0.0);

  alfaIni = asin(radiEsc/d);
  FOV = 2*alfaIni;
  perspectiva = true;

  znear = d - radiEsc;
  zfar = d + radiEsc;

  right = radiEsc;
  left = -radiEsc;
  top = radiEsc;
  bottom = -radiEsc;

  // Escena 2
  FOV = float(M_PI/3.0);

  projectTransform ();
  viewTransform ();
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
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Pintem Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricio ();

  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  glUniform1i(matvaca, 0);

  // Pintem la vaca
  glBindVertexArray (VAO_Vaca);

  modelTransformVaca ();

  glDrawArrays(GL_TRIANGLES, 0, vaca.faces().size()*3);
  
  glUniform1i(matvaca, 1);

  glBindVertexArray(0);

}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;
  rav = float(ample) / float(alt);
  if (rav < 1.0) FOV = 2.0*atan(tan(alfaIni)/rav);
  else FOV = alfaIni * 2.0;

  projectTransform();
}

void MyGLWidget::modelTransformVaca ()
{
  float alcada = 0.5;
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(1.0, -1.0*alcada, 0.0));
  TG = glm::rotate(TG, angleModels, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::rotate(TG, float(-M_PI/2.0), glm::vec3(0.0, 1.0, 0.0));
  TG = glm::rotate(TG, float(-M_PI/2.0), glm::vec3(1.0, 0.0, 0.0));
  TG = glm::scale(TG, alcada * glm::vec3(escalaVaca, escalaVaca, escalaVaca));
  TG = glm::translate(TG, -centreVaca);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()
{
  float alcada = 0.25;
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(1.0, -0.25*alcada, 0.0));
  TG = glm::rotate(TG, angleModels, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, alcada * glm::vec3(escalaPatr, escalaPatr, escalaPatr));
  TG = glm::translate(TG, -centrePatr);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (perspectiva)
    Proj = glm::perspective(FOV, rav, znear, zfar);
  else
    Proj = glm::ortho(left, right, bottom, top, znear, zfar);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;  // Matriu de posició i orientació
  // Escena 2
  View = glm::translate(glm::mat4(1.f), glm::vec3(-1, 1, -1));
  View = glm::translate(View, glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica
      perspectiva = !perspectiva;
      projectTransform ();
      break;
    }
    case Qt::Key_R: {
        angleModels += M_PI/6.0;
        break;
    }
    case Qt::Key_X: {
        if(teclaX == 0) {
            teclaX = 1;
            glUniform1i(franges, 0);
        }
        else {
            teclaX = 0;
            glUniform1i(franges, 1);
        }
        break;
    }
    case Qt::Key_L: {
      if(teclaL == 0) {
          teclaL = 1;
          glUniform1i(tipusFocus, 0);
          emit setFocCam(false);
          emit setFocEsc(true);
      }
      else {
          teclaL = 0;
          glUniform1i(tipusFocus, 1);
          emit setFocCam(true);
          emit setFocEsc(false);
      }
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ZOOM;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;
    viewTransform ();
  }
  if (DoingInteractive == ZOOM) {
      int novaY = e->y();
      if(yClick > novaY and (FOV - 0.1/((novaY - yClick)) > 0.0)) {
          FOV -= 0.1/((yClick - novaY));
      }
      if(yClick < novaY and (FOV + 0.1/((novaY - yClick)) < (float)M_PI)) {
          FOV += 0.1/((novaY - yClick));
      }
      projectTransform();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
  int FOVbarra = int(FOV/M_PI/180);
  emit setFOV(FOVbarra);
}

void MyGLWidget::calculaCapsaModel (Model &model, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = model.vertices()[0];
  miny = maxy = model.vertices()[1];
  minz = maxz = model.vertices()[2];
  for (unsigned int i = 3; i < model.vertices().size(); i+=3)
  {
    if (model.vertices()[i+0] < minx)
      minx = model.vertices()[i+0];
    if (model.vertices()[i+0] > maxx)
      maxx = model.vertices()[i+0];
    if (model.vertices()[i+1] < miny)
      miny = model.vertices()[i+1];
    if (model.vertices()[i+1] > maxy)
      maxy = model.vertices()[i+1];
    if (model.vertices()[i+2] < minz)
      minz = model.vertices()[i+2];
    if (model.vertices()[i+2] > maxz)
      maxz = model.vertices()[i+2];
  }
  Pmin.x = minx;
  Pmin.y = miny;
  Pmin.z = minz;
  Pmax.x = maxx;
  Pmax.y = maxy;
  Pmax.z = maxz;
  escala = 1.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = (miny+maxy)/2.0; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::canviaFOV(int f) {
    makeCurrent();
    FOV = ((float)f * (float)M_PI / float(180));
    projectTransform();
    update();
}

void MyGLWidget::canviaFocusCam() {
    makeCurrent();
    teclaL = 0;
    glUniform1i(tipusFocus, 0);
    update();
}

void MyGLWidget::canviaFocusEsc() {
    makeCurrent();
    teclaL = 1;
    glUniform1i(tipusFocus, 1);
    update();
}

void MyGLWidget::createBuffersVaca() {
    createBuffersModels(vaca, "./models/cow.obj", &VAO_Vaca, escalaVaca, centreVaca);
}

void MyGLWidget::createBuffersPatricio() {
    createBuffersModels(patr, "./models/Patricio.obj", &VAO_Patr, escalaPatr, centrePatr);
}

void MyGLWidget::createBuffersModels (Model &model, const char *filename, GLuint *VAO_Models, float &escala, glm::vec3 &centreBase)
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  model.load(filename);

  // Calculem la capsa contenidora del model
  calculaCapsaModel (model, escala, centreBase);
  
  // Creació del Vertex Array Object del model
  glGenVertexArrays(1, VAO_Models);
  glBindVertexArray(*VAO_Models);

  // Creació dels buffers del model
  GLuint VBO_Model[6];
  // Buffer de posicions
  glGenBuffers(6, VBO_Model);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3, model.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}


void MyGLWidget::createBuffersTerraIParet ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[12] = {
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(2.0, 1.0, -2.0)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };

  // Definim el material del terra
  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.8,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[6];
  glGenBuffers(6, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicLlumShader.frag");
  vs.compileSourceFile("shaders/basicLlumShader.vert");
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
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  matvaca = glGetUniformLocation (program->programId(), "matvaca");
  franges = glGetUniformLocation (program->programId(), "franges");
  tipusFocus = glGetUniformLocation(program->programId(), "tipusFocus");
}


