#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits> 
#include <vector> 
#include <algorithm> 
#include <stdexcept>
#include <iomanip>   
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
    cin.clear(); 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
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
        {1, "Classic Burger", 25, 55.00},
        {2, "BigByte Burger", 25, 75.00},
        {3, "CheEZyBacon Burger", 25, 90.00},
        {4, "Crispy-Chicken Burger", 25, 90.00},
        {5, "Chili Burger", 25, 80.00}
    }},
    {"fries", {
        {1, "Small Fries", 25, 40.00},
        {2, "Medium Fries", 25, 50.00},
        {3, "Large Fries", 25, 60.00},
        {4, "CheEZy Bacon Fries", 25, 100.00},
        {5, "HashBytes", 25, 65.00}
    }},
    {"salads", {
        {1, "Garden Salad", 25, 50.00},
        {2, "Caesar Salad", 25, 70.00},
        {3, "Vegan's Choice Salad", 25, 85.00},
        {4, "Macaroni Salad", 25, 95.00},
        {5, "Coleslaw Salad", 25, 65.00}
    }},
    {"drinks", {
        {1, "Coke", 25, 30.00},
        {2, "Sprite", 25, 30.00},
        {3, "Iced Tea", 25, 25.00},
        {4, "Pineapple Juice", 25, 25.00},
        {5, "Water", 25, 20.00}
    }},
    {"desserts", {
        {1, "Brownie Bytes", 25, 30.00},
        {2, "CheEZe Sticks", 25, 45.00},
        {3, "Choco Fudge", 25, 25.00},
        {4, "Byte-Sized Smidgets", 25, 15.00},
        {5, "CheEZe Cake", 25, 50.00}
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
        it->quantity += item.quantity;
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
        return true; 
    }
    
    cartContent = "";
    int index = 1;
    for (const auto& item : usercart) 
    {
        cartContent += "\t\t\t\t     " + to_string(index) + ". " + item.name + " --- " + to_string(item.quantity) + "x --- P" + to_string(item.price) + "\n";
        index++;
    }
    return false; 
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
        cout << "Choose the number of category:" << endl;
        cout << "1. Burgers" << endl;
        cout << "2. Fries" << endl;
        cout << "3. Salads" << endl;
        cout << "4. Drinks" << endl;
        cout << "5. Desserts and Snacks" << endl;
        cout << "6. Go back to Main Menu" << endl;
        try {
            cout << "\nEnter the number of your choice: ";
            cin >> choice;
            cout << endl;
            if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

            switch (choice) {
                case 1:
                    setTextColor(6);
                    cout << "\n\t\t\t\t\t===========================================" << endl;
                    cout << "\t\t\t\t\t                Burgers Menu:" << endl;
                    for (const auto& item : menu["burgers"]) {
                        cout << "\t\t\t\t              "<<item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    cout << "\t\t\t\t\t===========================================" << endl;
                    resetTextColor();  
                    break;
                case 2:
                    setTextColor(6);
                    cout << "\n\t\t\t\t\t===========================================" << endl;
                    cout << "\t\t\t\t\t                Fries Menu:" << endl;
                    for (const auto& item : menu["fries"]) {
                        cout << "\t\t\t\t             "<<item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    cout << "\t\t\t\t\t===========================================" << endl;
                    resetTextColor();  
                    break;
                case 3:
                    setTextColor(6);
                    cout << "\n\t\t\t\t\t===========================================" << endl;
                    cout << "\t\t\t\t\t                Salads Menu:" << endl;
                    for (const auto& item : menu["salads"]) {
                        cout << "\t\t\t\t             "<<item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    cout << "\t\t\t\t\t===========================================" << endl;
                    resetTextColor();  
                    break;
                case 4:
                    setTextColor(6);
                    cout << "\n\t\t\t\t\t===========================================" << endl;
                    cout << "\t\t\t\t\t                Drinks Menu:" << endl;
                    for (const auto& item : menu["drinks"]) {
                        cout << "\t\t\t\t\t           "<<item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    cout << "\t\t\t\t\t===========================================" << endl;
                    resetTextColor();  
                    break;
                case 5:
                    setTextColor(6);
                    cout << "\n\t\t\t\t\t===========================================" << endl;
                    cout << "\t\t\t\t\t         Desserts and snacks Menu:" << endl;
                    for (const auto& item : menu["desserts"]) {
                        cout << "\t\t\t\t              "<< item.id << ". " << item.name << " - $" << item.price << endl;
                    }
                    cout << "\t\t\t\t\t===========================================" << endl;
                    resetTextColor();                   
                    break;
                case 6:
                    break;
                default:
                    setTextColor(12);
                    cout << "\n\t\t\t\t  ==============================================" << endl;
                    cout << "\t\t\t\t            Please input a valid choice" << endl;
                    cout << "\t\t\t\t  ==============================================\n" << endl;
                    resetTextColor();
                    break;
            }
        } catch (const invalid_argument& e) {
            setTextColor(12);
            cout << "\n\t\t\t\t  ==============================================" << endl;
            cout << "\t\t\t\t            "<< e.what() << endl;
            cout << "\t\t\t\t  ==============================================\n" << endl;
            resetTextColor();
        }
    } while (choice != 6);
    main();
}

void registerUser() {
        setTextColor(6);
        cout << "\n\t\t\t\t-----------REGISTER NEW USER-----------\n" << endl;
        resetTextColor();
    while (true) {
        string username;
        string password;
        string phoneNumber;
        string address;
        float balance = 0.0;

        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password (must be 8 characters long): ";
        cin >> password;

        if (username.length() < 4) {
            setTextColor(12);
            cout << "\n\t\t\t\t  ===========================================" << endl;
            cout << "\t\t\t\t  Username must be at least 4 characters long" << endl;
            cout << "\t\t\t\t  ===========================================" << endl;
            resetTextColor();
            break;
        }

        while (password.length() < 8) {
            setTextColor(12);
            cout << "\n\t\t\t\t    ===========================================" << endl;
            cout << "\t\t\t\t    Password must be at least 8 characters long" << endl;
            cout << "\t\t\t\t    ===========================================\n" << endl;
            resetTextColor();
            cout << "Password (must be 8 characters long): ";
            cin >> password;
        }

        if (user_accounts.find(username) != user_accounts.end()) {
            setTextColor(12);
            cout << "\n\t\t\t======================================================\n";
            cout << "\t\t\tUsername already exists. Please choose a different one\n";
            cout << "\t\t\t======================================================\n\n";
            resetTextColor();
            break;
        }

        cout << "Enter your phone number (must be 11 digits): ";
        cin >> phoneNumber;
        while (phoneNumber.length() != 11 || !all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit)) {
            setTextColor(12);
            cout << "\n\t\t\t\t  ============================================" << endl;
            cout << "\t\t\t\t   Phone number must be exactly 11 digits long" << endl;
            cout << "\t\t\t\t   ============================================\n" << endl;
            resetTextColor();
            cout << "Enter your phone number (must be 11 digits): ";
            cin >> phoneNumber;
        }

        cout << "Enter your address: ";
        cin.ignore();
        getline(cin, address);

        setTextColor(2);
        cout << "\n\t\t\t\t\t-------------------------------" << endl;
        cout << "\t\t\t\t\tAccount registered successfully" << endl;
        cout << "\t\t\t\t\t-------------------------------\n" << endl;
        resetTextColor();

        UserDetails newUser = {password, phoneNumber, address, 0.0, Cart()};
        user_accounts[username] = newUser;
        saveUserAccounts();
        
        break;
        main();
    }
}

void userlogin() {
    while (true) {
        setTextColor(6);
        cout << "\n\t\t\t\t       -----------USER LOGIN-----------\n" << endl;
        resetTextColor();
        string username;
        string password;

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (user_accounts.find(username) == user_accounts.end()) {
            setTextColor(12);
            cout << "\n\t\t\t\t  ===========================================" << endl;
            cout << "\t\t\t\t   Username does not exist. Please try again" << endl;
            cout << "\t\t\t\t  ===========================================\n" << endl;
            resetTextColor();
            break;
        }

        if (user_accounts[username].password != password) {
            setTextColor(12);
            cout << "\n\t\t\t\t  ===========================================" << endl;
            cout << "\t\t\t\t      Incorrect password. Please try again" << endl;
            cout << "\t\t\t\t   ==========================================\n" << endl;
            resetTextColor();
            break;
        }
        
        cout << "Login succesful!" <<endl;
        user_menu(username);
        break;
    }
    main();
}


void user_menu(const string& username) {
    int choice;
    while (choice != 6) {
        setTextColor(6);
        cout << "\n===================================================" << endl;
        cout << "       Logged in as " << username << " -- Balance: P " << user_accounts[username].balance << endl;
        cout << "===================================================\n" << endl;
        resetTextColor();
        cout << "1. Add to cart" << endl;
        cout << "2. Edit your cart" << endl;
        cout << "3. Top-up Ezbyte Wallet" << endl;
        cout << "4. View Cart and Proceed to Payment" << endl;
        cout << "5. Check Credentials" << endl;
        cout << "6. Log out" << endl << endl;

        try {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
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
                setTextColor(2);
                cout << "\n\t\t\t\t\t-----------------------------------" << endl;
                cout << "\t\t\t\t\t    User: '" << username << "' has logged out!" << endl;
                cout << "\t\t\t\t\t-----------------------------------\n" << endl;
                resetTextColor();
                main();
                break;
            default:
                setTextColor(12);
                cout << "\n\t\t\t\t  ==============================================" << endl;
                cout << "\t\t\t\t            Please input a valid choice" << endl;
                cout << "\t\t\t\t  ==============================================\n" << endl;
                resetTextColor();
        }
        } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
        }
    }
    main();
}

