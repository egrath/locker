locker.exe: locker.obj locker.res
    link /subsystem:windows /out:locker.exe locker.res locker.obj
    
locker.res: locker.rc
    rc locker.rc
    
locker.obj: locker.cpp
    cl /GA /c /EHsc locker.cpp
    
clean:
    del /q locker.exe locker.obj locker.res
    