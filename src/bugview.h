#pragma once
#include <QTableWidget>
#include "issue.h"
#include "threadcontroller.h"


class BugView : public QTableWidget
{
public:
    enum ViewMode{
        Issue,
        User,
        Project,
    };
    Q_OBJECT
public:
    BugView(ThreadController* controller, QWidget* parent);
    void fillRows(ViewMode mode);
    void switchMode(ViewMode m) {mode = m; fillRows(m);}
private:
    ViewMode mode = ViewMode::Issue;
    ThreadController* controller;

public slots:
    void fillIssues();
    void fillUsers();
    void fillProjects();

};

