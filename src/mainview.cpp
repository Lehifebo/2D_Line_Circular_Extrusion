#include "mainview.h"

#include <QDateTime>
#include <QOpenGLShaderProgram>

/**
 * @brief MainView::MainView Constructs a new main view.
 *
 * @param parent Parent widget.
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "MainView constructor";

  connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
  qDebug() << "MainView destructor";

  glDeleteBuffers(2,&VBO);
  glDeleteVertexArrays(2,&VAO);
  makeCurrent();
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions.
 */
void MainView::initializeGL() {
  qDebug() << ":: Initializing OpenGL";
  initializeOpenGLFunctions();

  connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
          SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

  if (debugLogger.initialize()) {
    qDebug() << ":: Logging initialized";
    debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
  }

  QString glVersion{reinterpret_cast<const char *>(glGetString(GL_VERSION))};
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);


  glGenBuffers(1,&VBO);
  glGenVertexArrays(1,&VAO);

  modelTransformation.setToIdentity();


  projectionTransformation.setToIdentity();
  projectionTransformation.perspective(60.0f,16/9,30.0f,200.0f);

  this->rotateX = 0;
  this->rotateY = 0;
  this->rotateZ = 0;
  this->scale = 1;

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,myPoints.size()* sizeof(vertex),myPoints.data(),GL_STATIC_DRAW);
  mypointsSize = myPoints.size();
  myPoints.clear(); //empty the array


  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(vertex),(void *)offsetof(vertex, x));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D),
                        reinterpret_cast<GLvoid *>(0));

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  // Set the color to be used by glClear. This is, effectively, the background
  // color.
  glClearColor(0.37f, 0.42f, 0.45f, 0.0f);

  createShaderProgram();
}

/**
 * @brief MainView::createShaderProgram Creates a new shader program with a
 * vertex and fragment shader.
 */
void MainView::createShaderProgram() {
  shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                        ":/shaders/vertshader.glsl");
  shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                        ":/shaders/fragshader.glsl");
  shaderProgram.link();
}

/**
 * @brief MainView::paintGL Actual function used for drawing to the screen.
 *
 */
void MainView::paintGL() {
  // Clear the screen before rendering
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.bind();

  shaderProgram.setUniformValue("modelTransform",modelTransformation);
  shaderProgram.setUniformValue("projectionTransform",projectionTransformation);

  glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, mypointsSize);

  shaderProgram.release();
}

/**
 * @brief MainView::resizeGL Called upon resizing of the screen.
 *
 * @param newWidth The new width of the screen in pixels.
 * @param newHeight The new height of the screen in pixels.
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    float newRation = ((float)newWidth) /  newHeight;
    projectionTransformation.setToIdentity();
    projectionTransformation.perspective(20.0f,newRation,0.2f,20.0f);
}

/*
* @brief MainView::updateModel used for updating the model transformation
*
*/
void MainView::updateModel() {
    //piramid
    modelTransformation.setToIdentity();
    modelTransformation.translate(0.0f, 0.0f, -6.0f);

    modelTransformation.scale(this->scale);
    modelTransformation.rotate(this->rotateX,QVector3D(1.0f, 0.0f, 0.0f));
    modelTransformation.rotate(this->rotateY,QVector3D(0.0f, 1.0f, 0.0f));
    modelTransformation.rotate(this->rotateZ,QVector3D(0.0f, 0.0f, 1.0f));
    update();
}



/**
 * @brief MainView::setRotation Changes the rotation of the displayed objects.
 * @param rotateX Number of degrees to rotate around the x axis.
 * @param rotateY Number of degrees to rotate around the y axis.
 * @param rotateZ Number of degrees to rotate around the z axis.
 */
void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
  qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << ","
           << rotateZ << ")";
  this->rotateX = rotateX;
  this->rotateY = rotateY;
  this->rotateZ = rotateZ;
  updateModel();
}

/**
 * @brief MainView::setScale Changes the scale of the displayed objects.
 * @param scale The new scale factor. A scale factor of 1.0 should scale the
 * mesh to its original size.
 */
