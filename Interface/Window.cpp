#include "Window.hpp"

Window::Window(QWidget *parent)
{
    // Вспомогательные компоненты
    listOfLayout = new QStackedLayout(this);
    setLayout(listOfLayout);
    // Вспомогательные компоненты

    // Вход
    login_Widget = new QWidget;
    login_Layout = new QVBoxLayout;
    login_Email = new QLineEdit;
    login_Email->setPlaceholderText("Enter the email");
    login_Email->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    login_Password = new QLineEdit;
    login_Password->setPlaceholderText("Enter the password");
    login_Password->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    login_LoginBttn = new QPushButton("LogIn");
    login_LoginBttn->setBaseSize(login_Email->sizeHint());

    login_RegBttn = new QPushButton("Registration");
    login_RegBttn->setBaseSize(login_Email->sizeHint());

    login_Layout->addWidget(login_Email);
    login_Layout->addWidget(login_Password);
    login_Layout->addWidget(login_LoginBttn);
    login_Layout->addWidget(login_RegBttn);

    login_Layout->setAlignment(Qt::AlignCenter);
    login_Layout->setSpacing(7);

    login_Widget->setLayout(login_Layout);

    // Вход

    // Основное меню
    main_Widget = new QWidget(this);

    main_Layout = new QVBoxLayout;

    main_SearchRoom = new QLineEdit;
    main_SearchRoom->setPlaceholderText("ID");
    main_SearchRoom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    main_FindRoomBttn = new QPushButton("Search");
    main_FindRoomBttn->setBaseSize(main_SearchRoom->sizeHint());

    main_CreateRoomBttn = new QPushButton("Create own room");
    main_CreateRoomBttn->setBaseSize(main_SearchRoom->sizeHint());

    main_LogoutBttn = new QPushButton("Logout");
    main_LogoutBttn->setBaseSize(main_SearchRoom->sizeHint());

    main_Layout->addWidget(main_SearchRoom);
    main_Layout->addWidget(main_FindRoomBttn);
    main_Layout->addWidget(main_CreateRoomBttn);
    main_Layout->addWidget(main_LogoutBttn);

    main_Layout->setAlignment(Qt::AlignCenter);
    main_Layout->setSpacing(7);

    main_Widget->setLayout(main_Layout);

    // Основное меню

    // Регистрация

    reg_Widget = new QWidget(this);
    reg_Layout = new QVBoxLayout;

    reg_Username = new QLineEdit;
    reg_Username->setPlaceholderText("Enter username");
    reg_Username->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reg_Email = new QLineEdit;
    reg_Email->setPlaceholderText("Enter email");
    reg_Email->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reg_Password = new QLineEdit;
    reg_Password->setPlaceholderText("Enter password");
    reg_Password->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reg_ConfirmPassword = new QLineEdit;
    reg_ConfirmPassword->setPlaceholderText("Confirm password");
    reg_ConfirmPassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reg_SubmitBttn = new QPushButton("Check In");
    reg_SubmitBttn->setBaseSize(reg_Username->sizeHint());

    reg_BackBttn = new QPushButton("Back");
    reg_BackBttn->setBaseSize(reg_Username->sizeHint());

    reg_Layout->addWidget(reg_Username);
    reg_Layout->addWidget(reg_Email);
    reg_Layout->addWidget(reg_Password);
    reg_Layout->addWidget(reg_ConfirmPassword);
    reg_Layout->addWidget(reg_SubmitBttn);
    reg_Layout->addWidget(reg_BackBttn);

    reg_Layout->setAlignment(Qt::AlignHCenter);
    reg_Layout->setSpacing(7);

    reg_Widget->setLayout(reg_Layout);
    // Регистрация

    listOfLayout->addWidget(login_Widget);
    listOfLayout->addWidget(main_Widget);
    listOfLayout->addWidget(reg_Widget);
    listOfLayout->setAlignment(Qt::AlignCenter);
}

void Window::login()
{
    listOfLayout->setCurrentWidget(login_Widget);
}

void Window::main()
{
    listOfLayout->setCurrentWidget(main_Widget);
}

void Window::registration()
{
    qDebug() << "Hello!";
    listOfLayout->setCurrentWidget(reg_Widget);
}

void Window::connect()
{
    QObject::connect(login_LoginBttn, &QPushButton::clicked, this, main);
    QObject::connect(main_LogoutBttn, &QPushButton::clicked, this, login);
    QObject::connect(login_RegBttn, &QPushButton::clicked, this, registration);
    QObject::connect(reg_BackBttn, &QPushButton::clicked, this, login);
}


