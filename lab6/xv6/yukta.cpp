#include <bits/stdc++.h>
using namespace std;

int main()
{
    int a = 9;
    int * d = &a;
    void *p = d;
    int b = *((int *)p);
    cout<<b<<endl;
}
