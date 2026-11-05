
#ifndef EXPENSE_H
#define EXPENSE_H

#include "Transaction.h"

class Expense : public Transaction {

public:

    Expense(int i, double a, string c, string d)
        : Transaction(i, a, c, d) {}

    string getType() const override {
        return "EXPENSE";
    }

    void display() override {
        cout << "\n----- EXPENSE DETAILS -----\n";
        Transaction::display();
        cout << "Type: Expense" << endl;
    }
};

#endif
