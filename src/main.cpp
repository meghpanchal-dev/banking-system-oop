#include "Admin.h"

int main() {
    BankSystem bank;
    Admin      admin;
    int        choice;

    do {
        cout << "\n==============================\n";
        cout << "     BANKING SYSTEM\n";
        cout << "==============================\n";
        cout << "  1. Sign Up\n"
                "  2. Customer Login\n"
                "  3. Admin Login\n"
                "  4. Exit\n"
                "  Choice: ";
        cin >> choice;

        if (choice == 1) {
            string u, p;
            cout << "  Username: "; cin >> u;
            cout << "  Password: "; cin >> p;
            bank.signUp(u, p);

        } else if (choice == 2) {
            string u, p;
            cout << "  Username: "; cin >> u;
            cout << "  Password: "; cin >> p;
            Customer* c = bank.login(u, p);
            if (c) { cout << "  Login Success!\n"; c->menu(bank.getCustomers(), bank.getCustCount()); }
            else     cout << "  Invalid credentials\n";

        } else if (choice == 3) {
            string u, p;
            cout << "  Username: "; cin >> u;
            cout << "  Password: "; cin >> p;
            if (admin.login(u, p)) { cout << "  Admin Login Success!\n"; admin.panel(bank); }
            else cout << "  Wrong admin credentials\n";
        }

    } while (choice != 4);

    cout << "\n  Goodbye!\n";
    return 0;
}
