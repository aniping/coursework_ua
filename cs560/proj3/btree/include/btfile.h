/*
 * btfile.h
 *
 * sample header file
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */
 
#ifndef _BTREE_H
#define _BTREE_H

#include "btindex_page.h"
#include "btleaf_page.h"
#include "index.h"
#include "btreefilescan.h"
#include "bt.h"

// Define your error code for B+ tree here
// enum btErrCodes  {...}

class BTreeFile: public IndexFile
{
  public:
    BTreeFile(Status& status, const char *filename);
    // an index with given filename should already exist,
    // this opens it.
    
    BTreeFile(Status& status, const char *filename, const AttrType keytype, const int keysize);
    // if index exists, open it; else create it.
    
    ~BTreeFile();
    // closes index
    
    Status destroyFile();
    // destroy entire index file, including the header page and the file entry
    
    Status insert2(const void *key, const RID rid);
    Status insert(const void *key, const RID rid);
    // insert <key,rid> into appropriate leaf page
    
    Status Delete(const void *key, const RID rid);
    // delete leaf entry <key,rid> from the appropriate leaf
    // you need not implement merging of pages when occupancy
    // falls below the minimum threshold (unless you want extra credit!)
    
    IndexFileScan *new_scan(const void *lo_key = NULL, const void *hi_key = NULL);
    // create a scan with given keys
    // Cases:
    //      (1) lo_key = NULL, hi_key = NULL
    //              scan the whole index
    //      (2) lo_key = NULL, hi_key!= NULL
    //              range scan from min to the hi_key
    //      (3) lo_key!= NULL, hi_key = NULL
    //              range scan from the lo_key to max
    //      (4) lo_key!= NULL, hi_key!= NULL, lo_key = hi_key
    //              exact match ( might not unique)
    //      (5) lo_key!= NULL, hi_key!= NULL, lo_key < hi_key
    //              range scan from lo_key to hi_key

    int keysize();
    void printLeafNode(BTLeafPage *leaf_ptr, int print_all);

    
  private:
    char fname[MAX_KEY_SIZE1];
    struct header_page {
        PageId root_pid; // page ID of the root page of B+tree
        nodetype root_type; // node type for the root page
        AttrType key_type;
        int key_size;
    };
    struct header_page *headerPage;
    PageId headerPID;

    struct _childentry {
        void *key; // smallest key of the new child
        PageId pid; // page ID of the new child
    };

    struct _leafentry {
        char key[MAX_KEY_SIZE1];
        RID rid;
    };

    /* helper functions */
    int getLeafEntrySize();
    int getIndexEntrySize();
    void insertEntry2LeafNode(PageId pid, const void *key,
                              const RID rid, _childentry*& newchild_entry);
    void insertEntry2LeafNode2(PageId pid, const void *key,
                               const RID rid, _childentry*& newchild_entry);
    void insertEntry2IndexNode(PageId pid, const void *key, const RID rid,
                               _childentry*& newchild_entry);
    Status deleteEntryFromIndexNode(PageId pid, const void *key, const RID rid);
    Status deleteEntryFromLeafNode(PageId pid, const void *key, const RID rid);


    void splitLeafNode(BTLeafPage *leaf_ptr, _childentry*& newchild_entry);
    void splitIndexNode(BTIndexPage *index_ptr, _childentry*& newchild_entry);
    PageId getLeftMostLeafPage();
    void advance2midpoint(Page *pptr, nodetype nt);

    /* for debug purpose */
    void printIndexNode(BTIndexPage *index_ptr);
    void printTree(PageId pid);
    void printKey(const char *prefix, const void *key);
    void printChildEntry(_childentry *entry);

};

#endif
