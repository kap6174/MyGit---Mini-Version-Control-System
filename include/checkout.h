#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <string>

using namespace std;

string decompress_data_checkout(const string& compressed_data);
void restore_object(const string& permission, const string& type, const string& object_hash, const string& path);
void checkout_commit(const string& commit_sha);

#endif // CHECKOUT_H
