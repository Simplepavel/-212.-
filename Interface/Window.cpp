#include "Window.hpp"

Window::Window(QWidget *parent)
{

    // Неигровые параметры
    screen_Objc = QApplication::primaryScreen();
    QRect screen_Rect = screen_Objc->geometry();
    screen_Height = screen_Rect.height() * screen_Objc->devicePixelRatio();
    screen_Width = screen_Rect.width() * screen_Objc->devicePixelRatio();
    // Неигровые параметры

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
    play_Layout = new QVBoxLayout;

    play_EnemyName = new QLabel("User1");
    play_EnemyName->setStyleSheet("font-size: 32px; font-family: Calibri;");
    play_EnemyName->setAlignment(Qt::AlignCenter);

    play_NextBttn = new QPushButton("Next");
    play_NextBttn->setStyleSheet("font-size: 32px; font-family: Calibri;");

    play_StopBttn = new QPushButton("Stop");
    play_StopBttn->setStyleSheet("font-size: 32px; font-family: Calibri;");

    play_BoardLayot = new QGridLayout;
    play_BoardLayot->setSpacing(0);

    play_Layout->addWidget(play_EnemyName);
    play_Layout->addLayout(play_BoardLayot);
    play_Layout->addWidget(play_NextBttn, Qt::AlignBottom);
    play_Layout->addWidget(play_StopBttn, Qt::AlignBottom);
    play_Layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    play_Widget->setLayout(play_Layout);

    // Игровое полотно

    listOfLayout->addWidget(login_Widget);
    listOfLayout->addWidget(main_Widget);
    listOfLayout->addWidget(reg_Widget);
    listOfLayout->addWidget(play_Widget);
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
    listOfLayout->setCurrentWidget(reg_Widget);
}

void Window::play() // скорее всего мы должны передать сюда какие-то параметры
{
    listOfLayout->setCurrentWidget(play_Widget);
}

void Window::UpdateBoard(Board &NewBoard) // заполнили согласно новой расстановке
{
    int new_bttn_size = screen_Height / 15;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Figure alfa = NewBoard[i * 8 + j];

            QLayoutItem *OldBttn = play_BoardLayot->itemAtPosition(i, j);
            QWidget *old = OldBttn->widget();
            MyPushButton *current_bttn = static_cast<MyPushButton *>(old);
            current_bttn->SetFigure(&NewBoard[i * 8 + j]);
        }
    }
}

void Window::connect()
{
    // QObject::connect(login_LoginBttn, &QPushButton::clicked, this, main);
    // QObject::connect(main_LogoutBttn, &QPushButton::clicked, this, login);
    QObject::connect(login_RegBttn, &QPushButton::clicked, this, registration);
    QObject::connect(reg_BackBttn, &QPushButton::clicked, this, login);
    // QObject::connect(main_PlayBttn, &QPushButton::clicked, this, play);
}

std::vector<MyPushButton *> Window::FillBoard()
{
    int new_bttn_size = screen_Height / 15;
    int idx;
    std::vector<MyPushButton *> NewBttns;
    NewBttns.reserve(64);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            MyPushButton *new_bttn = new MyPushButton(i, j); // к каждой кнопке добавить как то фунцию MakeMove DurakOnline
            NewBttns.push_back(new_bttn);
            new_bttn->setFixedSize(new_bttn_size, new_bttn_size);
            play_BoardLayot->addWidget(new_bttn, i, j, Qt::AlignCenter);
        }
    }
    return NewBttns;
}