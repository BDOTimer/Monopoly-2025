#include <iostream>

struct Card
{
    unsigned    type         ; /// ��� ��������.

    int         money        ; /// ���� ��� ����� � ������� �������(��� �����).
    int         moneyBank    ; /// ���� ��� ����� � ������� �������(� ������).
    unsigned    steps[2]     ; /// ���-�� ����� �� � ��.
    unsigned    purchaseGoods; /// ����� �� ������������ ������.
    unsigned    changeStatus ; /// ����� ������� ������.
    unsigned    moveChoose   ; /// ��� �� �����.
    std::string description  ; /// ���������� ������.
};

Card card[]
{
    {  0, -100, {0,0}, 0, 0, 0, "����� ����� 100 ������� �����!"        },
    {100,    0, {0,0}, 0, 0, 0, "�� ����� � ������� 200 ������� �����!" },
    {                                                                   }

    /// � ��� ����� ...
};


int main()
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}
