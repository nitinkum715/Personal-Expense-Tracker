
#include <iostream>
#include "Tracker.h"

using namespace std;

int main() {

    Tracker tracker;

    int choice;

    do {

        cout << "\n";
        cout << "========================================\n";
        cout << "       PERSONAL EXPENSE TRACKER         \n";
        cout << "========================================\n";

        cout << "1.  Add Income\n";
        cout << "2.  Add Expense\n";
        cout << "3.  View All Transactions\n";
        cout << "4.  Search by Category\n";
        cout << "5.  Delete Transaction\n";
        cout << "6.  View Total Income\n";
        cout << "7.  View Total Expenses\n";
        cout << "8.  Check Remaining Balance\n";
        cout << "9.  Monthly Expense Report\n";
        cout << "10. Set Monthly Budget\n";
        cout << "11. View Budget Status\n";
        cout << "12. Exit\n";

        cout << "========================================\n";

        cout << "Enter Your Choice: ";
        cin >> choice;

        if (cin.fail()) {

            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid input! Enter a number.\n";
            continue;
        }

        switch (choice) {

            case 1:
                tracker.addIncome();
                break;

            case 2:
                tracker.addExpense();
                break;

            case 3:
                tracker.viewTransactions();
                break;

            case 4:
                tracker.searchByCategory();
                break;

            case 5:
                tracker.deleteTransaction();
                break;

            case 6:
                tracker.viewTotalIncome();
                break;

            case 7:
                tracker.viewTotalExpenses();
                break;

            case 8:
                tracker.viewBalance();
                break;

            case 9:
                tracker.monthlyExpenseReport();
                break;

            case 10:
                tracker.setMonthlyBudget();
                break;

            case 11:
                tracker.viewBudgetStatus();
                break;

            case 12:
                cout << "\nThank you for using Personal Expense Tracker!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 12);

    return 0;
}
