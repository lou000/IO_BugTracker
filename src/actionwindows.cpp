#include "actionwindows.h"

ActionWindows::ActionWindows(QWidget* parent, ThreadController* controller)
               :controller(controller)
{
    this->setParent(parent);
    this->close();
    initIssueWindow();
    initUserWindow();
    initProjectWindow();
}

void ActionWindows::infoBox(const QString &str)
{
    QMessageBox msgbox;
    msgbox.setText(str);
    msgbox.exec();
}

void ActionWindows::initIssueWindow()
{
    this->addIssueWindow = new QDockWidget(this->parentWidget());
    addIssueWindow->setAttribute(Qt::WA_DeleteOnClose, false);
    addIssueWindow->setWindowTitle("Add Issue:");
    addIssueWindow->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addIssueWindow->setFloating(true);
    auto issueTypeWidget = new QWidget(this->parentWidget());
    addIssueWindow->setWidget(issueTypeWidget);
    auto addIssueLayout = new QGridLayout();
    issueTypeWidget->setLayout(addIssueLayout);

    auto issueType = new QComboBox(this);
    auto&& issueMetaEnum = QMetaEnum::fromType<IssueTicket::IssueType>();
    for(int i=0; i<issueMetaEnum.keyCount(); i++)
    {
        issueType->addItem(issueMetaEnum.key(i));
    }
    addIssueLayout->addWidget(issueType, 0, 1);
    issueType->setCurrentIndex(-1);
    addIssueLayout->addWidget(new QLabel("Issue type:"),0 ,0, Qt::AlignRight);

    auto s_desc = new QTextEdit();
    s_desc->setPlaceholderText("Short Description");
    addIssueLayout->addWidget(s_desc, 2, 0, 2, 4);

    auto desc = new QTextEdit();
    desc->setPlaceholderText("Full Description");
    addIssueLayout->addWidget(desc, 4, 0, 4, 4);


    auto status = new QComboBox(this);
    auto&& statusMetaEnum = QMetaEnum::fromType<IssueTicket::Status>();
    for(int i=0; i<statusMetaEnum.keyCount(); i++)
    {
        status->addItem(statusMetaEnum.key(i));
    }

    addIssueLayout->addWidget(status, 0, 3);
    status->setCurrentIndex(-1);
    addIssueLayout->addWidget(new QLabel("Status:"),0 ,2, Qt::AlignRight);

    auto proj_id = new QComboBox(this);
    for(auto proj : controller->projects)
        proj_id->addItem(proj->name());
    addIssueLayout->addWidget(proj_id, 1, 1);
    addIssueLayout->addWidget(new QLabel("Project ID:"),1 ,0, Qt::AlignRight);

    auto accept_button = new QPushButton("Add");
    addIssueLayout->addWidget(accept_button, 8, 0, 2, 4);

    QObject::connect(accept_button, &QPushButton::clicked, this, [=]
        {
        if(s_desc->toPlainText().length()<1 || status->currentIndex()==-1 || issueType->currentIndex()==-1)
            infoBox("Please fill out all the required fields.");
        else
        {
            emit controller->addIssue(stringToEnum<IssueTicket::IssueType>(issueType->currentText()),
                                      s_desc->toPlainText(), desc->toPlainText(),
                                      stringToEnum<IssueTicket::Status>(status->currentText()),
                                      controller->projects.at(proj_id->currentIndex())->id());
            issueType->setCurrentIndex(-1);
            s_desc->clear();
            desc->clear();
            status->setCurrentIndex(-1);
            if(addIssueWindow->isFloating())
                addIssueWindow->close();
        }
        });
    QObject::connect(controller, &ThreadController::updateIssues, this, [=]
    {
        int lastIndex = proj_id->currentIndex();
        proj_id->clear();
        for(auto proj : controller->projects)
            proj_id->addItem(proj->name());
        proj_id->setCurrentIndex(lastIndex);
    });

    addIssueWindow->close();
}

