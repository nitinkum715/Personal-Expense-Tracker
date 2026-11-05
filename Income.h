
#ifndef INCOME_H
#define INCOME_H

#include "Transaction.h"

class Income : public Transaction {

public:

    Income(int i, double a, string c, string d)
        : Transaction(i, a, c, d) {}

    string getType() const override {
        return "INCOME";
    }

    void display() override {
        cout << "\n----- INCOME DETAILS -----\n";
        Transaction::display();
        cout << "Type: Income" << endl;
    }
};

#endif
