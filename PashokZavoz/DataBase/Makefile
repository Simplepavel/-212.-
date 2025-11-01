QT_BASE = /opt/homebrew/Cellar/qtbase/6.9.3
QT_INCLUDE = -I$(QT_BASE)/lib/QtWidgets.framework/Headers \
             -I$(QT_BASE)/lib/QtCore.framework/Headers \
             -I$(QT_BASE)/lib/QtGui.framework/Headers
QT_FRAMEWORKS = -F$(QT_BASE)/lib
QT_LIBS = -framework QtWidgets -framework QtGui -framework QtCore



PQXX_INCLUDE = -I/opt/homebrew/include
PQXX_LIBS = -L/opt/homebrew/lib -lpqxx -lpq

all:
	g++ database.cpp --std=c++17 -c $(QT_INCLUDE) $(QT_FRAMEWORKS) $(QT_LIBS) $(PQXX_INCLUDE) $(PQXX_LIBS) -o ../database.o

