#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include <QGraphicsView>
#include <string>
#include <mapview.h>
#include <QVBoxLayout>
#include "SceneBuilder.h"
#include <shortpath.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
private:
  //=============================================
  //Start
  vector<pair<QString,idType>> MyPlaces ;
  void fill_MyPlaces();
  //Initilaization of the Sourse Node, and Destination Node
  idType SourceS = 1545694404;
  idType DestinationD = 1545694404;
  //End
  //=============================================
  bool m_leftMousePressed;
  Ui::MainWindow *ui;
  MapView *m_mapView;
  SceneBuilder *m_sceneBuilder;
  //    QVBoxLayout *m_layoutView;
  Model *m_model;
  qreal m_scale;
  void loadFile(string );
  void resizeEvent(QResizeEvent *event);
  void viewSizeAdjust(QResizeEvent *event);
  void sendCancelRoute();
  //    void wheelEvent(QWheelEvent *event);

signals:
  void drawRoute(Path route);
  void sendSearchName(QString name);
  void changeToSearchPlace();
  void changeToInit();
  void changeToSearch();
  void cancelRoute();
  void changeToRoute();

public slots:
  void getRoutePath(idType src, idType dest);
  void getSearchName();

private slots:
  void on_Source_QB_activated(const QString &arg1);
  void on_Destination_QB_activated(const QString &arg1);
  void on_Navigate_Button_clicked();
  void on_actionQuit_triggered();
  void on_action_Open_File_triggered();
  void on_Cancel_Navigation_clicked();
};
#endif // MAINWINDOW_H
