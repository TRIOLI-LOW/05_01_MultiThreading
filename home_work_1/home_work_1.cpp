#include <iostream>
#include <thread>
#include <Windows.h>
#include <chrono>
bool clientFlag = false;  // Флаг клиента
bool operatorFlag = false;  // Флаг операциониста

int clientCount = 0;  // Счетчик клиентов
const int maxClients = 10;  // Максимальное количество клиентов

// Функция, имитирующая поток клиента
void clientThread()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 

        if (clientCount < maxClients)
        {
            clientFlag = true;  // Установка флага клиента
            ++clientCount;
            std::cout << "Пришел клиент. Всего клиентов: " << clientCount << std::endl;
            operatorFlag = true;  // Установка флага операциониста
        }
    }
}

// Функция, имитирующая поток операциониста
void operatorThread()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));  

        if (operatorFlag)
        {
            if (clientCount > 0)
            {
                --clientCount;
                std::cout << "Обслужен клиент. Осталось клиентов: " << clientCount << std::endl;
            }
            else
            {
                operatorFlag = false;  // Сброс флага операциониста
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    std::thread client(clientThread);  
    std::thread oper(operatorThread);  

    client.join();  
    oper.join();  

    return 0;
}