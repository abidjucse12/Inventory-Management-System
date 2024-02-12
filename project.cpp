
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Review
{
public:
    Review(string customerName, string comment, int rating)
        : customerName(customerName), comment(comment), rating(rating) {}

    void display() const
    {
        cout << "Customer: " << customerName << ", Rating: " << rating << "/5\nComment: " << comment << endl;
    }

private:
    string customerName;
    string comment;
    int rating;
};

class StoreItem
{
public:
    StoreItem(string name, string price, string category)
        : name(name), price(price), category(category) {}

    virtual ~StoreItem() {}

    virtual void display() const
    {
        cout << "Name: " << name << ", Price: " << price << ", Category: " << category << endl;
        displayReviews();
    }

    string getName() const
    {
        return name;
    }

    string getPrice() const
    {
        return price;
    }

    string getCategory() const
    {
        return category;
    }

    void addReview(const Review& review)
    {
        reviews.push_back(review);
    }

protected:
    void displayReviews() const
    {
        if (!reviews.empty())
        {
            cout << "Reviews:\n";
            for (const auto& review : reviews)
            {
                review.display();
            }
        }
        else
        {
            cout << "No reviews yet.\n";
        }
    }

private:
    string name;
    string price;
    string category;
    vector<Review> reviews;
};

class Inventory
{
public:
    void addItem(StoreItem* item)
    {
        items.push_back(item);
    }

    void removeItem(const string& itemName)
    {
        auto it = remove_if(items.begin(), items.end(),
                            [itemName](const StoreItem* item)
                            {
                                return item->getName() == itemName;
                            });

        items.erase(it, items.end());
    }

    void displayInventoryByCategory(const string& category) const
    {
        cout << "Inventory for Category: " << category << "\n";
        for (const auto& item : items)
        {
            if (item->getCategory() == category)
            {
                item->display();
                cout << "-----------------------------\n";
            }
        }
        if (items.empty()) {
            cout << "No items found in the inventory.\n";
        }
    }

    const StoreItem* findItem(const string& itemName) const
    {
        auto it = find_if(items.begin(), items.end(),
                          [itemName](const StoreItem* item)
                          {
                              return item->getName() == itemName;
                          });

        return (it != items.end()) ? *it : nullptr;
    }

private:
    vector<StoreItem*> items;
};

class Customer
{
public:
    void viewInventoryByCategory(const Inventory& inventory) const
    {
        string category;
        cout << "Choose a category:\n"
             << "1. Electronics\n"
             << "2. Food & Beverages\n"
             << "3. Beauty Products\n"
             << "4. Stationeries\n"
             << "Select category: ";
        cin >> category;
        cin.ignore();

        switch (category[0])
        {
        case '1':
            inventory.displayInventoryByCategory("Electronics");
            break;
        case '2':
            inventory.displayInventoryByCategory("Food & Beverages");
            break;
        case '3':
            inventory.displayInventoryByCategory("Beauty Products");
            break;
        case '4':
            inventory.displayInventoryByCategory("Stationeries");
            break;
        default:
            cout << "Invalid category choice.\n";
            break;
        }
    }

    void viewItemDetails(const Inventory& inventory) const
    {
        string itemName;
        cout << "Enter the name of the item to view details: ";
        getline(cin, itemName);

        const StoreItem* item = inventory.findItem(itemName);

        if (item)
        {
            cout << "Item details:\n";
            item->display();
            giveReview(item);
        }
        else
        {
            cout << "Item not found.\n";
        }
    }

private:
    void giveReview(const StoreItem* item) const
    {
        string customerName, comment;
        int rating;

        cout << "Enter your name: ";
        getline(cin, customerName);
        cout << "Enter your rating (1-5): ";
        cin >> rating;
        cin.ignore();
        cout << "Enter your comment: ";
        getline(cin, comment);

        Review review(customerName, comment, rating);
        const_cast<StoreItem*>(item)->addReview(review);

        cout << "Review added successfully!\n";
    }
};

