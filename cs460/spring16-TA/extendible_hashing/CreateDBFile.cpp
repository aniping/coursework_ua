#include <iostream>
#include <fstream>

#include "SPMRecord.hpp"

using namespace std;

//const string input_fname = "spmcat.dat";
const string out_fname = "spmcat.bin";

bool
DBFileExists(fstream& db_file);

void
CreateDBFile(ifstream& text_file, fstream& db_file);

void
PrintFirst5Last5(fstream& db_file);

int
main(int argc, const char *argv[])
{
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " /path/to/input_file" << endl;
    return -1;
  }

  fstream db_file(out_fname.c_str(), ios::in | ios::binary);
  if (DBFileExists(db_file)) {
    cout << "Binary DB file " << out_fname << " has already been created." << endl;
    PrintFirst5Last5(db_file);
    return 0;
  } else {
    db_file.close(); // close for write
    ifstream text_file(argv[1], ios::in);
    db_file.open(out_fname, ios::out | ios::binary);
    CreateDBFile(text_file, db_file);

    text_file.close();
    db_file.close();
    db_file.open(out_fname, ios::in | ios::binary); // reopen for read
  }
}

bool
DBFileExists(fstream& db_file)
{
  if (db_file.is_open() && db_file.peek() != ifstream::traits_type::eof()) {
    return true;
  } else {
    return false;
  }
}

void
CreateDBFile(ifstream& text_file, fstream& db_file)
{
  if (text_file.is_open() && db_file.is_open()) {
    string line;
    SPMRecord record;
    while (getline(text_file, line)) {
      record.writeRecord(db_file, line);
    }
  } else {
    cout << "Open files failed!" << endl;
  }
}

void
PrintFirst5Last5(fstream& db_file)
{
  db_file.seekg(0, ios_base::end);
  int file_len = db_file.tellg(); // length of the file
  int num_records = file_len / SPMRecord::length();
  cout << "File length: " << file_len << endl;
  cout << "Record length: " << SPMRecord::length() << endl;
  cout << "Number of records: " << num_records << endl;

  db_file.seekg(ios_base::beg);
  SPMRecord record;
  int i;
  for (i = 0; i < 5; ++i) {
    record.readRecord(db_file);
    record.print();
  }

  db_file.seekg((num_records-5) * SPMRecord::length(), ios_base::beg);
  for (i = 0; i < 5; ++i) {
    record.readRecord(db_file);
    record.print();
  }
}
