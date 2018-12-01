#include "Directory.hpp"
#include "SPMRecord.hpp"
#include <iostream>
#include <cstring>

Directory::Directory(string index_fname)
  : globalDepth(1)
  , offset2IndexFile(0)
{
  entries = new struct entry[getNumEntries()];

  // initialize the first 10 entries
  for (int i = 0; i < Directory::cardinality; ++i) {
    entries[i].prefix = to_string(i);
    entries[i].offset = -1;
  }

  // open the file and discard the existing content
  indexFileStream.open(index_fname, ios::out | ios::binary | ios::trunc);
  indexFileStream.close();

  // open it again for read/write
  indexFileStream.open(index_fname, ios::in | ios::out | ios::binary);

  writeIndexFileHeader();
}

Directory::Directory(string index_fname, bool read_only)
{
  // open it for read only
  indexFileStream.open(index_fname, ios::in | ios::out | ios::binary);
  if (!indexFileStream.good()) {
    cout << "bad input stream!" << endl;
    cout << indexFileStream.good() << indexFileStream.eof()
         << indexFileStream.fail() << indexFileStream.bad() << endl;
  }
  readIndexFileHeader();

  indexFileStream.seekg(0, ios::end);
  indexFileSize = indexFileStream.tellg();

  indexFileStream.seekg(offset2IndexFile, ios::beg);
  bucketsSize = indexFileStream.tellg();
  bucketsSize -= headerSize;
  dirSize = indexFileSize - bucketsSize - headerSize;
}

void
Directory::dumpDirEntries()
{
  writeIndexFileHeader(); // update header

  indexFileStream.seekp(0, ios_base::end);
  for (int i = 0; i < pow(cardinality, globalDepth); ++i) {
    if (entries[i].offset >= 0) { // skip those entries pointing to no bucket
      const char *prefix = entries[i].prefix.c_str();
      indexFileStream.write(prefix, globalDepth + 1); // null terminated c string
      indexFileStream.write(reinterpret_cast<const char *>(&(entries[i].offset)),
                            sizeof(int));
    }
  }
}

void
Directory::loadDirEntries()
{
  int num_entries = dirSize/getEntrySize();
  // allocate space for dir entries
  entries = new struct entry[num_entries];

  int offset;
  char *prefix_cs = new char[globalDepth+1];
  for (int i = 0; i < num_entries; ++i) {
    indexFileStream.read(prefix_cs, globalDepth+1);
    indexFileStream.read(reinterpret_cast<char *>(&offset), sizeof(int));
    string prefix(prefix_cs);
    entries[i].prefix = prefix;
    entries[i].offset = offset;
  }
}

void
Directory::writeIndexFileHeader()
{
  indexFileStream.seekp(0, ios_base::end);
  offset2IndexFile = indexFileStream.tellp();

  indexFileStream.seekp(0, ios_base::beg);
  indexFileStream.write(reinterpret_cast<const char *>(&globalDepth), sizeof(int));
  indexFileStream.write(reinterpret_cast<const char *>(&offset2IndexFile), sizeof(int));
}

void
Directory::readIndexFileHeader()
{
  indexFileStream.seekg(0, ios_base::beg);
  indexFileStream.read(reinterpret_cast<char *>(&globalDepth), sizeof(int));
  indexFileStream.read(reinterpret_cast<char *>(&offset2IndexFile), sizeof(int));
}

