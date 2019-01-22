mainmake: main.cpp common.cpp communicator.cpp mainProgram.cpp portal.cpp receiver.cpp utils.cpp
	mpiCC -g main.cpp common.cpp communicator.cpp mainProgram.cpp portal.cpp receiver.cpp utils.cpp -lpthread -o main
