#include <iostream>
#include <chrono>
#include <iomanip>

int main()
{

    int now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << std::fixed << now << "\n";

    return 0;
}