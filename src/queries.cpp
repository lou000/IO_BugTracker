#include "queries.h"

void Queries::initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "BugTracker");
    QString connectString = "Driver={SQL Server};"; // Driver is now {SQL Server}
    connectString.append("Server=192.168.1.128,1433;"); // IP,Port//128
    connectString.append("Database=IO_Bugtracker;");  // Schema
    connectString.append("Uid=Lewy;");           // User
    connectString.append("Pwd=lol;");           // Pass
    db.setDatabaseName(connectString);
    if (!db.open())
    {
        qDebug() << "IO_Bugtracker Database: ERROR connection with database fail";
    }
    else
    {
        qDebug() << "IO_Bugtracker Database: connection ok";
    }
}

bool Queries::isDbOpen()
{
    return QSqlDatabase::database("BugTracker").isOpen();
}

QSqlQuery Queries::getIssues()
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.getIssues;");
    return q;
}

QSqlQuery Queries::getUsers()
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.getUsers;");
    return q;
}

QSqlQuery Queries::getProjects()
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.getProjects;");
    return q;
}

QSqlQuery Queries::addIssue(IssueTicket::IssueType type, QString s_desc, QString desc, IssueTicket::Status status, int proj_id)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.addIssue :type, :s_desc, :desc, :status,"
              " :dateAdded, :dateStatus, :projID; " );
    q.bindValue(":type", type);
    q.bindValue(":s_desc", s_desc);
    q.bindValue(":desc", desc);
    q.bindValue(":status", status);
    q.bindValue(":dateAdded", QDateTime::currentDateTime());
    q.bindValue(":dateStatus", QDateTime::currentDateTime());
    q.bindValue(":projID", proj_id);
    return q;
}


QSqlQuery Queries::addUser(QString login, QString password, QString name, QString surname,
                           User::UserPosition position, User::UserPermissionsFlags permissions)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.addUser :login, :password, :name,"
              " :surname, :position, :permissions" );
    q.bindValue(":login", login);
    q.bindValue(":password", password);
    q.bindValue(":name", name);
    q.bindValue(":surname", surname);
    q.bindValue(":position", position);
    q.bindValue(":permissions", static_cast<int>(permissions));
    return q;
}

QSqlQuery Queries::addProject(QString name, QString desc)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.addProject :name, :desc");
    q.bindValue(":name", name);
    q.bindValue(":desc", desc);
    return q;
}

QSqlQuery Queries::updateIssue(int id, IssueTicket::IssueType type, QString s_desc, QString desc, IssueTicket::Status status,
                               int proj_id, QDateTime dateStatus)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.updateIssue :id, :type, :s_desc, :desc, :status,"
              " :dateStatus, :projID; " );
    q.bindValue(":id", id);
    q.bindValue(":type", type);
    q.bindValue(":s_desc", s_desc);
    q.bindValue(":desc", desc);
    q.bindValue(":status", status);
    q.bindValue(":dateStatus", dateStatus);
    q.bindValue(":projID", proj_id);
    return q;
}

QSqlQuery Queries::updateUser(int id, QString login, QString password, QString name, QString surname, User::UserPosition position,
                              User::UserPermissionsFlags permissions)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.updateUser :id, :login, :password, :name,"
              " :surname, :position, :permissions" );
    q.bindValue(":id", id);
    q.bindValue(":login", login);
    q.bindValue(":password", password);
    q.bindValue(":name", name);
    q.bindValue(":surname", surname);
    q.bindValue(":position", position);
    q.bindValue(":permissions", static_cast<int>(permissions));
    return q;
}

QSqlQuery Queries::updateProject(int id, QString name, QString desc)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.updateUser :id, :name, :desc");
    q.bindValue(":id", id);
    q.bindValue(":name", name);
    q.bindValue(":desc", desc);
    return q;
}

QSqlQuery Queries::deleteIssue(int id)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.deleteIssue :id");
    q.bindValue(":id", id);
    return q;
}

QSqlQuery Queries::deleteUser(int id)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.deleteUser :id");
    q.bindValue(":id", id);
    return q;
}

QSqlQuery Queries::deleteProject(int id)
{
    QSqlDatabase db = QSqlDatabase::database("BugTracker");
    if(!db.isValid() || !db.isOpen())
        return QSqlQuery();
    QSqlQuery q(db);
    q.setForwardOnly(true);
    q.prepare("EXEC IO_BugTracker.dbo.deleteProject :id");
    q.bindValue(":id", id);
    return q;
}
