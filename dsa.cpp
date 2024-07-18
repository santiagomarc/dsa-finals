#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits> 
#include <vector> //data structure para sa menu, cart
#include <algorithm> // all_of
#include <stdexcept> // For std::invalid_argument
#include <iomanip>   // For std::setprecision and std::fixed
#include <fstream>
#include <Windows.h>

using namespace std;

void userlogin();
void user_menu(const string& username);
void addUserCart(const string& username);
void editUserCart(const string& username);
void proceedOrder(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change);
void displayOrderSummary(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change);
void top_up(const string& username);
void check_credentials(const string& username);
void saveUserAccounts();
void loadUserAccounts();
int main();

void clearInputBuffer() 
{
    cin.clear(); // Clear the error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
}
void setTextColor(int colorCode) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}
void resetTextColor() 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

float cashAmount = 0.0;
float change = 0.0;
int paymentChoice;
float totalCost = 0.0;


struct CartItem {
    int id;
    string name;
    int quantity;
    float price;
};

map<string, vector<CartItem>> menu = {
    {"burgers", {
        {1, "burger1", 10, 10.00},
        {2, "burger2", 10, 10.00},
        {3, "burger3", 10, 10.00},
        {4, "burger4", 10, 10.00},
        {5, "burger5", 10, 10.00}
    }},
    {"fries", {
        {1, "fries1", 10, 5.00},
        {2, "fries2", 10, 5.00},
        {3, "fries3", 10, 5.00},
        {4, "fries4", 10, 5.00},
        {5, "fries5", 10, 5.00}
    }},
    {"salads", {
        {1, "salad1", 10, 8.00},
        {2, "salad2", 10, 8.00},
        {3, "salad3", 10, 8.00},
        {4, "salad4", 10, 8.00},
        {5, "salad5", 10, 8.00}
    }},
    {"drinks", {
        {1, "drink1", 10, 3.00},
        {2, "drink2", 10, 3.00},
        {3, "drink3", 10, 3.00},
        {4, "drink4", 10, 3.00},
        {5, "drink5", 10, 3.00}
    }},
    {"desserts", {
        {1, "dessert1", 10, 6.00},
        {2, "dessert2", 10, 6.00},
        {3, "dessert3", 10, 6.00},
        {4, "dessert4", 10, 6.00},
        {5, "dessert5", 10, 6.00}
    }}
};

class Cart {
    private:
    vector<CartItem> usercart;
    
    public:
    void addToCart(const CartItem& item) {
    auto it = find_if(usercart.begin(), usercart.end(), [&item](const CartItem& cartItem) {
        return cartItem.name == item.name;
    });

    if (it != usercart.end()) {
        it->quantity += item.quantity; //add quantity pag exist sa cart
    } else {
        usercart.push_back(item);
    }
}
    void deleteFromCart(int id) {
        auto it = remove_if(usercart.begin(), usercart.end(), [id](const CartItem& item) { return item.id == id; });
        if (it != usercart.end()) {
            usercart.erase(it, usercart.end());
            cout << "Item removed from cart successfully." << endl;
        } else {
            cout << "Item not found in the cart." << endl;
        }

    }

    void editCart(int index, int newQuantity) {
            if (index >= 0 && index < usercart.size()) {
            usercart[index].quantity = newQuantity;
        } else {
            cout << "Invalid item index." << endl;
        }
    }

    bool viewCart(string& cartContent) const {
    if (usercart.empty()) {
        cartContent = "Your cart is empty!";
        return true; //EMPTY
    }
    
    cartContent = "";
    int index = 1;
    for (const auto& item : usercart) 
    {
        cartContent += to_string(index) + ". " + item.name + " - " + to_string(item.quantity) + " x P" + to_string(item.price) + "\n";
        index++;
    }
    return false; // NOT EMPTY
    }

    void clearCart() {
        usercart.clear();
    }

    vector<CartItem>& getUserCart() {
        return usercart;
    }
};

struct UserDetails {
    string password;
    string phoneNumber;
    string address;
    float balance = 0;
    Cart cart;
};

map <string, UserDetails> user_accounts; 

