@echo off

g++ src/*.cpp libs/glad/src/glad.c ^
-Ilibs/glfw/include ^
-Ilibs/glad/include ^
-Ilibs/glm ^
-Llibs/glfw/lib-mingw-w64 ^
-lglfw3 -lopengl32 -lgdi32 ^
-o game.exe

if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b
)

echo Running game...
game.exe