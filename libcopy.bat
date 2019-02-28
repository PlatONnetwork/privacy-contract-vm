::@echo off


SET ROOT_DIR=E:\work\local
SET PLANG_DIR=%ROOT_DIR%\private-contract-compiler
SET JIT_DIR=%ROOT_DIR%\private-contract-compiler
SET VM_DIR=%ROOT_DIR%\private-contract-vm2


copy %JIT_DIR%\build\Debug\bin\mpc-jit.pdb %VM_DIR%\build\bin\Debug\
copy %JIT_DIR%\build\Debug\bin\mpc-jit.pdb %VM_DIR%\build_without-platon\bin\Debug\
copy %JIT_DIR%\build32\Debug\bin\mpc-jit.pdb %VM_DIR%\build32\bin\Debug\

copy /Y %JIT_DIR%\build\Debug\bin\mpc-jit.dll %VM_DIR%\build\bin\Debug\
copy /Y %JIT_DIR%\build\Release\bin\mpc-jit.dll %VM_DIR%\build\bin\Release\
copy /Y %JIT_DIR%\build\Debug\bin\mpc-jit.dll %VM_DIR%\build_without-platon\bin\Debug\
copy /Y %JIT_DIR%\build\Release\bin\mpc-jit.dll %VM_DIR%\build_without-platon\bin\Release\
copy /Y %JIT_DIR%\build32\Debug\bin\mpc-jit.dll %VM_DIR%\build32\bin\Debug\
copy /Y %JIT_DIR%\build32\Release\bin\mpc-jit.dll %VM_DIR%\build32\bin\Release\

copy /Y %JIT_DIR%\build\Debug\bin\mpc-jit.dll %VM_DIR%\third-code\jit\lib\win\x64\Debug\
copy /Y %JIT_DIR%\build\Release\bin\mpc-jit.dll %VM_DIR%\third-code\jit\lib\win\x64\Release\
copy /Y %JIT_DIR%\build32\Debug\bin\mpc-jit.dll %VM_DIR%\third-code\jit\lib\win\Win32\Debug\
copy /Y %JIT_DIR%\build32\Release\bin\mpc-jit.dll %VM_DIR%\third-code\jit\lib\win\Win32\Release\

copy /Y %JIT_DIR%\build\Debug\lib\mpc-jit.lib %VM_DIR%\third-code\jit\lib\win\x64\Debug\
copy /Y %JIT_DIR%\build\Release\lib\mpc-jit.lib %VM_DIR%\third-code\jit\lib\win\x64\Release\
copy /Y %JIT_DIR%\build32\Debug\lib\mpc-jit.lib %VM_DIR%\third-code\jit\lib\win\Win32\Debug\
copy /Y %JIT_DIR%\build32\Release\lib\mpc-jit.lib %VM_DIR%\third-code\jit\lib\win\Win32\Release\


copy /Y %PLANG_DIR%\build\Debug\bin\plang.exe %VM_DIR%\tools\plang\win\x64\Debug\
copy /Y %PLANG_DIR%\build\Release\bin\plang.exe %VM_DIR%\tools\plang\win\x64\Release\
copy /Y %PLANG_DIR%\build32\Debug\bin\plang.exe %VM_DIR%\tools\plang\win\Win32\Debug\
copy /Y %PLANG_DIR%\build32\Release\bin\plang.exe %VM_DIR%\tools\plang\win\Win32\Release\



rem pause 5 seconds
ping 127.0.0.1 -n 5 > nul
