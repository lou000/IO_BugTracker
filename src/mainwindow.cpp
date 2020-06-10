#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->controller = new ThreadController(120000);
    this->mainView = new BugView(controller, this);
    this->aWindows = new ActionWindows(this, controller);

    auto leftBar = new QDockWidget(this);
    leftBar->setWindowTitle("Actions:");
    leftBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    auto leftBarWidget = new QWidget();
    leftBar->setWidget(leftBarWidget);
    auto leftBarLayout = new QGridLayout();
    leftBarWidget->setLayout(leftBarLayout);

    auto viewMode = new QComboBox(this);
    viewMode->addItem("Issues");
    viewMode->addItem("Projects");
    viewMode->addItem("Users");
    leftBarLayout->addWidget(viewMode, 0, 0, 1, 2, Qt::AlignTop);

    auto addIssueButton = new QPushButton("Add Issue");
    addIssueButton->setMinimumHeight(70);
    leftBarLayout->addWidget(addIssueButton, 1, 0, 1, 2, Qt::AlignTop);

    auto addUserButton = new QPushButton("Add User");
    addUserButton->setMinimumHeight(70);
    leftBarLayout->addWidget(addUserButton, 2, 0, 1, 2, Qt::AlignTop);

    auto addProjectButton = new QPushButton("Add Project");
    addProjectButton->setMinimumHeight(70);
    leftBarLayout->addWidget(addProjectButton, 3, 0, 1, 2, Qt::AlignTop);

    leftBarLayout->addItem(new QSpacerItem(100, 500), 4, 0, 4, 2);


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
    QObject::connect(addIssueButton, &QPushButton::clicked, aWindows, [=]{aWindows->showAddIssueWindow();});
    QObject::connect(addUserButton, &QPushButton::clicked, aWindows, [=]{aWindows->showAddUserWindow();});
    QObject::connect(addProjectButton, &QPushButton::clicked, aWindows, [=]{aWindows->showAddProjectWindow();});

    QObject::connect(controller, SIGNAL(updateIssues()), mainView, SLOT(fillIssues()));
    QObject::connect(controller, SIGNAL(updateUsers()), mainView, SLOT(fillUsers()));
    QObject::connect(controller, SIGNAL(updateProjects()), mainView, SLOT(fillProjects()));



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