class Manager
{
public:
    void manageInventory(Inventory& inventory)
    {
        char choice;
        do
        {
            cout << "Choose an action:\n"
                 << "1. Add new item\n"
                 << "2. Remove item\n"
                 << "3. Display inventory\n"
                 << "4. Log off\n";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case '1':
                addItem(inventory);
                break;
            case '2':
                removeItem(inventory);
                break;
            case '3':
                displayInventoryByCategory(inventory);
                break;
            case '4':
                cout << "Logging off...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != '4');
    }

private:
    void addItem(Inventory& inventory)
    {
        string name, price, category;

        cout << "Enter item details:\n";
        cout << "Name: ";
        getline(cin, name);
        cout << "Price: ";
        getline(cin, price);
        cout << "Category: ";
        getline(cin, category);

        StoreItem* newItem = new StoreItem(name, price, category);
        inventory.addItem(newItem);

        // Append the new item to the file
        ofstream outputFile("inventory.txt", ios::app);
        if (!outputFile)
        {
            cerr << "Error opening output file." << endl;
            return;
        }

        outputFile << name << " " << price << " " << category << endl;

        outputFile.close();

        cout << "Item added successfully!\n";
    }

    void removeItem(Inventory& inventory)
    {
        string itemName;
        cout << "Enter the name of the item to remove: ";
        getline(cin, itemName);

        inventory.removeItem(itemName);

        cout << "Item removed successfully!\n";
    }

    void displayInventoryByCategory(const Inventory& inventory) const
    {
        string category;
        cout << "Choose a category:\n"
             << "1. Electronics\n"
             << "2. Food & Beverages\n"
             << "3. Beauty Products\n"
             << "4. Stationeries\n"
             << "Select category: ";
        cin >> category;
        cin.ignore();

        switch (category[0])
        {
        case '1':
            inventory.displayInventoryByCategory("Electronics");
            break;
        case '2':
            inventory.displayInventoryByCategory("Food & Beverages");
            break;
        case '3':
            inventory.displayInventoryByCategory("Beauty Products");
            break;
        case '4':
            inventory.displayInventoryByCategory("Stationeries");
            break;
        default:
            cout << "Invalid category choice.\n";
            break;
        }
    }
};

class Store
{
public:
    void run()
    {
        ifstream inputFile("inventory.txt");
        if (!inputFile)
        {
            cerr << "Error opening input file." << endl;
            return;
        }

        string name, price, category;
        while (inputFile >> name >> price >> category)
        {
            // Create a new StoreItem object and add it to the inventory
            StoreItem* item = new StoreItem(name, price, category);
            inventory.addItem(item);
        }

        inputFile.close();

        char mainChoice;
        do
        {
            cout << "Choose one:\n"
                 << "1. Log in\n"
                 << "2. Quit\n";
            cin >> mainChoice;
            cin.ignore();

            switch (mainChoice)
            {
            case '1':
                isLoggedIn = true;
                loggedInMenu();
                break;
            case '2':
                cout << "Quitting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (mainChoice != '2');
    }

private:
    void loggedInMenu()
    {
        char userType;
        do
        {
            cout << " Choose user type:\n"
                 << "-------------------\n"
                 << "1. Manager\n"
                 << "2. Customer\n"
                 << "3. Log off\n";
            cin >> userType;
            cin.ignore(); // Consume the newline character

            switch (userType)
            {
            case '1':
                managerView();
                break;
            case '2':
                customerView();
                break;
            case '3':
                cout << "Logging off...\n";
                isLoggedIn = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (userType != '3');
    }

    void managerView()
    {
        manager.manageInventory(inventory);
    }

    void customerView() const
    {
        char choice;
        do
        {
            cout << "Choose an action:\n"
                 << "1. View inventory by category\n"
                 << "2. View details of searched item\n"
                 << "3. Log off\n";
            cin >> choice;
            cin.ignore(); // Consume the newline character

            switch (choice)
            {
            case '1':
                customer.viewInventoryByCategory(inventory);
                break;
            case '2':
                customer.viewItemDetails(inventory);
                break;
            case '3':
                cout << "Logging off customer view...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != '3');
    }

private:
    Inventory inventory;
    Manager manager;
    Customer customer;
    bool isLoggedIn = false;
};

int main()
{
    Store myStore;
    myStore.run();

    return 0;
}
