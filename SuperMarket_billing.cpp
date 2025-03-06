#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

class Product {
    int pno;
    string name;
    float price, qty, tax, dis;

public:
    void create_product() {
        cout << "\nPlease Enter The Product No. of The Product: ";
        cin >> pno;
        cin.ignore();
        cout << "\nPlease Enter The Name of The Product: ";
        getline(cin, name);
        cout << "\nPlease Enter The Price of The Product: ";
        cin >> price;
        cout << "\nPlease Enter The Discount (%): ";
        cin >> dis;
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
    cout << "\nProduct has been created successfully!\n";
}

void display_all() {
    fp.open("Shop.dat", ios::in | ios::binary);
    if (!fp) {
        cout << "\nFile could not be opened!";
        return;
    }
    cout << "\n\n\tDISPLAY ALL RECORDS\n\n";
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
        cout << "\nRecord not found!";
}

void modify_product() {
    int no, found = 0;
    cout << "\nEnter Product No. to Modify: ";
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
            cout << "\nRecord Updated!";
            break;
        }
    }
    fp.close();
    if (!found)
        cout << "\nRecord Not Found!";
}

void delete_product() {
    int no;
    cout << "\nEnter Product No. to Delete: ";
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
    cout << "\nRecord Deleted!";
}

void menu() {
    fp.open("Shop.dat", ios::in | ios::binary);
    if (!fp) {
        cout << "\nError! File could not be opened. Go to Admin Menu to create the file.";
        return;
    }
    cout << "\n\tPRODUCT MENU\n";
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
            default: cout << "\nInvalid Choice!";
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
            default: cout << "\nInvalid Choice!";
        }
    } while (ch != '3');
    return 0;
}
