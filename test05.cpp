/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 23:13:27
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 15:24:45
 */
#include <iostream> 
#include <list> 
using namespace std; 
  
int main() 
{ 
    // declaration of list container 
    list<int> mylist;
    if(mylist.begin() == mylist.end()){
        cout << "yes" << endl;
    }
    if((++mylist.begin()) == mylist.end()){
        cout << "yes" << endl;
    }
    if((mylist.begin()++) == mylist.end()){
        cout << "yes" << endl;
    }
    mylist.insert(++mylist.begin(), 999);
    for (auto it = mylist.begin(); it != mylist.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    cout << mylist.size() << endl;
    cout << mylist.empty() << endl;
    return 0; 
}