mainmake: main.cpp common.cpp communicator.cpp mainProgram.cpp portal.cpp receiver.cpp utils.cpp
	mpiCC main.cpp common.cpp communicator.cpp mainProgram.cpp portal.cpp receiver.cpp utils.cpp -lpthread -o main
