includes_pqxx = -I D:/Libs++/PQXX/dist/include
libs_pqxx = -L D:/Libs++/PQXX/dist/lib -lpqxx -lpq -lws2_32
includes_qt = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtWidgets \
			-I C:\msys64\ucrt64\include\qt6\QtCore -I C:\msys64\ucrt64\include\qt6\QtGui
libs_qt = -L C:\msys64\ucrt64\lib -l Qt6Widgets -l Qt6Core -l Qt6Gui




object_file = currentuser.o database.o durakclient.o interface.o protocol.o figure.o board.o qboard.o
sources = main.cpp DurakOnline.cpp moc_DurakOnline.cpp moc_Client.cpp

.PHONY:all backendTarget gameTarget mocTarget

backendTarget:
	cd BackEnd/Client && make && cd ../..
	cd BackEnd/Game/Board && make && cd ../../..
	cd BackEnd/Game/Figure && make && cd ../../..
	cd BackEnd/Protocol && make && cd ../..

mocTarget:
	C:\msys64\ucrt64\share\qt6\bin\moc.exe DurakOnline.hpp -o moc_DurakOnline.cpp
	C:\msys64\ucrt64\share\qt6\bin\moc.exe BackEnd/Client/Client.hpp -o moc_Client.cpp

all: mocTarget backendTarget
	cd Interface && make && cd ..
	cd DataBase && make && cd ..
	cd CurrentUser && make && cd ..
	
	g++ $(sources) $(object_file) $(includes_pqxx) $(libs_pqxx) $(includes_qt) $(libs_qt) -o main

	rm *.o -f
	rm *_moc -f


