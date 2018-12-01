#include "HashBucket.hpp"
#include <iostream>

void
HashBucket::dumpBucket(fstream& index_fstream, int offset)
{
  index_fstream.seekp(offset, ios_base::beg);
  index_fstream.write(reinterpret_cast<const char *>(&header), sizeof(header));
  index_fstream.write(reinterpret_cast<const char *>(slots),
                      sizeof(struct _slot) * MaxNumSlots);
}

void
HashBucket::fetchBucket(fstream& index_fstream, int offset)
{
  index_fstream.seekg(offset, ios_base::beg);
  index_fstream.read(reinterpret_cast<char *>(&header), sizeof(header));
  index_fstream.read(reinterpret_cast<char *>(slots),
                     sizeof(struct _slot) * MaxNumSlots);
}

void
HashBucket::printBucket() const
{
  cout << "Header: local depth=" << header.local_depth
       << ", filled entries=" << header.filled_slot_count << endl;
  for (int i = 0; i < header.filled_slot_count; ++i) {
    cout << "(" << slots[i].objID << ", " << slots[i].offset << ")" << endl;
  }
}

HashBucket *
HashBucket::redistributeBucket(int local_depth, string key_prefix_after)
{
  HashBucket * new_bucket = new HashBucket(local_depth);
  for (int i = 0; i < MaxNumSlots; ++i) {
    string key_prefix = to_string(slots[i].objID).substr(0, local_depth);
    if (key_prefix == key_prefix_after) {
      new_bucket->appendSlot(slots[i].objID, slots[i].offset);
    }
  }

  return new_bucket;
}
