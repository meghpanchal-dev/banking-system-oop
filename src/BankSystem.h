#pragma once
#include "Customer.h"

class BankSystem {
    Customer customers[10];
    int      custCount;

public:
    BankSystem() : custCount(0) {}

    Customer* getCustomers()    { return customers; }
    int       getCustCount() const { return custCount; }

    void signUp(const string& u, const string& p) {
        if (custCount >= 10) { cout << "  Max users reached!\n"; return; }
        customers[custCount] = Customer(u, p);
        // every new user gets a Savings + Current account by default
        customers[custCount].addAccount(new SavingsAccount("S_" + u, 1000));
        customers[custCount].addAccount(new CurrentAccount("C_" + u, 2000));
        custCount++;
        cout << "  Signup successful for " << u << "\n";
    }

    Customer* login(const string& u, const string& p) {
        for (int i = 0; i < custCount; i++)
            if (customers[i].login(u, p)) return &customers[i];
        return nullptr;
    }
};
