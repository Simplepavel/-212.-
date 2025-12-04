libs = -L C:\msys64\ucrt64\lib  -l Qt6Widgets -l Qt6Core -l Qt6Gui
includes = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtWidgets \
			-I C:\msys64\ucrt64\include\qt6\QtCore -I C:\msys64\ucrt64\include\qt6\QtGui

.PHONY: CellButton Utils ProfileButton

all: CellButton Utils RoundedAvatar ProfileButton
	g++ Window.cpp $(includes) $(libs) -o ../interface.o -c

CellButton:
	g++ CellButton/CellButton.cpp $(includes) $(libs) -o ../cellbutton.o -c

ProfileButton:
	g++ ProfileButton/ProfileButton.cpp $(includes) $(libs) -o ../profilebutton.o -c

RoundedAvatar::
	g++ RoundedAvatar/RoundedAvatar.cpp $(includes) $(libs) -o ../roundedavatar.o -c

Utils:
	g++ Utils.cpp -o ../utils.o -c $(includes) $(libs)
