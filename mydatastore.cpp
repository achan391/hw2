#include "mydatastore.h"
#include "util.h"
#include <iostream>

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for(vector<Product*>::iterator it = products_.begin();
        it != products_.end(); ++it)
    {
        delete *it;
    }

    for(map<string, User*>::iterator it = users_.begin();
        it != users_.end(); ++it)
    {
        delete it->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);

    set<string> keys = p->keywords();

    for(set<string>::iterator it = keys.begin();
        it != keys.end(); ++it)
    {
        string key = convToLower(*it);
        keywordMap_[key].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string username = convToLower(u->getName());

    users_[username] = u;
    carts_[username] = vector<Product*>();
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    vector<Product*> output;

    if(terms.size() == 0) {
        return output;
    }

    set<Product*> results;

    // AND
    if(type == 0) {
        string first = convToLower(terms[0]);

        if(keywordMap_.find(first) == keywordMap_.end()) {
            return output;
        }

        results = keywordMap_[first];

        for(size_t i = 1; i < terms.size(); i++) {
            string term = convToLower(terms[i]);

            if(keywordMap_.find(term) == keywordMap_.end()) {
                results.clear();
                break;
            }

            set<Product*> next = keywordMap_[term];
            results = setIntersection(results, next);
        }
    }
    // OR
    else {
        for(size_t i = 0; i < terms.size(); i++) {
            string term = convToLower(terms[i]);

            if(keywordMap_.find(term) != keywordMap_.end()) {
                set<Product*> next = keywordMap_[term];
                results = setUnion(results, next);
            }
        }
    }

    for(set<Product*>::iterator it = results.begin();
        it != results.end(); ++it)
    {
        output.push_back(*it);
    }

    return output;
}

bool MyDataStore::addToCart(const string& username, Product* p)
{
    string name = convToLower(username);

    if(users_.find(name) == users_.end()) {
        return false;
    }

    carts_[name].push_back(p);
    return true;
}

void MyDataStore::viewCart(const string& username)
{
    string name = convToLower(username);

    if(users_.find(name) == users_.end()) {
        cout << "Invalid username" << endl;
        return;
    }

    vector<Product*>& cart = carts_[name];

    for(size_t i = 0; i < cart.size(); i++) {
        cout << "Item " << i + 1 << endl;
        cout << cart[i]->displayString() << endl;
        cout << endl;
    }
}

void MyDataStore::buyCart(const string& username)
{
    string name = convToLower(username);

    if(users_.find(name) == users_.end()) {
        cout << "Invalid username" << endl;
        return;
    }

    User* user = users_[name];
    vector<Product*>& cart = carts_[name];

    vector<Product*> remaining;

    for(size_t i = 0; i < cart.size(); i++) {
        Product* p = cart[i];

        if(p->getQty() > 0 &&
           user->getBalance() >= p->getPrice())
        {
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
        }
        else {
            remaining.push_back(p);
        }
    }

    cart = remaining;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;

    for(vector<Product*>::iterator it = products_.begin();
        it != products_.end(); ++it)
    {
        (*it)->dump(ofile);
    }

    ofile << "</products>" << endl;

    ofile << "<users>" << endl;

    for(map<string, User*>::iterator it = users_.begin();
        it != users_.end(); ++it)
    {
        it->second->dump(ofile);
    }

    ofile << "</users>" << endl;
}