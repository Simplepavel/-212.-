includes_qt = -I C:\msys64\ucrt64\include\qt6 -I C:\msys64\ucrt64\include\qt6\QtCore
libs_qt = -L C:\msys64\ucrt64\lib -l Qt6Core

includes_pqxx = -I D:/Libs++/PQXX/dist/include
libs_pqxx = -L D:/Libs++/PQXX/dist/lib -lpqxx -lpq -lws2_32

all:
	g++ main.cpp database.cpp $(includes_qt) $(includes_pqxx) $(libs_qt) $(libs_pqxx)