#pragma once

#include <QObject>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QSpacerItem>
#include "user.h"
#include "threadcontroller.h"

class ActionWindows : public QWidget
{
public:
    ActionWindows(QWidget* parent, ThreadController* controller);
    QDockWidget* addIssueWindow;
    QDockWidget* addUserWindow;
    QDockWidget* addProjectWindow;


private:
    ThreadController* controller;

    void infoBox(const QString &str);
    void initIssueWindow();
    void initUserWindow();
    void initProjectWindow();

public slots:
    void showAddIssueWindow();
    void showAddUserWindow();
    void showAddProjectWindow();

    void closeAddIssueWindow();
    void closeAddUserWindow();
    void closeAddProjectWindow();

};
