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

    login_Widget = new QWidget(this);
    login_Layout = new QVBoxLayout;
    login_Email = new QLineEdit;
    login_Email->setPlaceholderText("Enter the email");
    login_Email->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    login_Password = new QLineEdit;
    login_Password->setEchoMode(QLineEdit::Password);
    login_Password->setPlaceholderText("Enter the password");

    login_Password->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    login_LoginBttn = new QPushButton("LogIn");

    login_RegBttn = new QPushButton("Registration");

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
    main_Layout = new QHBoxLayout;

    main_LeaderBoardWidget = new QWidget;
    main_LeaderBoardLayout = new QVBoxLayout(main_LeaderBoardWidget);

    main_LeaderBoardLayout->setSpacing(0);
    main_LeaderBoardLayout->setAlignment(Qt::AlignTop);

    main_LeaderBoardScroll = new QScrollArea;
    main_LeaderBoardScroll->setWidgetResizable(true);
    main_LeaderBoardScroll->setWidget(main_LeaderBoardWidget);

    main_LeaderBoardLabel = new QLabel("LeaderBoard");
    main_LeaderBoardLabel->setFont(QFont("Calibri", BIG));

    main_SearchLayout = new QHBoxLayout;
    main_SearchLine = new QLineEdit;
    main_SearchLine->setPlaceholderText("Find enemy...");
    main_SearchButton = new QPushButton("Search");

    main_SearchLayout->addWidget(main_SearchLine);
    main_SearchLayout->addWidget(main_SearchButton);

    main_LeaderBoardLayout->addWidget(main_LeaderBoardLabel);
    main_LeaderBoardLayout->addLayout(main_SearchLayout);

    main_ProfileLayout = new QVBoxLayout;
    main_LogoutBttn = new QPushButton("Logout");
    main_PlayBttn = new QPushButton("Play");
    main_MyProfile = new QPushButton("Profile");

    main_ProfileLayout->addWidget(main_PlayBttn);
    main_ProfileLayout->addWidget(main_LogoutBttn);
    main_ProfileLayout->addWidget(main_MyProfile);

    main_ProfileLayout->setAlignment(Qt::AlignCenter);
    main_ProfileLayout->setSpacing(7);

    main_Layout->addWidget(main_LeaderBoardScroll, 2, Qt::AlignLeft);
    main_Layout->addLayout(main_ProfileLayout, Qt::AlignLeft);
    main_Layout->addStretch(2);

    main_Widget->setLayout(main_Layout);
    // Основное меню

    // Регистрация

    reg_Widget = new QWidget(this);
    reg_Layout = new QVBoxLayout;

    // reg_Layout->setContentsMargins(20, 20, 20, 20);

    reg_Username = new QLineEdit;
    reg_Username->setPlaceholderText("Enter username");
    reg_Username->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    reg_Username->setMinimumWidth(250);

    reg_Email = new QLineEdit;
    reg_Email->setPlaceholderText("Enter email");
    reg_Email->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    reg_Username->setMinimumWidth(250);

    reg_Password = new QLineEdit;
    reg_Password->setEchoMode(QLineEdit::Password);
    reg_Password->setPlaceholderText("Enter password");
    reg_Password->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    reg_Username->setMinimumWidth(250);

    reg_ConfirmPassword = new QLineEdit;
    reg_ConfirmPassword->setEchoMode(QLineEdit::Password);
    reg_ConfirmPassword->setPlaceholderText("Confirm password");
    reg_ConfirmPassword->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    reg_SubmitBttn = new QPushButton("Check In");

    reg_BackBttn = new QPushButton("Back");

    reg_Layout->addWidget(reg_Username);
    reg_Layout->addWidget(reg_Email);
    reg_Layout->addWidget(reg_Password);
    reg_Layout->addWidget(reg_ConfirmPassword);
    reg_Layout->addWidget(reg_SubmitBttn);
    reg_Layout->addWidget(reg_BackBttn);

    reg_Layout->setAlignment(Qt::AlignCenter);
    reg_Layout->setSpacing(7);

    reg_Widget->setLayout(reg_Layout);
    // Регистрация

    // Игровое полотно
    play_Widget = new QWidget(this);
    play_Layout = new QHBoxLayout(play_Widget);

    play_ScrollWidget = new QWidget(play_Widget);

    play_ScrollLayout = new QVBoxLayout(play_ScrollWidget);
    play_ScrollLayout->setSpacing(0);
    play_ScrollLayout->setAlignment(Qt::AlignTop);

    play_ScrollArea = new QScrollArea;
    play_ScrollArea->setWidgetResizable(true);
    play_ScrollArea->setWidget(play_ScrollWidget);

    play_GameLayout = new QVBoxLayout;
    play_EnemyName = new QLabel();
    play_EnemyName->setFont(QFont("Calibri", BIGGEST));
    play_EnemyName->setAlignment(Qt::AlignCenter);

    play_NextBttn = new QPushButton("Next");
    play_NextBttn->setFont(QFont("Calibri", BIGGEST));

    play_StopBttn = new QPushButton("Stop");
    play_StopBttn->setFont(QFont("Calibri", BIGGEST));

    play_BoardLayot = new QGridLayout;
    play_BoardLayot->setSpacing(0);

    play_GameLayout->addWidget(play_EnemyName);
    play_GameLayout->addLayout(play_BoardLayot);
    play_GameLayout->addWidget(play_NextBttn, Qt::AlignBottom);
    play_GameLayout->addWidget(play_StopBttn, Qt::AlignBottom);
    play_GameLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    // play_Layout->addWidget(play_ScrollWidget, Qt::AlignLeft);
    play_Layout->addWidget(play_ScrollArea, Qt::AlignLeft);
    play_Layout->addLayout(play_GameLayout);

    play_Layout->setStretch(0, 2);
    play_Layout->setStretch(1, 6);
    play_Layout->addStretch(2);

    play_Widget->setLayout(play_Layout);

    // Игровое полотно

    // Поиск соперника
    wait_Widget = new QWidget(this);
    wait_Layout = new QVBoxLayout;

    wait_Label = new QLabel("Find opponents. Please wait"); // здесь найдпись: Идет поиск соперника. Ожидайте...
    QFont NewLabelFont = QFont("Calibri", BIGGEST);
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

    // Профиль

    profile_Widget = new QWidget(this);
    profile_Layout = new QHBoxLayout(profile_Widget);

    profile_BackBttn = new QPushButton("<");
    profile_BackBttn->setFixedWidth(30);
    profile_BackBttn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    profile_UserInfoWidget = new QWidget(profile_Widget);
    profile_UserInfoLayout = new QVBoxLayout(profile_UserInfoWidget);

    profile_Avatar = new RoundedAvatar(screen_Height * 0.3, screen_Height * 0.3);

    profile_UsernameLayout = new QHBoxLayout;
    profile_ChangeUsernameBttn = new QPushButton("...");
    profile_ChangeUsernameBttn->setFixedSize(30, 30);
    profile_Username = new QLabel("Username");
    profile_Username->setMaximumWidth(0.3 * screen_Height);

    profile_Username->setFont(QFont("Calibri", BIGGEST));

    profile_UsernameLayout->addWidget(profile_Username);
    profile_UsernameLayout->addWidget(profile_ChangeUsernameBttn);
    profile_UsernameLayout->setAlignment(Qt::AlignCenter);

    profile_ChangePhoto = new QPushButton("Change photo");

    profile_Rank = new QLabel("BigBoss");
    profile_Rank->setFont(QFont("Calibri", BIG, 4));
    QPalette rank_palette = profile_Rank->palette();
    rank_palette.setColor(QPalette::WindowText, QColor(255, 215, 0));
    profile_Rank->setPalette(rank_palette);
    profile_Rank->setAlignment(Qt::AlignCenter);

    profile_UserInfoLayout->setSpacing(5);
    profile_UserInfoLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    profile_UserInfoLayout->addWidget(profile_Avatar);
    profile_UserInfoLayout->addWidget(profile_ChangePhoto, 0, Qt::AlignHCenter);
    profile_UserInfoLayout->addLayout(profile_UsernameLayout);
    profile_UserInfoLayout->addWidget(profile_Rank);

    profile_Layout->addWidget(profile_BackBttn, Qt::AlignTop);
    profile_Layout->addWidget(profile_UserInfoWidget);

    // Профиль

    // Профиль соперника

    ProfileEnemyWidget = new QWidget(this);
    ProfileEnemyLayout = new QHBoxLayout(ProfileEnemyWidget);

    profile_EnemyBackBttn = new QPushButton("<"); // сделал 2, т.к один виджет - один родитель
    profile_EnemyBackBttn->setFixedWidth(30);
    profile_EnemyBackBttn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    profile_EnemyInfoWidget = new QWidget(ProfileEnemyWidget);
    profile_EnemyInfoLayout = new QVBoxLayout(profile_EnemyInfoWidget);

    profile_EnemyAvatar = new RoundedAvatar(screen_Height * 0.3, screen_Height * 0.3);

    profile_EnemyName = new QLabel("EnemyName");
    profile_EnemyName->setMaximumWidth(0.3 * screen_Height);
    profile_EnemyName->setFont(QFont("Calibri", BIGGEST));
    profile_EnemyName->setAlignment(Qt::AlignCenter);

    profile_EnemyRank = new QLabel("BigBoss");
    profile_EnemyRank->setFont(QFont("Calibri", BIG, 4));

    profile_EnemyRank->setPalette(rank_palette);
    profile_EnemyRank->setAlignment(Qt::AlignCenter);

    profile_Status = new QLabel("online");
    profile_Status->setAlignment(Qt::AlignCenter);
    profile_Status->setFont(QFont("Calibri", SMALLEST));

    QPalette status_palette = profile_Status->palette();
    status_palette.setColor(QPalette::WindowText, QColor(0, 255, 0));
    profile_Status->setPalette(status_palette);

    profile_Invite = new QPushButton("Invite to the game");

    profile_EnemyInfoLayout->setSpacing(5);
    profile_EnemyInfoLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    profile_EnemyInfoLayout->addWidget(profile_EnemyAvatar);
    profile_EnemyInfoLayout->addWidget(profile_Status);
    profile_EnemyInfoLayout->addWidget(profile_EnemyName);
    profile_EnemyInfoLayout->addWidget(profile_EnemyRank);
    profile_EnemyInfoLayout->addWidget(profile_Invite);

    ProfileEnemyLayout->addWidget(profile_EnemyBackBttn, Qt::AlignTop);
    ProfileEnemyLayout->addWidget(profile_EnemyInfoWidget);

    // Профиль соперника

    listOfLayout->addWidget(login_Widget);
    listOfLayout->addWidget(main_Widget);
    listOfLayout->addWidget(reg_Widget);
    listOfLayout->addWidget(play_Widget);
    listOfLayout->addWidget(wait_Widget);
    listOfLayout->addWidget(profile_Widget);
    listOfLayout->addWidget(ProfileEnemyWidget);
    listOfLayout->setAlignment(Qt::AlignCenter);

    // Загрузка изображений
    CellButton::LoadChessImages(cell_Size);

    // Загрука LeaderBoard
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