void saveUserAccounts() {
    ofstream outFile("user.txt");

    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (map<string, UserDetails>::const_iterator it = user_accounts.begin(); it != user_accounts.end(); ++it) {
        outFile << it->first << "," << it->second.password << ","
                << it->second.phoneNumber << "," << it->second.address << ","
                << it->second.balance << endl;
    }
    outFile.close();
}

void displayMenu() {
    int choice;
    do { 
        cout << "Choose a category:" << endl;
        cout << "1. Burgers" << endl;
        cout << "2. Fries" << endl;
        cout << "3. Salads" << endl;
        cout << "4. Drinks" << endl;
        cout << "5. Desserts" << endl;
        cout << "6. Go back to Main Menu" << endl;
        try {
            cout << "Enter the number of your choice: ";
            cin >> choice;
            if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

            switch (choice) {
                case 1:
                    cout << "Burgers Menu:" << endl;
                    for (const auto& item : menu["burgers"]) {
                        cout << item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    break;
                case 2:
                    cout << "Fries Menu:" << endl;
                    for (const auto& item : menu["fries"]) {
                        cout << item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    break;
                case 3:
                    cout << "Salads Menu:" << endl;
                    for (const auto& item : menu["salads"]) {
                        cout << item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    break;
                case 4:
                    cout << "Drinks Menu:" << endl;
                    for (const auto& item : menu["drinks"]) {
                        cout << item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    break;
                case 5:
                    cout << "Desserts Menu:" << endl;
                    for (const auto& item : menu["desserts"]) {
                        cout << item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    break;
                case 6:
                    break;
                default:
                    cout << "******Invalid choice. Please choose a valid category******" << endl;
                    break;
            }
        } catch (const invalid_argument& e) {
            cout << "\n****************************" << endl;
            cout << e.what() << endl;
            cout << "****************************\n" << endl;
        }
    } while (choice != 6);
    main();
}

void registerUser() {
    while (true) {
        cout << "\n-----------REGISTER NEW USER-----------" << endl;
        cout << "Please input your username and password: \n" << endl;
        string username;
        string password;
        string phoneNumber;
        string address;
        float balance = 0.0;

        cout << "Enter a username: ";
        cin >> username;
        cout << "Password (must be 8 characters long): ";
        cin >> password;

        if (username.length() < 4) {
            cout << "\n*******************************************" << endl;
            cout << "Username must be at least 4 characters long." << endl;
            cout << "********************************************" << endl;
            continue;
        }

        while (password.length() < 8) {
            cout << "\n*******************************************" << endl;
            cout << "Password must be at least 8 characters long." << endl;
            cout << "********************************************" << endl;
            cout << "Password (must be 8 characters long): ";
            cin >> password;
        }

        if (user_accounts.find(username) != user_accounts.end()) {
            cout << "\n******************************************************\n";
            cout << "Username already exists. Please choose a different one.\n";
            cout << "******************************************************\n";
            continue;
        }

        cout << "Enter your phone number (must be 11 digits): ";
        cin >> phoneNumber;
        while (phoneNumber.length() != 11 || !all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit)) {
            cout << "\n********************************************" << endl;
            cout << "Phone number must be exactly 11 digits long." << endl;
            cout << "********************************************" << endl;
            cout << "Enter your phone number (must be 11 digits): ";
            cin >> phoneNumber;
        }

        cout << "Enter your address: ";
        cin.ignore();
        getline(cin, address);

        cout << "\n-------------------------------" << endl;
        cout << "Account registered successfully" << endl;
        cout << "-------------------------------\n" << endl;

        UserDetails newUser = {password, phoneNumber, address, 0.0, Cart()};
        user_accounts[username] = newUser;
        saveUserAccounts();
        
        break;
        main();
    }
}

void userlogin() {
    while (true) {
        cout << "\n-----------USER LOGIN-----------" << endl;
        cout << "Enter your username and password to login: \n" << endl;
        string username;
        string password;

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (user_accounts.find(username) == user_accounts.end()) {
            cout << "\n***************************************" << endl;
            cout << "Username does not exist. Please try again." << endl;
            cout << "***************************************" << endl;
            break;
        }

        if (user_accounts[username].password != password) {
            cout << "\n*************************************" << endl;
            cout << "Incorrect password. Please try again." << endl;
            cout << "*************************************" << endl;
            break;
        }

        cout << "\n-------------------------------" << endl;
        cout << "Login successful!" << endl;
        cout << "-------------------------------\n" << endl;
        user_menu(username);
        break;
    }
    main();
}


void user_menu(const string& username) {
    int choice;
    while (choice != 6) {
        try {
            cout << "Logged in as " << username << " ---- Balance: P " << user_accounts[username].balance << endl;
            cout << "1. Add to cart" << endl;
            cout << "2. Edit your cart" << endl;
            cout << "3. Top-up Ezbyte Wallet" << endl;
            cout << "4. View Cart and Proceed to Payment" << endl;
            cout << "5. Check Credentials" << endl;
            cout << "6. Log out" << endl << endl;

            int choice;
            cin >> choice;

            if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Invalid input");
            }

            switch (choice) {
                case 1:
                    addUserCart(username);
                    break;
                case 2:
                    editUserCart(username);
                    break;
                case 3:
                    top_up(username);
                    break;
                case 4:
                    proceedOrder(username, paymentChoice, totalCost, cashAmount, change);
                    break;
                case 5:
                    check_credentials(username);
                    break;
                case 6:
                    cout << "\n----------------------------------" << endl;
                    cout << "  User: '" << username << "' has logged out!" << endl;
                    cout << "----------------------------------\n" << endl;
                    main();
                    break;
                default:
                    cout << "\n***************************" << endl;
                    cout << "Please input a valid choice" << endl;
                    cout << "***************************\n" << endl;
            }
        } catch (const invalid_argument& e) {
            cout << "\n***************************" << endl;
            cout << e.what() << endl;
            cout << "***************************\n" << endl;
        }
    }
    main();
}

void addUserCart (const string& username) {
    cout << "Select a category:\n";
    int categoryIndex = 1;
    map<int, string> categoryMap;
    
    for (const auto& category : menu) {
        cout << categoryIndex << ". " << category.first << endl;
        categoryMap[categoryIndex] = category.first;
        categoryIndex++;
    }

    int ctgChoice;
    cin >> ctgChoice;

    if (categoryMap.find(ctgChoice) == categoryMap.end()) {
        cout << "Invalid category selected.\n";
        return;
    }

    string selectedCategory = categoryMap[ctgChoice];
    vector<CartItem>& selectedCategoryitems = menu[selectedCategory];

    cout << "---SELECT AN ITEM---\n";
    for (const auto& item : selectedCategoryitems) {
        cout << item.id << ". " << item.name << " - P" << item.price << " (" << item.quantity << " in stock)\n";

    }
    int itemId;
    cin >> itemId;

    auto it = find_if(selectedCategoryitems.begin(), selectedCategoryitems.end(), [itemId](const CartItem& item) {return item.id == itemId; });

    if (it == selectedCategoryitems.end()){
        cout << "Invalid item selected.\n";
        return;
    }

    if (it->quantity == 0) {
        cout << "Item is out of stock and cannot be ordered.\n";
        return;
    }

    int quantity;
    do {   // Change item.quantity to item.stock
        cout << "Enter the quantity of the item you want to order:";
        cin >> quantity;
        if (quantity <= 0 || quantity > it->quantity) {
            cout << "Invalid quantity entered! " << it->quantity << " stocks left for " << it->name << "!\n";
        }
    } while (quantity <= 0 || quantity > it->quantity);

    CartItem selectedItem = *it;
    selectedItem.quantity = quantity;

    user_accounts[username].cart.addToCart(selectedItem);
    it->quantity -= quantity; // Decrement stock
    cout << "Item added to cart successfully.\n";
    return;
}

void editUserCart(const string& username) {
    // Step 1: Display the user's cart
    string cartContent;
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        cout << cartContent << endl;  
        return; // if empty nothing to delete
    } else {
        cout << "\n------------------- Your Cart -------------------" << endl;
        cout << cartContent << endl;
        cout << "-------------------------------------------------" << endl;
    }

    int itemId;
    cout << "Enter the index of the item you want to edit: ";
    cin >> itemId;

    if (itemId < 1 || itemId > user_accounts[username].cart.getUserCart().size()) {
        cout << "Invalid item index. Please enter a valid index." << endl;
        return;
    }

    cout << "Do you want to: "<< endl;
    cout << "1. Edit item quantity" << endl;
    cout << "2. Delete item from your cart" << endl;
    cout << "3. Go back to user menu\n" << endl;
    int choice;
    cin >> choice;

    if (choice == 1) {// Edit 
        int newQuantity;
        cout << "Enter the new quantity: ";
        cin >> newQuantity;

        if (newQuantity <= 0) {
            cout << "Invalid quantity entered. Please enter a positive value." << endl;
            return;
        }

        user_accounts[username].cart.editCart(itemId - 1, newQuantity);
        cout << "Item quantity updated successfully.\n";

    } else if (choice == 2) {
        auto& userCart = user_accounts[username].cart.getUserCart();
        userCart.erase(userCart.begin() + (itemId - 1));
        cout << "Item removed from cart successfully.\n";
    } else if (choice == 3) {
        return;
    } else {
        cout << "Invalid choice. Please enter 1 or 2.\n";
        return;
    }
}

void proceedOrder(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change) {
    // Step 1: Display the user's cart
    string cartContent;
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        cout << "Your cart is empty. Please add items to proceed with your order.\n";
        return;
    }

    // Step 3: Calculate total cost
    const vector<CartItem>& userCart = user_accounts[username].cart.getUserCart();
    for (const auto& item : userCart) {
        totalCost += item.price * item.quantity;
    }

    cout << "\n------------------- Your Cart -------------------" << endl;
    cout << cartContent;
    cout << "\nTotal cost of your order: " << totalCost << " pesos" << endl;
    cout << "-------------------------------------------------" << endl;

    // Step 2: Confirm the order
    string confirm;
    cout << "Proceed with this order? (yes/no): ";
    cin >> confirm;

    if (confirm != "yes") {
        cout << "Order canceled. Returning to user menu.\n";
        totalCost = 0.0;
        return;
    }

    cout << "\nSelect payment method:" << endl;
    cout << "1. Ezbyte Wallet (Current balance: " << user_accounts[username].balance << " pesos)" << endl;
    cout << "2. Cash-on-Delivery" << endl;
    cout << "Enter your choice: ";
    cin >> paymentChoice;


    if (paymentChoice == 1) {
        if (user_accounts[username].balance < totalCost) {
            cout << "Insufficient balance. Please top-up your Ezbyte Wallet.\n";
            return;
        }
        user_accounts[username].balance -= totalCost;
        cashAmount = totalCost;
        change = 0.0;
        cout << "Payment successful via Ezbyte Wallet.\n";

    } else if (paymentChoice == 2) {
        do {
            cout << "Enter cash amount: ";
            cin >> cashAmount;

            if (cashAmount < totalCost) {
                cout << "Insufficient amount. Please enter the exact amount.\n";
            }
        } while (cashAmount < totalCost);

        change = cashAmount - totalCost;
        cout << "Payment successful via Cash-on-Delivery. Your change: " << change << " pesos" << endl;

    } else {
        cout << "Invalid payment method selected.\n";
        return;
    }

    displayOrderSummary(username, paymentChoice, totalCost, cashAmount, change);
    user_accounts[username].cart.clearCart();
    saveUserAccounts();
}

void displayOrderSummary(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change) {
    // Display user details from user_accounts directly
    cout << "\n\n";
    setTextColor(6);
    cout << "               ======================================= Order Summary ======================================\n";
    resetTextColor();
    cout << "                  Customer Name: " << username << "\n\n";
    cout << "                  Delivery Address: " << user_accounts[username].address << "\n\n";
    cout << "                  Phone Number: " << user_accounts[username].phoneNumber << "\n\n";
    cout << "                  Payment Mode: " << paymentChoice << ". " << (paymentChoice == 1 ? "Ezbyte Wallet" : "Cash-on-Delivery") << "\n\n";
    cout << "                  Payment Amount: " << cashAmount << " Pesos\n\n";
    cout << "                  Change: " << change << " Pesos\n\n";

    setTextColor(6);
    cout << "                                                       Ordered Items                                      \n";
    cout << "               ===========================================================================================" << endl;
    resetTextColor();
    cout << "                                                                                                          \n";

    // Display user's cart using viewCart method
    string cartContent;
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        cout << "Error: Cart is empty.\n";
        return;
    }

    cout << cartContent;

    cout << "                                                                                                          \n";
    cout << "                                                                                                          \n";
    cout << fixed << setprecision(2);
    cout << "                                                Total cost: " << totalCost << " Pesos\n";
    setTextColor(6);
    cout << "               ===========================================================================================\n\n\n";
    resetTextColor();
}

