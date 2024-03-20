#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector3D>

#include "vertex.h"

/**
 * @brief The MainView class is responsible for the actual content of the main
 * window.
 */
class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

 public:
  MainView(QWidget *parent = nullptr);
  ~MainView() override;

  // Functions for widget input events
  void setRotation(int rotateX, int rotateY, int rotateZ);
  void setScale(float scale);
  void updatePoints(QVector<QPointF> &newPoints);
  int  edges = 10;
 protected:

  void initializeGL() override;
  void resizeGL(int newWidth, int newHeight) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *ev) override;

  // Function to update the model when scale/rotation changes
  void updateModel();


 private slots:
  void onMessageLogged(QOpenGLDebugMessage Message);

 private:
  QVector<vertex> myPoints;
  size_t mypointsSize;

  float mapToRange(float value, float inputMin, float inputMax, float outputMin, float outputMax);
  vertex rotateYAndReturn(const vertex& point, float angle);
  QVector3D calculateNormal(const vertex& A, const vertex& B, const vertex& C);

  // transformation matrices
  QMatrix4x4 modelTransformation;
  QMatrix4x4 projectionTransformation;

  QOpenGLDebugLogger debugLogger;
  QTimer timer;  // timer used for animation

  GLuint VBO, colorVBO;
  GLuint VAO;
  QOpenGLShaderProgram* shader;
  QOpenGLShaderProgram shaderProgram;

  void updateBuffer();
  void createShaderProgram();
  int rotateX, rotateY, rotateZ;
  float scale;
};

#endif  // MAINVIEW_H
