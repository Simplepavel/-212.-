QT_BASE = /opt/homebrew/Cellar/qtbase/6.9.3
QT_INCLUDE = -I$(QT_BASE)/lib/QtWidgets.framework/Headers \
             -I$(QT_BASE)/lib/QtCore.framework/Headers \
             -I$(QT_BASE)/lib/QtGui.framework/Headers
QT_FRAMEWORKS = -F$(QT_BASE)/lib
QT_LIBS = -framework QtWidgets -framework QtGui -framework QtCore

all:
	g++ Window.cpp --std=c++17 -g -c $(QT_INCLUDE) $(QT_FRAMEWORKS) $(QT_LIBS) -o ../interface.o