void top_up (const string& username) {
    try {
        string input;
        cout << "\n-----Top-up Ezbyte Wallet-----\n";
        cout << "Enter the amount to top up (leave blank to cancel transcation): ";
        getline(cin >> ws, input);  //allow blank input

        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Invalid input. Please enter a valid number.");
            }

        if (input.empty()) {
            cout << "\n--------------------------------------------------------" << endl;
            cout << "                TRANSACTION CANCELLED" << endl;
            cout << "--------------------------------------------------------\n" << endl;
            return;

        }
        float amount = stof(input);
        if (amount > 0){
            user_accounts[username].balance += amount; 
            saveUserAccounts();

            cout << fixed << setprecision(2);
            cout << "\n------------------------------------------------------" << endl;
            cout << "      Top-up successful. New balance: P " << user_accounts[username].balance << endl;
            cout << "------------------------------------------------------\n"<< endl;
        } else {
            cout << " \nInvalid amount. Please enter a positive value.\n" << endl;
            return;
        }
    } catch (const invalid_argument& e) {
        cout << "\n***************************" << endl;
        cout << "Invalid input. Please enter a valid amount." << endl;
        cout << "***************************\n" << endl;
    } 
}

void check_credentials (const string& username) {
    cout << "\n------------------- User Information -------------------" << endl;
    cout << "Username: " << username << endl;
    cout << "Password: " << user_accounts[username].password << endl;

    string cartContent;
    bool isEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (!isEmpty) {
        cout << cartContent << endl; // Display "Your cart is empty!" message
    } else {
        cout << "User's Cart is empty!" << endl;
    }

    cout << "Balance: " << fixed << setprecision(2) << user_accounts[username].balance << endl;
    cout << "-------------------------------------------------------" << endl;

}
void loadUserAccounts() {
    ifstream inFile("user.txt");

    if (!inFile) {
        cerr << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string username, password, phoneNumber, address;
        float balance;

        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, phoneNumber, ',');
        getline(ss, address, ',');
        ss >> balance;

        user_accounts[username] = {password, phoneNumber, address, balance};
    }

    inFile.close();
}

