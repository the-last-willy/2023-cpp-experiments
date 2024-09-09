#include "dvector.hpp"

#include <iostream>

void print(dyn::vector<int>& vec)
{
    std::cout << "cap=" << vec.capacity() << " size=" << vec.size();
    if(vec.size() > 0)
        std::cout << " back=" << vec.back();
    std::cout << std::endl;
}

int main()
{
    auto vec = dyn::vector<int>();

    print(vec);
    for(int i = 0; i < 5; ++i)
    {
        vec.push_back(i);
        print(vec);
    }
}