void Window::profile()
{
    listOfLayout->setCurrentWidget(profile_Widget);
}

void Window::EnemyProfile()
{
    listOfLayout->setCurrentWidget(ProfileEnemyWidget);
}

void Window::InsertMessage(Owners own, bool DeleteOld)
{
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
        layout = main_ProfileLayout;
        k = 3;
        break;
    case (PLAY):
        layout = play_ScrollLayout; // Тут будет другой LAYOUT. Слева от игровой доски
        break;
    case (PROFILE):
        layout = profile_UserInfoLayout;
        k = 4;
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
                if (layout->count() == 0)
                {
                    break;
                }
            }
            else
            {
                ++i;
            }
        }
    }
    // Удалем старые препреждения если есть
    auto i = StateMessages.begin();
    QFont MessageFont;
    MessageFont.setFamily("Calibri");
    while (i != StateMessages.end())
    {
        if (i->owner == own)
        {
            QLabel *warning = new QLabel(i->message);
            MessageFont.setPointSize(i->size);
            warning->setFont(MessageFont);
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
            layout->insertWidget(0, warning, 0);
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
            CellButton *current_bttn = static_cast<CellButton *>(old);
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

std::vector<CellButton *> Window::FillBoard()
{
    std::vector<CellButton *> NewBttns;
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
            CellButton *new_bttn = new CellButton(i, j);
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

std::vector<ProfileButton *> Window::FillLeaderBoard()
{
    std::vector<ProfileButton *> ProfileBttns;
    ProfileBttns.reserve(50);
    for (int i = 0; i < 50; ++i)
    {
        QHBoxLayout *box = new QHBoxLayout;
        ProfileButton *profile = new ProfileButton("profile");
        QLabel *name = new QLabel;
        profile->hide();
        name->hide();
        box->addWidget(name, 1, Qt::AlignLeft);
        box->addStretch(1);
        box->addWidget(profile, 1, Qt::AlignRight);
        main_LeaderBoardLayout->addLayout(box);
        ProfileBttns.push_back(profile);
    }
    return ProfileBttns;
}

void Window::UpdateLeaderBoard(const std::string &username, uint32_t user_id, const std::string &rating, int idx)
{
    QLayoutItem *item = main_LeaderBoardLayout->itemAt(idx + 2);
    QHBoxLayout *box = static_cast<QHBoxLayout *>(item->layout());

    QLabel *UsernameLabel = static_cast<QLabel *>(box->itemAt(0)->widget());
    ProfileButton *CheckProfileButton = static_cast<ProfileButton *>(box->itemAt(2)->widget());

    UsernameLabel->show();
    CheckProfileButton->show();
    CheckProfileButton->set_id(user_id); // На профиль какого игрока ссылка
    std::string result = username + "-" + rating;
    UsernameLabel->setText(QString::fromStdString(result));
}

QString Window::GetNewName()
{
    QDialog *window = new QDialog(this);
    window->setModal(true);
    QLineEdit *line = new QLineEdit(window);
    line->setPlaceholderText("Enter new name");

    QVBoxLayout *outer = new QVBoxLayout;
    QHBoxLayout *inner = new QHBoxLayout;

    QPushButton *ok = new QPushButton("Ok", window);
    QPushButton *close = new QPushButton("Cancel", window);

    inner->addWidget(ok);
    inner->addWidget(close);

    outer->addWidget(line);
    outer->addLayout(inner);

    window->setLayout(outer);

    QObject::connect(ok, &QPushButton::clicked, window, &QDialog::accept);
    QObject::connect(close, &QPushButton::clicked, window, &QDialog::reject);

    if (window->exec() == QDialog::Accepted)
        return line->text();
    else
        return "";
}

QString Window::GetNewPhoto()
{
    QFileDialog FileDialog(this);
    FileDialog.setFileMode(QFileDialog::ExistingFile);
    FileDialog.setNameFilter("Images (*.png *.jpg)");
    if (FileDialog.exec() == QDialog::Accepted)
    {
        QString file = FileDialog.selectedFiles()[0];
        return file;
    }
    return "";
}

// int Window::DialogWindow(const QString &txt) // сохрнаить заметку а еще удалить заметку и тп?
// {
//     QDialog *dialog = new QDialog(this);
//     dialog->setModal(true);
//     QLabel *lbl = new QLabel(txt, dialog);
//     QPushButton *ok = new QPushButton("Ок", dialog);
//     QPushButton *cancel = new QPushButton("Отмена", dialog);
//     QHBoxLayout *inner = new QHBoxLayout;
//     inner->addWidget(ok);
//     inner->addWidget(cancel);

//     QVBoxLayout *outer = new QVBoxLayout;

//     outer->addWidget(lbl);
//     outer->addLayout(inner);

//     dialog->setLayout(outer);

//     QObject::connect(ok, &QPushButton::clicked, dialog, &QDialog::accept);
//     QObject::connect(cancel, &QPushButton::clicked, dialog, &QDialog::reject);

//     return dialog->exec();
// }