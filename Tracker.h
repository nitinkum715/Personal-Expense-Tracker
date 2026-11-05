
#ifndef TRACKER_H
#define TRACKER_H

#include "Income.h"
#include "Expense.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <map>
#include <string>

using namespace std;

class Tracker {

private:

    vector<Transaction*> transactions;
    map<string, double> monthlyBudgets;

    int nextId = 1;

    // Save all transactions
    void saveTransactions() {

        ofstream file("transactions.txt");

        if (!file) {
            cout << "Error opening transactions.txt!\n";
            return;
        }

        for (Transaction* t : transactions) {

            file << t->getId() << "|"
                 << t->getType() << "|"
                 << fixed << setprecision(2)
                 << t->getAmount() << "|"
                 << t->getCategory() << "|"
                 << t->getDate() << "\n";
        }

        file.close();
    }


    // Load saved transactions
    void loadTransactions() {

        ifstream file("transactions.txt");

        if (!file)
            return;

        string line;

        while (getline(file, line)) {

            if (line.empty())
                continue;

            stringstream ss(line);

            string idStr, type, amountStr, category, date;

            getline(ss, idStr, '|');
            getline(ss, type, '|');
            getline(ss, amountStr, '|');
            getline(ss, category, '|');
            getline(ss, date);

            try {

                int id = stoi(idStr);
                double amount = stod(amountStr);

                if (amount <= 0)
                    continue;

                if (type == "INCOME") {

                    transactions.push_back(
                        new Income(id, amount, category, date)
                    );
                }
                else if (type == "EXPENSE") {

                    transactions.push_back(
                        new Expense(id, amount, category, date)
                    );
                }
                else {
                    continue;
                }

                if (id >= nextId)
                    nextId = id + 1;
            }
            catch (const exception&) {

                cout << "Invalid transaction skipped: "
                     << line << endl;
            }
        }

        file.close();

        cout << "\nSaved Transactions Loaded: "
             << transactions.size() << endl;
    }


    // Save monthly budgets
    void saveBudgets() {

        ofstream file("budget.txt");

        if (!file) {
            cout << "Error opening budget.txt!\n";
            return;
        }

        for (const auto& budget : monthlyBudgets) {

            file << budget.first << "|"
                 << fixed << setprecision(2)
                 << budget.second << "\n";
        }

        file.close();
    }


    // Load monthly budgets
    void loadBudgets() {

        ifstream file("budget.txt");

        if (!file)
            return;

        string line;

        while (getline(file, line)) {

            stringstream ss(line);

            string monthYear, amountStr;

            getline(ss, monthYear, '|');
            getline(ss, amountStr);

            try {

                double amount = stod(amountStr);

                if (!monthYear.empty() && amount > 0) {
                    monthlyBudgets[monthYear] = amount;
                }
            }
            catch (const exception&) {
                continue;
            }
        }

        file.close();
    }


    // Generate month-year key
    string makeMonthKey(int month, int year) {

        string monthStr = (month < 10 ? "0" : "")
                          + to_string(month);

        return monthStr + "-" + to_string(year);
    }


    // Calculate expenses for a selected month
    double calculateMonthlyExpenses(int month, int year,
                                    map<string, double>* categoryTotals = nullptr,
                                    int* transactionCount = nullptr) {

        double total = 0;
        int count = 0;

        for (Transaction* t : transactions) {

            if (t->getType() != "EXPENSE")
                continue;

            string date = t->getDate();

            // Expected format: DD-MM-YYYY
            if (date.length() != 10 ||
                date[2] != '-' || date[5] != '-') {
                continue;
            }

            try {

                int transactionMonth = stoi(date.substr(3, 2));
                int transactionYear = stoi(date.substr(6, 4));

                if (transactionMonth == month &&
                    transactionYear == year) {

                    total += t->getAmount();
                    count++;

                    if (categoryTotals != nullptr) {
                        (*categoryTotals)[t->getCategory()]
                            += t->getAmount();
                    }
                }
            }
            catch (const exception&) {
                continue;
            }
        }

        if (transactionCount != nullptr) {
            *transactionCount = count;
        }

        return total;
    }


public:

