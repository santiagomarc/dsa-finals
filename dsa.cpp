#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits> 
#include <vector> //data structure para sa menu, cart
#include <algorithm> // all_of

using namespace std;

void userlogin();
void user_menu(const string& username);
void addUserCart(const string& username);
int main();

void clearInputBuffer() {
    cin.clear(); // Clear the error flag
    while (cin.get() != '\n'); // Ignore remaining input
}

struct CartItem {
    int id;
    string name;
    int quantity;
    double price;
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

void displayMenu() {
    cout << "Choose a category:" << endl;
    cout << "1. Burgers" << endl;
    cout << "2. Fries" << endl;
    cout << "3. Salads" << endl;
    cout << "4. Drinks" << endl;
    cout << "5. Desserts" << endl;

    int choice;
    cout << "Enter your choice (1-5): ";
    cin >> choice;

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
        default:
            cout << "Invalid choice. Please choose a valid category." << endl;
            break;
    }
}


class Cart {
private:
    vector<CartItem> usercart;

public:
    void addToCart(const CartItem& item) {
        usercart.push_back(item);
    }

    void deleteFromCart(int id) {
        auto it = remove_if(usercart.begin(), usercart.end(), [id](const CartItem& item) { return item.id == id; });
        usercart.erase(it, usercart.end());
    }

    void editCart(int id, int newQuantity) {
        for (auto& item : usercart) {
            if (item.id == id) {
                item.quantity = newQuantity;
                break;
            }
        }
    }

    void viewCart() const {
        for (const auto& item : usercart) {
            cout << item.id << ". " << item.name << " - " << item.quantity << " x $" << item.price << endl;
        }
    }
};

struct UserDetails {
    string password;
    string phoneNumber;
    string address;
    double balance = 0;
    Cart cart;
};

map <string, UserDetails> user_accounts; 

void registerUser() {
    while (true) {
        cout << "\n-----------REGISTER NEW USER-----------" << endl;
        cout << "Please input your username and password: \n" << endl;
        string username;
        string password;
        string phoneNumber;
        string address;

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
        break;
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
            continue;
        }

        if (user_accounts[username].password != password) {
            cout << "\n*************************************" << endl;
            cout << "Incorrect password. Please try again." << endl;
            cout << "*************************************" << endl;
            continue;
        }

        cout << "\n-------------------------------" << endl;
        cout << "Login successful!" << endl;
        cout << "-------------------------------\n" << endl;
        user_menu(username);
        break;
    }
}