void ActionWindows::initUserWindow()
{
    this->addUserWindow = new QDockWidget(this->parentWidget());
    addUserWindow->setAttribute(Qt::WA_DeleteOnClose, false);

    addUserWindow->setWindowTitle("Add User:");
    addUserWindow->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addUserWindow->setFloating(true);
    auto addUserWidget = new QWidget(this->parentWidget());
    addUserWindow->setWidget(addUserWidget);
    auto addUserLayout = new QGridLayout();
    addUserWidget->setLayout(addUserLayout);
    auto login = new QLineEdit();
    login->setPlaceholderText("Login");
    addUserLayout->addWidget(login, 0, 0, 1, 2);

    auto name = new QLineEdit();
    name->setPlaceholderText("Name");
    addUserLayout->addWidget(name, 0, 2, 1, 2);

    auto surname = new QLineEdit();
    surname->setPlaceholderText("Surname");
    addUserLayout->addWidget(surname, 2, 2, 1, 2);


    auto position = new QComboBox(this);
    auto&& positionMetaEnum = QMetaEnum::fromType<User::UserPosition>();
    for(int i=0; i<positionMetaEnum.keyCount(); i++)
    {
        position->addItem(positionMetaEnum.key(i));
    }
    addUserLayout->addWidget(position, 2, 1, Qt::AlignLeft);
    position->setCurrentIndex(-1);
    addUserLayout->addWidget(new QLabel("Position:", this),2 ,0, Qt::AlignRight);

    QVector<QCheckBox*> permissionBoxes;
    auto&& permissionsMetaEnum = QMetaEnum::fromType<User::UserPermission>();
    addUserLayout->addItem(new QSpacerItem(300,10), 3, 0, 1, 4,Qt::AlignHCenter);
    addUserLayout->addWidget(new QLabel("Permissions:", this),4 ,0, Qt::AlignBottom);
    for(int i=0; i<permissionsMetaEnum.keyCount(); i++)
    {
        auto box = new QCheckBox(permissionsMetaEnum.key(i), this);
        if(i<4)
            addUserLayout->addWidget(box, 5+i, 0, 1, 2, Qt::AlignTop);
        else
            addUserLayout->addWidget(box, 5+i-4, 2, 1, 2, Qt::AlignTop);
        permissionBoxes.append(box);
    }
    auto accept_button = new QPushButton("Add");
    addUserLayout->addWidget(accept_button, 9, 0, 2, 4);


    QObject::connect(accept_button, &QPushButton::clicked, this, [=]
        {
        if(login->text().isEmpty() || name->text().isEmpty() || surname->text().isEmpty() || position->currentIndex()==-1)
            infoBox("Please fill out all the required fields.");
        else
        {
            auto&& permissionsMetaEnum = QMetaEnum::fromType<User::UserPermission>();
            User::UserPermissionsFlags flags;
            for(int i=0; i<permissionBoxes.length(); i++)
            {
                flags |= static_cast<User::UserPermission>(permissionsMetaEnum.value(i));
            }
            emit controller->addUser(login->text(), QString(), name->text(), surname->text(),
                                     stringToEnum<User::UserPosition>(position->currentText()),
                                     flags);
            login->clear();
            name->clear();
            surname->clear();
            position->setCurrentIndex(-1);
            for(int i=0; i<permissionBoxes.length(); i++)
            {
                permissionBoxes.at(i)->setCheckState(Qt::CheckState::Unchecked);
            }
        }
        });

    addUserWindow->close();
}

void ActionWindows::initProjectWindow()
{
    this->addProjectWindow = new QDockWidget(this->parentWidget());
    addProjectWindow->setAttribute(Qt::WA_DeleteOnClose, false);

    addProjectWindow->setWindowTitle("Add Project:");
    addProjectWindow->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addProjectWindow->setFloating(true);
    auto addProjectWidget = new QWidget(this->parentWidget());
    addProjectWindow->setWidget(addProjectWidget);
    auto addProjectLayout = new QGridLayout();
    addProjectWidget->setLayout(addProjectLayout);

    auto name = new QLineEdit(this);
    name->setPlaceholderText("Name");
    addProjectLayout->addWidget(name, 0, 0, 1, 2);

    auto desc = new QTextEdit(this);
    desc->setPlaceholderText("Description");
    addProjectLayout->addWidget(desc, 1, 0, 1, 2);

    auto accept_button = new QPushButton("Add");
    addProjectLayout->addWidget(accept_button, 2, 0, 2, 2);

    QObject::connect(accept_button, &QPushButton::clicked, this, [=]
        {
        if(desc->toPlainText().isEmpty() || name->text().isEmpty())
            infoBox("Please fill out all the required fields.");
        else
        {
            emit controller->addProject(name->text(), desc->toPlainText());
            name->clear();
            desc->clear();
        }
        });
    addProjectWindow->close();

}

void ActionWindows::showAddIssueWindow()
{
    addIssueWindow->setWindowFlag(Qt::WindowStaysOnTopHint);//Windows is retarded and has to be reminded everytime
    addIssueWindow->show();
    addIssueWindow->raise();
}

void ActionWindows::showAddUserWindow()
{
    addUserWindow->setWindowFlag(Qt::WindowStaysOnTopHint);
    addUserWindow->show();
    addUserWindow->raise();
}

void ActionWindows::showAddProjectWindow()
{
    addProjectWindow->setWindowFlag(Qt::WindowStaysOnTopHint);
    addProjectWindow->show();
    addProjectWindow->raise();
}

void ActionWindows::closeAddIssueWindow()
{

}

void ActionWindows::closeAddUserWindow()
{

}

void ActionWindows::closeAddProjectWindow()
{

}
