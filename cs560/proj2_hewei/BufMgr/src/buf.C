/*****************************************************************************/
/*************** Implementation of the Buffer Manager Layer ******************/
/*****************************************************************************/


#include "buf.h"

//#include "system_defs.h"
//extern DB *GlobalDB;

using namespace std;

// Define buffer manager error messages here
//enum bufErrCodes  {...};

// Define error message here
static const char* bufErrMsgs[] = { 
  // error message strings go here
  "Not enough memory to allocate hash entry",
  "Inserting a duplicate entry in the hash table",
  "Removing a non-existing entry from the hash table",
  "Page not in hash table",
  "Not enough memory to allocate queue node",
  "Poping an empty queue",
  "OOOOOOPS, something is wrong",
  "Buffer pool full",
  "Not enough memory in buffer manager",
  "Page not in buffer pool",
  "Unpinning an unpinned page",
  "Freeing a pinned page"
};

// Create a static "error_string_table" object and register the error messages
// with minibase system 
static error_string_table bufTable(BUFMGR,bufErrMsgs);

//*************************************************************
//** This is the implementation of BufMgr
//************************************************************
HashTable HTable;
MRU_List LoveList;
LRU_List HateList;

BufMgr::BufMgr (int numbuf, Replacer *replacer) {
  	// put your code here
  	numBuffers = numbuf;
  	bufDescr = (Descr *)malloc(sizeof(Descr) * numbuf);
  	bufPool = (Page *)malloc(sizeof(Page) * numbuf);
  	
  	int i;
  	for (i = 0; i < numbuf; ++i)
  		bufDescr[i].page_number = -1;
}

//*************************************************************
//** This is the implementation of ~BufMgr
//************************************************************
BufMgr::~BufMgr(){
  	// put your code here
  	int i;
  	Status sta;
  	Descr *des;
  	for (i = 0; i < numBuffers; ++i)
  	{
  		des = &bufDescr[i];
  		if (des->page_number != -1 && des->dirtybit == true)
  		{
  			sta = flushPage(des->page_number);
  		}
  	}
  	free(bufDescr);
}

//*************************************************************
//** This is the implementation of pinPage
//************************************************************
Status BufMgr::pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage) {
	// put your code here
	Status sta;
	Descr *des;
	//Page *page;
	int fid, hid;
	
	//sta = find_descr_in_buff(PageId_in_a_DB, fid, des);
	fid = HTable.findFrameNumber(PageId_in_a_DB);
	if (fid != -1)
	{
		//increment pin count
		des = &bufDescr[fid];
		des->pin_count++;
		page = &bufPool[fid];
	}
	else
	{
		if (curr_numbuf < numBuffers)
		{
			//need to: read page into buffer, insert new descr, insert new hash entry （no need to update lru and mru list here)
			
			sta = find_empty_frame_in_buff(fid, page);
			//TODO: read page into buffer
			if (emptyPage != true)
			{
				minibase_globals->GlobalDB->read_page(PageId_in_a_DB, page);
			}
			
			des = &bufDescr[fid];
			*des = Descr(PageId_in_a_DB, 1, false);
			
			HashEntry entry(PageId_in_a_DB, fid);
			HTable.insertHashEntry(entry);
			
			curr_numbuf++;
			printf("curr_numbuf = %d\n", curr_numbuf);
		}
		else
		{
			//need to: find a frame using replacement policy, flush dirty page, update old descr, delete old hash entry, read page into buffer, insert new hash entry
			PageId replace = 0;
			replace = HateList.pop_from_list();
			if (replace == -1)
				replace = LoveList.pop_from_list();
			if (replace == -1)
				return FAIL;
				
			fid = HTable.findFrameNumber(replace);
			des = &bufDescr[fid];
			if (des->dirtybit == true)
			{
				flushPage(replace);
			}
			*des = Descr(PageId_in_a_DB, 1, false);
			
			HTable.deleteHashEntry(replace);
			//TODO: read page into buffer
			if (emptyPage != true)
			{
				minibase_globals->GlobalDB->read_page(PageId_in_a_DB, &bufPool[fid]);
			}
			
			HashEntry entry(PageId_in_a_DB, fid);
			HTable.insertHashEntry(entry);
			page = &bufPool[fid];
		}
	}
	
	return OK;
}//end pinPage

