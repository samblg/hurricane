@IF "%KAKE_HOME%"=="" (
    @echo The environment variable KAKE_HOME is not set!
    @echo Please set KAKE_HOME before run kake
) ELSE (
    @python3 "%KAKE_HOME%\src\main.py" %*
)
