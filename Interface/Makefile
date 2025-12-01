libs = -L C:\msys64\ucrt64\lib  -l Qt6Widgets -l Qt6Core -l Qt6Gui
libs_pqxx = -L D:/Libs++/PQXX/dist/lib -lpqxx -lpq -lws2_32
includes = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtWidgets \
			-I C:\msys64\ucrt64\include\qt6\QtCore -I C:\msys64\ucrt64\include\qt6\QtGui
includes_pqxx = -I D:/Libs++/PQXX/dist/include

.PHONY: MyPushButton Utilts

all: MyPushButton Utilts
	g++ Window.cpp $(includes) $(libs) $(includes_pqxx) $(libs_pqxx) -o ../interface.o -c

MyPushButton:
	g++ MyPushButton/MyPushButton.cpp $(includes) $(libs) -o ../mypushbutton.o -c

Utilts:
	g++ Utilts.cpp -o ../utilts.o -c $(includes) $(libs)

# moc:
# 	rm -f moc_*.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe Window.h -o moc_window.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe MyLineEdit.h -o moc_LineEdit.cpp
# 	C:\msys64\ucrt64\share\qt6\bin\moc.exe MyPushButton.h -o moc_PushButton.cpp