//*************************************************************
//** This is the implementation of unpinPage
//************************************************************
Status BufMgr::unpinPage(PageId page_num, int dirty=FALSE, int hate = FALSE){
	// put your code here
	Status sta;
	Descr *des;
	Page *page;
	int fid, hid;
	
	//sta = find_descr_in_buff(PageId_in_a_DB, fid, des);
	fid = HTable.findFrameNumber(page_num);
	if (fid != -1)
	{
		//decrement pin count
		des = &bufDescr[fid];
		if (des->pin_count == 0)
			return MINIBASE_FIRST_ERROR(BUFMGR, 10);
		
		des->pin_count--;
		if (des->pin_count == 0)
		{
			if (dirty == true || des->dirtybit == true)
			{
				flushPage(page_num);
			}
			
			if (hate == true)
			{
				HateList.add_to_list(page_num);
			}
			else
			{
				LoveList.add_to_list(page_num);
			}
			//des->page_number = -1;
			//curr_numbuf--;
			//HTable.deleteHashEntry(page_num);
		}
	}
	else
	{
		return MINIBASE_FIRST_ERROR(BUFMGR, 9);
	}
	
	
	return OK;
}

//*************************************************************
//** This is the implementation of newPage
//************************************************************
Status BufMgr::newPage(PageId& firstPageId, Page*& firstpage, int howmany) {
	// put your code here
	Status sta;
	
	sta = minibase_globals->GlobalDB->allocate_page(firstPageId, howmany);
	if (sta != OK)
		return FAIL;
	
	if (curr_numbuf < numBuffers)
	{	
		pinPage(firstPageId, firstpage, 1);
		return OK;
	}
	else
	{
		sta = minibase_globals->GlobalDB->deallocate_page(firstPageId, howmany);
		return FAIL;
	}
	
	return OK;
}

//*************************************************************
//** This is the implementation of freePage
//************************************************************
Status BufMgr::freePage(PageId globalPageId){
	// put your code here
	//need to: delete hash entry, delete descr, delete from hate/love list, call GlobalDB to delete page
	
	int fid;
	Descr *des;
	
	fid = HTable.findFrameNumber(globalPageId);
	des = &bufDescr[fid];
	*des = Descr();
	curr_numbuf--;
	
	HTable.deleteHashEntry(globalPageId);
	
	LoveList.delete_from_list(globalPageId);
	HateList.delete_from_list(globalPageId);
	
	minibase_globals->GlobalDB->deallocate_page(globalPageId);
	
	return OK;
}

//*************************************************************
//** This is the implementation of flushPage
//************************************************************
Status BufMgr::flushPage(PageId pageid) {
	// put your code here
	int fid;
	Descr *des;
	
	fid = HTable.findFrameNumber(pageid);
	if (fid == -1)
		return FAIL;
		
	minibase_globals->GlobalDB->write_page(pageid, &bufPool[fid]);
	
	return OK;
}
    
//*************************************************************
//** This is the implementation of flushAllPages
//************************************************************
Status BufMgr::flushAllPages(){
	//put your code here
	int i;
	Descr *des;
	
	for (i = 0; i < numBuffers; ++i)
	{
		des = &bufDescr[i];
		if(des->dirtybit == true)
			flushPage(des->page_number);
	}
	return OK;
}


/*** Methods for compatibility with project 1 ***/
//*************************************************************
//** This is the implementation of pinPage
//************************************************************
Status BufMgr::pinPage(PageId PageId_in_a_DB, Page*& page, int emptyPage, const char *filename){
	//put your code here
	pinPage(PageId_in_a_DB, page, emptyPage);
	return OK;
}

