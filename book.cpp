#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>

using namespace std;

Book::Book(const string& name, double price, int qty,
           const string& isbn, const string& author)
    : Product("book", name, price, qty),
      isbn_(isbn),
      author_(author)
{
}

set<string> Book::keywords() const
{
    set<string> keys = parseStringToWords(name_);
    set<string> authorKeys = parseStringToWords(author_);

    keys = setUnion(keys, authorKeys);
    keys.insert(isbn_);

    return keys;
}

string Book::displayString() const
{
    stringstream ss;

    ss << name_ << endl;
    ss << "Author: " << author_ << " ISBN: " << isbn_ << endl;
    ss << fixed << setprecision(2)
       << price_ << " " << qty_ << " left.";

    return ss.str();
}

void Book::dump(ostream& os) const
{
    Product::dump(os);
    os << isbn_ << endl;
    os << author_ << endl;
}