init:
	$(CXX) -Iextlibs\SFML\include\
		-Lextlibs\SFML\lib -o bin\Debug\ProjetTetris.exe src\main.cpp \
		-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio
