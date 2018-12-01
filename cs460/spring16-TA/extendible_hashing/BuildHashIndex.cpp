#include <iostream>
#include <fstream>
#include <ctime>

#include "SPMRecord.hpp"
#include "Directory.hpp"

using namespace std;

const string db_file_name = "spmcat.bin";
const string index_file_name = "index.bin";

int
main(int argc, const char *argv[])
{
  // open DB file for read
  fstream db_file(db_file_name.c_str(), ios::in | ios::binary);

  Directory dir(index_file_name);

  db_file.seekg(0, ios_base::end); // move stream pointer to the end of the file
  int file_len = db_file.tellg(); // length of the DB file
  int num_records = file_len / SPMRecord::length();

  SPMRecord record;
  db_file.seekg(0); // reposition the stream pointer to the beginning
  //for (int i = 0; i < 5000; ++i) {
  time_t begin = time(NULL);
  for (int i = 0; i < num_records; ++i) {
    record.readRecord(db_file);
    dir.buildIndex(record.getObjID(), i * SPMRecord::length());
    if (i % 10000 == 0)
      cout << "Indexes has been built for " << i << " records." << endl;
  }
  dir.dumpDirEntries();
  time_t end = time(NULL);
  cout << "Time spent for building indexes: " << end-begin << " seconds." << endl;
}
