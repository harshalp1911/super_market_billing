#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

class Product {
    int pno;
    string name;
    float price, qty, tax, dis;

public:
    void create_product() {
        cout << GREEN "\nEnter Product No.: " RESET;
        while (!(cin >> pno)) {
            cout << RED "Invalid input! Enter a valid number: " RESET;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore();
        cout << GREEN "Enter Product Name: " RESET;
        getline(cin, name);
        cout << GREEN "Enter Price: " RESET;
        while (!(cin >> price) || price <= 0) {
            cout << RED "Invalid price! Enter a positive number: " RESET;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cout << GREEN "Enter Discount (%): " RESET;
        while (!(cin >> dis) || dis < 0 || dis > 100) {
            cout << RED "Invalid discount! Enter a value between 0-100: " RESET;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    void show_product() const {
        cout << "\nProduct No.: " << pno;
        cout << "\nName: " << name;
        cout << "\nPrice: " << price;
        cout << "\nDiscount: " << dis << "%";
    }

    int retpno() const { return pno; }
    float retprice() const { return price; }
    string retname() const { return name; }
    int retdis() const { return dis; }
};

fstream fp;
Product pr;

void write_product() {
    fp.open("Shop.dat", ios::out | ios::app | ios::binary);
    pr.create_product();
    fp.write(reinterpret_cast<char*>(&pr), sizeof(Product));
    fp.close();
    cout << GREEN "\nProduct has been added successfully!\n" RESET;
}

void display_all() {
    fp.open("Shop.dat", ios::in | ios::binary);
    if (!fp) {
        cout << RED "\nFile could not be opened!" RESET;
        return;
    }
    cout << GREEN "\n\tALL PRODUCTS\n" RESET;
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        pr.show_product();
        cout << "\n-----------------------------\n";
    }
    fp.close();
}

void display_sp(int n) {
    int found = 0;
    fp.open("Shop.dat", ios::in | ios::binary);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.retpno() == n) {
            pr.show_product();
            found = 1;
            break;
        }
    }
    fp.close();
    if (!found)
        cout << RED "\nRecord not found!" RESET;
}

void modify_product() {
    int no, found = 0;
    cout << GREEN "\nEnter Product No. to Modify: " RESET;
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::out | ios::binary);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.retpno() == no) {
            pr.show_product();
            cout << "\nEnter new details:\n";
            pr.create_product();
            int pos = -static_cast<int>(sizeof(pr));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&pr), sizeof(Product));
            found = 1;
            cout << GREEN "\nRecord Updated!" RESET;
            break;
        }
    }
    fp.close();
    if (!found)
        cout << RED "\nRecord Not Found!" RESET;
}

void delete_product() {
    int no;
    cout << GREEN "\nEnter Product No. to Delete: " RESET;
    cin >> no;
    fstream fp2;
    fp.open("Shop.dat", ios::in | ios::binary);
    fp2.open("Temp.dat", ios::out | ios::binary);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.retpno() != no)
            fp2.write(reinterpret_cast<char*>(&pr), sizeof(Product));
    }
    fp.close();
    fp2.close();
    remove("Shop.dat");
    rename("Temp.dat", "Shop.dat");
    cout << GREEN "\nRecord Deleted!" RESET;
}

void menu() {
    fp.open("Shop.dat", ios::in | ios::binary);
    if (!fp) {
        cout << RED "\nError! File could not be opened." RESET;
        return;
    }
    cout << GREEN "\n\tPRODUCT MENU\n" RESET;
    cout << "========================================\n";
    cout << "P.NO.\tNAME\tPRICE\n";
    cout << "========================================\n";
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        cout << pr.retpno() << "\t" << pr.retname() << "\t" << pr.retprice() << endl;
    }
    fp.close();
}

void admin_menu() {
    char ch2;
    do {
        cout << "\n\n\tADMIN MENU";
        cout << "\n1. CREATE PRODUCT";
        cout << "\n2. DISPLAY ALL PRODUCTS";
        cout << "\n3. QUERY PRODUCT";
        cout << "\n4. MODIFY PRODUCT";
        cout << "\n5. DELETE PRODUCT";
        cout << "\n6. VIEW PRODUCT MENU";
        cout << "\n7. BACK TO MAIN MENU";
        cout << "\nEnter choice (1-7): ";
        cin >> ch2;
        switch (ch2) {
            case '1': write_product(); break;
            case '2': display_all(); break;
            case '3': {
                int num;
                cout << "\nEnter Product No.: ";
                cin >> num;
                display_sp(num);
                break;
            }
            case '4': modify_product(); break;
            case '5': delete_product(); break;
            case '6': menu(); break;
            case '7': return;
            default: cout << RED "\nInvalid Choice!" RESET;
        }
    } while (ch2 != '7');
}

int main() {
    char ch;
    do {
        cout << "\n\n\tMAIN MENU";
        cout << "\n1. CUSTOMER";
        cout << "\n2. ADMINISTRATOR";
        cout << "\n3. EXIT";
        cout << "\nEnter choice (1-3): ";
        cin >> ch;
        switch (ch) {
            case '1': menu(); break;
            case '2': admin_menu(); break;
            case '3': exit(0);
            default: cout << RED "\nInvalid Choice!" RESET;
        }
    } while (ch != '3');
    return 0;
}
