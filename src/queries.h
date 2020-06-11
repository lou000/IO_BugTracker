#pragma once
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QDateTime>
#include "issue.h"
#include "user.h"

namespace Queries{

void initDb();
bool isDbOpen();

QSqlQuery getIssues();
QSqlQuery getUsers();
QSqlQuery getProjects();

QSqlQuery addIssue(IssueTicket::IssueType type, QString s_desc, QString desc, IssueTicket::Status status, int proj_id);
QSqlQuery addUser(QString login, QString password, QString name, QString surname, User::UserPosition position, User::UserPermissionsFlags permissions);
QSqlQuery addProject(QString name, QString desc);

QSqlQuery updateIssue(int id, IssueTicket::IssueType type, QString s_desc, QString desc, IssueTicket::Status status, int proj_id, QDateTime dateStatus);
QSqlQuery updateUser(int id, QString name, QString surname, User::UserPosition position, User::UserPermissionsFlags permissions);
QSqlQuery updateProject(int id, QString name, QString desc);

QSqlQuery deleteIssue(int id);
QSqlQuery deleteUser(int id);
QSqlQuery deleteProject(int id);

QSqlQuery addUserToIssue(int id_user, int id_issue);
QSqlQuery removeUserFromIssue(int id_user, int id_issue);

};
