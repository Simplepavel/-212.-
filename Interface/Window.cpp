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

    main_LogoutBttn = new QPushButton("Logout");

    main_PlayBttn = new QPushButton("Play");
    main_Layout->addWidget(main_PlayBttn);
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

    // Игровое полотно
    play_Widget = new QWidget(this);
    play_Layout = new QGridLayout;

    play_EnemyLabel = new QLabel("Enemy: User");
    play_EnemyLabel->setStyleSheet("font-size: 32px; font-family: Calibli;");

    play_DeckInfoLayout = new QVBoxLayout; // та часть, что слева от игорного стола

    play_TrumpLabel = new QLabel("Trump: ♠️"); // козырь
    play_TrumpLabel->setStyleSheet("font-size: 32px; font-family: Calibli;");

    play_TakeCard = new QPushButton("Take one card"); // взять карты
    play_TakeCard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    play_TakeCard->setStyleSheet("font-size: 32px; font-family: Calibli;");

    play_GameField = new QWidget;

    play_MyCards = new QLabel("My cards:");
    play_MyCards->setStyleSheet("font-size: 32px; font-family: Calibli;");
    play_TurnInfo = new QLabel("Your turn"); // Ваш ход, ход соперника
    play_TurnInfo->setStyleSheet("font-size: 32px; font-family: Calibli;");

    play_DeckWidget = new QWidget;
    play_DeckWidget->setStyleSheet("background-color: red;");
    play_DeckLayout = new QHBoxLayout;
    play_DeckWidget->setLayout(play_DeckLayout);
    play_DeckInfoLayout->addWidget(play_TrumpLabel);
    play_DeckInfoLayout->addWidget(play_TakeCard);

    play_Layout->addWidget(play_EnemyLabel, 0, 0, 1, 6, Qt::AlignCenter);
    play_Layout->addLayout(play_DeckInfoLayout, 1, 0, 1, 1);
    play_Layout->addWidget(play_GameField, 1, 1, 1, 5, Qt::AlignCenter);
    play_Layout->addWidget(play_MyCards, 2, 0, 1, 3, Qt::AlignCenter);
    play_Layout->addWidget(play_TurnInfo, 2, 3, 1, 3, Qt::AlignLeft);
    play_Layout->addWidget(play_DeckWidget, 3, 0, 1, 6, Qt::AlignCenter);

    play_Layout->setRowStretch(0, 2);
    play_Layout->setRowStretch(1, 3);
    play_Layout->setRowStretch(2, 2);
    play_Layout->setRowStretch(3, 2);

    play_Widget->setLayout(play_Layout);

    listOfLayout->addWidget(login_Widget);
    listOfLayout->addWidget(main_Widget);
    listOfLayout->addWidget(reg_Widget);
    listOfLayout->addWidget(play_Widget);
    listOfLayout->setAlignment(Qt::AlignCenter);
    // Игровое полотно
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
    listOfLayout->setCurrentWidget(reg_Widget);
}

void Window::play()
{
    listOfLayout->setCurrentWidget(play_Widget);
    qDebug() << play_Layout->rowCount() << play_Layout->columnCount();
}

void Window::connect()
{
    // QObject::connect(login_LoginBttn, &QPushButton::clicked, this, main);
    // QObject::connect(main_LogoutBttn, &QPushButton::clicked, this, login);
    QObject::connect(login_RegBttn, &QPushButton::clicked, this, registration);
    QObject::connect(reg_BackBttn, &QPushButton::clicked, this, login);
    QObject::connect(main_PlayBttn, &QPushButton::clicked, this, play);
}
