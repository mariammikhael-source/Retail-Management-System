#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// Forward declarations
class CustomerProfile;
class StoreItem;
class RetailManager;

// Class to manage client accounts
class CustomerProfile {
private:
    string accountName;
    string passKey;
    double walletFunds;
    int rewardTokens;

public:
    CustomerProfile(string acc = "", string pk = "", double funds = 0.0, int tokens = 0) {
        accountName = acc;
        passKey = pk;
        walletFunds = funds;
        rewardTokens = tokens;
    }

    // Data Accessors
    string fetchAccountName() const { return accountName; }
    string fetchPassKey() const { return passKey; }
    double fetchFunds() const { return walletFunds; }
    int fetchTokens() const { return rewardTokens; }

    // Data Modifiers
    void updateFunds(double newAmount) { walletFunds = newAmount; }
    void insertTokens(int amount) { rewardTokens += amount; }

    // File Operations
    void writeToFile(ofstream& outStream) const {
        outStream << accountName << " " << passKey << " " << walletFunds << " " << rewardTokens << endl;
    }

    void readFromFile(ifstream& inStream) {
        inStream >> accountName >> passKey >> walletFunds >> rewardTokens;
    }

    // UI Output
    void showDetails() const {
        cout << "Profile Name: " << accountName << endl;
        cout << "Available Funds: $" << fixed << setprecision(2) << walletFunds << endl;
        cout << "Reward Tokens: " << rewardTokens << endl;
    }
};

// Class to manage items in the store
class StoreItem {
private:
    string itemTitle;
    string itemGroup;
    double itemCost;
    int availableUnits;

public:
    StoreItem(string title = "", string group = "", double cost = 0.0, int units = 0) {
        itemTitle = title;
        itemGroup = group;
        itemCost = cost;
        availableUnits = units;
    }

    // Data Accessors
    string getTitle() const { return itemTitle; }
    string getGroup() const { return itemGroup; }
    double getCost() const { return itemCost; }
    int getUnits() const { return availableUnits; }

    // Data Modifiers
    void updateUnits(int updatedCount) { availableUnits = updatedCount; }

    // UI Output
    void printItemDetails() const {
        cout << left << setw(20) << itemTitle 
             << setw(15) << itemGroup 
             << "$" << setw(10) << fixed << setprecision(2) << itemCost 
             << setw(5) << availableUnits << endl;
    }
};

// Main application class
class RetailManager {
private:
    vector<CustomerProfile> clientDatabase;
    vector<StoreItem> inventory;
    CustomerProfile* activeClient;

    // Populate default inventory
    void setupInventory() {
        // Apparel
        inventory.push_back(StoreItem("T-Shirt", "Apparel", 19.99, 50));
        inventory.push_back(StoreItem("Jeans", "Apparel", 49.99, 30));
        inventory.push_back(StoreItem("Jacket", "Apparel", 79.99, 20));

        // Groceries
        inventory.push_back(StoreItem("Bread", "Groceries", 3.99, 100));
        inventory.push_back(StoreItem("Milk", "Groceries", 2.49, 80));
        inventory.push_back(StoreItem("Cheese", "Groceries", 5.99, 60));

        // Produce
        inventory.push_back(StoreItem("Tomato", "Produce", 1.99, 120));
        inventory.push_back(StoreItem("Potato", "Produce", 0.99, 150));
        inventory.push_back(StoreItem("Carrot", "Produce", 1.49, 100));
    }

    // Retrieve records from disk
    void pullClientData() {
        ifstream fileIn("client_records.txt");
        if (fileIn.is_open()) {
            CustomerProfile tempClient;
            while (fileIn) {
                tempClient.readFromFile(fileIn);
                if (tempClient.fetchAccountName() != "") {
                    clientDatabase.push_back(tempClient);
                }
            }
            fileIn.close();
        }
    }

    // Store records to disk
    void pushClientData() {
        ofstream fileOut("client_records.txt");
        for (size_t i = 0; i < clientDatabase.size(); ++i) {
            clientDatabase[i].writeToFile(fileOut);
        }
        fileOut.close();
    }

    // Verify account presence
    bool isAccountTaken(const string& accName) {
        for (size_t i = 0; i < clientDatabase.size(); ++i) {
            if (clientDatabase[i].fetchAccountName() == accName) {
                return true;
            }
        }
        return false;
    }

    // Locate client pointer
    CustomerProfile* getClientPointer(const string& accName) {
        for (size_t i = 0; i < clientDatabase.size(); ++i) {
            if (clientDatabase[i].fetchAccountName() == accName) {
                return &clientDatabase[i];
            }
        }
        return nullptr;
    }

    // Handle authentication
    bool authenticate() {
        string inputUser, inputPass;
        cout << ">> Account Name: ";
        cin >> inputUser;
        cout << ">> Passkey: ";
        cin >> inputPass;

        CustomerProfile* matchingClient = getClientPointer(inputUser);
        if (matchingClient != nullptr && matchingClient->fetchPassKey() == inputPass) {
            activeClient = matchingClient;
            return true;
        }
        return false;
    }

