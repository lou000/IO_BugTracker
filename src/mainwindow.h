#pragma once
#include <QMainWindow>
#include <QDockWidget>
#include <QGridLayout>
#include <QPushButton>
#include "bugview.h"
#include "threadcontroller.h"
#include "src/actionwindows.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    BugView* mainView;
    ActionWindows* aWindows;
    ThreadController* controller;

};

