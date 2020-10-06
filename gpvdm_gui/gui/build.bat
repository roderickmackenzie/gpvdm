REM rd /s /q __pycahce__
REM rd /s /q dist
REM rd /s /q C:\tmp
REM mkdir c:\tmp
REM xcopy *.* C:\tmp
REM C:
REM cd C:\tmp\
pyinstaller --onefile gpvdm.py --icon=C:\Users\rod\Desktop\share\pub\images\icon.ico
move .\dist\*.exe C:\Users\rod\Desktop\share\pub\
REM move .\dist\*.exe Z:\pub\
time /t
REM z:
REM cd z:\pub\gui
