#include <QCoreApplication>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <chrono>

void worker(){

    std::cout << "Hello world ..." << std::endl;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::vector<std::thread> vt;

    for(int i = 0; i < 10; i++){

        vt.push_back(std::thread(worker));

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for(std::vector<std::thread>::iterator it = vt.begin(); it != vt.end(); it++){

        it->join();
    }

    return a.exec();
}
