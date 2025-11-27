#include "Window.hpp"

Window::Window(QWidget *parent)
{

    // Неигровые параметры
    screen_Objc = QApplication::primaryScreen();
    QRect screen_Rect = screen_Objc->geometry();
    screen_Height = screen_Rect.height() * screen_Objc->devicePixelRatio();
    screen_Width = screen_Rect.width() * screen_Objc->devicePixelRatio();

    cell_Size = screen_Height / 15;
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
    login_LoginBttn->setFixedSize(login_Email->sizeHint());

    login_RegBttn = new QPushButton("Registration");
    login_RegBttn->setFixedSize(login_Email->sizeHint());

    login_Layout->addWidget(login_Email, Qt::AlignCenter);
    login_Layout->addWidget(login_Password, Qt::AlignCenter);
    login_Layout->addWidget(login_LoginBttn, Qt::AlignCenter);
    login_Layout->addWidget(login_RegBttn, Qt::AlignCenter);

    login_Layout->setAlignment(Qt::AlignCenter);
    login_Layout->setSpacing(7);

    login_Widget->setLayout(login_Layout);

    // Вход

    // Основное меню
    main_Widget = new QWidget(this);

    main_Layout = new QVBoxLayout;

    main_LogoutBttn = new QPushButton("Logout");

    main_PlayBttn = new QPushButton("Play");

    main_MyProfile = new QPushButton("Profile");

    main_Layout->addWidget(main_PlayBttn);
    main_Layout->addWidget(main_LogoutBttn);
    main_Layout->addWidget(main_MyProfile);

    main_Layout->setAlignment(Qt::AlignCenter);
    main_Layout->setSpacing(7);

    main_Widget->setLayout(main_Layout);

    // Основное меню

    // Регистрация

    reg_Widget = new QWidget(this);
    reg_Layout = new QVBoxLayout;

    reg_Layout->setContentsMargins(20, 20, 20, 20);

    reg_Username = new QLineEdit;
    reg_Username->setPlaceholderText("Enter username");
    reg_Username->setFixedWidth(250);

    reg_Email = new QLineEdit;
    reg_Email->setPlaceholderText("Enter email");
    reg_Email->setFixedWidth(250);

    reg_Password = new QLineEdit;
    reg_Password->setPlaceholderText("Enter password");
    reg_Password->setFixedWidth(250);

    reg_ConfirmPassword = new QLineEdit;
    reg_ConfirmPassword->setPlaceholderText("Confirm password");
    reg_ConfirmPassword->setFixedWidth(250);

    reg_SubmitBttn = new QPushButton("Check In");
    reg_SubmitBttn->setFixedWidth(250);

    reg_BackBttn = new QPushButton("Back");
    reg_BackBttn->setFixedWidth(250);

    reg_Layout->addWidget(reg_Username, Qt::AlignCenter);
    reg_Layout->addWidget(reg_Email, Qt::AlignCenter);
    reg_Layout->addWidget(reg_Password, Qt::AlignCenter);
    reg_Layout->addWidget(reg_ConfirmPassword, Qt::AlignCenter);
    reg_Layout->addWidget(reg_SubmitBttn, Qt::AlignCenter);
    reg_Layout->addWidget(reg_BackBttn, Qt::AlignCenter);

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

    // Поиск соперника
    wait_Widget = new QWidget(this);
    wait_Layout = new QVBoxLayout;

    wait_Label = new QLabel("Find opponents. Please wait"); // здесь найдпись: Идет поиск соперника. Ожидайте...
    QFont NewLabelFont = QFont("Calibri", 32);
    wait_Label->setFont(NewLabelFont);
    wait_Label->setAlignment(Qt::AlignCenter);

    wait_StopBttn = new QPushButton("Stop");
    wait_StopBttn->setFont(NewLabelFont);
    wait_StopBttn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    wait_Timer = new QTimer(this);
    wait_Timer->setInterval(std::chrono::milliseconds(1000));

    wait_Layout->addWidget(wait_Label);
    wait_Layout->addWidget(wait_StopBttn, 0, Qt::AlignCenter);
    wait_Layout->setAlignment(Qt::AlignCenter);
    wait_Widget->setLayout(wait_Layout);
    // Поиск соперника

    listOfLayout->addWidget(login_Widget);
    listOfLayout->addWidget(main_Widget);
    listOfLayout->addWidget(reg_Widget);
    listOfLayout->addWidget(play_Widget);
    listOfLayout->addWidget(wait_Widget);
    listOfLayout->setAlignment(Qt::AlignCenter);

    // Загрузка изображений
    MyPushButton::LoadChessImages(cell_Size);
    // Загрузка изображений
}

void Window::login()
{
    InsertMessage(LOGIN);
    login_Email->clear();
    login_Password->clear();
    listOfLayout->setCurrentWidget(login_Widget);
}

