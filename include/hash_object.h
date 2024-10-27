#ifndef HASH_OBJECT_H
#define HASH_OBJECT_H

#include <string>

using namespace std;

string calculate_sha1(const string& data);
string hash_object(const string& fileName, bool writeFlag);

#endif // HASH_OBJECT_H
