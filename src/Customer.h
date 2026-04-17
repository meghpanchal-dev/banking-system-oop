#pragma once
#include "Account.h"

class Customer {
    string username;
    string password;
    bool   frozen;

    Account* accounts[5];
    int      accCount;

    bool   loanRequested;
    double requestedLoanAmt;

public:
    Customer()
        : username(""), password(""), frozen(false),
          accCount(0), loanRequested(false), requestedLoanAmt(0) {}

    Customer(string u, string p)
        : username(u), password(p), frozen(false),
          accCount(0), loanRequested(false), requestedLoanAmt(0) {}

    ~Customer() {
        for (int i = 0; i < accCount; i++) delete accounts[i];
    }

    bool   login(const string& u, const string& p) const { return u == username && p == password; }
    string getUsername()         const { return username; }
    bool   isAccountFrozen()     const { return frozen; }
    bool   hasPendingLoan()      const { return loanRequested; }
    double getRequestedLoanAmt() const { return requestedLoanAmt; }
    int    getAccCount()         const { return accCount; }

    void freeze()   { frozen = true; }
    void unfreeze() { frozen = false; }

    bool addAccount(Account* a) {
        if (accCount >= 5) { cout << "  Account limit reached!\n"; return false; }
        accounts[accCount++] = a;
        return true;
    }

    void deleteAccount(int id) {
        if (id < 0 || id >= accCount) { cout << "  Invalid index\n"; return; }
        delete accounts[id];
        for (int i = id; i < accCount - 1; i++)  // shift left
            accounts[i] = accounts[i + 1];
        accCount--;
        cout << "  Account deleted\n";
    }

    void listAccounts() const {
        if (accCount == 0) { cout << "  No accounts\n"; return; }
        for (int i = 0; i < accCount; i++) {
            cout << "  " << i << ". ";
            accounts[i]->display();
        }
    }

    void showTotalBalance() const {
        double total = 0;
        for (int i = 0; i < accCount; i++)
            total += accounts[i]->getBalance();
        cout << "  Total Balance: " << total << "\n";
    }

    // Loan 
    void requestLoan(double amt) {
        if (loanRequested) { cout << "  Loan request already pending!\n"; return; }
        for (int i = 0; i < accCount; i++)
            if (accounts[i]->getType() == "Loan") {
                cout << "  Active loan exists.\n"; return;
            }
        loanRequested    = true;
        requestedLoanAmt = amt;
        cout << "  Loan request of " << amt << " submitted. Awaiting admin.\n";
    }

    void approveLoan() {
        string no = "L_" + username;
        addAccount(new LoanAccount(no, requestedLoanAmt));
        loanRequested    = false;
        requestedLoanAmt = 0;
    }

    void clearLoanRequest() { loanRequested = false; requestedLoanAmt = 0; }

    // Transfer
    bool transfer(Customer& to, int fromId, int toId, double amt) {
        if (fromId < 0 || fromId >= accCount)      return false;
        if (toId   < 0 || toId   >= to.accCount)   return false;
        if (accounts[fromId]->getBalance() < amt) {
            cout << "  Insufficient balance\n"; return false;
        }
        accounts[fromId]->setBalance(accounts[fromId]->getBalance() - amt);
        to.accounts[toId]->setBalance(to.accounts[toId]->getBalance() + amt);
        accounts[fromId]->addTransaction("Transfer Out", amt);
        to.accounts[toId]->addTransaction("Transfer In",  amt);
        cout << "  Transfer of " << amt << " successful\n";
        return true;
    }

    // Customer Menu
    void menu(Customer* all, int total) {
        if (frozen) { cout << "  Account frozen. Contact admin.\n"; return; }
        int ch;
        do {
            cout << "\n==============================\n";
            cout << "  Customer: " << username << "\n";
            cout << "==============================\n";
            cout << "  1. View Accounts\n"
                    "  2. Deposit\n"
                    "  3. Withdraw\n"
                    "  4. Transactions\n"
                    "  5. Request Loan\n"
                    "  6. Pay Loan EMI\n"
                    "  7. Apply Interest (Savings)\n"
                    "  8. Transfer Funds\n"
                    "  9. Total Balance\n"
                    " 10. Delete Account\n"
                    " 11. Logout\n"
                    "  Choice: ";
            cin >> ch;

            if (ch == 1) {
                listAccounts();

            } else if (ch == 2) {
                listAccounts();
                int id; double amt;
                cout << "  Account index: "; cin >> id;
                cout << "  Amount: ";        cin >> amt;
                if (id >= 0 && id < accCount) accounts[id]->deposit(amt);

            } else if (ch == 3) {
                listAccounts();
                int id; double amt;
                cout << "  Account index: "; cin >> id;
                cout << "  Amount: ";        cin >> amt;
                if (id >= 0 && id < accCount) accounts[id]->withdraw(amt);

            } else if (ch == 4) {
                listAccounts();
                int id; cout << "  Account index: "; cin >> id;
                if (id >= 0 && id < accCount) accounts[id]->showHistory();

            } else if (ch == 5) {
                double amt; cout << "  Loan amount: "; cin >> amt;
                requestLoan(amt);

            } else if (ch == 6) {
                for (int i = 0; i < accCount; i++)
                    if (accounts[i]->getType() == "Loan") {
                        dynamic_cast<LoanAccount*>(accounts[i])->payEMI();
                        break;
                    }

            } else if (ch == 7) {
                for (int i = 0; i < accCount; i++)
                    if (accounts[i]->getType() == "Savings")
                        dynamic_cast<SavingsAccount*>(accounts[i])->applyInterest();

            } else if (ch == 8) {
                listAccounts();
                int fromId; cout << "  Your account index: "; cin >> fromId;
                cout << "  --- Customers ---\n";
                for (int i = 0; i < total; i++)
                    cout << "  " << i << ". " << all[i].getUsername() << "\n";
                int custId; cout << "  Recipient index: "; cin >> custId;
                if (custId < 0 || custId >= total) continue;
                all[custId].listAccounts();
                int toId; cout << "  Recipient account index: "; cin >> toId;
                double amt; cout << "  Amount: "; cin >> amt;
                transfer(all[custId], fromId, toId, amt);

            } else if (ch == 9) {
                showTotalBalance();

            } else if (ch == 10) {
                listAccounts();
                int id; cout << "  Account index to delete: "; cin >> id;
                deleteAccount(id);
            }

        } while (ch != 11);
    }
};
