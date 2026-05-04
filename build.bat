@echo off
g++ -std=c++17 -Wall src/main.cpp src/TaskManager.cpp -I src -o taskr.exe
if %errorlevel%==0 (echo build OK) else (echo build FAILED)