void MainView::setScale(float scale) {
  qDebug() << "Scale changed to " << scale;
  this->scale = scale;
  updateModel();
}

/**
 * @brief MainView::onMessageLogged OpenGL logging function, do not change.
 *
 * @param Message The message to be logged.
 */
void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}

/**
 * @brief MainView::updatePoints update the points with the newly generetaed ones, and update the buffer and model
 * @param newPoints
 */
void MainView::updatePoints(QVector<QPointF> &newPoints){
    if(newPoints.isEmpty()){
        qDebug() << "No vertex to process" ;
        return;
    }

    QVector<vertex> temporary;
    int tempSize=0;
    for(const QPointF& point : newPoints){
        vertex node;
        vertex center;
        //center.y=0;
        center.z=0;
        center.x=0;

        node.z = 0;
        //map from canvas space, to world space
        node.x = mapToRange(point.x(), 0, 300, -1, 1);
        node.y = mapToRange(point.y(), 0, 550, 1, -1);

        int step = 360/edges;
        //rotate the point around the Y axis, and add it to the array
        for(float i=0;i<360;i+=step){
            vertex aux  = rotateYAndReturn(node,i);
            temporary.push_back(aux);
        }
        temporary.push_back(node);
    }

    //clear the memory
    newPoints.clear();
    myPoints.clear();

    tempSize = temporary.size();
    int lineWidth = edges+1;

    //construct the triangles for the surface
    for(int i = lineWidth+1 ; i<tempSize-lineWidth; i++){
        vertex A, B, C, D;
        A = temporary[i-lineWidth];
        B = temporary[i];
        C = temporary[i-lineWidth+1];
        D = temporary[i-1];

        QVector3D normal = calculateNormal(C, A, B);
        QVector3D normal2 = calculateNormal(D, B, A);
        A.colors = (normal+normal2)/2.0f;
        B.colors = (normal+normal2)/2.0f;
        C.colors = (normal+normal2)/2.0f;
        D.colors = (normal+normal2)/2.0f;

        //first triangle
        myPoints.push_back(A);
        myPoints.push_back(B);
        myPoints.push_back(C);

        //second triangle
        myPoints.push_back(A);
        myPoints.push_back(D);
        myPoints.push_back(B);

    }

    updateBuffer();
    updateModel();
}

/**
 * @brief MainView::updateBuffer update the buffer with the current contents of the myPoints array
 */
void MainView::updateBuffer() {

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,myPoints.size()* sizeof(vertex),myPoints.data(),GL_STATIC_DRAW);
    mypointsSize = myPoints.size();
    myPoints.clear(); //empty the array

}
/**
 * @brief MainView::mapToRange map a value from one space to another
 * @param value the value to be mapped
 * @param inputMin lower bound original space
 * @param inputMax upper bound original space
 * @param outputMin lower bound mapped space
 * @param outputMax upper bound mapped space
 * @return
 */
float MainView::mapToRange(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
    return ((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) + outputMin;
}

/**
 * @brief MainView::rotateYAndReturn rotate a point some degrees around the Y axis
 * @param point the point to be rotated
 * @param angle the angle (in degrees) to add to the original position
 * @return the rotated point
 */
vertex MainView::rotateYAndReturn(const vertex& point, float angle) {
    vertex rotatedPoint;
    float theta = angle * M_PI / 180.0f;

    rotatedPoint.x = point.x * cos(theta) + point.z * sin(theta);
    rotatedPoint.y = point.y;
    rotatedPoint.z = point.z * cos(theta) - point.x * sin(theta);

    return rotatedPoint;
}

/**
 * @brief MainView::calculateNormal calculate the normal for a triangle
 * @param A the first vertex, will be used as the "head" of the triangle, meaning BA and CA will be computed
 * @param B the second vertex
 * @param C the third vertex
 * @return
 */
QVector3D MainView::calculateNormal(const vertex& A, const vertex& B, const vertex& C) {
    QVector3D edge1(B.x - A.x, B.y - A.y, B.z - A.z);
    QVector3D edge2(C.x - A.x, C.y - A.y, C.z - A.z);
    return QVector3D::crossProduct(edge1, edge2).normalized();
}