void addUserCart (const string& username) {
    int categoryIndex = 1;
    map<int, string> categoryMap;
    cout << endl;
    for (const auto& category : menu) {
        cout << categoryIndex << ". " << category.first << endl;
        categoryMap[categoryIndex] = category.first;
        categoryIndex++;
    }

    try {
    cout << "\nPlease select a category: ";
    int ctgChoice;
    cin >> ctgChoice;
    if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

    if (categoryMap.find(ctgChoice) == categoryMap.end()) {
        setTextColor(12);
        cout << "\n\t\t\t\t    ========================================" << endl;
        cout << "\t\t\t\t            Invalid category selected" << endl;
        cout << "\t\t\t\t    ========================================\n" << endl;
        resetTextColor();
        return;
    }

    string selectedCategory = categoryMap[ctgChoice];
    vector<CartItem>& selectedCategoryitems = menu[selectedCategory];

    
    cout << endl;
    setTextColor(12);
    cout << "\n\t\t\t\t    ========================================" << endl;
    for (const auto& item : selectedCategoryitems) {
        cout << "\t\t\t\t    " << item.id << ". " << item.name << " - P" << item.price << endl;
    }
    cout << "\t\t\t\t    ========================================" << endl;
    resetTextColor();
    cout << "\nPlease select an item: ";
    int itemId;
    cin >> itemId;
    if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

    auto it = find_if(selectedCategoryitems.begin(), selectedCategoryitems.end(), [itemId](const CartItem& item) {return item.id == itemId; });

    if (it == selectedCategoryitems.end()){
        setTextColor(12);
        cout << "\n\t\t\t\t    ========================================" << endl;
        cout << "\t\t\t\t              Invalid item selected" << endl;
        cout << "\t\t\t\t    ========================================\n" << endl;
        resetTextColor();
        return;
    }

    if (it->quantity == 0) {
        setTextColor(12);
        cout << "\n\t\t\t\t    ==============================================" << endl;
        cout << "\t\t\t\t      Item is out of stock and cannot be ordered" << endl;
        cout << "\t\t\t\t    ==============================================\n" << endl;
        resetTextColor();
        return;
    }

    int quantity;
    do { 
        cout << "Enter the quantity of the item you want to order: ";
        cin >> quantity;
        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }
        if (quantity <= 0 || quantity > it->quantity) {
            setTextColor(12);
            cout << "\n\t\t\t\t=======================================================" << endl;
            cout << "\t\t\t\t Invalid quantity entered! " << it->quantity << " stocks left for " << it->name << "!" << endl;
            cout << "\t\t\t\t=======================================================\n" << endl;
            resetTextColor();

        }
    } while (quantity <= 0 || quantity > it->quantity);

    CartItem selectedItem = *it;
    selectedItem.quantity = quantity;

    user_accounts[username].cart.addToCart(selectedItem);
    it->quantity -= quantity; 
    setTextColor(2);
    cout << "\n\t\t\t\t\t-----------------------------------" << endl;
    cout << "\t\t\t\t\t  Item added to cart successfully!" << endl;
    cout << "\t\t\t\t\t-----------------------------------\n" << endl;
    resetTextColor();
    return;
    } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
        }
}

