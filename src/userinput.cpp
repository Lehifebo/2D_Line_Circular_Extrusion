#include <QDebug>

#include "mainview.h"
#include "canvas.h"


/**
 * @brief MainView::mousePressEvent Triggered when pressing any mouse button.
 * @param ev Mouse event.
 */
void MainView::mousePressEvent(QMouseEvent *ev) {
  qDebug() << "Mouse button pressed:" << ev->button();

  update();
  // Do not remove the line below, clicking must focus on this widget!
  this->setFocus();
}


void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        points.clear(); // Start a new drawing
        points.append(event->position());
    }
}

/**
 * @brief Canvas::mouseMoveEvent get the coordonates of the mouse, while left click is pressed
 * @param event
 */
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        //only add points that are within the visible canvas
        if(event->position().x()>= 0 && event->position().x() < 300 && event->position().y() < 550 && event->position().y() >= 0){
            points.append(event->position());
            update(); // Trigger a repaint
        }

    }
}