    // Constructor
    Tracker() {
        loadTransactions();
        loadBudgets();
    }


    // Add Income
    void addIncome() {

        double amount;
        string category, date;

        cout << "\nEnter Income Amount: Rs. ";
        cin >> amount;

        if (cin.fail() || amount <= 0) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid amount!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Income Category: ";
        getline(cin, category);

        cout << "Enter Date (DD-MM-YYYY): ";
        getline(cin, date);

        if (category.empty() || date.empty()) {
            cout << "Category and date cannot be empty!\n";
            return;
        }

        transactions.push_back(
            new Income(nextId++, amount, category, date)
        );

        saveTransactions();

        cout << "\nIncome Added Successfully!\n";
    }


    // Add Expense
    void addExpense() {

        double amount;
        string category, date;

        cout << "\nEnter Expense Amount: Rs. ";
        cin >> amount;

        if (cin.fail() || amount <= 0) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid amount!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Expense Category: ";
        getline(cin, category);

        cout << "Enter Date (DD-MM-YYYY): ";
        getline(cin, date);

        if (category.empty() || date.empty()) {
            cout << "Category and date cannot be empty!\n";
            return;
        }

        transactions.push_back(
            new Expense(nextId++, amount, category, date)
        );

        saveTransactions();

        cout << "\nExpense Added Successfully!\n";
    }


    // View All Transactions
    void viewTransactions() {

        cout << "\n========== ALL TRANSACTIONS ==========\n";

        if (transactions.empty()) {
            cout << "No Transactions Found!\n";
            return;
        }

        for (Transaction* t : transactions) {

            t->display();

            cout << "--------------------------------------\n";
        }
    }


    // Search by Category
    void searchByCategory() {

        string searchCategory;
        bool found = false;

        cout << "\nEnter Category to Search: ";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, searchCategory);

        for (char &ch : searchCategory) {
            ch = tolower(static_cast<unsigned char>(ch));
        }

        cout << "\n========== SEARCH RESULTS ==========\n";

        for (Transaction* t : transactions) {

            string category = t->getCategory();

            for (char &ch : category) {
                ch = tolower(static_cast<unsigned char>(ch));
            }

            if (category == searchCategory) {

                t->display();
                cout << "--------------------------------------\n";

                found = true;
            }
        }