void editUserCart(const string& username) {
    string cartContent;
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        cout << cartContent << endl;  
        return; 
    } else {
        setTextColor(6);
        cout << "\n\t\t\t\t   ------------------- Your Cart -------------------" << endl;
        cout << cartContent;
        cout << "\t\t\t\t   -------------------------------------------------" << endl;
        resetTextColor();
    }
    try {
    int itemId;
    cout << "\nEnter the index of the item you want to edit: ";
    cin >> itemId;
    if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

    if (itemId < 1 || itemId > user_accounts[username].cart.getUserCart().size()) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t    Invalid item index. Please enter a valid index" << endl;
        cout << "\t\t\t\t    ==============================================\n" << endl;
        resetTextColor();
        return;
    }
    
    cout << "\nDo you want to: "<< endl;
    cout << "1. Edit item quantity" << endl;
    cout << "2. Delete item from your cart" << endl;
    cout << "3. Go back to user menu\n" << endl;
    int choice;
    cin >> choice;
    cout << endl;
    if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }
    
    if (choice == 1) {
        int newQuantity;
        cout << "Enter the new quantity: ";
        cin >> newQuantity;
        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

        if (newQuantity <= 0) {
            setTextColor(12);
            cout << "\n\t\t\t  =======================================================" << endl;
            cout << "\t\t\t  Invalid quantity entered. Please enter a positive value" << endl;
            cout << "\t\t\t  =======================================================\n" << endl;
            resetTextColor();
            return;
        }

        user_accounts[username].cart.editCart(itemId - 1, newQuantity);
        setTextColor(2);
        cout << "\n\t\t\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t\t\tItem quantity updated successfully!" << endl;
        cout << "\t\t\t\t\t-----------------------------------\n" << endl;
        resetTextColor();

    } else if (choice == 2) {
        auto& userCart = user_accounts[username].cart.getUserCart();
        userCart.erase(userCart.begin() + (itemId - 1));
        setTextColor(2);
        cout << "\n\t\t\t\t\t------------------------------------" << endl;
        cout << "\t\t\t\t\tItem removed from cart successfully!" << endl;
        cout << "\t\t\t\t\t------------------------------------\n" << endl;
        resetTextColor();
    } else if (choice == 3) {
        return;
    } else {
        setTextColor(12);
        cout << "\n\t\t\t\t =================================================" << endl;
        cout << "\t\t\t\t Invalid choice. Please enter a number from 1 to 3" << endl;
        cout << "\t\t\t\t =================================================\n" << endl;
        resetTextColor();
        return;
    }
    } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
        }
}