void Window::main()
{
    InsertMessage(MAIN);
    listOfLayout->setCurrentWidget(main_Widget);
}

void Window::registration()
{
    InsertMessage(CHECKIN);
    reg_Username->clear();
    reg_Email->clear();
    reg_Password->clear();
    reg_ConfirmPassword->clear();
    listOfLayout->setCurrentWidget(reg_Widget);
}

void Window::play()
{
    listOfLayout->setCurrentWidget(play_Widget);
}

void Window::wait()
{
    listOfLayout->setCurrentWidget(wait_Widget);
}

void Window::InsertMessage(Owners own, bool DeleteOld)
{

    QFont MessageFont("Calibri", 8);
    QPalette palette;
    QVBoxLayout *layout = nullptr;
    int k = 0; // Сколько виджетов есть у данного Layout по умолчанию
    switch (own)
    {
    case (CHECKIN):
        layout = reg_Layout;
        k = 6;
        break;
    case (LOGIN):
        layout = login_Layout;
        k = 4;
        break;
    case (MAIN):
        layout = main_Layout;
        k = 3;
        break;
    case (PLAY):
        layout = play_Layout; // Тут будет другой LAYOUT. Слева от игровой доски
        break;
    default:
        return;
    }

    // Удалем старые препреждения если есть и надо
    if (DeleteOld)
    {
        int count = layout->count();
        int i = 0;
        while (i < count - k)
        {
            QLayoutItem *item = layout->itemAt(i);
            if (QLabel *LabelToDelete = dynamic_cast<QLabel *>(item->widget()))
            {
                layout->removeWidget(LabelToDelete);
                delete LabelToDelete;
            }
            else
            {
                ++i;
            }
        }
    }
    // Удалем старые препреждения если есть
    auto i = StateMessages.begin();
    while (i != StateMessages.end())
    {
        if (i->owner == own)
        {
            QLabel *warning = new QLabel(i->message);
            warning->setFont(MessageFont);
            warning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            if (i->type == ERR)
            {
                palette.setColor(QPalette::WindowText, QColor(220, 0, 0));
                warning->setPalette(palette);
            }
            else if (i->type == SUCCES)
            {
                palette.setColor(QPalette::WindowText, QColor(0, 255, 0));
                warning->setPalette(palette);
            }
            layout->insertWidget(0, warning);
            i = StateMessages.erase(i);
        }
        else
        {
            ++i;
        }
    }
    // Вставка новых предупреждений
}

void Window::AddStateMessage(const Message &value)
{
    StateMessages.push_back(value);
}

void Window::UpdateBoard(Board &NewBoard, FigureColor MyColor) // заполнили согласно новой расстановке
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int position = i * 8 + j;
            QLayoutItem *OldBttn = play_BoardLayot->itemAtPosition(i, j);
            QWidget *old = OldBttn->widget();
            MyPushButton *current_bttn = static_cast<MyPushButton *>(old);
            current_bttn->SetFigure(&NewBoard[position]);
            current_bttn->repaint();
            current_bttn->update();
        }
    }
}

void Window::connect()
{
    // QObject::connect(login_LoginBttn, &QPushButton::clicked, this, main);
    // QObject::connect(main_LogoutBttn, &QPushButton::clicked, this, login);
    QObject::connect(login_RegBttn, &QPushButton::clicked, this, registration);
    QObject::connect(reg_BackBttn, &QPushButton::clicked, this, login);
    QObject::connect(wait_Timer, QTimer::timeout, this, &Window::UpdateWaitLabel);
    // QObject::connect(main_PlayBttn, &QPushButton::clicked, this, play);
}

std::vector<MyPushButton *> Window::FillBoard()
{
    std::vector<MyPushButton *> NewBttns;
    NewBttns.reserve(64);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (QLayoutItem *layout = play_BoardLayot->itemAtPosition(i, j))
            {
                QWidget *old_widget = layout->widget();
                play_BoardLayot->removeWidget(old_widget);
                delete old_widget;
            }
            MyPushButton *new_bttn = new MyPushButton(i, j);
            NewBttns.push_back(new_bttn);
            new_bttn->setFixedSize(cell_Size, cell_Size);
            play_BoardLayot->addWidget(new_bttn, i, j, Qt::AlignCenter);
        }
    }
    return NewBttns;
}

void Window::UpdateWaitLabel()
{
    QString CurrentText = wait_Label->text();
    if (CurrentText.size() == 27) // Длина "Find opponents. Please wait"
    {
        CurrentText.push_back(".");
    }
    else if (CurrentText.size() == 28) // Длина "Find opponents. Please wait."
    {
        CurrentText.push_back(".");
    }
    else if (CurrentText.size() == 29) // Длина "Find opponents. Please wait.."
    {
        CurrentText.push_back(".");
    }
    else if (CurrentText.size() == 30)
    {
        CurrentText = "Find opponents. Please wait";
    }
    wait_Label->setText(CurrentText);
}
// Private slots
