libs = -L C:\msys64\ucrt64\lib  -l Qt6Widgets -l Qt6Core -l Qt6Gui
includes = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtWidgets \
			-I C:\msys64\ucrt64\include\qt6\QtCore -I C:\msys64\ucrt64\include\qt6\QtGui

.PHONY: MyPushButton

all: MyPushButton
	g++ Window.cpp $(includes) $(libs) -o ../interface.o -c

MyPushButton:
	g++ MyPushButton/MyPushButton.cpp $(includes) $(libs) -o ../mypushbutton.o -c

# moc:
# 	rm -f moc_*.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe Window.h -o moc_window.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe MyLineEdit.h -o moc_LineEdit.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe MyPushButton.h -o moc_PushButton.cpp