@echo off

slice2cpp.exe --underscore -I %ICE_ROOT%\slice *.ice
slice2java.exe --underscore -I %ICE_ROOT%\slice *.ice


rem xcopy /Y /S net E:\work\java\juzixmpcvmsdk\src\main\java\net\
rem copy /Y cfg.client* E:\work\java\config\
rem copy /Y cfg.server* E:\work\java\platonGo\dynamic\


rem pause 2 seconds
ping 127.0.0.1 -n 2 > nul