struct itemNode {
    int index;
    string name;
    int stock;
    float price;
    struct itemNode *next;
};

class linkedmenu {
private:
    itemNode *head, *tail;

public:
    linkedmenu() : head(nullptr), tail(nullptr) {}

    void addItem(int index, const string& name, int stock, float price) {  // Changed to float
        itemNode* newItem = new itemNode{index, name, stock, price, nullptr};
        if (!head) {
            head = newItem;
            tail = newItem;
        } else {
            tail->next = newItem;
            tail = newItem;
        }
    }

    void editStock(int index, int newStock, map<string, vector<CartItem>>& menu, const string& category) {
        itemNode* temp = head;
        while (temp) {
            if (temp->index == index) {
                temp->stock = newStock;
                for (auto& item : menu[category]) {
                    if (item.id == index) {
                        item.quantity = newStock;
                        cout << "Stock for " << item.name << " updated to " << newStock << "." << endl;
                        return;
                    }
                }
            }
            temp = temp->next;
        }
        cout << "Item not found." << endl;
    }

    void editPrice(int index, float newPrice, map<string, vector<CartItem>>& menu, const string& category) {  // Changed to float
        itemNode* temp = head;
        while (temp) {
            if (temp->index == index) {
                temp->price = newPrice;
                for (auto& item : menu[category]) {
                    if (item.id == index) {
                        item.price = newPrice;
                        cout << "Price for " << item.name << " updated to " << newPrice << " pesos." << endl;
                        return;
                    }
                }
            }
            temp = temp->next;
        }
        cout << "Item not found." << endl;
    }