void proceedOrder(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change) {
    string cartContent;
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ===============================================================" << endl;
        cout << "\t\t\t\t  Your cart is empty. Please add items to proceed with your order" << endl;
        cout << "\t\t\t\t  ===============================================================\n" << endl;
        resetTextColor();
        return;
    }

    const vector<CartItem>& userCart = user_accounts[username].cart.getUserCart();
    for (const auto& item : userCart) {
        totalCost += item.price * item.quantity;
    }
    setTextColor(6);
    cout << "\n\t\t\t\t     ------------------- Your Cart -------------------" << endl;
    cout << cartContent;
    cout << "\n\t\t\t\t            Total cost of your order: " << totalCost << " pesos" << endl;
    cout << "\t\t\t\t     -------------------------------------------------" << endl;
    resetTextColor();

    
    string confirm;
    cout << "\nProceed with this order? Type 'yes' to proceed, any character to cancel: ";
    cin >> confirm;

    if (confirm != "yes") {
        setTextColor(12);
        cout << "\n\t\t\t\t    =================================================" << endl;
        cout << "\t\t\t\t          Order canceled. Returning to user menu" << endl;
        cout << "\t\t\t\t    =================================================\n" << endl;
        resetTextColor();
        totalCost = 0.0;
        return;
    }
    try {
    cout << "\nSelect payment method:" << endl;
    cout << "1. Ezbyte Wallet (Current balance: " << user_accounts[username].balance << " pesos)" << endl;
    cout << "2. Cash-on-Delivery" << endl;
    cout << "\nEnter your choice: ";
    cin >> paymentChoice;
    if (cin.fail()) {
        clearInputBuffer();
        throw invalid_argument("Please Input a valid number!");
    }


    if (paymentChoice == 1) {
        if (user_accounts[username].balance < totalCost) {
            setTextColor(12);
            cout << "\n\t\t\t\t    ======================================================" << endl;
            cout << "\t\t\t\t    Insufficient balance. Please top-up your Ezbyte Wallet" << endl;
            cout << "\t\t\t\t    ======================================================\n" << endl;
            resetTextColor();
            totalCost = 0.0;
            return;
        }
        user_accounts[username].balance -= totalCost;
        cashAmount = totalCost;
        change = 0.0;
        cout << "\nPayment successful via Ezbyte Wallet!\n";

    } else if (paymentChoice == 2) {
        do {
            cout << "Enter cash amount: ";
            cin >> cashAmount;
            if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
            }

            if (cashAmount < totalCost) {
                setTextColor(12);
                cout << "\n\t\t\t\t    ======================================================" << endl;
                cout << "\t\t\t\t      Insufficient amount. Please enter a valid amount" << endl;
                cout << "\t\t\t\t    ======================================================\n" << endl;
                resetTextColor();
                totalCost = 0.0;
                return;
            }
        } while (cashAmount < totalCost);

        change = cashAmount - totalCost;
        cout << "\nPayment successful via Cash-on-Delivery. Your change: " << change << " pesos" << endl;

    } else {
        setTextColor(12);
        cout << "\n\t\t\t\t    =================================================" << endl;
        cout << "\t\t\t\t              Invalid payment method selected" << endl;
        cout << "\t\t\t\t    =================================================\n" << endl;
        resetTextColor();
        totalCost = 0.0;
        return;
    }

    displayOrderSummary(username, paymentChoice, totalCost, cashAmount, change);
    saveUserAccounts();
    } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
        }
    
}

