md build
cd build
REM cmake -G "Visual Studio 14 2015 Win64" ..\ -Dgtest_force_shared_crt=on
REM cmake -G "Visual Studio 15 2017 Win64" ..\ -Dgtest_force_shared_crt=on
cmake -G "Visual Studio 17 2022" -A x64 ..\ -Dgtest_force_shared_crt=on
PAUSE