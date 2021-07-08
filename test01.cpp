#include <list>
#include <iostream>

int main(int argc, char const *argv[])
{
    std::list<int>* p = new std::list<int>();
    p->push_back(1);
    p->push_back(1);
    p->push_back(1);
    for(auto i = p->begin(); i != p->end(); i++){
        std::cout << *i << std::endl;
    }
    return 0;
}
