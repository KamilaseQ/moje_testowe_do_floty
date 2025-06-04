#ifndef BALANCE_H
#define BALANCE_H


class Balance {

public:
    Balance(double initial_balance);
    double getBalance(void);

    Balance& operator+=(double added_amount);
    Balance& operator-=(double subtracted_amount);

private:
    double balance_;
};


#endif // BALANCE_H
