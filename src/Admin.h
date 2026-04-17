#pragma once
#include "BankSystem.h"

class Admin {
    string user = "admin";
    string pass = "admin123";

public:
    bool login(const string& u, const string& p) {
        return u == user && p == pass;
    }

    void panel(BankSystem& bank) {
        int ch;
        do {
            cout << "\n==============================\n";
            cout << "        ADMIN PANEL\n";
            cout << "==============================\n";
            cout << "  1. View All Customers\n"
                    "  2. Freeze Account\n"
                    "  3. Unfreeze Account\n"
                    "  4. Loan Requests\n"
                    "  5. Back\n"
                    "  Choice: ";
            cin >> ch;

            if (ch == 1) {
                for (int i = 0; i < bank.getCustCount(); i++) {
                    Customer& c = bank.getCustomers()[i];
                    cout << "  " << i << ". " << c.getUsername();
                    if (c.isAccountFrozen())  cout << " [FROZEN]";
                    if (c.hasPendingLoan())   cout << " [LOAN PENDING]";
                    cout << "\n";
                }

            } else if (ch == 2) {
                int id; cout << "  Customer index: "; cin >> id;
                if (id >= 0 && id < bank.getCustCount())
                    bank.getCustomers()[id].freeze();

            } else if (ch == 3) {
                int id; cout << "  Customer index: "; cin >> id;
                if (id >= 0 && id < bank.getCustCount())
                    bank.getCustomers()[id].unfreeze();

            } else if (ch == 4) {
                bool any = false;
                for (int i = 0; i < bank.getCustCount(); i++) {
                    Customer& c = bank.getCustomers()[i];
                    if (c.hasPendingLoan()) {
                        cout << "  " << i << ". " << c.getUsername()
                             << " — Loan: " << c.getRequestedLoanAmt() << "\n";
                        any = true;
                    }
                }
                if (!any) { cout << "  No pending loans\n"; continue; }

                int id; cout << "  Customer index to process: "; cin >> id;
                if (id >= 0 && id < bank.getCustCount() &&
                    bank.getCustomers()[id].hasPendingLoan()) {
                    int c; cout << "  1. Approve  2. Reject: "; cin >> c;
                    if (c == 1) { bank.getCustomers()[id].approveLoan();     cout << "  Loan Approved\n"; }
                    else        { bank.getCustomers()[id].clearLoanRequest(); cout << "  Loan Rejected\n"; }
                }
            }

        } while (ch != 5);
    }
};
