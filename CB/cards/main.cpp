#include <iostream>
#include <vector>

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

std::vector<Card> card
{
    {1,    0, -100, {0,0}, 0, 0, 0, "����� ����� 100 ������� �����!"        },
    {2 , 200,    0, {0,0}, 0, 0, 0, "�� ����� � ������� 200 ������� �����!" },
    {                                                                      }

    /// � ��� ����� ...
};


int main()
{
    std::cout << "Hello world!" << std::endl;



    return 0;
}
