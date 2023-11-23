#ifndef COIN_H
#define COIN_H

#include <map>
#include <string>
#include <set>

// Coin.h defines the coin structure for managing currency in the system. 
#define DELIM ","  // delimiter

// enumeration representing the various types of currency available in the system. 
enum Denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};


// various maps used as constants to transform to and from denomination values.
const std::map<int, Denomination> intToDenomination = {
        {5, Denomination::FIVE_CENTS}, {10, Denomination::TEN_CENTS}, {20, Denomination::TWENTY_CENTS},
        {50, Denomination::FIFTY_CENTS}, {100, Denomination::ONE_DOLLAR}, {200, Denomination::TWO_DOLLARS},
        {500, Denomination::FIVE_DOLLARS}, {1000, Denomination::TEN_DOLLARS}
};

const std::map<Denomination, std::string> denominationToString = {
        {Denomination::FIVE_CENTS, "5 Cents"}, {Denomination::TEN_CENTS, "10 Cents"},
        {Denomination::TWENTY_CENTS, "20 Cents"}, {Denomination::FIFTY_CENTS, "50 Cents"},
        {Denomination::ONE_DOLLAR, "1 Dollar"}, {Denomination::TWO_DOLLARS, "2 Dollars"},
        {Denomination::FIVE_DOLLARS, "5 Dollar"}, {Denomination::TEN_DOLLARS, "10 Dollars"}
}; 

const std::map<Denomination, std::string> denominationToValueAsString = {
        {Denomination::FIVE_CENTS, "5"}, {Denomination::TEN_CENTS, "10"}, {Denomination::TWENTY_CENTS, "20"},
        {Denomination::FIFTY_CENTS, "50"}, {Denomination::ONE_DOLLAR, "100"}, {Denomination::TWO_DOLLARS, "200"}, 
        {Denomination::FIVE_DOLLARS, "500"}, {Denomination::TEN_DOLLARS, "1000"}
};

const std::map<int, int> cashToIndex = {
        {5, 7}, {10, 6}, {20, 5}, {50, 4}, {100, 3}, {200, 2}, {500, 1}, {1000, 0}
};

const std::set<int> validDenominations = {5, 10, 20, 50, 100, 200, 500, 1000};


// represents a coin type stored in the cash register perhaps. Each demonination
// will have exactly one of these in the cash register.
class Coin
{
public:
    // the denomination type
    enum Denomination denom;
    
    // the count of how many of these are in the cash register
    unsigned count;
};

#endif // COIN_H
