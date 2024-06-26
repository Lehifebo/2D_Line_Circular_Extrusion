#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class is the main application window. Among other
 * things, it handles the inputs from the screen widgets.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  Ui::MainWindow *ui;

  explicit MainWindow(QWidget *parent = nullptr);
  void renderToFile();
  ~MainWindow() override;
  QVector<int> stepAllowed = {2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 30, 36, 40, 45, 60, 72, 90};
  int previousValue=2;
  int stepPreviousValue = 0;
 private slots:

  void on_ResetRotationButton_clicked(bool checked);
  void on_RotationDialX_sliderMoved(int value);
  void on_RotationDialY_sliderMoved(int value);
  void on_RotationDialZ_sliderMoved(int value);

  void on_ResetScaleButton_clicked(bool checked);
  void on_ScaleSlider_sliderMoved(int value);
  void on_GenerateButton_clicked(bool clicked);
  void on_edges_valueChanged(int value);
};

#endif  // MAINWINDOW_H
