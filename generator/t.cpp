#include <iostream>
using namespace std;
int func(int **p)
{
    *p = (int *)8;
    return 0;
}

int main()
{
    int a = 9;
    int *p = &a;
    cout << *p << " " << p << "\n";
    func(&p);
    cout << *p << " " << p << "\n";
}