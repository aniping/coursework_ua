/*
 * btreefilescan.h
 *
 * sample header file
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 *
 */
 
#ifndef _BTREEFILESCAN_H
#define _BTREEFILESCAN_H

#include "btfile.h"

// errors from this class should be defined in btfile.h

class BTreeFileScan : public IndexFileScan {
public:
    friend class BTreeFile;

    // get the next record
    Status get_next(RID & rid, void* keyptr);

    // delete the record currently scanned
    Status delete_current();

    int keysize(); // size of the key
    void init();

    // destructor
    ~BTreeFileScan();
private:
    int done; // indicate if scan is done or not
    void *key_lo;
    void *key_hi;
    PageId start_pid;
    PageId current_pid;
    RID cur_rid;
    AttrType key_type;
    int key_size;
    int rec_retrieved_in_cur_page;
    char *first_key;
    RID first_rid;
    char cur_key[MAX_KEY_SIZE1];
    //BTreeFile *bf;
};

#endif
