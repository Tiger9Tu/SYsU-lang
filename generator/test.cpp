#include <iostream>
#include <memory>
union U
{
    int i;
    float f;
    double d;
};

int main()
{
    U un;
    un.i = 1213;
    std::cout << un.i << "\n";
    std::cout << float(un.i) << "\n";
}