    void editName(int index, const string& newName, map<string, vector<CartItem>>& menu, const string& category) {
        itemNode* temp = head;
        while (temp) {
            if (temp->index == index) {
                temp->name = newName;
                for (auto& item : menu[category]) {
                    if (item.id == index) {
                        cout << "Name for " << item.name << " updated to " << newName << "." << endl;
                        item.name = newName;
                        return;
                    }
                }
            }
            temp = temp->next;
        }
        cout << "Item not found." << endl;
    }
};

map<string, linkedmenu> menuLists;

void initializeMenu(const map<string, vector<CartItem>>& menu) {
    for (const auto& category : menu) {
        linkedmenu list;
        for (const auto& item : category.second) {
            list.addItem(item.id, item.name, item.quantity, item.price);
        }
        menuLists[category.first] = list;
    }
}

void adminUpdate() {
    string category;
    int choice, index, newStock;
    double newPrice;
    string newName;

    do {
        cout << " [burgers, fries, salads, drinks, desserts]\n";
        cout << "\nEnter the name of your chosen category to edit: ";
        cin >> category;

        if (menuLists.find(category) == menuLists.end()) {
            cout << "Category not found.\n";
            continue;
        }
        
        cout << "\n======Admin Update Menu=====" << endl;
        cout << "1. Edit Number of Item Stocks" << endl;
        cout << "2. Edit Price of an Item" << endl;
        cout << "3. Edit Item Name" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                cout << "Enter item index: ";
                cin >> index;
                cout << "Enter new stock amount: ";
                cin >> newStock;
                menuLists[category].editStock(index, newStock, menu, category);
                break;
            case 2:
                cout << "Enter item index: ";
                cin >> index;
                cout << "Enter new price: ";
                cin >> newPrice;
                menuLists[category].editPrice(index, newPrice, menu, category);
                break;
            case 3:
                cout << "Enter item index: ";
                cin >> index;
                cout << "Enter new name: ";
                cin >> newName;
                menuLists[category].editName(index, newName, menu, category);
                break;
            case 4:
                cout << "Returning to main menu.\n";
                main();
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 4);
}


