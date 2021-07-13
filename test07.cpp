/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 14:58:16
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 15:02:46
 */
#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct test07
{
    int a;
    int b;
};



int main ()
{
    std::list<test07*> mylist;
    
    test07* a = new test07();
    a->a = 1;
    a->b = 2;
    cout << a->b << endl;
    mylist.push_back(a);
    a = nullptr;
    if(a == nullptr){
        cout << "test1" << endl;
    }
    for(auto it = mylist.begin(); it != mylist.end(); it++){
        cout << (*it)->b << endl;
    }
    return 0;
}