CXX = g++
CXXFLAGS = -I"./lib/raylib/raylib-6.0_win64_mingw-w64/include"
LDFLAGS = -L"./lib/raylib/raylib-6.0_win64_mingw-w64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm
SOURCES = main.cpp Cell/Cell.cpp Rules/Rules.cpp GameGrid/GameGrid.cpp GameRenderer/GameRenderer.cpp Game/Game.cpp raygui_impl.cpp
TARGET = game_of_life.exe

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	del $(TARGET) 2>nul || rm -f $(TARGET)

.PHONY: clean