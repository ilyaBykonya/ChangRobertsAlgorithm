#include "TestModule/TestModule.h"

int main()
{
    //Здесь мы можем указать путь к любому файлу с тестовой конфигурацией и файлу, в который будет записан ответ
    string configFileName = "D:/Work/Freelance/ChangRobertsAlgorithm/test_1.txt";
    string outputFileName = "D:/Work/Freelance/ChangRobertsAlgorithm/result_1.txt";

    uint16_t result_leader = start_test(configFileName);
    ofstream outFile(outputFileName);
    outFile << "Leader has uid: " << result_leader;
}
