#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include "LinkedList.h"

void mainMenu(LinkedList& stock, std::vector<Coin> coins, std::string stockFileName, std::string coinFileName);
void printMenu();

void display_stock(const LinkedList &stock_list);
void displayCoins(std::vector<Coin> coins);

void purchaseItem(LinkedList& stock, std::vector<Coin>& coins);
bool isInteger(const std::string& str);
bool changeHandler(int totalCash, std::vector<Coin>& coins, std::string itemName,  std::vector<int> changeAdded);

void removeItem(LinkedList& stock);
void resetCoins(std::vector<Coin>& coins, int defaultAmount);

void addItem(LinkedList& stock);
std::string generateId(LinkedList& stock);
bool isPriceValid(unsigned dollars, unsigned cents);

LinkedList getStock(std::string fileName);
std::vector<Coin> getCoins(std::string fileName);
void saveStockToFile(std::string stockFileName, LinkedList& stock);
void saveCoinsToFile(std::string coinFileName, std::vector<Coin> coins);
bool checkFileExistance(std::string fileName);
bool checkStockFileFormat(std::string fileName);
bool compareByDenomination(const Coin& coin1, const Coin& coin2);

std::vector<std::string> split(const std::string &s, char delimiter);
/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char* argv[])
{
    //checks if the correct amount of arguments have been entered
    if (argc != 3) {
        std::cout << argc - 1 <<" arguments found. 2 arguments expected.\n";
    }else{

        //getting command line arguments for file names
        std::string stockFileName = argv[1];
        std::string coinsFileName = argv[2];

        //checks if the files are found in the system
        if (checkFileExistance(stockFileName) && checkFileExistance(coinsFileName)){
            
            
            //checks if the stock file is in a valid format
            if (checkStockFileFormat(stockFileName)){

                //load the stock file as a linked list and coins as an vector of coins
                LinkedList stock = getStock(stockFileName);
                std::vector<Coin> coins = getCoins(coinsFileName);
    
                //checks if the coins file was gotten correctly
                if (coins.size() == 8){
                    //run the main menu which handles inputs
                    mainMenu(stock, coins, stockFileName, coinsFileName);
                } else {
                    std::cout << "Issue with coins file format. Program aborted.\n";
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

//while loop to print the menu and check input. the input is checked against an if statement to run the entered command
void mainMenu(LinkedList& stock, std::vector<Coin> coins, std::string stockFileName, std::string coinFileName) {
    std::string input;
    bool running = true;
    while (running) {
        //print menu and get input
        printMenu();
        std::getline(std::cin, input);
        
        //Main Menu
        //display the stock
        if (input == "1"){
            display_stock(stock);
        
        //purchase an item
        }else if (input == "2"){
            purchaseItem(stock, coins);
        }

        //save and exit
        else if (input == "3"){
            saveStockToFile(stockFileName, stock);
            saveCoinsToFile(coinFileName, coins);
            running = false;
        }

        //Administrator-Only Menu
        //add new stock item
        else if (input == "4"){
            addItem(stock);

        }

        //remove stock item
        else if (input == "5"){
            removeItem(stock);


        //dispay the coins in system   
        }
        else if (input == "6"){
            displayCoins(coins);
        }

        //reset stock count to default value 20
        else if (input == "7"){
            //iterate over stock linked list -> set .onhand quantity to default 20).
            stock.setDefaultValue(stock.get_head(),DEFAULT_STOCK_LEVEL);
            std::cout << "\"All stock has been reset to default level of " 
                      << DEFAULT_STOCK_LEVEL << "\"\n";
            
        }

        //reset coins count to feault 20
        else if (input == "8"){
            //iterate over coins vector set to default amount 20.
            resetCoins(coins, DEFAULT_COIN_COUNT);
        }

        //abort program
        else if (input == "9"){
            running = false;
        }

        std::cout << std::endl;
    }
}

//print the main menu
void printMenu(){
    std::cout << "Main Menu:\n";
    std::cout << "  1.Display Items\n";
    std::cout << "  2.Purchase Items\n";
    std::cout << "  3.Save and Exit\n";
    std::cout << "Administrator-Only Menu:\n";
    std::cout << "  4.Add Item\n";
    std::cout << "  5.Remove Item\n";
    std::cout << "  6.Display Coins\n";
    std::cout << "  7.Reset Stock\n";
    std::cout << "  8.Reset Coins\n";
    std::cout << "  9.Abort Program\n";
    std::cout << "Select your option (1-9):\n";
}

// Helper function to split a string by a delimiter
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void display_stock(const LinkedList &stock_list) {
    std::cout << std::endl;
    std::cout << "Items Menu" << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << "ID   |Name" << std::setw(55) << "| Available | Price" << std::endl;
    std::cout << "-------------------------------------------------------------------" 
              << std::endl;
    


    Node *current = stock_list.get_head();
    while (current) {
    Stock *stock = current->data;
    std::cout << std::left << std::setw(IDLEN) << stock->id << "|"
              << std::setw(NAMELEN) << stock->name << "|"
              << std::right << stock->on_hand << std::setw(10) << " |"
              << "$ " << stock->price.dollars << "."
              << std::setw(2) << std::setfill('0') << stock->price.cents
              << std::setfill(' ') << std::endl; // Reset the fill character to space
    current = current->next;
    }
}


//load stock to a linked list
LinkedList getStock(std::string fileName) {

    // Create a LinkedList object
    LinkedList stock_list;

    // Open the stock file
    std::ifstream stock_file(fileName);
    std::string line;

    // Read the file line by line and enter values to list
    while (std::getline(stock_file, line)) {
        std::vector<std::string> tokens = split(line, '|');

        Stock *stock = new Stock();
        stock->id = tokens[0].substr(0, IDLEN);
        stock->name = tokens[1].substr(0, NAMELEN);
        stock->description = tokens[2].substr(0, DESCLEN);
        double price = std::stod(tokens[3]);
        stock->price.dollars = static_cast<int>(price);
        stock->price.cents = static_cast<int>((price - stock->price.dollars) * 100);
        stock->on_hand = std::stoi(tokens[4]);
            
        stock_list.add_node(stock);

    }

    // Sort the list and close the file
    stock_list.sort();
    stock_file.close();
    return stock_list;
}

//load coins to a coins array
std::vector<Coin> getCoins(std::string fileName){
    
    std::vector<Coin> coins;

    //loop through the lines in the file and add to vector
    std::ifstream coins_file(fileName);
    std::string line;

    bool validFormat = true;

    while (std::getline(coins_file, line) && validFormat) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() == 2){
            if (isInteger(tokens[0]) && isInteger(tokens[1])) {
                if (validDenominations.count(std::stoi(tokens[0])) && std::stoi(tokens[0]) > -1){
                    Coin coin;
                    coin.denom = intToDenomination.at(std::stoi(tokens[0]));
                    coin.count = std::stoi(tokens[1]);
                    coins.push_back(coin);
                } else {validFormat = false;}
            } else {validFormat = false;}
        } else {validFormat = false;}
    }

    //this turns the file into a set to check unique number entries
    std::set<Denomination> coinsUniqueCheckSet;
    for (auto coin : coins) {
        coinsUniqueCheckSet.insert(coin.denom);
    }

    coins_file.close();

    //empties the list if it is invalid in any way
    if(validFormat == false or coinsUniqueCheckSet.size() != 8){
        coins.clear();
    }

    //sorts the vector by denomination
    std::sort(coins.begin(), coins.end(), compareByDenomination);

    return coins;
}

//compare coins by their enum
bool compareByDenomination(const Coin& coin1, const Coin& coin2) {
    return coin1.denom < coin2.denom;
}

//function to check if a file is valid or not
bool checkFileExistance(std::string fileName) {
    std::ifstream file(fileName);
    bool validFile = (file.good()) ? true : false;
    file.close();
    if (validFile == false) {std::cout << fileName <<" cannot be found. Program aborting.\n";}
    return validFile;
    
}

//this checks if the stock file is in a valid format, if it isnt it returns false
bool checkStockFileFormat(std::string fileName){
    std::ifstream file(fileName);
    std::string line;

    //loop through file and check if all values are in the correct format
    bool validFormat = true;
    while (std::getline(file, line) && validFormat) {
        std::vector<std::string> tokens = split(line, '|');
        if (tokens.size() == 5) {
            //check if any inputs are empty
            for (int i = 0; i < 5; i++) {if (tokens[i].empty()) {validFormat = false;}}
            
            //check if coin is in right format
            std::vector<std::string> priceTokens = split(tokens[3], '.');
            if (priceTokens.size() == 2) {
                if (isInteger(tokens[4]) == false 
                    or isInteger(priceTokens[0])  == false 
                    or isInteger(priceTokens[1])  == false
                    or priceTokens[1].size() != 2) {validFormat = false;}

            } else {validFormat = false;}

        } else {
            validFormat = false;
        }
    }
    
    if (validFormat == false) {
        std::cout << "Stock file in invalid found. Program aborting.\n";
    }

    file.close();
    return validFormat;
}

//display the coins and their value
void displayCoins(std::vector<Coin> coins){
    
    std::cout << std::endl;
    std::cout << "Coins Summary\n";
    std::cout << "-------------\n";
    std::cout << "Denomination    |     Count\n";
    std::cout << "---------------------------\n";
    

    //loop through coins vector and display values. name is gotten by being passes through a map to get string value
    for (int i = 0; i < static_cast<int>(coins.size()); i++) {
        
        std::cout << std::left << std::setw(16) << denominationToString.at(coins[i].denom) 
                  << std::setfill(' ')  << "|";
        std::cout << std::right << std::setw(10) << std::setfill(' ')  <<coins[i].count << "\n";
    }
}

//handles purcahsing an item
void purchaseItem(LinkedList& stock, std::vector<Coin>& coins){
    std::cout << std::endl;
    std::cout << "Purchase Item\n";
    std::cout << "-------------\n";
    std::cout << "Please enter the id of the item you wish to purchase:\n";
    
    //gets input
    std::string id;
    std::getline(std::cin, id);

    //seaches for and returns the node
    Node* item = stock.search_node(id);

    //if entry is empty or item cant be found the function ends
    if (id.empty()){
        //let the function pass
    } else if (item == nullptr){
        std::cout << "Item ID not found. Returning to main menu\n"; 
    } else if (item->data->on_hand == 0) {
        std::cout << "There are no " << item->data->name 
        << "s on hand. Returning to main menu.\n "; 
    } else {
        std::cout << "You have selected \"" << item->data->name << " - " 
                  << item->data->description << "\". This will cost you $"
                  << item->data->price.dollars << "."
                  << item->data->price.cents <<".\n";

        std::cout << "Please hand over the money - type in the value of each note/coin in cents.\n" 
                  << "Press enter or ctrl-d on a new line to cancel this purchase:\n";
                  


        //keeps checking entry and adding to a sum until it exceeds price of product
        int itemCost = (item->data->price.dollars * 100) + item->data->price.cents;
        int runningValue = 0;
        std::vector<int> changeAdded;
        bool emptyLineCheck = true;

        while (itemCost > runningValue && emptyLineCheck) {
            std::cout << "You still need to give us $"
                      << std::fixed << std::setprecision(2)
                      << (static_cast<double>(itemCost - runningValue) / 100.0) << ":\n";
            
            std::string entry;
            std::getline(std::cin, entry);

            if (entry.empty()){
                emptyLineCheck = false;

            //check if input is an integer
            } else if (isInteger(entry)) {

                //check if input is a valid denomination
                if (validDenominations.count(std::stoi(entry))){
                    runningValue += std::stoi(entry);
                    changeAdded.push_back(std::stoi(entry));
                } else {
                    std::cout << "Error: $"
                          << std::fixed << std::setprecision(2)
                          << (static_cast<double>(std::stoi(entry)) / 100.0)  
                          << " is not a valid denomination of money. Please try again." << "\n";
                }
            }  else {//integer check fail
                std::cout << "Please enter a valid amount of money.\n";
            }
        }

        if (emptyLineCheck) {
            std::cout << "Here is your " << item->data->name << " and your change of $"
                      << std::fixed << std::setprecision(2) 
                      << (static_cast<double>(runningValue - itemCost) / 100.0) <<":";
    
            //handles returning change, if there is a valid amount of change it takes away an item from hand. also adds money added to system.
            if (changeHandler(runningValue - itemCost, coins, item->data->name, changeAdded)){
                for (std::size_t i =  0; i < changeAdded.size(); ++i){            
                    coins[cashToIndex.at(changeAdded[i])].count += 1;
                }

                item->data->on_hand -= 1;
            }
        }
    }
}

//this manages change for a purchase
bool changeHandler(int totalCash, std::vector<Coin>& coins, std::string itemName, std::vector<int> changeAdded) {
    int denominations[] = {1000, 500, 200, 100, 50, 20, 10, 5};
    std::vector<int> changeList;
    int cashLump = totalCash;
    bool validAmountOfChange;
    /*
     *  runs through all the denominations and removes the dem value from a sum. if the cash lump exceeds the denomination
     *  or there are no more coins of the denomination left it moves on to the next denomination. it finishes when the 
     *  cash lump is equal to zero, giving the exact amount of change, or there is not enough coins left, in which it
     *  fails in getting change.
     */
    for (int i =  0; i < 8; ++i) {
        //for cointing the coins take away the value added to the machine so it is included
        int coinsCounter = 0 - std::count(changeAdded.begin(), 
                                          changeAdded.end(),   
                                          denominations[i]);
        
        
        while (cashLump - denominations[i] >= 0 && 
               static_cast<int>(coins[i].count) > coinsCounter){

            changeList.push_back(denominations[i]);
            cashLump = cashLump - denominations[i];
            coinsCounter += 1;
        }
    }

    //displays change returned and removes from the vector
    if (cashLump == 0){
        for (std::size_t i =  0; i < changeList.size(); ++i){
            if (changeList[i] > 99) {
                std::cout << " $" << changeList[i]/100 << "";
            } else {
                std::cout << " " << changeList[i] << "c";
            }
            
            coins[cashToIndex.at(changeList[i])].count -= 1;
        }
        validAmountOfChange = true;
        std::cout << "\n";

    } else {
        std::cout << " No valid quantity of change availible \n";
        validAmountOfChange = false;
    }

    return validAmountOfChange;
}


//checks if a string is an integer for checking valid input
bool isInteger(const std::string& str) {
    bool isInt = true;
  
    if (str.empty() || std::isspace(str[0])) {
        isInt =  false;
    }

    for (std::size_t i =  0; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            isInt =  false;
        }
    }

  return isInt;
}


void removeItem(LinkedList& stock){
    std::cin.sync(); //clean up input buffer so can get user input.

    std::string removeIdInput;
    Node* deletingItemNodePtr = nullptr;
    bool validInput = false, isRemoved = false;

    //keeps iterating through while loop till user gives valid input.
    while (!validInput) {
        std::cout << "\nEnter the item id of the item to remove from the menu: ";
        std::getline(std::cin, removeIdInput);  

        //checks if the input is empty
        if (!removeIdInput.empty()) {
            deletingItemNodePtr = stock.search_node(removeIdInput);

            if (deletingItemNodePtr != nullptr) {
                std::cout << "\"" << deletingItemNodePtr->data->id << " - " 
                          << deletingItemNodePtr->data->name << " - " 
                          << deletingItemNodePtr->data->description << "\""
                          << " has been removed from the system.\n";

                stock.delete_node(removeIdInput);
                isRemoved = true;
            }
            validInput = true;  //if it finds a stock with same id as input then leaves loop to delete it.
        } else {
            std::cout << "ID cannot be empty. Please try again.\n";
        }
    }

    if (!isRemoved) {
        std::cout << "No matching item found. Returning to main menu.\n";
    }
}

//this saves the stock list to a file.
void saveStockToFile(std::string stockFileName, LinkedList& stock) {
    std::ofstream stockFile;
    stockFile.open(stockFileName);
    Node *current = stock.get_head();

    //moves through the list, adding its values to the file
    while (current) {
        Stock *stock = current->data;
        stockFile << stock->id << "|"
                  << stock->name << "|"
                  << stock->description << "|"
                  << stock->price.dollars << "."
                  << std::setw(2) << std::setfill('0') << stock->price.cents << "|"
                  << stock->on_hand
                  << std::endl;
        current = current->next;
    }
    stockFile.close();
}

//saves the coin vector to file
void saveCoinsToFile(std::string coinFileName, std::vector<Coin> coins) {
    std::ofstream coinFile(coinFileName);

    //loops through the coin vector and writes its value to the file
    for (int i = 0; i < 8; i++) {
        coinFile << denominationToValueAsString.at(coins[i].denom) 
        << "," << coins[i].count << std::endl;
    }

    coinFile.close();
}

//set coins vector to default amount 20
void resetCoins(std::vector<Coin>& coins, int defaultAmount){
    //iterate over coins refernce and set count to default 20
    for (int i = 0; i < static_cast<int>(coins.size()); i++) {
        coins[i].count = defaultAmount;
    }

    std::cout << "All coins have been reset to the default level of " << defaultAmount << "\n";
}

//Add new item creted from user to stock linked list
void addItem(LinkedList& stock) {
    std::cin.sync();

    std::string newStockName;
    std::cout << "Enter the item name: \n";
    std::getline(std::cin, newStockName);
    //check if stockName is not empty and check is not a digit.
    while (newStockName.empty() || std::all_of(newStockName.begin(), newStockName.end(), ::isdigit)) {
        std::cout << "Name cannot be empty or numeric. Please try again.\n";
        std::getline(std::cin, newStockName);
    }

    std::string newStockDescription;
    std::cout << "Enter the item description: \n";
    std::getline(std::cin, newStockDescription);
    //check stock description is not empty or digit value.
    while (newStockDescription.empty() || std::all_of(newStockDescription.begin(), newStockDescription.end(), ::isdigit)) {
        std::cout << "Description cannot be empty or numeric. Please try again.\n";
        std::getline(std::cin, newStockDescription);
    }

    bool validPrice = false;
    std::string priceInput, dollarsStr, centsStr;
    unsigned dollars, cents;
    std::istringstream ss;
    //check that price give is in the correct format and not empty.
    while (!validPrice) {
        std::cout << "Enter the price for the item (format: X.XX): \n";
        std::getline(std::cin, priceInput);

        ss.str(priceInput);
        ss.clear();

        // Get the value before the '.' delimiter
        std::getline(ss, dollarsStr, '.');

        // Get the value after the '.' delimiter
        std::getline(ss, centsStr, '\n');
        //check not empty is only length 2 and is a digit not like 7w.50.
        if (dollarsStr.empty() || centsStr.length() != 2 || !std::all_of(dollarsStr.begin(), dollarsStr.end(), ::isdigit) || !std::all_of(centsStr.begin(), centsStr.end(), ::isdigit)) {
            std::cout << "Price must be a valid number in format X.XX. Please try again.\n";
        } else {
            //take the sting value and convert to integers.
            dollars = std::stoul(dollarsStr);
            cents = std::stoul(centsStr);

            //check using method isPriceValid if price can be made up from given denominations it accepts.
            if (isPriceValid(dollars, cents)) {
                validPrice = true;
            } else {
                std::cout << "The price is not a valid denomination. Please enter a valid price.\n";
            }
        }
    }

    //create price object for stock
    Price price;
    price.dollars = dollars;
    price.cents = cents;
    //generate unique id for stock.
    std::string newStockId = generateId(stock);
    int newStockOnHand = DEFAULT_STOCK_LEVEL;
    //create new stock value from user inputs and add node to linked list and sort by id's.
    Stock* newStock = new Stock(newStockId, newStockName, newStockDescription, price, newStockOnHand);
    stock.add_node(newStock);
    stock.sort();
}

//checks stock list and starts from I0000 and increments till finds unique value to assign to new stock.
std::string generateId(LinkedList& stock) {
    std::string newId = "IDXXX";
    bool found = false;

    for (int idNumber = 1; idNumber <= 9999 && !found; idNumber++) {
        //max length if id.
        char id[99];
        snprintf(id, sizeof(id), "I%04d", idNumber);  //make idNumebr start from value I0000 to find unique id.

        if (stock.search_node(id) == nullptr) {  //check if the id is already created.
            newId = id;  //if unique set as new id.
            found = true;
        }
    }
    //if no valid id's are left, newId will remain as "IDXXX"
    return newId;
}

bool isPriceValid(unsigned dollars, unsigned cents) {
    //convert price value to cents.
    unsigned total_cents = dollars * 100 + cents;

    //give in denominations to check if valid.
    unsigned denominations[] = {5, 10, 20, 50, 100, 200, 500, 1000};

    //increment from largest value till can break down price to denominations.
    for (int i = 7; i >= 0; --i) {
        while (total_cents >= denominations[i]) {
            total_cents -= denominations[i];
        }
    }
    //if total_cents is 0 then know it can be broken down by accepted denominations and is valid price.
    return total_cents == 0;
}