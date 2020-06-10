#pragma once
#include <QThread>
#include <QObject>
#include "user.h"
#include "issue.h"
#include "project.h"
#include "sqlworker.h"

class ThreadController : public QObject
{
    Q_OBJECT
public:
    ThreadController(int sqlUpdateInterval);
    SqlWorker* worker;
    QVector<User*> users;
    QVector<IssueTicket*> issues;
    QVector<Project*> projects;

private:
    QThread* sqlThread;
    int sqlUpdateInterval;


signals:
    void timerStart();
    void updateIssues();
    void updateUsers();
    void updateProjects();
    void addIssue(IssueTicket::IssueType type, QString s_desc, QString desc, IssueTicket::Status status, int proj_id);
    void addUser(const QString &login, const QString &password, const QString &name, const QString &surname,
                 User::UserPosition position, User::UserPermissionsFlags permissions);
    void addProject(const QString name, const QString desc);

public slots:
    void receiveIssues(const QVector<IssueTicket*> &issues);
    void receiveUsers(const QVector<User *> &users);
    void receiveProjects(const QVector<Project *> &projects);
};

