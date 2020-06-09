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

QSqlQuery addIssue(IssueType type, QString s_desc, QString desc, Status status, int proj_id);
QSqlQuery addUser(QString login, QString password, QString name, QString surname, UserPosition position, UserPermission permissions);
QSqlQuery addProject(QString name, QString desc);

QSqlQuery updateIssue(int id, IssueType type, QString s_desc, QString desc, Status status, int proj_id, QDateTime dateStatus);
QSqlQuery updateUser(int id, QString login, QString password, QString name, QString surname, UserPosition position, UserPermission permissions);
QSqlQuery updateProject(int id, QString name, QString desc);

QSqlQuery deleteIssue(int id);
QSqlQuery deleteUser(int id);
QSqlQuery deleteProject(int id);

};
