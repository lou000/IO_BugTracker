#include "sqlworker.h"
#include "issue.h"
#include "QSqlError"

SqlWorker::SqlWorker(int timerInterval)
{
    this->timerInterval = timerInterval;
    this->updateTimer = new QTimer(this);
    QObject::connect(updateTimer, &QTimer::timeout, [=]() {
        checkDbAndUpdate();
    });
}


void SqlWorker::checkDbAndUpdate()
{
    if(!Queries::isDbOpen())
        Queries::initDb();
    if(Queries::isDbOpen())
    {
        updateData();
    }
}

void SqlWorker::updateData()
{
    auto q = Queries::getIssues();
    if(!q.exec())
        qDebug()<<q.lastError();
    QVector<IssueTicket*> temp;

    while(q.next())
    {
        auto type = static_cast<IssueTicket::IssueType>(q.value(1).toInt());
        class IssueTicket* issue;
        switch (type)
        {
        case IssueTicket::IssueType::Bug:
            issue = new BugTicket(q.value(0).toInt(), q.value(7).toString(), q.value(2).toString(), q.value(3).toString(),
                                  static_cast<IssueTicket::Status>(q.value(4).toInt()), q.value(5).toDateTime(), q.value(6).toDateTime());
            break;
        case IssueTicket::IssueType::Task:
            issue = new TaskTicket(q.value(0).toInt(), q.value(7).toString(), q.value(2).toString(), q.value(3).toString(),
                                   static_cast<IssueTicket::Status>(q.value(4).toInt()), q.value(5).toDateTime(), q.value(6).toDateTime());
        default:
            break;
        }
        temp.append(issue);
    }
    q.finish();
    qDebug()<<"Issues Updated!!";
    emit resultIssues(temp);

    q = Queries::getUsers();
    if(!q.exec())
        qDebug()<<q.lastError();
    QVector<User*> temp2;

    while(q.next())
    {
        auto user = new User(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(3).toString(),
                             static_cast<User::UserPosition>(q.value(5).toInt()), static_cast<User::UserPermissionsFlags>(q.value(4).toInt()));
        temp2.append(user);
    }
    q.finish();
    qDebug()<<"Users Updated!!";
    emit resultUsers(temp2);

    q = Queries::getProjects();
    if(!q.exec())
        qDebug()<<q.lastError();
    QVector<Project*> temp3;

    while(q.next())
    {
        auto project = new Project(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
        temp3.append(project);
    }
    q.finish();
    qDebug()<<"Projects Updated!!";
    emit resultProjects(temp3);

}

void SqlWorker::handleAddIssue(IssueTicket::IssueType type, const QString &s_desc, const QString &desc, IssueTicket::Status status, int proj_id)
{
     auto q = Queries::addIssue(type, s_desc, desc, status, proj_id);
     if(!q.exec())
         qDebug()<<q.lastError();
     updateData();
}

void SqlWorker::handleAddUser(const QString &login, const QString &password, const QString &name, const QString &surname,
                              User::UserPosition position, User::UserPermissionsFlags permissions)
{
    auto q = Queries::addUser(login, password, name, surname, position, permissions);
    if(!q.exec())
        qDebug()<<q.lastError();
    updateData();
}

void SqlWorker::handleAddProject(const QString name, const QString desc)
{
    auto q = Queries::addProject(name, desc);
    q.exec();
    updateData();
}
