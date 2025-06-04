#include "Balance.h"


Balance::Balance(double initial_balance)
{
    balance_ = initial_balance;
}


double Balance::getBalance(void)
{
    return balance_;
}


Balance& Balance::operator+=(double added_amount)
{
    balance_ += added_amount;
    return *this;
}


Balance& Balance::operator-=(double subtracted_amount)
{
    balance_ -= subtracted_amount;
    return *this;
}