void
Directory::buildIndex(uint32_t objID, uint32_t offset)
{
  // extract key prefix
  string key_prefix = to_string(objID).substr(0, globalDepth);
  int index = locateEntry(key_prefix);

  int bucket_offset = entries[index].offset;
  if (bucket_offset < 0) { // no bucket created for this entry
    HashBucket bucket(1); // local depth always starts at 1
    bucket.appendSlot(objID, offset);

    // append the newly created bucket to the end of the index file
    indexFileStream.seekp(0, ios_base::end);
    entries[index].offset = indexFileStream.tellp(); // update the offset field
    bucket.dumpBucket(indexFileStream, entries[index].offset);
  } else {
    HashBucket bucket;
    // bring the bucket into memory
    bucket.fetchBucket(indexFileStream, entries[index].offset);

    if (!bucket.isFull()) { // bucket is not full yet
      bucket.appendSlot(objID, offset);
      bucket.dumpBucket(indexFileStream, entries[index].offset);
    } else {
      if (globalDepth == bucket.getLocalDepth()) { // "double" directory and split bucket
        string key_prefix_before(key_prefix);
        // split the bucket
        map<string, HashBucket *> *prefix2bucket = splitBucket_V1(key_prefix_before, bucket);
        doubleDirectory(prefix2bucket, key_prefix_before);
        // recursive call to build the index
        buildIndex(objID, offset);
        //this->printDirectory();
      } else if (globalDepth > bucket.getLocalDepth()) {
        // split the bucket without "doubling the directory"

        // local depth before splitting
        int local_depth_before = bucket.getLocalDepth();

        // key prefix that we are splitting on. e.g, '3' split into '30, 31, ..., 39'
        string key_prefix_before = to_string(objID).substr(0, local_depth_before);
        map<string, HashBucket *> *prefix2bucket = splitBucket_V2(key_prefix_before,
                                                                  bucket);
        string prefix = key_prefix.substr(0, key_prefix.size()-1);
        // update the directory entries and write split buckets to file
        for (char c = '0'; c <= '9'; ++c) {
          string bucket_index = key_prefix_before + c;
          HashBucket *bucket = (*prefix2bucket)[bucket_index];

          string search_key(bucket_index);
          for (int j = local_depth_before+1; j < globalDepth; ++j) {
            search_key += '0';
          }
          int consec_entries = pow(cardinality, globalDepth-local_depth_before-1);
          int entry_index = locateEntry(search_key);

          if (c == '0') {
            bucket->dumpBucket(indexFileStream, entries[entry_index].offset); // resue
          } else {
            indexFileStream.seekp(0, ios_base::end);
            entries[entry_index].offset = indexFileStream.tellp();
            int offset = entries[entry_index].offset; // remember the offset
            bucket->dumpBucket(indexFileStream, entries[entry_index].offset);

            for (int i = entry_index+1; i < (consec_entries+entry_index); ++i) {
              entries[i].offset = offset;
            }
          }
        }

        // recursive call to build the index
        buildIndex(objID, offset);
      }
    }
  }
}

void
Directory::doubleDirectory(map<string, HashBucket *> *prefix2bucket,
                           string key_prefix_before)
{
  int num_entries = getNumEntries(); // number of entries before doubling
  globalDepth++; // increment the global depth

  // allocate space for new entries
  struct entry *new_entries = new struct entry[getNumEntries()];

  for (int i = 0; i < num_entries; ++i) {
    if (entries[i].prefix != key_prefix_before) {
      for (char c = '0'; c <= '9'; ++c) {
        // construct new key prefix
        string new_key_prefix(entries[i].prefix);
        new_key_prefix += c;

        new_entries[i*cardinality + c-'0'].prefix = new_key_prefix; // update prefix
        // all 10 new entries point to the same bucket
        new_entries[i*cardinality + c-'0'].offset = entries[i].offset;
      }
    } else {
      for (char c = '0'; c <= '9'; ++c) {
        // construct new key prefix
        string new_key_prefix(key_prefix_before);
        new_key_prefix += c;

        new_entries[i*cardinality + c-'0'].prefix = new_key_prefix; // update key prefix
        HashBucket *bucket = (*prefix2bucket)[new_key_prefix]; // new bucket caused by split
        if (c == '0') {
          // the first bucket that is being split will reuse the original bucket,
          // but need to rewrite the content
          new_entries[i*cardinality + c-'0'].offset = entries[i].offset;
          bucket->dumpBucket(indexFileStream, entries[i].offset);
        } else {
          // the other split buckets will be appended at the end of the file
          indexFileStream.seekp(0, ios_base::end);
          new_entries[i*cardinality + c-'0'].offset = indexFileStream.tellp();
          bucket->dumpBucket(indexFileStream, new_entries[i*cardinality + c-'0'].offset);
        }
      }
    }
  }

  delete[] entries; // delete the original entries
  entries = new_entries; // update with new entries
}

