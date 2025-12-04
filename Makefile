includes_pqxx = -I D:/Libs++/PQXX/dist/include
libs_pqxx = -L D:/Libs++/PQXX/dist/lib -lpqxx -lpq -lws2_32
includes_qt = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtWidgets \
			-I C:\msys64\ucrt64\include\qt6\QtCore -I C:\msys64\ucrt64\include\qt6\QtGui
libs_qt = -L C:\msys64\ucrt64\lib -l Qt6Widgets -l Qt6Core -l Qt6Gui



object_file = currentuser.o database.o durakclient.o interface.o protocol.o figure.o board.o cellbutton.o utilts.o roundedavatar.o profilebutton.o base64.o
sources = main.cpp DurakOnline.cpp
moc_sources =  moc_DurakOnline.cpp moc_Client.cpp moc_CellButton.cpp moc_RoundedAvatar.cpp

.PHONY:all backendTarget gameTarget mocTarget

backendTarget:
	cd BackEnd/Client && make && cd ../..
	cd BackEnd/Game/Board && make && cd ../../..
	cd BackEnd/Game/Figure && make && cd ../../..
	cd BackEnd/Protocol && make && cd ../..
	cd BackEnd/Base64 && make && cd ../..

mocTarget:
	C:\msys64\ucrt64\share\qt6\bin\moc.exe DurakOnline.hpp -o moc_DurakOnline.cpp
	C:\msys64\ucrt64\share\qt6\bin\moc.exe BackEnd/Client/Client.hpp -o moc_Client.cpp
	C:\msys64\ucrt64\share\qt6\bin\moc.exe Interface/CellButton/CellButton.hpp -o moc_CellButton.cpp
	C:\msys64\ucrt64\share\qt6\bin\moc.exe Interface/RoundedAvatar/RoundedAvatar.hpp -o moc_RoundedAvatar.cpp

all: mocTarget backendTarget
	cd DataBase && make && cd ..
	cd CurrentUser && make && cd ..
	cd Interface && make && cd ..
	
	g++ $(sources) $(moc_sources) $(object_file) $(includes_pqxx) $(libs_pqxx) $(includes_qt) $(libs_qt) -o main

	rm *.o -f
	rm moc_* -f