void adminLogin() {
    while (true) {
        string username, password;
        cout << "Enter admin username: ";
        cin >> username;
        cout << "Enter admin password: ";
        cin >> password;

        if (username == "admin" && password == "adminpass") {
            cout << "Admin login successful.\n";
            adminUpdate(); // Go to admin update menu
            break;
        } else {
            cout << "\n*****Invalid admin credentials. Attempt to login denied*****\n" << endl;
            continue;
        }
    }
}


int main () {
    initializeMenu(menu);
    loadUserAccounts();

    while (true)
    {
        try {
            cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "           Welcome to the EZBYTE Food Ordering System!" << endl;
            cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
            cout << "1. Register User" << endl;
            cout << "2. Log in" << endl;
            cout << "3. Admin Log in"  << endl;
            cout << "4. Display Menu" << endl;
            cout << "5. Exit Program\n" << endl;

            int choice;
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Invalid input. Please enter a valid number.");
                continue;
            }
            cout << endl;

            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                userlogin();
            } else if (choice == 3) {
                adminLogin();
            } else if (choice == 4) {
                displayMenu();
            } else if (choice == 5) {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "      Thank you for using this EZBYTE Food Ordering System!" << endl;
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
                exit(1);
            } else {
                cout << "\n***************************" << endl;
                cout << "Please input a valid option" << endl;
                cout << "***************************\n" << endl;
            }
        }
        catch (const invalid_argument& e) {
        cout << "\n******************************************" << endl;
        cout << e.what() << endl;
        cout << "******************************************\n" << endl;
        } 
    }
    return 0;
}

