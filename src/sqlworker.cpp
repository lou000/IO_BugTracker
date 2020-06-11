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
    auto q = Queries::getUsers();
    if(!q.exec())
        qDebug()<<q.lastError();
    QVector<User*> temp2;

    while(q.next())
    {
        auto user = new User(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(3).toString(),
                             static_cast<User::UserPosition>(q.value(4).toInt()), static_cast<User::UserPermissionsFlags>(q.value(5).toInt()));
        temp2.append(user);
    }
    q.finish();
    qDebug()<<"Users Updated!!";
    emit resultUsers(temp2);

    q = Queries::getIssues();
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
        QVector<User*> programmers;
        QVector<User*> testers;
        QVector<User*> menagers;
        for(QString id : q.value(8).toString().split(", "))
        {
            for(auto user : temp2)
                if(user->id() == id.toInt())
                    switch (user->position()) {
                    case User::UserPosition::Programmer:
                        programmers.append(user);
                        break;
                    case User::UserPosition::Tester:
                        testers.append(user);
                        break;
                    case User::UserPosition::Manager:
                        menagers.append(user);
                        break;
                    default:
                        break;
                    }
        }
        issue->setTesters(testers);
        issue->setMenagers(menagers);
        issue->setProgrammers(programmers);
        temp.append(issue);
    }
    qDebug()<<"Issues Updated!!";
    emit resultIssues(temp);
    q.finish();

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

void SqlWorker::handleEditIssue(int id, IssueTicket::IssueType type, const QString &s_desc, const QString &desc, IssueTicket::Status status, int proj_id, QDateTime statusDate)
{
    auto q = Queries::updateIssue(id, type, s_desc, desc, status, proj_id, statusDate);
    if(!q.exec())
        qDebug()<<q.lastError();
    updateData();
}

void SqlWorker::handleEditUser(int id, const QString &name, const QString &surname, User::UserPosition position, User::UserPermissionsFlags permissions)
{
    auto q = Queries::updateUser(id, name, surname, position, permissions);
    if(!q.exec())
        qDebug()<<q.lastError();
    updateData();
}

void SqlWorker::handleEditProject(int id, const QString name, const QString desc)
{
    auto q = Queries::updateProject(id, name, desc);
    q.exec();
    updateData();
}

void SqlWorker::handleDeleteIssue(int id)
{
    auto q = Queries::deleteIssue(id);
    q.exec();
    updateData();
}

void SqlWorker::handleDeleteUser(int id)
{
    auto q = Queries::deleteUser(id);
    q.exec();
    updateData();
}

void SqlWorker::handleDeleteProject(int id)
{
    auto q = Queries::deleteProject(id);
    q.exec();
    updateData();
}

void SqlWorker::handleAddUserToIssue(int id_user, int id_issue)
{
    auto q = Queries::addUserToIssue(id_user, id_issue);
    q.exec();
    updateData();
}

void SqlWorker::handleRemoveUserFromIssue(int id_user, int id_issue)
{
    auto q = Queries::removeUserFromIssue(id_user, id_issue);
    q.exec();
    updateData();
}
