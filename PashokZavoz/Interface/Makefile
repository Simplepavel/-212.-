CXX = g++

# ПРАВИЛЬНЫЕ ПУТИ К QT FRAMEWORKS
QT_BASE = /opt/homebrew/Cellar/qtbase/6.9.3
QT_INCLUDE = -I$(QT_BASE)/lib/QtWidgets.framework/Headers \
             -I$(QT_BASE)/lib/QtCore.framework/Headers \
             -I$(QT_BASE)/lib/QtGui.framework/Headers
QT_FRAMEWORKS = -F$(QT_BASE)/lib
QT_LIBS = -framework QtWidgets -framework QtGui -framework QtCore

all:
	g++ Window.cpp --std=c++17 -g -c $(QT_INCLUDE) $(QT_FRAMEWORKS) $(QT_LIBS) -o ../interface.o

# # MOC КОМПИЛЯТОР
# MOC = /opt/homebrew/Cellar/qtbase/6.9.3/share/qt/libexec/moc

# # ФЛАГИ КОМПИЛЯЦИИ
# CXXFLAGS = -std=c++17 -Wall -g -с $(QT_INCLUDE) $(QT_FRAMEWORKS)
# LDFLAGS = $(QT_FRAMEWORKS) $(QT_LIBS)

# # ИСХОДНЫЕ ФАЙЛЫ
# SOURCES = main.cpp DurakOnline.cpp
# MOC_SOURCES = moc_DurakOnline.cpp
# OBJECTS = $(SOURCES:.cpp=.o) $(MOC_SOURCES:.cpp=.o)

# # ЦЕЛИ СБОРКИ
# all: $(TARGET)

# $(TARGET): $(OBJECTS)
# 	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# # Генерация moc файлов
# moc_DurakOnline.cpp: DurakOnline.hpp
# 	$(MOC) $< -o $@

# # Компиляция обычных .cpp файлов
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Компиляция moc сгенерированных файлов
# moc_%.o: moc_%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Отдельная цель для генерации moc файлов
# moc: $(MOC_SOURCES)

# run: $(TARGET)
# 	./$(TARGET)

# clean:
# 	rm -f $(TARGET) *.o moc_*.cpp

# clean-moc:
# 	rm -f moc_*.cpp

# info:
# 	@echo "=== QT BUILD INFORMATION ==="
# 	@echo "Qt path: $(QT_BASE)"
# 	@echo "Compiler: $(CXX)"
# 	@echo "Sources: $(SOURCES)"
# 	@echo "MOC sources: $(MOC_SOURCES)"
# 	@echo "Objects: $(OBJECTS)"
# 	@echo "Include flags: $(QT_INCLUDE)"

# .PHONY: all run clean clean-moc info moc