//*************************************************************
//** This is the implementation of unpinPage
//************************************************************
Status unpinPage(PageId globalPageId_in_a_DB, int dirty, const char *filename){
	//put your code here
	unpinPage(globalPageId_in_a_DB, dirty, 0);
	return OK;
}

//*************************************************************
//** This is the implementation of getNumUnpinnedBuffers
//************************************************************
unsigned int BufMgr::getNumUnpinnedBuffers(){
	//put your code here
	return numBuffers - curr_numbuf;
	//return 0;
}

//*************************************************************
Status BufMgr::find_empty_frame_in_buff(int &fid, Page*& page)
{
	int i;
	Descr *des;
	
	for (i = 0; i < numBuffers; ++i)
	{
		des = &bufDescr[i];
		if (des->page_number == -1)
		{
			fid = i;
			page = &bufPool[fid];
			return OK;
		}
	}
	
	fid = -1;
	page = NULL;
	return FAIL;
}
//============================================================
bool MRU_List::add_to_list(PageId pid)
{
	mru_ls.push_front(pid);
	
	return true;
}
//------------------------------------------------------------
PageId MRU_List::pop_from_list()
{
	if (mru_ls.empty())
		return -1;
		
	PageId front = mru_ls.front();
	mru_ls.pop_front();
	
	return front;
}
//------------------------------------------------------------
bool MRU_List::refresh(PageId pid)
{
	mru_ls.remove(pid);
	mru_ls.push_front(pid);
	
	return true;
}
//------------------------------------------------------------
bool MRU_List::delete_from_list(PageId pid)
{
	mru_ls.remove(pid);
	
	return true;
}
//------------------------------------------------------------
bool LRU_List::add_to_list(PageId pid)
{
	lru_ls.push_front(pid);
	
	return true;
}
//------------------------------------------------------------
PageId LRU_List::pop_from_list()
{
	if (lru_ls.empty())
		return -1;
		
	PageId back = lru_ls.back();
	lru_ls.pop_back();
	
	return back;
}
//------------------------------------------------------------
bool LRU_List::refresh(PageId pid)
{
	lru_ls.remove(pid);
	lru_ls.push_front(pid);
	
	return true;
}
//------------------------------------------------------------
bool LRU_List::delete_from_list(PageId pid)
{
	lru_ls.remove(pid);
	
	return true;
}
//------------------------------------------------------------
int HashTable::findFrameNumber(PageId pid)
{
	int bn;
	bn = hash_function(pid);
	
	Bucket *bukt = &directory[bn];
	//print_elements(bukt, bn);
	list<HashEntry>::iterator iter;
	for (iter = bukt->begin(); iter != bukt->end(); ++iter)
	{
		if (iter->page_id == pid)
			return iter->frame_id;
	}
	
	return -1;
}
//------------------------------------------------------------
void HashTable::print_elements(Bucket *bukt, int bn)
{
	list<HashEntry>::iterator iter;
	for (iter = bukt->begin(); iter != bukt->end(); ++iter)
	{
		printf("bucket: %d, element: %d\n", bn, iter->page_id);
	}
}
//------------------------------------------------------------
Status HashTable::insertHashEntry(HashEntry entry)
{
	int bn;
	bn = hash_function(entry.page_id);
	
	Bucket *bukt = &directory[bn];
	bukt->push_back(entry);
	
	//print_elements(bukt, bn);
	
	return OK;
}
//-------------------------------------------------------------
Status HashTable::deleteHashEntry(PageId pid)
{
	int bn;
	bn = hash_function(pid);
	
	Bucket *bukt = &directory[bn];
	list<HashEntry>::iterator iter, iter_hold;
	for (iter = bukt->begin(); iter != bukt->end(); ++iter)
	{
		if (iter->page_id == pid)
		{
			bukt->erase(iter);
			print_elements(bukt, bn);
			return OK;
		}
	}
	
	return OK;
}
//------------------------------------------------------------
int HashTable::hash_function(PageId pid)
{
	return (pid + 57) % HTSIZE;
}



