echo deleting file

cd build

del "**.sln" /q
del "**.vcxproj" /q
del "**.vcxproj.filters" /q
del "**.vcxproj.user" /q
rmdir "obj" /s /q
rmdir ".vs" /s /q

echo Done!