    // Handle new account creation
    void createAccount() {
        string desiredUser, desiredPass;
        double startingCash;

        cout << ">> Choose an Account Name: ";
        cin >> desiredUser;

        if (isAccountTaken(desiredUser)) {
            cout << "[!] That account name is already taken. Try again.\n";
            return;
        }

        cout << ">> Choose a Passkey: ";
        cin >> desiredPass;

        cout << ">> Deposit initial funds: $";
        while (!(cin >> startingCash) || startingCash < 0) {
            cout << "[!] Invalid entry. Enter a valid positive amount: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        clientDatabase.push_back(CustomerProfile(desiredUser, desiredPass, startingCash, 0));
        pushClientData();
        cout << "[+] Account created successfully!\n";
    }

    // Filter and show items
    void renderItemsByGroup(const string& groupFilter) {
        cout << "\n=== " << groupFilter << " Catalog ===\n";
        cout << left << setw(20) << "Item" << setw(15) << "Type" 
             << setw(10) << "Cost" << setw(5) << "Qty\n";
        cout << "--------------------------------------------------\n";

        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inventory[i].getGroup() == groupFilter) {
                inventory[i].printItemDetails();
            }
        }
    }

    // Show entire catalog
    void renderFullCatalog() {
        cout << "\n=== Complete Store Catalog ===\n";
        cout << left << setw(20) << "Item" << setw(15) << "Type" 
             << setw(10) << "Cost" << setw(5) << "Qty\n";
        cout << "--------------------------------------------------\n";

        for (size_t i = 0; i < inventory.size(); ++i) {
            inventory[i].printItemDetails();
        }
    }

    // Locate inventory item
    StoreItem* searchItem(const string& targetTitle) {
        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inventory[i].getTitle() == targetTitle) {
                return &inventory[i];
            }
        }
        return nullptr;
    }

    // Handle transaction
    void executeTransaction() {
        string targetItem;
        int requestedQty;

        renderFullCatalog();
        cout << "\n>> Type the name of the item you wish to buy: ";
        cin.ignore();
        getline(cin, targetItem);

        StoreItem* itemPtr = searchItem(targetItem);
        if (itemPtr == nullptr) {
            cout << "[!] Item not recognized in our inventory.\n";
            return;
        }

        cout << ">> How many would you like? ";
        while (!(cin >> requestedQty) || requestedQty <= 0) {
            cout << "[!] Please enter a whole positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (requestedQty > itemPtr->getUnits()) {
            cout << "[!] Insufficient stock for this request.\n";
            return;
        }

        double finalBill = itemPtr->getCost() * requestedQty;
        if (finalBill > activeClient->fetchFunds()) {
            cout << "[!] Transaction failed: Insufficient funds.\n";
            return;
        }

        // Apply changes
        activeClient->updateFunds(activeClient->fetchFunds() - finalBill);
        itemPtr->updateUnits(itemPtr->getUnits() - requestedQty);
        
        // Reward token logic ($10 = 1 token)
        int newlyEarnedTokens = static_cast<int>(finalBill / 10.0);
        activeClient->insertTokens(newlyEarnedTokens);

        cout << "\n[+] Transaction Approved!\n";
        cout << "Amount Billed: $" << fixed << setprecision(2) << finalBill << endl;
        cout << "Tokens Acquired: " << newlyEarnedTokens << endl;
        cout << "Funds Remaining: $" << activeClient->fetchFunds() << endl;
        cout << "Token Balance: " << activeClient->fetchTokens() << endl;

        pushClientData(); // Save changes to disk
    }

    // Authenticated user interface
    void clientDashboard() {
        int opt;
        bool sessionActive = true;

        while (sessionActive) {
            cout << "\n--- Customer Dashboard ---\n";
            cout << "1. Browse Full Catalog\n";
            cout << "2. Browse Apparel\n";
            cout << "3. Browse Groceries\n";
            cout << "4. Browse Produce\n";
            cout << "5. Buy an Item\n";
            cout << "6. My Profile\n";
            cout << "7. Sign Out\n";
            cout << ">> Select an option: ";
            cin >> opt;

            switch (opt) {
                case 1: renderFullCatalog(); break;
                case 2: renderItemsByGroup("Apparel"); break;
                case 3: renderItemsByGroup("Groceries"); break;
                case 4: renderItemsByGroup("Produce"); break;
                case 5: executeTransaction(); break;
                case 6: activeClient->showDetails(); break;
                case 7: 
                    cout << "[*] Signing out...\n";
                    sessionActive = false;
                    break;
                default:
                    cout << "[!] Unrecognized option.\n";
            }
        }
    }

public:
    RetailManager() {
        activeClient = nullptr;
        setupInventory();
        pullClientData();
    }

    // Program entry loop
    void start() {
        int opt;
        bool appRunning = true;

        while (appRunning) {
            cout << "\n*** RETAIL MANAGEMENT SYSTEM ***\n";
            cout << "1. Sign In\n";
            cout << "2. Create Account\n";
            cout << "3. Quit\n";
            cout << ">> Select an option: ";
            cin >> opt;

            switch (opt) {
                case 1:
                    if (authenticate()) {
                        cout << "\n[+] Authentication successful. Welcome!\n";
                        clientDashboard();
                    } else {
                        cout << "\n[!] Authentication failed. Check your credentials.\n";
                    }
                    break;
                case 2:
                    createAccount();
                    break;
                case 3:
                    cout << "[*] Shutting down system...\n";
                    appRunning = false;
                    break;
                default:
                    cout << "[!] Unrecognized option.\n";
            }
        }
    }
};

int main() {
    RetailManager app;
    app.start();
    return 0;
}