map<string, HashBucket *> *
Directory::splitBucket_V1(string key_prefix_before, HashBucket& bucket)
{
  map<string, HashBucket *> *prefix2bucket = new map<string, HashBucket *>();
  int local_depth = bucket.getLocalDepth() + 1; // increment local depth

  for (char c = '0'; c <= '9'; ++c) {
    string key_prefix_after(key_prefix_before);
    key_prefix_after += c;
    (*prefix2bucket)[key_prefix_after] = bucket.redistributeBucket(local_depth,
                                                                   key_prefix_after);
    //(*prefix2bucket)[key_prefix_after]->printBucket();
  }

  return prefix2bucket;
}

map<string, HashBucket *> *
Directory::splitBucket_V2(string key_prefix_before, HashBucket& bucket)
{
  map<string, HashBucket *> *prefix2bucket = new map<string, HashBucket *>();
  int local_depth = bucket.getLocalDepth() + 1; // increment local depth

  for (char c = '0'; c <= '9'; ++c) {
    string key_prefix_after(key_prefix_before);
    key_prefix_after += c;
    // cout << "key prefix after: " << key_prefix_after << endl;
    (*prefix2bucket)[key_prefix_after] = bucket.redistributeBucket(local_depth,
                                                                   key_prefix_after);
    //(*prefix2bucket)[key_prefix_after]->printBucket();
  }

  return prefix2bucket;
}

