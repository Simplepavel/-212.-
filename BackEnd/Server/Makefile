libs = -L D:/Libs++/PQXX/dist/lib -lpqxx -lpq -lws2_32
pqxx_includes = -I D:/Libs++/PQXX/dist/include

sources = main.cpp Server.cpp ../Protocol/Protocol.cpp Session.cpp ../Game/Board/Board.cpp ../Game/Figure/Figure.cpp ../../DataBase/database.cpp


all:
	g++ $(sources) $(libs) $(pqxx_includes) -o main -g