# --- Переменные для путей ---
# Путь к вашим исходникам
SRC_DIR = .
# Путь к скомпилированной библиотеке Raylib в вашем проекте
LIBS_DIR = ./libs/raylib

# --- Флаги компилятора и линковщика ---
# Флаги для компилятора: указываем, где искать заголовочные файлы (.h)
CXXFLAGS = -I$(LIBS_DIR)/include

# Флаги для линковщика: указываем, где искать библиотеки (.a)
LDFLAGS = -L$(LIBS_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# --- Список всех .cpp файлов в проекте ---
# find ищет все .cpp файлы, tr и sed форматируют их для Make
SOURCES = main.cpp \
          Game/Game.cpp \
          GameRenderer/GameRenderer.cpp \
          GameGrid/GameGrid.cpp \
          Rules/Rules.cpp \
          Cell/Cell.cpp \
          raygui_impl.cpp

# Имя итогового исполняемого файла
TARGET = game_of_life.exe

# --- Правила сборки ---
# $^ - все зависимости (список SOURCES)
# $@ - цель (имя TARGET)
$(TARGET): $(SOURCES)
    g++ $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

# Вспомогательная цель 'clean' для удаления результатов сборки
clean:
    del /F $(TARGET) 2>nul || rm -f $(TARGET)

# Указываем, что 'clean' и 'all' — это не файлы, а имена целей
.PHONY: clean