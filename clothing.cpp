#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>

using namespace std;

Clothing::Clothing(const string& name, double price, int qty,
                   const string& size, const string& brand)
    : Product("clothing", name, price, qty),
      size_(size),
      brand_(brand)
{
}

set<string> Clothing::keywords() const
{
    set<string> keys = parseStringToWords(name_);
    set<string> brandKeys = parseStringToWords(brand_);

    keys = setUnion(keys, brandKeys);

    return keys;
}

string Clothing::displayString() const
{
    stringstream ss;

    ss << name_ << endl;
    ss << "Size: " << size_ << " Brand: " << brand_ << endl;
    ss << fixed << setprecision(2)
       << price_ << " " << qty_ << " left.";

    return ss.str();
}

void Clothing::dump(ostream& os) const
{
    Product::dump(os);
    os << size_ << endl;
    os << brand_ << endl;
}