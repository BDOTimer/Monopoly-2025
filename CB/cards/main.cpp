#include <iostream>

struct Card
{
    unsigned    type         ; /// Тип карточки.

    int         money        ; /// Приз или штраф в золотых монетах(без банка).
    int         moneyBank    ; /// Приз или штраф в золотых монетах(с банком).
    unsigned    steps[2]     ; /// Кол-во шагов от и до.
    unsigned    purchaseGoods; /// Право на приобретение товара.
    unsigned    changeStatus ; /// Право сменить статус.
    unsigned    moveChoose   ; /// Ход на выбор.
    std::string description  ; /// Уникальная запись.
};

Card card[]
{
    {  0, -100, {0,0}, 0, 0, 0, "Штраф банку 100 золотых монет!"        },
    {100,    0, {0,0}, 0, 0, 0, "Вы нашли в мусорке 200 золотых монет!" },
    {                                                                   }

    /// и так далее ...
};


int main()
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}
