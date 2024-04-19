#include <iostream>
#include <memory>

class testa
{
public:
    class inner
    {
    public:
        int _num = 0;
        inner(int a) : _num(a) {}
    };

    std::shared_ptr<inner> _ptr;

    testa() : _ptr(std::make_shared<inner>(11)) {}

    explicit testa(int a) : _ptr(std::make_shared<inner>(11))
    {

        std::cout << "yes\n";
    }

    void change_const() const
    {
        _ptr->_num = 12;
    }
};

testa make()
{
    return testa();
}

int main()
{
    testa a;

    testa &&a1 = make();

    auto a2 = a1;

    std::cout
        << "Compile Successfully!\n"
        << a2._ptr->_num << '\n';
    return 0;
}