        if (!found)
            cout << "No transactions found in this category.\n";
    }


    // Delete Transaction
    void deleteTransaction() {

        int searchId;

        cout << "\nEnter Transaction ID to Delete: ";
        cin >> searchId;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid ID!\n";
            return;
        }

        for (auto it = transactions.begin();
             it != transactions.end(); it++) {

            if ((*it)->getId() == searchId) {

                cout << "\nTransaction Found!\n";
                (*it)->display();

                char confirm;

                cout << "\nAre you sure? (Y/N): ";
                cin >> confirm;

                if (confirm == 'Y' || confirm == 'y') {

                    delete *it;
                    transactions.erase(it);

                    saveTransactions();

                    cout << "\nTransaction Deleted Successfully!\n";
                }
                else {
                    cout << "\nDeletion Cancelled.\n";
                }

                return;
            }
        }

        cout << "\nTransaction ID Not Found!\n";
    }


    // Total Income
    void viewTotalIncome() {

        double totalIncome = 0;

        for (Transaction* t : transactions) {

            if (t->getType() == "INCOME")
                totalIncome += t->getAmount();
        }

        cout << fixed << setprecision(2);

        cout << "\n========== TOTAL INCOME ==========\n";
        cout << "Total Income: Rs. " << totalIncome << endl;
    }


    // Total Expenses
    void viewTotalExpenses() {

        double totalExpenses = 0;

        for (Transaction* t : transactions) {

            if (t->getType() == "EXPENSE")
                totalExpenses += t->getAmount();
        }

        cout << fixed << setprecision(2);

        cout << "\n========== TOTAL EXPENSES ==========\n";
        cout << "Total Expenses: Rs. " << totalExpenses << endl;
    }


    // Remaining Balance
    void viewBalance() {

        double totalIncome = 0;
        double totalExpenses = 0;

        for (Transaction* t : transactions) {

            if (t->getType() == "INCOME")
                totalIncome += t->getAmount();

            else if (t->getType() == "EXPENSE")
                totalExpenses += t->getAmount();
        }

        cout << fixed << setprecision(2);

        cout << "\n========== ACCOUNT BALANCE ==========\n";
        cout << "Total Income: Rs. " << totalIncome << endl;
        cout << "Total Expenses: Rs. " << totalExpenses << endl;
        cout << "Remaining Balance: Rs. "
             << totalIncome - totalExpenses << endl;
    }


    // Monthly Expense Report
    void monthlyExpenseReport() {

        int month, year;

        cout << "\nEnter Month (1-12): ";
        cin >> month;

        cout << "Enter Year: ";
        cin >> year;

        if (cin.fail() || month < 1 || month > 12 || year < 1) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Month or Year!\n";
            return;
        }

        map<string, double> categoryExpenses;
        int count = 0;

        double totalExpenses = calculateMonthlyExpenses(
            month, year, &categoryExpenses, &count
        );

        cout << fixed << setprecision(2);

        cout << "\n========== MONTHLY EXPENSE REPORT ==========\n";
        cout << "Month: " << month << "/" << year << endl;

        if (count == 0) {
            cout << "\nNo expenses found for this month.\n";
            return;
        }

        cout << "\n----- CATEGORY-WISE EXPENSES -----\n";

        for (const auto& item : categoryExpenses) {

            cout << item.first
                 << " : Rs. " << item.second << endl;
        }

        cout << "\nTotal Transactions: " << count << endl;

        cout << "Total Monthly Expenses: Rs. "
             << totalExpenses << endl;

        cout << "============================================\n";
    }


    // Set Monthly Budget
    void setMonthlyBudget() {

        int month, year;
        double budget;

        cout << "\nEnter Month (1-12): ";
        cin >> month;

        cout << "Enter Year: ";
        cin >> year;

        if (cin.fail() || month < 1 || month > 12 || year < 1) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Month or Year!\n";
            return;
        }

        cout << "Enter Monthly Budget: Rs. ";
        cin >> budget;

        if (cin.fail() || budget <= 0) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Budget!\n";
            return;
        }

        string key = makeMonthKey(month, year);

        monthlyBudgets[key] = budget;

        saveBudgets();

        cout << "\nMonthly Budget Saved Successfully!\n";
    }


    // View Monthly Budget Status
    void viewBudgetStatus() {

        int month, year;

        cout << "\nEnter Month (1-12): ";
        cin >> month;

        cout << "Enter Year: ";
        cin >> year;

        if (cin.fail() || month < 1 || month > 12 || year < 1) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Month or Year!\n";
            return;
        }

        string key = makeMonthKey(month, year);

        auto it = monthlyBudgets.find(key);

        if (it == monthlyBudgets.end()) {

            cout << "\nNo Budget Set for This Month!\n";
            return;
        }

        double budget = it->second;

        double spent = calculateMonthlyExpenses(month, year);

        double remaining = budget - spent;
        double percentage = (spent / budget) * 100;

        cout << fixed << setprecision(2);

        cout << "\n========== BUDGET STATUS ==========\n";

        cout << "Monthly Budget: Rs. " << budget << endl;
        cout << "Total Spent: Rs. " << spent << endl;
        cout << "Remaining Budget: Rs. " << remaining << endl;
        cout << "Budget Used: " << percentage << "%\n";

        if (percentage >= 100) {
            cout << "\nWARNING: MONTHLY BUDGET EXCEEDED!\n";
        }
        else if (percentage >= 80) {
            cout << "\nWARNING: You have used 80% or more of your budget!\n";
        }
        else {
            cout << "\nYou are within your monthly budget.\n";
        }

        cout << "==================================\n";
    }


    // Destructor
    ~Tracker() {

        for (Transaction* t : transactions) {
            delete t;
        }

        transactions.clear();
    }

};

#endif
