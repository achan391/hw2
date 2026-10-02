#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <map>
#include <set>
#include <vector>
#include <string>

class MyDataStore : public DataStore
{
public:
    MyDataStore();
    ~MyDataStore();

    void addProduct(Product* p);
    void addUser(User* u);

    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    void dump(std::ostream& ofile);

    bool addToCart(const std::string& username, Product* p);
    void viewCart(const std::string& username);
    void buyCart(const std::string& username);

private:
    std::vector<Product*> products_;
    std::map<std::string, User*> users_;

    // keyword -> products containing that keyword
    std::map<std::string, std::set<Product*> > keywordMap_;

    // username -> products in FIFO order
    std::map<std::string, std::vector<Product*> > carts_;
};

#endif