void displayOrderSummary(const string& username, int& paymentChoice, float& totalCost, float& cashAmount, float& change) {
    string cartContent;
    const vector<CartItem>& userCart = user_accounts[username].cart.getUserCart();
    bool cartEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (cartEmpty) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ===============================================================" << endl;
        cout << "\t\t\t\t  Your cart is empty. Please add items to proceed with your order" << endl;
        cout << "\t\t\t\t  ===============================================================\n" << endl;
        resetTextColor();
        return;
    }
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
    cout << cartContent;
    cout << "                                                                                                          \n";
    cout << "                                                                                                          \n";
    cout << fixed << setprecision(2);
    cout << "                                                Total cost: " << totalCost << " Pesos\n";
    setTextColor(6);
    cout << "               ===========================================================================================\n\n\n";
    resetTextColor();
    user_accounts[username].cart.clearCart();
}

void top_up (const string& username) {
    try {
        setTextColor(6);
        cout << "\n\t\t\t\t=========================================" << endl;
        cout << "\t\t\t\t          Top-up Ezbyte Wallet" << endl;
        cout << "\t\t\t\t=========================================\n" << endl;
        resetTextColor();
        int amount;
        cout << "Enter the amount to top up: ";
        cin>> amount;

        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Invalid input. Please enter a valid amount.");
            }

        if (amount > 0){
            user_accounts[username].balance += amount; 
            saveUserAccounts();
            setTextColor(2);
            cout << fixed << setprecision(2);
            cout << "\n\t\t\t\t------------------------------------------------------" << endl;
            cout << "\t\t\t\t      Top-up successful. New balance: P " << user_accounts[username].balance << endl;
            cout << "\t\t\t\t------------------------------------------------------\n" << endl;
            resetTextColor();
        } else {
            setTextColor(12);
            cout << "\n\t\t\t\t  =============================================" << endl;
            cout << "\t\t\t\t  Invalid amount. Please enter a positive value" << endl;
            cout << "\t\t\t\t  =============================================\n" << endl;
            resetTextColor();
        }
    } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
    } 
}

