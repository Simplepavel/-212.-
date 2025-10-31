QT_BASE = /opt/homebrew/Cellar/qtbase/6.9.3
QT_INCLUDE = -I$(QT_BASE)/lib/QtWidgets.framework/Headers \
             -I$(QT_BASE)/lib/QtCore.framework/Headers \
             -I$(QT_BASE)/lib/QtGui.framework/Headers
QT_FRAMEWORKS = -F$(QT_BASE)/lib
QT_LIBS = -framework QtWidgets -framework QtGui -framework QtCore
PQXX_INCLUDE = -I/opt/homebrew/include
PQXX_LIBS1 = -L/opt/homebrew/lib -lpqxx
PQXX_LIBS2 = -L/opt/homebrew/opt/libpq/lib/ -lpq
all: moc
	g++ DurakOnline.cpp main.cpp moc_DurakOnline.cpp interface.o currentuser.o database.o --std=c++17 $(QT_INCLUDE) $(QT_LIBS) $(QT_FRAMEWORKS) $(PQXX_INCLUDE) $(PQXX_LIBS1) $(PQXX_LIBS2) -o main.exe


moc:
	rm -f moc_*.cpp
	/opt/homebrew/Cellar/qtbase/6.9.3/share/qt/libexec/moc DurakOnline.hpp -o moc_DurakOnline.cpp