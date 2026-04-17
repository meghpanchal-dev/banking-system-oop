#pragma once
#include <iostream>
#include <string>
using namespace std;

// Transaction
class Transaction {
    string type;
    double amount;
public:
    Transaction() : type(""), amount(0) {}
    Transaction(string t, double a) : type(t), amount(a) {}

    void display() {
        cout << "  " << type << " : " << amount << "\n";
    }
};

// Base Account
class Account {
protected:
    string accNo;
    double balance;
    Transaction history[20];
    int txnCount;

public:
    Account(string no = "", double bal = 0.0)
        : accNo(no), balance(bal), txnCount(0) {}

    virtual ~Account() {}

    void addTransaction(const string& type, double amt) {
        if (txnCount < 20)
            history[txnCount++] = Transaction(type, amt);
    }

    virtual void deposit(double amt) {
        if (amt <= 0) { cout << "  Invalid amount!\n"; return; }
        balance += amt;
        addTransaction("Deposit", amt);
        cout << "  Deposited " << amt << " | Balance: " << balance << "\n";
    }

    virtual void withdraw(double amt) = 0;

    virtual void display() const {
        cout << "  [" << getType() << "] " << accNo
             << " | Balance: " << balance << "\n";
    }

    void showHistory() {
        cout << "\n  --- Transactions: " << accNo << " ---\n";
        if (txnCount == 0) { cout << "  No transactions\n"; return; }
        for (int i = 0; i < txnCount; i++) history[i].display();
    }

    string getAccNo()    const { return accNo; }
    double getBalance()  const { return balance; }
    void   setBalance(double b) { balance = b; }
    virtual string getType() const { return "Account"; }
};

// Savings Account 
class SavingsAccount : public Account {
public:
    SavingsAccount(string no = "", double bal = 0.0)
        : Account(no, bal) {}

    void withdraw(double amt) override {
        if (amt <= 0) { cout << "  Invalid amount!\n"; return; }
        if (balance - amt < 500) {
            cout << "  Min balance of 500 required!\n"; return;
        }
        balance -= amt;
        addTransaction("Withdraw", amt);
        cout << "  Withdrawn " << amt << " | Balance: " << balance << "\n";
    }

    void applyInterest() {
        double interest = balance * 0.04;
        balance += interest;
        addTransaction("Interest", interest);
        cout << "  Interest added: " << interest
             << " | New Balance: " << balance << "\n";
    }

    string getType() const override { return "Savings"; }
};

// Current Account 
class CurrentAccount : public Account {
    double overdraftLimit;
public:
    CurrentAccount(string no = "", double bal = 0.0, double od = 1000.0)
        : Account(no, bal), overdraftLimit(od) {}

    void withdraw(double amt) override {
        if (amt <= 0) { cout << "  Invalid amount!\n"; return; }
        if (balance - amt < -overdraftLimit) {
            cout << "  Overdraft limit exceeded!\n"; return;
        }
        balance -= amt;
        addTransaction("Withdraw", amt);
        cout << "  Withdrawn " << amt << " | Balance: " << balance << "\n";
    }

    string getType() const override { return "Current"; }
};

// ─── Loan Account
class LoanAccount : public Account {
    double totalLoan;
    double emi;
    int remainingMonths;
public:
    LoanAccount(string no = "", double loan = 0.0,
                double rate = 0.07, int months = 12)
        : Account(no, 0.0)
    {
        totalLoan      = loan + (loan * rate);
        remainingMonths = months;
        emi            = totalLoan / months;
        addTransaction("Loan Taken", totalLoan);
        cout << "  Loan Approved: " << loan
             << " + Interest = " << totalLoan
             << " | EMI: " << emi << "\n";
    }

    void withdraw(double) override {
        cout << "  Cannot withdraw from Loan account\n";
    }

    void payEMI() {
        if (remainingMonths <= 0 || totalLoan <= 0) {
            cout << "  Loan already cleared!\n"; return;
        }
        totalLoan -= emi;
        remainingMonths--;
        addTransaction("EMI Paid", emi);
        cout << "  EMI paid: " << emi;
        if (totalLoan <= 0 || remainingMonths == 0) {
            cout << "\n  Loan fully repaid!\n";
            totalLoan = 0;
        } else {
            cout << " | Remaining: " << totalLoan
                 << " | Months left: " << remainingMonths << "\n";
        }
    }

    void display() const override {
        cout << "  [Loan] " << accNo
             << " | Remaining: " << totalLoan
             << " | EMI: " << emi
             << " | Months left: " << remainingMonths << "\n";
    }

    string getType() const override { return "Loan"; }
};