void check_credentials (const string& username) {
    setTextColor(2);
    cout << "\n\t\t\t\t------------------- User Information -------------------" << endl;
    cout << "\t\t\t\tUsername: " << username << endl;
    cout << "\t\t\t\tPassword: " << user_accounts[username].password << endl;

    string cartContent;
    bool isEmpty = user_accounts[username].cart.viewCart(cartContent);

    if (!isEmpty) {
        cout << cartContent << endl;
    } else {
        cout << "\t\t\t\tUser's Cart is empty!" << endl;
    }

    cout << "\t\t\t\tBalance: " << fixed << setprecision(2) << user_accounts[username].balance << endl;
    cout << "\t\t\t\t-------------------------------------------------------" << endl;
    resetTextColor();

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

    void addItem(int index, const string& name, int stock, float price) {  
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
                        setTextColor(2);
                        cout << "\n\t\t\t\t------------------------------------------------------" << endl;
                        cout << "\t\t\t\t            Stocks for " << item.name << " updated to " << newStock << endl;
                        cout << "\t\t\t\t------------------------------------------------------\n" << endl;
                        resetTextColor();
                        return;
                    }
                }
            }
            temp = temp->next;
        }
        cout << "Item not found." << endl;
    }

    void editPrice(int index, float newPrice, map<string, vector<CartItem>>& menu, const string& category) { 
        itemNode* temp = head;
        while (temp) {
            if (temp->index == index) {
                temp->price = newPrice;
                for (auto& item : menu[category]) {
                    if (item.id == index) {
                        item.price = newPrice;
                        setTextColor(2);
                        cout << "\n\t\t\t\t------------------------------------------------------" << endl;
                        cout << "\t\t\t\t          Price for " << item.name << " updated to " << newPrice << " pesos" << endl;
                        cout << "\t\t\t\t------------------------------------------------------\n" << endl;
                        resetTextColor();
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
                        setTextColor(2);
                        cout << "\n\t\t\t\t------------------------------------------------------" << endl;
                        cout << "\t\t\t\t          Name for " << item.name << " updated to " << newName << "." << endl;
                        cout << "\t\t\t\t------------------------------------------------------\n" << endl;
                        resetTextColor();
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
    float newPrice;
    string newName;

    do {
        setTextColor(6); // Yellow color for prompts
        cout << "[burgers, fries, salads, drinks, desserts]\n";
        cout << "Enter the name of your chosen category (type 'exit' to go back): ";
        cin >> category;
        resetTextColor();

        if (category == "exit"){
            main();
            return;
        }


        if (menuLists.find(category) == menuLists.end()) {
            setTextColor(12); // Red color for error
            cout << "\n\t\t\t\t     ====================================" << endl;
            cout << "\t\t\t\t              Category not found" << endl;
            cout << "\t\t\t\t     ====================================\n" << endl;
            resetTextColor();
            continue;
        }
        try {
            setTextColor(6); 
            cout << "\n\t\t\t\t       ======Admin Update Menu=====" << endl;
            resetTextColor();
            cout << "1. Edit Number of Item Stocks" << endl;
            cout << "2. Edit Price of an Item" << endl;
            cout << "3. Edit Item Name" << endl;
            cout << "4. Back to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cout << endl;
            if (cin.fail()) {
                    clearInputBuffer();
                    throw invalid_argument(" Invalid input. Please enter a valid number");
                }
            
            switch(choice) {
                case 1:
                    for (const auto& item : menu[category]){
                        setTextColor(6);
                        cout << item.id << ". " << item.name << "  ---  " << item.price << "  ---  (" << item.quantity << "  stocks)" << endl;
                        resetTextColor();
                    }
                    cout << "\nEnter item index: ";
                    cin >> index;
                    if (cin.fail()) {
                        clearInputBuffer();
                        throw invalid_argument(" Invalid input. Please enter a valid index");
                    }
                    cout << "Enter new stock amount: ";
                    cin >> newStock;
                    if (cin.fail()) {
                        clearInputBuffer();
                        throw invalid_argument(" Invalid input. Please enter a valid amount");
                    }
                    menuLists[category].editStock(index, newStock, menu, category);
                    break;
                case 2:
                    for (const auto& item : menu[category]){
                        setTextColor(6);
                        cout << item.id << ". " << item.name << "  ---  " << item.price << "  ---  (" << item.quantity << "  stocks)" << endl;
                        resetTextColor();                    
                    }
                    cout << "\nEnter item index: ";
                    cin >> index;
                    if (cin.fail()) {
                        clearInputBuffer();
                        throw invalid_argument(" Invalid input. Please enter a valid index");
                    }
                    cout << "Enter new price: ";
                    cin >> newPrice;
                    if (cin.fail()) {
                        clearInputBuffer();
                        throw invalid_argument(" Invalid input. Please enter a valid amount");
                    }
                    menuLists[category].editPrice(index, newPrice, menu, category);
                    break;
                case 3:
                    for (const auto& item : menu[category]){
                        setTextColor(6);
                        cout << item.id << ". " << item.name << "  ---  " << item.price << "  ---  (" << item.quantity << "  stocks)" << endl;
                        resetTextColor();
                    }
                    cout << "\nEnter item index: ";
                    cin >> index;
                    if (cin.fail()) {
                        clearInputBuffer();
                        throw invalid_argument(" Invalid input. Please enter a valid index");
                    }
                    cout << "Enter new name: ";
                    cin >> newName;
                    menuLists[category].editName(index, newName, menu, category);
                    break;
                case 4:
                    main();
                    break;
                default:
                    setTextColor(12);
                    cout << "\n\t\t\t\t  ==============================================" << endl;
                    cout << "\t\t\t\t            Please input a valid choice" << endl;
                    cout << "\t\t\t\t  ==============================================\n" << endl;
                    resetTextColor();
            }   
        } catch (const invalid_argument& e) {
            setTextColor(12); 
            cout << "\n\t\t\t\t============================================" << endl;
            cout << "\t\t\t\t" << e.what() << endl;
            cout << "\t\t\t\t============================================\n" << endl;
            resetTextColor();
        }
    } while (choice != 4);
}

void adminLogin() {
    while (true) {
        setTextColor(6);
        cout << "\n\t\t\t\t       -----------ADMIN LOGIN-----------\n" << endl;
        resetTextColor();
        string username, password;
        cout << "Enter admin username: ";
        cin >> username;
        cout << "Enter admin password: ";
        cin >> password;

        if (username == "admin" && password == "adminpass") {
            cout << "Admin login successful!\n\n";
            adminUpdate();
            break;
        } else {
            setTextColor(12); 
            cout << "\n\t\t\t\t==============================================" << endl;
            cout << "\t\t\t\t  Invalid admin credentials. Attempt to login denied" << endl;
            cout << "\t\t\t\t==============================================\n" << endl;
            resetTextColor();
            continue;
        }
    }
}


int main () {
    initializeMenu(menu);
    loadUserAccounts();

    while (true)
    {
        setTextColor(6);
        cout << "\n\t\t\t==============================================================" << endl;
        cout << "\t\t\t           Welcome to the EZBYTE Food Ordering System!" << endl;
        cout << "\t\t\t==============================================================\n" << endl;
        resetTextColor();
        cout << "1. Register User" << endl;
        cout << "2. Log in Ezbyte Account" << endl;
        cout << "3. Admin Log in"  << endl;
        cout << "4. Display Menu" << endl;
        cout << "5. Exit Ezbyte\n" << endl;
        try {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
                clearInputBuffer();
                throw invalid_argument("Please Input a valid number!");
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
            setTextColor(6);
            cout << "               ===========================================================================================" << endl;
            cout << "               ||                                                                                       ||" << endl;
            cout << "               ||                   _____ _                 _     __   __          _                    ||" << endl;
            cout << "               ||                  |_   _| |__   __ _ _ __ | | __ \\ \\ / /__  _   _| |                   ||" << endl;
            cout << "               ||                    | | | '_ \\ / _` | '_ \\| |/ /  \\ V / _ \\| | | | |                   ||" << endl;
            cout << "               ||                    | | | | | | (_| | | | |   <    | | (_) | |_| |_|                   ||" << endl;
            cout << "               ||                    |_| |_| |_|\\__,_|_| |_|_|\\_\\   |_|\\___/ \\__,_(_)                   ||" << endl;
            cout << "               ||                                                                                       ||" << endl;
            cout << "               ||                       for using the EZByte Food Ordering System!                      ||" << endl;
            cout << "               ||                 -----------------------------------------------------                 ||" << endl;
            cout << "               ||                                                                                       ||" << endl;
            cout << "               ||                                                                                       ||" << endl;
            cout << "               ===========================================================================================" << endl;
            resetTextColor();
            exit(1);
        } else {
            setTextColor(12);
            cout << "\n\t\t\t\t  ==============================================" << endl;
            cout << "\t\t\t\t            Please input a valid option"<< endl;
            cout << "\t\t\t\t  ==============================================\n" << endl;
            resetTextColor();
            }
        } catch (const invalid_argument& e) {
        setTextColor(12);
        cout << "\n\t\t\t\t  ==============================================" << endl;
        cout << "\t\t\t\t            "<< e.what() << endl;
        cout << "\t\t\t\t  ==============================================\n" << endl;
        resetTextColor();
        } 
    }
    return 0;
}

