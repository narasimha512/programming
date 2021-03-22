#include <iostream>
#include <ostream>
#include <enum.h>

class Enum
{
public:
    enum hello
    {
        One = 1,
        Two,
        Last
    };
};

class EnumDeriv : public Enum
{
public:
    enum Enum::hello
    {
        Three = Enum::Last,
        Four,
        Five
    };
};

class Abc
{
  public:
  Enum::hello eval;

  Abc(Enum::hello _eval): eval(_eval)
  {}

};

void printEnum(Enum::hello input)
{
    std::cout << input << std::endl;
}

int main()
{
    std::cout << EnumDeriv::One << std::endl;
    std::cout << EnumDeriv::Four << std::endl;

    std::cout << "printing" << std::endl;
    Abc a1(EnumDeriv::One);
    Abc a2(EnumDeriv::Four);
    return 0;
}

