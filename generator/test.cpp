#include <iostream>
#include <memory>

int main()
{
    unsigned int ui = 2147483649;
    unsigned int *ptr = &ui;
    int i = *ptr;
    std::cout << "int i = " << i << "\n";
}