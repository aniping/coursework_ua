#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctype.h>

#include "SPMRecord.hpp"
#include "Directory.hpp"

using namespace std;

const string db_file_name = "spmcat.bin";
const string index_file_name = "index.bin";

bool is_digits(const string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

int
main(int argc, const char *argv[])
{
  // open DB file for read
  fstream db_file(db_file_name.c_str(), ios::in | ios::binary);

  Directory dir(index_file_name, true);
  cout << "\n------------Begin printing stats--------------" << endl;
  dir.printStats();
  cout << "-----------End of printing stats--------------\n" << endl;
  dir.loadDirEntries();

  string key;
  while (true) {
    cout << ">>> Please enter a search key (<= 8-digit number, enter 'q' to quit): ";
    getline(cin, key);

    if (key == "q")
      break;

    // sanity check
    if (key.size() > 9) {
      cout << "Size of the key must be less than 9!" << endl;
      continue;
    }

    // sanity check
    if (!is_digits(key)) {
      cout << "Search key must contain digit only!" << endl;
      continue;
    }

    dir.query(db_file, key);
  }
}
