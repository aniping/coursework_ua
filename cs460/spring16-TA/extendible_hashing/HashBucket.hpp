#ifndef _HASHBUCKET_HPP_
#define _HASHBUCKET_HPP_

#include <fstream>

using namespace std;

/**
 * Hash bucket structure which contains a 8-byte header and
 * 50 slots. Each slot is 8 bytes with the first 4 byte being
 * the object ID of a record in the DB file and second 4 bytes
 * being the offset of the record within the DB file.
 */
class HashBucket {
public:
  HashBucket() {}

  HashBucket(int local_depth)
  {
    header.local_depth = local_depth;
    header.filled_slot_count = 0;
  }

  static int
  bucketSize()
  {
    return sizeof(header) + sizeof(struct _slot) * MaxNumSlots;
  }

  int
  getLocalDepth() const { return header.local_depth; }

  int
  getNumFilledSlots() const { return header.filled_slot_count; }

  bool
  isFull() const
  {
    return header.filled_slot_count >= MaxNumSlots ? true : false;
  }

  /**
   * Append a slot <objID, offset> to the bucket.
   * Must make sure the bucket is not full before doing this!
   */
  void
  appendSlot(uint32_t objID, uint32_t offset)
  {
    slots[header.filled_slot_count].objID = objID;
    slots[header.filled_slot_count].offset = offset;
    header.filled_slot_count++;
  }

  /**
   * Dump the content of hash bucket to the index file
   * at the byte offset 'offset'.
   */
  void
  dumpBucket(fstream& index_fstream, int offset);

  /**
   * Fetch the content of hash bucket from the index file
   * at the byte offset 'offset'.
   */
  void
  fetchBucket(fstream& index_fstream, int offset);

  /**
   * Redistribute the slots of the current bucket to a new bucket
   * with local depth as 'local_depth' and slots of objID that
   * starts with 'key_prefix_after'.
   *
   * Return the newly created bucket.
   */
  HashBucket *
  redistributeBucket(int local_depth, string key_prefix_after);

  int
  getObjID(int slot_index)
  {
    return slots[slot_index].objID;
  }

  int
  getOffset(int slot_index)
  {
    return slots[slot_index].offset;
  }

  void
  printBucket() const;

public:
  static const int MaxNumSlots = 50;

private:
  // header structure of the bucket
  struct __attribute__((packed)) _header {
    uint32_t filled_slot_count; // how many slots have been filled
    uint32_t local_depth; // local depth of the bucket
  } header;

  // slot structure of the bucket
  struct __attribute__((packed)) _slot {
    uint32_t objID; // object ID of a SPM record
    uint32_t offset; // offset to the record within the DB file
  } slots[MaxNumSlots];
};

#endif // _HASHBUCKET_HPP_