void user_menu(const string& username) {
    while (true) {
        try {
            cout << "Logged in as " << username << " ---- Balance: $" << user_accounts[username].balance << endl;
            cout << "1. Add to cart" << endl;
            cout << "2. Edit cart" << endl;
            cout << "3. Top-up Account" << endl;
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
                    //user_accounts[username].cart.addToCart(item);
                    break;
                case 3:
                    //top_up(username);
                    break;
                case 4:
                    //user_accounts[username].cart.addToCart(item);
                    break;
                case 5:
                    //check_credentials(username);
                    break;
                case 6:
                    cout << "\n----------------------------------" << endl;
                    cout << "  User: '" << username << "' has logged out!" << endl;
                    cout << "----------------------------------\n" << endl;
                    main();
                    return;
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
    const vector<CartItem>& selectedCategoryitems = menu[selectedCategory];

    cout << "---SELECT AN ITEM---\n";
    for (const auto& item : selectedCategoryitems) {
        cout << item.id << ". " << item.name << " - $" << item.price << " (" << item.quantity << " in stock)\n";

    }
    int itemId;
    cin >> itemId;

    auto it = find_if(selectedCategoryitems.begin(), selectedCategoryitems.end(), [itemId](const CartItem& item) {return item.id == itemId; });

    if (it == selectedCategoryitems.end()){
        cout << "Invalid item selected.\n";
        return;
    }

    int quantity;
    cout << "Enter the quantity of the item you want to order:";
    cin >> quantity;

    if (quantity <= 0 || quantity > it->quantity) {
        cout << "Invalid quantity entered.\n";
        return;
    }

    CartItem selectedItem = *it;
    selectedItem.quantity = quantity;

    user_accounts[username].cart.addToCart(selectedItem);
    cout << "Item added to cart successfully.\n";
}

void deleteUserCart (const string& username) {
    user_accounts[username].cart.viewCart();

    // for (const auto& item : user_accounts[username].cart.usercart ) {

    // }


}
void top_up (const string& username) {

}

void check_credentials (const string& username) {

}
/// @brief 
void admin_login();
void admin_menu();

void admin_login() 
{
    string username;
    string password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "adminpass") 
    {
        cout << "\nADMIN LOGIN SUCCESSFUL! \n" << endl;
        admin_menu();
    } 
    else 
    {
        cout << "Please put the correct information" << endl;
        admin_login();
    }
}

void admin_update();
void display_category(const string& category);
void edit_item_stock(const string& category);
void edit_item_price(const string& category);
void edit_item_name(const string& category);

void admin_menu() 
{
    string choice;

    cout << "Admin Menu:" << endl;
    cout << "1. Update menu" << endl;
    cout << "2. Logout" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    while (true) {
        if (choice == "1") 
        {
            admin_update();
        } 
        else if (choice == "2") 
        {
            cout << "Logging Out" << endl;
            displayMenu();
            break; // Exit the loop after logging out
        } 
        else 
        {
            cout << "Invalid choice. Please enter 1 or 2: ";
            cin >> choice;
        }
    }
}

void admin_update() 
{
    string choice;

    cout << "Admin Update:" << endl;
    cout << "1. Edit number of item stocks" << endl;
    cout << "2. Edit price of an item" << endl;
    cout << "3. Edit item name" << endl;
    cout << "4. Back to main menu" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    while (true) 
    {
        if (choice == "1") 
        {
            string category;
            cout << "Select category to edit stocks:" << endl;
            cout << "burgers\nfries\nsalads\ndrinks\ndesserts\nExit" << endl;
            cin >> category;
            if (category == "Exit") 
            {
                admin_update();
            } 
            else 
            {
                if (menu.find(category) != menu.end()) 
                {
                    edit_item_stock(category);
                } 
                else 
                {
                    cout << "Invalid category. Returning to admin update." << endl;
                    admin_update();
                }
            }
        } 
        else if (choice == "2") 
        {
            string category;
            cout << "Select category to edit price:" << endl;
            cout << "burgers\nfries\nsalads\ndrinks\ndesserts\nExit" << endl;
            cin >> category;
            if (category == "Exit") 
            {
                admin_update();
            } 
            else 
            {
                if (menu.find(category) != menu.end()) 
                {
                    edit_item_price(category);
                } 
                else 
                {
                    cout << "Invalid category. Returning to admin update." << endl;
                    admin_update();
                }
            }
        } 
        else if (choice == "3") 
        {
            string category;
            cout << "Select category to edit name:" << endl;
            cout << "burgers\nfries\nsalads\ndrinks\ndesserts\nExit" << endl;
            cin >> category;
            if (category == "Exit") 
            {
                admin_update();
            } 
            else 
            {
                if (menu.find(category) != menu.end()) {
                    edit_item_name(category);
                } else {
                    cout << "Invalid category. Returning to admin update." << endl;
                    admin_update();
                }
            }
        } 
        else if (choice == "4") 
        {
            admin_menu();
            break; // Exit the loop after logging out
        } 
        else 
        {
            cout << "Invalid choice. Please enter a number between 1 and 4: ";
            cin >> choice;
        }
    }
}

void display_category(const string& category) {
    cout << "=========================================" << endl;
    cout << "   Item       STOCKS    PRICE" << endl;
    for (const auto& item : menu[category]) 
    {
        cout << item.id << ". " << item.name << "      " << item.quantity << "      $" << item.price << endl;
    }
    cout << "=========================================" << endl;
}

void edit_item_stock(const string& category) 
{
    display_category(category);

    int item_id, new_quantity;
    cout << "Choose item number to edit stocks: ";
    cin >> item_id;

    for (auto& item : menu[category]) 
    {
        if (item.id == item_id) 
        {
            cout << "Enter the new stock quantity: ";
            cin >> new_quantity;
            item.quantity = new_quantity;
            cout << "Item stock updated successfully." << endl;
            admin_update();
            return;
        }
        else
        {
            cout << "Item not found." << endl;
        }
    }
    cout << "Item not found." << endl;
    admin_update();
}

void edit_item_price(const string& category) {
    display_category(category);

    int item_id;
    double new_price;
    cout << "Choose item number to edit price: ";
    cin >> item_id;

    for (auto& item : menu[category]) 
    {
        if (item.id == item_id) 
        {
            cout << "Enter the new price: ";
            cin >> new_price;
            item.price = new_price;
            cout << "Item price updated successfully." << endl;
            admin_update();
            return;
        }
    }
    cout << "Item not found." << endl;
    admin_update();
}

void edit_item_name(const string& category) 
{
    display_category(category);

    int item_id;
    string new_name;
    cout << "Choose item number to edit name: ";
    cin >> item_id;

    for (auto& item : menu[category]) 
    {
        if (item.id == item_id) {
            cout << "Enter the new name: ";
            cin.ignore(); // To ignore the newline character left in the buffer
            getline(cin, new_name);
            item.name = new_name;
            cout << "Item name updated successfully." << endl;
            admin_update();
            return;
        }
    }

    cout << "Item not found." << endl;
    admin_update();
}



int main () {
    admin_login();
    return 0;
}