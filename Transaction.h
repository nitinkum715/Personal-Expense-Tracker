
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
using namespace std;

class Transaction {

protected:
    int id;
    double amount;
    string category;
    string date;

public:

    Transaction(int i, double a, string c, string d) {
        id = i;
        amount = a;
        category = c;
        date = d;
    }

    int getId() const {
        return id;
    }

    double getAmount() const {
        return amount;
    }

    string getCategory() const {
        return category;
    }

    string getDate() const {
        return date;
    }

    virtual string getType() const = 0;

    virtual void display() {
        cout << "ID: " << id << endl;
        cout << "Amount: Rs. " << amount << endl;
        cout << "Category: " << category << endl;
        cout << "Date: " << date << endl;
    }

    virtual ~Transaction() {}
};

#endif
