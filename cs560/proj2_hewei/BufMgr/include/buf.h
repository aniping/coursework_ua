///////////////////////////////////////////////////////////////////////////////
/////////////  The Header File for the Buffer Manager /////////////////////////
///////////////////////////////////////////////////////////////////////////////


#ifndef BUF_H
#define BUF_H

#include "db.h"
#include "page.h"
#include "new_error.h"
//#include "system_defs.h"
#include <list>

#define NUMBUF 20   
// Default number of frames, artifically small number for ease of debugging.

#define HTSIZE 7
// Hash Table size

using namespace std;

/*******************ALL BELOW are purely local to buffer Manager********/
//extern DB *GlobalDB;
//-----------------------------------------------------------------------
struct Descr
{
	PageId page_number;
	int pin_count;
	bool dirtybit;
	
	Descr()
	{
		page_number = -1;
		pin_count = 0;
		dirtybit = false;
	}
	
	Descr(PageId pid, int pc, bool dbit)
	{
		page_number = pid;
		pin_count = pc;
		dirtybit = dbit;
	}
};
//-----------------------------------------------------------------------
struct HashEntry
{
	int page_id;
	int frame_id;
	
	HashEntry()
	{
		page_id = -1;
		frame_id = -1;
	}
	
	HashEntry(int pid, int fid)
	{
		page_id = pid;
		frame_id = fid;
	}
	
	~HashEntry()
	{
		//page_id = -1;
		//frame_id = -1;
	}
};

typedef list<HashEntry> Bucket;
//const int HTSIZE = 57;

class HashTable
{
public:
	int findFrameNumber(PageId pid);	//return framenumber, if not in buff, return -1
	
	Status insertHashEntry(HashEntry entry);
	
	Status deleteHashEntry(PageId pid);
	
private:
	int hash_function(PageId pid);		//return the index of bucket
	
	void print_elements(Bucket *bukt, int bn);
	
	Bucket directory[HTSIZE];
	int bucket_cnt;
};

//HashTable HTable;
//-----------------------------------------------------------------------
class MRU_List
{
public:
	bool add_to_list(PageId pid);	//return true if successful, false if fail
	
	PageId pop_from_list();		//return -1 if empty, delete when pop
	
	bool refresh(PageId pid);	//should first search for pid in the list. return true if successful, false if fail
	
	bool delete_from_list(PageId pid);
	
private:
	list<PageId> mru_ls;
};

//MRU_List LoveList;
//-----------------------------------------------------------------------
class LRU_List
{
public:
	bool add_to_list(PageId pid);	//return true if successful, false if fail
	
	PageId pop_from_list();		//return -1 if empty, delete when pop
	
	bool refresh(PageId pid);	//return true if successful, false if fail
	
	bool delete_from_list(PageId pid);
	
private:
	list<PageId> lru_ls;
};

//LRU_List HateList;
//-----------------------------------------------------------------------

// You could add more enums for internal errors in the buffer manager.
enum bufErrCodes  {HASHMEMORY, HASHDUPLICATEINSERT, HASHREMOVEERROR, HASHNOTFOUND, QMEMORYERROR, QEMPTY, INTERNALERROR, 
			BUFFERFULL, BUFMGRMEMORYERROR, BUFFERPAGENOTFOUND, BUFFERPAGENOTPINNED, BUFFERPAGEPINNED};

class Replacer; // may not be necessary as described below in the constructor


class BufMgr {

private: 
   unsigned int    numBuffers;
   // fill in this area
   unsigned int curr_numbuf;	//current number of buffer in use
   Descr *bufDescr;	//array of descriptors, always keep compacted
   
   //Status find_page_in_buff(PageId pid, int &fid, Page*& page);	//scan bufPool, page = NULL if not in buffer
   
   //Status find_descr_in_buff(PageId pid, int &fid, Descr *& des);	//scan bufDescr with pid, des = NULL if not in buffer
   
   Status find_empty_frame_in_buff(int &fid, Page*& page);	//scan bufDescr, page = NULL if no empty slot
   
   //Status insert_new_descr(PageId pid, int pin_count, bool dirtybit, int &did, Descr *& des);	//des = NULL if insert fail
   
public:
    Page* bufPool; // The actual buffer pool
    
    

    BufMgr (int numbuf, Replacer *replacer = 0); 
   	// Initializes a buffer manager managing "numbuf" buffers.
	// Disregard the "replacer" parameter for now. In the full 
  	// implementation of minibase, it is a pointer to an object
	// representing one of several buffer pool replacement schemes.

    ~BufMgr();           // Flush all valid dirty pages to disk

    Status pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage);
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

    Status unpinPage(PageId globalPageId_in_a_DB, int dirty, const char *filename);
	// Should be equivalent to the above unpinPage()
	// Necessary for backward compatibility with project 1
    
    unsigned int getNumBuffers() const { return numBuffers; }
	// Get number of buffers

    unsigned int getNumUnpinnedBuffers();
	// Get number of unpinned buffers
};

#endif
