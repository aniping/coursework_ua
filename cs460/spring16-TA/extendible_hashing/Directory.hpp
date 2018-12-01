#ifndef _DIRECTORY_HPP_
#define _DIRECTORY_HPP_

#include <string>
#include <fstream>
#include <cmath>
#include <map>

#include "SPMRecord.hpp"
#include "HashBucket.hpp"

using namespace std;

/**
 * Directory structure for extendible hashing index.
 */
class Directory {
public:
  static const int cardinality = 10; // cardinality of the alphabet set
  static const int headerSize = 2 * sizeof(int);

public:
  Directory(string index_fname);
  Directory(string index_fname, bool read_only);

  int
  getGlobalDepth() const { return globalDepth; }

  int
  getEntrySize() const { return globalDepth + 1 + sizeof(int); }

  int
  getNumEntries() const
  {
    if (offset2IndexFile > 0)
      return dirSize/getEntrySize();
    else
      return pow(cardinality, globalDepth);
  }

  /**
   * Build index for the record 'record' with the byte-offset
   * 'offset 'into the DB file.
   */
  void
  buildIndex(uint32_t objID, uint32_t offset);

  void
  readIndexFileHeader();

  void
  dumpDirEntries();

  void loadDirEntries();

  void
  printStats();

  void
  query(fstream& db_file, string search_key);

private:
  /**
   * Handle queries with length of 'search_key' less than global depth.
   */
  int
  query_v1(fstream& db_file, string search_key);

  /**
   * Handle queries with length of 'search_key' equals global depth.
   */
  int
  query_v2(fstream& db_file, string search_key);

  /**
   * Handle queries with length of 'search_key' greater than global depth.
   */
  int
  query_v3(fstream& db_file, string search_key);

  /**
   * Split hash bucket 'bucket' when its local depth equals with global depth.
   * This will cause the directory to double.
   * 'key_prefix_before' is the key prefix in the directory before the splitting.
   * e.g., if key_prefix_before is "32", then after splitting, there will be 10
   * entries in the directory: "320", "321", ... , "329".
   */
  map<string, HashBucket *> *
  splitBucket_V1(string key_prefix_before, HashBucket& bucket);

  /**
   * Split hash bucket 'bucket' when its local depth less than global depth.
   * This will not cause the directory to double.
   * 'key_prefix_before' is the key prefix we are splitting on. The length
   * of key_prefix_before is the local depth of the current bucket.
   *
   * e.g., if key_prefix_before is '2' and global depth is 3. We are splitting
   * the current bucket into 10 new buckets: '20', '21', '22' ..., '29'.
   * Then in the directory '200,201,...209' will point to new bucket '20',
   * '290, 291,...299' will point to new bucket '29'.
   */
  map<string, HashBucket *> *
  splitBucket_V2(string key_prefix_before, HashBucket& bucket);

  /**
   * Use binary search to find the entry with the prefix as
   * 'key_prefix'. Return the corresponding entry index.
   * We assume that such entry must exists.
   */
  int locateEntry(string key_prefix);

  /**
   * Double the directory which is caused by a bucket split when its
   * local depth is equal to the global depth.
   * 'prefix2bucket' is a map that maps keys to split buckets.
   * 'key_prefix_before' is the key prefix that caused the split.
   */
  void
  doubleDirectory(map<string, HashBucket *> *prefix2bucket,
                  string key_prefix_before);

  void
  writeIndexFileHeader();

private:
  int globalDepth;
  int offset2IndexFile; // byte-offset of the directory into the index file
  int indexFileSize;
  int bucketsSize; // byte size of the buckets in the index file
  int dirSize; // byte size of the directory in the index file

  // entry of the directory
  struct entry {
    string prefix; // key prefix
    int offset; // offset of the hash bucket the prefix refers to
                // -1 indicates this prefix doesn't refer to any bucket
  };

  // pointer to the array of entries. Note that entries must be
  // sorted by 'prefix' in order to perform binary search.
  struct entry *entries;
  fstream indexFileStream;

public:
  // For debugging
  void
  printDirectory() const;

  void
  printEntry(struct entry entry) const
  {
    cout << "key prefix:" << entry.prefix << ", offset to bucket:" << entry.offset << endl;
  }
};

#endif // _DIRECTORY_HPP_