int
Directory::locateEntry(string key_prefix)
{
  int index = -1, low, high, mid;
  low = 0; high = getNumEntries() - 1;
  while (low <= high) {
    mid = (high - low) / 2 + low;
    if (key_prefix.compare(entries[mid].prefix) == 0) {
      index = mid;
      break;
    } else if (key_prefix.compare(entries[mid].prefix) < 0) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return index;
}

void
Directory::printDirectory() const
{
  cout << "--------Beginning of directory structure---------" << endl;
  cout << "Global depth: " << globalDepth << endl;
  cout << "First 10 entries:" << endl;
  for (int i = 0; i < 10; ++i) {
    printEntry(entries[i]);
  }

  cout << "\nLast 10 entries:" << endl;
  for (int i = getNumEntries() - 10; i < getNumEntries(); ++i) {
    printEntry(entries[i]);
  }
  cout << "--------end of directory structure---------" << endl;
}

void
Directory::printStats()
{
  int num_buckets = bucketsSize/HashBucket::bucketSize();
  double bucket_occupancy_sum = 0.0;
  uint64_t spm_sum = 0;

  cout << "Global depth: " << globalDepth << endl;
  //cout << "Offset: " << offset2IndexFile << endl;
  cout << "Index file size: " << indexFileSize << " bytes" << endl;
  cout << "Directory size: " << dirSize << " bytes" << endl;
  cout << "Buckets size: " << bucketsSize << " bytes" << endl;
  //  cout << "Bucket size: " << HashBucket::bucketSize() << " bytes" << endl;
  cout << "Number of directory entries pointing to buckets: " << dirSize/getEntrySize() << endl;
  cout << "Number of buckets: " << num_buckets << endl;

  int num_index_entries = 0;
  for (int i = 0; i < num_buckets; ++i) {
    HashBucket bucket;
    bucket.fetchBucket(indexFileStream, i * HashBucket::bucketSize() + headerSize);
    bucket_occupancy_sum += (double) bucket.getNumFilledSlots() / HashBucket::MaxNumSlots;
    for (int j = 0; j < bucket.getNumFilledSlots(); ++j) {
      spm_sum += bucket.getObjID(j);
    }
    num_index_entries += bucket.getNumFilledSlots();
  }

  cout << "Average bucket occupancy: "
       << 100 * bucket_occupancy_sum/num_buckets
       << "%" << endl;
  cout << "Arithmetic mean of SPM values: " << spm_sum / num_index_entries << endl;
}

int
Directory::query_v1(fstream& db_file, string search_key)
{
  // since length of search_key < globalDepth, there could
  // be more than one bucket that possibly contain the matching records.

  // find the first matching bucket.
  // e.g., search key is '34', global depth is 4, then the first
  // match would be '3400'
  string tmp_key(search_key);
  for (int j = search_key.size(); j < globalDepth; ++j) {
    tmp_key += '0';
  }
  int entry_index = locateEntry(tmp_key);
  if (entry_index < 0) {
    return -1;
  } else {
    int num_matches = 0;
    // number of consecutive matching entries, e.g., '3401,3402,...,3499'
    int consec_entries = pow(cardinality, globalDepth-search_key.size());
    cout << "# of consecutive entries: " << consec_entries << endl;
    HashBucket bucket;
    int counter = 0;
    for (int i = 0; i < consec_entries; ) {
      bucket.fetchBucket(indexFileStream, entries[i+entry_index].offset);

      int local_depth = bucket.getLocalDepth();
      // ignore the entries that point to the same bucket
      i += pow(cardinality, globalDepth - local_depth);

      // every slot of the found bucket should be a match
      SPMRecord record;
      for (int j = 0; j < bucket.getNumFilledSlots(); ++j) {
        if (num_matches < 50) {
          record.fetchRecord(db_file, bucket.getOffset(j));
          record.print();
        }
        num_matches++;
      }
      //num_matches += bucket.getNumFilledSlots();
    }
    return num_matches;
  }
}

int
Directory::query_v2(fstream& db_file, string search_key)
{
  // since length of search_key == globalDepth, at most one
  // bucket could possibly contain the matching records.
  int entry_index = locateEntry(search_key);

  if (entry_index < 0) {
    return -1;
  }

  int num_matches = 0;
  HashBucket bucket;
  bucket.fetchBucket(indexFileStream, entries[entry_index].offset);

  // every slot of the found bucket should be a match
  for (int i = 0; i < bucket.getNumFilledSlots(); ++i) {
    SPMRecord record;
    record.fetchRecord(db_file, bucket.getOffset(i));
    record.print();
    num_matches++;
  }

  return num_matches;
}

int
Directory::query_v3(fstream& db_file, string search_key)
{
  // since length of search_key > globalDepth, at most one
  // bucket could possibly contain the matching records.
  string search_key_prefix = search_key.substr(0, globalDepth);
  int entry_index = locateEntry(search_key_prefix);

  if (entry_index < 0) {
    return -1;
  }

  HashBucket bucket;
  int num_matches = 0;
  bucket.fetchBucket(indexFileStream, entries[entry_index].offset);
  for (int i = 0; i < bucket.getNumFilledSlots(); ++i) {
    int obj_id = bucket.getObjID(i);
    if (search_key == to_string(obj_id).substr(0, search_key.size())) {
      SPMRecord record;
      record.fetchRecord(db_file, bucket.getOffset(i));
      record.print();
      num_matches++;
    }
  }
  return num_matches;
}

void
Directory::query(fstream& db_file, string search_key)
{
  int num_matches;
  if (search_key.size() < globalDepth) {
    num_matches = query_v1(db_file, search_key);
  } else if (search_key.size() == globalDepth) {
    num_matches = query_v2(db_file, search_key);
  } else {
    num_matches = query_v3(db_file, search_key);
  }

  if (num_matches <= 0) {
    cout << "No match found for the search key: " << search_key << endl;
  } else {
    cout << "Number of matches for the key (" << search_key
         << "): " << num_matches << endl;
  }
}
