#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->controller = new ThreadController(120000);
    this->mainView = new BugView(controller, this);
    this->aWindows = new ActionWindows(this, controller);

    mainView->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
    auto editAction = new QAction("Edit");
    mainView->addAction(editAction);
    auto deleteAction = new QAction("Delete");
    mainView->addAction(deleteAction);
    auto addUserAction = new QAction("Add/Remove recources");
    mainView->addAction(addUserAction);

    auto leftBar = new QDockWidget(this);
    leftBar->setWindowTitle("Actions:");
    leftBar->setMinimumWidth(150);
    leftBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    auto leftBarWidget = new QWidget();
    leftBar->setWidget(leftBarWidget);
    auto leftBarLayout = new QGridLayout();
    leftBarWidget->setLayout(leftBarLayout);

    auto viewMode = new QComboBox(this);
    viewMode->addItem("Issues");
    viewMode->addItem("Projects");
    viewMode->addItem("Users");
    viewMode->setMinimumHeight(30);
    leftBarLayout->addWidget(new QLabel("Select View:"), 0, 0, 1, 2, Qt::AlignBottom);
    leftBarLayout->addWidget(viewMode, 1, 0, 1, 3);

    auto addProjectButton = new QPushButton("Add Item");
    addProjectButton->setMinimumHeight(50);
    leftBarLayout->addWidget(addProjectButton, 4, 0, 1, 2);

    leftBarLayout->addItem(new QSpacerItem(1, 200), 5, 0, 1, 2);

    auto editSelection = new QPushButton("Edit Selection");
    editSelection->setMinimumHeight(50);
    leftBarLayout->addWidget(editSelection, 6, 0, 1, 2);

    auto addResources = new QPushButton("Add/Remove recources");
    addResources->setMinimumHeight(50);
    leftBarLayout->addWidget(addResources, 8, 0, 1, 2);

    auto deleteSelection = new QPushButton("Delete Selection");
    deleteSelection->setMinimumHeight(50);
    leftBarLayout->addWidget(deleteSelection, 7, 0, 1, 2);

    leftBarLayout->addItem(new QSpacerItem(1, 700), 9, 0, 1, 2);


    QObject::connect(viewMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int v){
        switch (v) {
        case 0:
            mainView->switchMode(BugView::ViewMode::Issue);
            break;
        case 1:
            mainView->switchMode(BugView::ViewMode::Project);
            break;
        case 2:
            mainView->switchMode(BugView::ViewMode::User);
            break;
        default:
            break;
        }
    });

    QObject::connect(controller, SIGNAL(updateIssues()), mainView, SLOT(fillIssues()));
    QObject::connect(controller, SIGNAL(updateUsers()), mainView, SLOT(fillUsers()));
    QObject::connect(controller, SIGNAL(updateProjects()), mainView, SLOT(fillProjects()));

    QObject::connect(addProjectButton, &QPushButton::clicked, aWindows, [=]
    {
        switch (mainView->getMode()) {
        case BugView::ViewMode::Issue:
            aWindows->showIssueWindow();
            break;
        case BugView::ViewMode::Project:
            aWindows->showProjectWindow();
            break;
        case BugView::ViewMode::User:
            aWindows->showUserWindow();
            break;
        default:
            break;
        }
    });

    auto editLambda = [=]
    {
        auto selection = mainView->selectedRanges();
        if(selection.length() == 1 && selection.at(0).rowCount()==1)
        {
            int row = selection.at(0).topRow();
            switch (mainView->getMode()) {
            case BugView::ViewMode::Issue:
                if(row<controller->issues.length())
                    aWindows->showIssueWindow(controller->issues.at(row));
                else
                    aWindows->showIssueWindow();
                break;

            case BugView::ViewMode::Project:
                if(row<controller->projects.length())
                    aWindows->showProjectWindow(controller->projects.at(row));
                else
                    aWindows->showProjectWindow();
                break;

            case BugView::ViewMode::User:
                if(row<controller->users.length())
                    aWindows->showUserWindow(controller->users.at(row));
                else
                    aWindows->showUserWindow();
                break;

            default:
                break;
            }
        }
    };

    QObject::connect(editSelection, &QPushButton::clicked, aWindows, editLambda);
    QObject::connect(editAction, &QAction::triggered, aWindows, editLambda);
    QObject::connect(mainView, &QTableWidget::cellDoubleClicked, aWindows, editLambda);


    auto deleteLambda = [=]
    {
        auto selection = mainView->selectedRanges();
        if(selection.length() == 1 && selection.at(0).rowCount()==1)
        {
            QMessageBox msgBox;
            msgBox.setText("Are you sure you want to delete selected item?");
            msgBox.setInformativeText("This action will be permament.");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if(ret == QMessageBox::Yes)
            {
            int row = selection.at(0).topRow();
                switch (mainView->getMode()) {
                case BugView::ViewMode::Issue:
                    if(row<controller->issues.length())
                        controller->deleteIssue(controller->issues.at(row)->id());
                    break;
                case BugView::ViewMode::Project:
                    if(row<controller->projects.length())
                        controller->deleteProject(controller->projects.at(row)->id());
                    break;
                case BugView::ViewMode::User:
                    if(row<controller->users.length())
                        controller->deleteUser(controller->users.at(row)->id());
                    break;
                default:
                    break;
                }
            }
        }
    };
    QObject::connect(deleteSelection, &QPushButton::clicked, aWindows, deleteLambda);
    QObject::connect(deleteAction, &QAction::triggered, aWindows, deleteLambda);


    auto addUserToIssueLambda = [=]
    {
        auto selection = mainView->selectedRanges();
        if(selection.length() == 1 && selection.at(0).rowCount()==1)
        {
            int row = selection.at(0).topRow();
            if(mainView->getMode() == BugView::ViewMode::Issue)
                if(row<controller->issues.length())
                    aWindows->showAddUserToIssue(controller->issues.at(row));
        }
    };

    QObject::connect(addUserAction, &QAction::triggered, aWindows, addUserToIssueLambda);
    QObject::connect(addResources, &QPushButton::clicked, aWindows, addUserToIssueLambda);
    QObject::connect(mainView, &BugView::modeChanged, addResources, [=](BugView::ViewMode mode)
    {
        if(mode == BugView::ViewMode::Issue)
            addResources->show();
        else
            addResources->hide();
    });

    setCentralWidget(mainView);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, leftBar);
    setWindowIcon(QIcon());
    this->setWindowTitle("BugTracker   -   Project IO   -   by Lewicki Maciej and Jakub Kościołowski");
    this->setFixedSize(QSize(1800, 900));
    this->setContentsMargins(10, 10, 10, 10);
}

MainWindow::~MainWindow()
{
}

