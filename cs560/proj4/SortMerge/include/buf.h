///////////////////////////////////////////////////////////////////////////////
/////////////  The Header File for the Buffer Manager /////////////////////////
///////////////////////////////////////////////////////////////////////////////


#ifndef BUF_H
#define BUF_H

#include "db.h"
#include "page.h"
#include "new_error.h"
#include <stack>
#include <queue>
#include <algorithm>

#define NUMBUF 20   
// Default number of frames, artifically small number for ease of debugging.

#define HTSIZE 7
// Hash Table size



/*******************ALL BELOW are purely local to buffer Manager********/

// You could add more enums for internal errors in the buffer manager.
enum bufErrCodes  {HASHMEMORY, HASHDUPLICATEINSERT, HASHREMOVEERROR, HASHNOTFOUND, QMEMORYERROR, QEMPTY, INTERNALERROR, 
                   BUFFERFULL, BUFMGRMEMORYERROR, BUFFERPAGENOTFOUND, BUFFERPAGENOTPINNED, BUFFERPAGEPINNED};

class Replacer; // may not be necessary as described below in the constructor


class HashTable {
 private:
    // Hash table entry
    struct HTEntry {
        PageId pid;
        int frame_num;
        HTEntry *next; // the next entry in this bucket
    };

    // array of hash buckets. Each bucket is a list of hash table entries.
    // NULL means empty bucket.
    HTEntry *entries[HTSIZE];

    // hash function, given a page id, return the bucket number (0 ~ HTSize - 1)
    // that the page id belongs to.
    int hash(int page_id)
    {
        return (3 * page_id + 5) % HTSIZE;
    }

 public:
    void init()
    {
        for (int i = 0; i < HTSIZE; ++i) {
            entries[i] = NULL; // empty bucket
        }
    }

    int insert(PageId pid, int frame_num)
    {
        int bucket = hash(pid);
        if (entries[bucket] == NULL) { // empty bucket
            // entries[bucket] = (HTEntry *) malloc(sizeof(HTEntry));
            entries[bucket] = new HTEntry;
            entries[bucket]->pid = pid;
            entries[bucket]->frame_num = frame_num;
            entries[bucket]->next = NULL;
        } else {
            //cout << "trying to insert page " << pid << ", frame " << frame_num << endl;
            HTEntry *entry = entries[bucket];
            while (entry->next != NULL) {
                entry = entry->next;
            }
            entry->next = new HTEntry;
            entry->next->pid = pid;
            entry->next->frame_num = frame_num;
            entry->next->next = NULL;
        }

        return bucket;
    }

    // return the frame number that contains the page 'pid'.
    int lookup(PageId pid)
    {
        int bucket = hash(pid);
        if (entries[bucket] == NULL) {
            return -1; // does not contain the requested page
        } else {
            HTEntry *entry = entries[bucket];
            while (entry != NULL) {
                if (entry->pid == pid) {
                    return entry->frame_num;
                }
                entry = entry->next;
            }
        }
        return -1;
    }
    void remove(PageId pid)
    {
        int bucket = hash(pid);
        if (entries[bucket] == NULL) {
            cout << "Error: trying to remove a non-existent entry from the hash table" << endl;
        } else {
            HTEntry *entry = entries[bucket];
            HTEntry *prev_entry = NULL;
            HTEntry *tmp_entry;
            while (entry != NULL) {
                if (entry->pid == pid) {
                    tmp_entry = entry;
                    if (prev_entry == NULL) { // delete the first slot on the list
                        if (entry->next == NULL) { // no more slots
                            entries[bucket] = NULL;
                        } else {
                            entries[bucket] = entry->next;
                        }
                    } else {
                        prev_entry->next = entry->next;
                    }
                    free(tmp_entry);
                }
                prev_entry = entry;
                entry = entry->next;
            }
        }
    }

    void printBucket(int bucket_num)
    {
        cout << "Bucket " << bucket_num << ": ";
        HTEntry *entry = entries[bucket_num];
        while (entry != NULL) {
            cout << "(" << "page:" << entry->pid << ", frame:" << entry->frame_num << ")";
            entry = entry->next;
        }
        cout << "\n";
    }

    void dump()
    {
        cout << "Dumping the content of the hast table..." << endl;
        for (int i = 0; i < HTSIZE; ++i) {
            printBucket(i);
        }
    }
};

class BufMgr {

 private:
    /**
     * Buffer frame descriptor.
     */
    struct FrameDesc {
        PageId pid;
        int pin_count;
        int dirtybit;
    };

    unsigned int numBuffers;
    FrameDesc *frameDesc;

    HashTable htable;

    /* for frame replacement policy */
    stack<int> hatedMRU; // most-recently-used list of hated frames
    queue<int> lovedLRU; // least-recently-used list of love frames

    void printFrameDescEntry(int entry)
    {
        cout << "frame " << entry << ": (page: " << frameDesc[entry].pid
             << ", pin count: " << frameDesc[entry].pin_count << ", dirty: "
             << frameDesc[entry].dirtybit << ")" << endl;
    }

    void printFrameDesc()
    {
        for (unsigned int i = 0; i < numBuffers; ++i) {
            printFrameDescEntry(i);
        }
    }

 public:
    Page* bufPool; // The actual buffer pool

    BufMgr (int numbuf, Replacer *replacer = 0); 
   	// Initializes a buffer manager managing "numbuf" buffers.
	// Disregard the "replacer" parameter for now. In the full 
  	// implementation of minibase, it is a pointer to an object
	// representing one of several buffer pool replacement schemes.

    ~BufMgr();           // Flush all valid dirty pages to disk

    Status pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage=FALSE);
    // Check if this page is in buffer pool, otherwise
    // find a frame for this page, read in and pin it.
    // also write out the old page if it's dirty before reading
    // if emptyPage==TRUE, then actually no read is done to bring
    // the page

    Status unpinPage(PageId globalPageId_in_a_DB, int dirty, int hate);
    // hate should be TRUE if the page is hated and FALSE otherwise
    // if pincount>0, decrement it and if it becomes zero,
    // put it in a group of replacement candidates.
    // if pincount=0 before this call, return error.

    Status newPage(PageId& firstPageId, Page*& firstpage, int howmany=1); 
    // call DB object to allocate a run of new pages and
    // find a frame in the buffer pool for the first page
    // and pin it. If buffer is full, ask DB to deallocate
    // all these pages and return error

    Status freePage(PageId globalPageId); 
    // User should call this method if it needs to delete a page
    // this routine will call DB to deallocate the page

    Status flushPage(PageId pageid);
    // Used to flush a particular page of the buffer pool to disk
    // Should call the write_page method of the DB class

    Status flushAllPages();
	// Flush all pages of the buffer pool to disk, as per flushPage.

    /*** Methods for compatibility with project 1 ***/
    Status pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage, const char *filename);
    // Should be equivalent to the above pinPage()
	// Necessary for backward compatibility with project 1

    //    Status unpinPage(PageId globalPageId_in_a_DB, int dirty, const char *filename);
    Status unpinPage(int globalPageId_in_a_DB,
                     int dirty=FALSE, const char *filename=NULL);
	// Should be equivalent to the above unpinPage()
	// Necessary for backward compatibility with project 1
    
    unsigned int getNumBuffers() const
    {
        return numBuffers;
    }
	// Get number of buffers

    unsigned int getNumUnpinnedBuffers();
	// Get number of unpinned buffers
};

#endif
