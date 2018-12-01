/*
 * key.C - implementation of <key,data> abstraction for BT*Page and 
 *         BTreeFile code.
 *
 * Gideon Glass & Johannes Gehrke  951016  CS564  UW-Madison
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */

#include <string.h>
#include <assert.h>

#include "bt.h"

/*
 * See bt.h for more comments on the functions defined below.
 */

/*
 * Reminder: keyCompare compares two keys, key1 and key2
 * Return values:
 *   - key1  < key2 : negative
 *   - key1 == key2 : 0
 *   - key1  > key2 : positive
 */
int keyCompare(const void *key1, const void *key2, AttrType t)
{
  // put your code here
  if (t == attrInteger) {
    int key1_val = *(int *)key1;
    int key2_val = *(int *)key2;
    if (key1_val < key2_val) {
      return -1;
    } else if (key1_val == key2_val) {
      return 0;
    } else {
      return 1;
    }
  } else { // string type
    char *key1_val = (char *) key1;
    char *key2_val = (char *) key2;

    //printf("Comparing key %s with key %s\n", key1_val, key2_val);
    return strncmp(key1_val, key2_val, MAX_KEY_SIZE1);
    //return strcmp(key1_val, key2_val);
  }

  return 0;
}

/*
 * make_entry: write a <key,data> pair to a blob of memory (*target) big
 * enough to hold it.  Return length of data in the blob via *pentry_len.
 *
 * Ensures that <data> part begins at an offset which is an even 
 * multiple of sizeof(PageNo) for alignment purposes.
 */
void make_entry(KeyDataEntry *target,
                AttrType key_type, const void *key,
                nodetype ndtype, Datatype data,
                int *pentry_len)
{
  int entry_len = 0;

  /* fill in key (string or int) */
  if (key_type == attrInteger) {
    memcpy(target, key, sizeof(int));
    entry_len += sizeof(int) * 2; // for alignment purpose
  } else if (key_type == attrString) {
    memcpy(target, key, MAX_KEY_SIZE1);
    entry_len += MAX_KEY_SIZE1;
  } else {
    cout << "In key.C, make_entry(): wrong key type!"<< endl;
  }

  /* fill in data (pageid for index node, rid for leaf node) */
  if (ndtype == LEAF) { // leaf node stores rid
    memcpy((char *)target+entry_len, &(data.rid), sizeof(RID));
    entry_len += sizeof(RID);
    // cout << "insert RID: "<< data.rid.pageNo << ", " << data.rid.slotNo << endl;
  } else if (ndtype == INDEX) { // index node stores pageid
    memcpy((char *)target+entry_len, &(data.pageNo), sizeof(PageId));
    entry_len += sizeof(PageId);
  } else {
    cout << "In key.C, make_entry(): wrong node type!"<< endl;
  }

  //cout << "entry length: " << entry_len << endl;
  *pentry_len = entry_len;

  return;
}


/*
 * get_key_data: unpack a <key,data> pair into pointers to respective parts.
 * Needs a) memory chunk holding the pair (*psource) and, b) the length
 * of the data chunk (to calculate data start of the <data> part).
 */
void get_key_data(void *targetkey, Datatype *targetdata,
                  KeyDataEntry *psource, int entry_len, nodetype ndtype)
{
  if (entry_len == (2*sizeof(int) + sizeof(RID))) {
    // leaf entry: int key + rid
    memcpy(targetkey, psource, sizeof(int));
    memcpy(targetdata, (char *)psource+2*sizeof(int), sizeof(RID));
  } else if (entry_len == (2*sizeof(int) + sizeof(PageId))) {
    // index entry: int key + pageid
    memcpy(targetkey, psource, sizeof(int));
    memcpy(targetdata, (char *)psource+2*sizeof(int), sizeof(PageId));
  } else if (entry_len == (MAX_KEY_SIZE1 + sizeof(RID))) {
    // leaf entry: string key + rid
    memcpy(targetkey, psource, MAX_KEY_SIZE1);
    memcpy(targetdata, (char *)psource+MAX_KEY_SIZE1, sizeof(RID));
    //cout << "geting key data for string key\n";
  } else if (entry_len == (MAX_KEY_SIZE1 + sizeof(PageId))) {
    // index entry: string key + pageid
    memcpy(targetkey, psource, MAX_KEY_SIZE1);
    memcpy(targetdata, (char *)psource+MAX_KEY_SIZE1, sizeof(int));
    //cout << "geting key pid for string key\n";
  } else {
    cout << "In key.C, get_key_data(): wrong entry length " << entry_len << endl;
  }

  return;
}

/*
 * get_key_length: return key length in given key_type
 */
int get_key_length(const void *key, const AttrType key_type)
{
 // put your code here
 return 0;
}
 
/*
 * get_key_data_length: return (key+data) length in given key_type
 */   
int get_key_data_length(const void *key, const AttrType key_type, 
                        const nodetype ndtype)
{
 // put your code here
 return 0;
}
