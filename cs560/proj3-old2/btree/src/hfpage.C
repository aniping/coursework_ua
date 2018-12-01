#include <iostream>
#include <stdlib.h>
#include <memory.h>

#include "hfpage.h"
#include "buf.h"
#include "db.h"


//***********************************************************
//-----------------------------------------------------------
//NEW: if found, return slot_id (index (from 0) in slot array, including the first one). Otherwise, return -1
/*short HFPage::find_empty_slot()
{
	if (slotCnt == 0)
		return 0;
	
	int i, j;
	slot_t *slot_iter;
	for (i = 1; i != slotCnt; ++i)
	{
		//j = data + (i - 1) * sizeof(slot_t);
		//slot_iter = (slot_t *)j;
		slot_iter = &(slot[i]);
		if (slot_iter->length == EMPTY_SLOT)
			return i;
	}
	
	if (i == slotCnt)
		return -1;
}

//---------------------------------------------------------
//given an rid, check whether the slot is valid or not (<slotCnt, nonempty, etc.) Return 1 if valid, 0 if empty, -1 if >= slotCnt.
int HFPage::check_valid_rid(RID rid)
{
	if (rid.slotNo < 0 || rid.slotNo >= slotCnt)
		return -1;
		
	//slot_t *slot_ptr = (slot_t *)(data + (rid.slotNo - 1) * sizeof(slot_t));
	slot_t *slot_ptr = &(slot[rid.slotNo]);
	if (slot_ptr->length == EMPTY_SLOT)
		return 0;
	else
		return 1;
}
*/
// **********************************************************
// page class constructor

void HFPage::init(PageId pageNo)
{
  	// fill in the body
	slotCnt = 0;	
	usedPtr = MAX_SPACE - DPFIXED;	//没有record，所以为最后一个index
	freeSpace = MAX_SPACE - DPFIXED + sizeof(slot_t);
	type = 0;	//TODO: what does type describe?
	prevPage = INVALID_PAGE;	//TODO: not sure
	nextPage = INVALID_PAGE;	//TODO: not sure
	curPage = pageNo;
	slot[0].offset = usedPtr;
	slot[0].length = 0;
}

// **********************************************************
// dump page utlity
void HFPage::dumpPage()
{
	int i;

	cout << "dumpPage, this: " << this << endl;
    cout << "curPage= " << curPage << ", nextPage=" << nextPage
         << ", prevPage=" << prevPage << endl;
	cout << "usedPtr=" << usedPtr << ",  freeSpace=" << freeSpace
	 << ", slotCnt=" << slotCnt << endl;

    // for (i=0; i < slotCnt; i++) {
    // cout << "slot["<< i <<"].offset=" << slot[i].offset
    //      << ", slot["<< i << "].length=" << slot[i].length << endl;
    // }
}

// **********************************************************
PageId HFPage::getPrevPage()
{
    	// fill in the body
    	return prevPage;
}

// **********************************************************
void HFPage::setPrevPage(PageId pageNo)
{
    	// fill in the body
    	prevPage = pageNo;
}

// **********************************************************
PageId HFPage::getNextPage()
{
    	// fill in the body
    	return nextPage;
}

// **********************************************************
void HFPage::setNextPage(PageId pageNo)
{
	// fill in the body
	nextPage = pageNo;
}

// **********************************************************
// Add a new record to the page. Returns OK if everything went OK
// otherwise, returns DONE if sufficient space does not exist
// RID of the new record is returned via rid parameter.
Status HFPage::insertRecord(char* recPtr, int recLen, /*out*/RID& rid)
{
    	// fill in the body
    	/*//calculate space
    	int space_begin = sizeof(slot_t) * (slotCnt - 1);
    	slot_t last_slot = (slot_t)(sizeof(slot_t) * (slotCnt - 2);
    	int space_end = last_slot.offset;
    	int curr_space = space_end - space_begin;
    	assert(curr_space >= 0);
    	int need_space = recLen + sizeof(slot_t);
    	 
    	if (need_space > curr_space)
    	{
    		return DONE;
    	}
    
    	//insert slot
    	//find empty slot
    	int sid = find_empty_slot();
    	if (sid != -1)
    	{
    		//write slot
    		
    	}
    	int rec_offset = space_end - recLen;
    	slot_t new_slot(rec_offset, recLen);
    	memcpy(data + space_begin, &new_slot, sizeof(slot_t));
    	slot
    	
    	//insert record
    	memcpy(data + rec_offset, recPtr, recLen);
    	
    	//compute rid
    	RID new_rid(curPage, )*/
    	
    	int need_space = recLen + sizeof(slot_t);
    	if (need_space > freeSpace)
    	{
    		return DONE;
    	}
    	
    	//insert record
    	int rec_offset = usedPtr - recLen;
    	memcpy(data + rec_offset, recPtr, recLen);
    	usedPtr = rec_offset;
    	freeSpace -= recLen;
    	
    	//insert slot
    	//int sid = find_empty_slot();
    	int sid;
    	if (slotCnt == 0)
    	{
		sid = 0;
	}
	else
	{
		int i, j;
		slot_t *slot_iter;
		for (i = 1; i != slotCnt; ++i)
		{
			//j = data + (i - 1) * sizeof(slot_t);
			//slot_iter = (slot_t *)j;
			slot_iter = &(slot[i]);
			if (slot_iter->length == EMPTY_SLOT)
			{
				sid = i;
				break;
			}
		}
	
		if (i == slotCnt)
			sid = -1;
	}	
		
    	slot_t new_slot;
    	new_slot.offset = rec_offset;
    	new_slot.length = recLen;
    	//(rec_offset, recLen);
    	int slot_offset;
    	if (sid != -1)
    	{
    		//write slot
    		if (sid == 0)	
    		{
    			//the first slot, in metadata section
    			slot[0].offset = new_slot.offset;
    			slot[0].length = new_slot.length;
    			if (slotCnt == 0)
    			{
    				++slotCnt;
    				freeSpace -= sizeof(slot_t);
    			}
    		}
    		else
    		{
    			//slot_offset = (sid - 1) * sizeof(slot_t);
    			memcpy(&(slot[sid]), &new_slot, sizeof(slot_t));
    		}
    		
    	}
    	else
    	{
    		//add a new slot
    		//slot_offset = (slotCnt - 1) * sizeof(slot_t);
    		memcpy(&(slot[slotCnt]), &new_slot, sizeof(slot_t));
    		slotCnt++;
    		freeSpace -= sizeof(slot_t);
    		sid = slotCnt - 1;
    	}
    	
    	//compute rid
    	rid.pageNo = curPage;
    	rid.slotNo = sid;
    	
    	return OK;
}

// **********************************************************
// Delete a record from a page. Returns OK if everything went okay.
// Compacts remaining records but leaves a hole in the slot array.
// Use memmove() rather than memcpy() as space may overlap.
Status HFPage::deleteRecord(const RID& rid)
{
	    // fill in the body
	    //int rid_test = check_valid_rid(rid);
	    int rid_test;
	    if (rid.slotNo < 0 || rid.slotNo >= slotCnt)
	    {
		rid_test = -1;
	    }
	    else
	    {
		//slot_t *slot_ptr = (slot_t *)(data + (rid.slotNo - 1) * sizeof(slot_t));
		slot_t *slot_ptr = &(slot[rid.slotNo]);
		if (slot_ptr->length == EMPTY_SLOT)
			rid_test = 0;

		else
			rid_test = 1;
	    }
		
	    if (rid_test == -1)
		return FAIL;
	    if (rid_test == 0)
		return DONE;
	    //clear slot
	    int sid = rid.slotNo;
	    short rm_offset;
	    short rm_length;
	    slot_t *rm_slot;
	    if (sid == 0)
	    {
	    	rm_offset = slot[0].offset;
	    	rm_length = slot[0].length;
	    	rm_slot = &slot[0];
	    }
	    else
	    {
	    	int slot_offset = (sid - 1) * sizeof(slot_t);
	    	rm_slot = (slot_t *)(data + slot_offset);
	    	rm_offset = rm_slot->offset;
	    	rm_length = rm_slot->length;
	    	
	    	//remove slot if it is the last one
		/*if (sid == slotCnt - 1)
		{
			--slotCnt;
		    	freeSpace += sizeof(slot_t);
		}*/
	    }
	    rm_slot->offset = -1;	
	    rm_slot->length = EMPTY_SLOT;
	    
	    //remove tail empty slots
	    int i, j, k;
	    k = slotCnt;
	    slot_t *slot_iter;
	    for (i = k - 1; i > 0; --i)
	    {
	    	//j = data + (i - 1) * sizeof(slot_t);
	    	//slot_iter = (slot_t *)j;
	    	slot_iter = &(slot[i]);
	    	if (slot_iter->length == EMPTY_SLOT)
	    	{
	    		--slotCnt;
	    		freeSpace += sizeof(slot_t);
	    	}
	    	else
	    	{
	    		break;
	    	}
	    }
	    if (i == 0 && slot[0].length == EMPTY_SLOT)
	    {
	    	--slotCnt;
	    }
	    if (slotCnt == 0)
	    	freeSpace += sizeof(slot_t);
	    
	    //move all subsequent records
	    memmove(data + usedPtr + rm_length, data + usedPtr, rm_offset - usedPtr);
	    
	    
	    //update the slot.offset of all subsequent records
	    //int i, j;
	    //slot_t *slot_iter;
	    for (i = 1; i < slotCnt; ++i)	//slot[0] does not follow any slot
	    {
	    	//j = data + (i - 1) * sizeof(slot_t);	//slot offset
	    	//slot_iter = (slot *)j;
	    	slot_iter = &(slot[i]);
	    	if (slot_iter->length != EMPTY_SLOT && slot_iter->offset < rm_offset)
	    		slot_iter->offset += rm_length ;
	    }
	    
	    
	    usedPtr = usedPtr + rm_length;
	    freeSpace += rm_length;
	    
	    return OK;
}

// **********************************************************
// returns RID of first record on page
Status HFPage::firstRecord(RID& firstRid)
{
	    // fill in the body
	    if (slotCnt <= 0)
	    	return DONE;
	    	
	    //find first nonempty slot
	    int i, j;
	    slot_t *slot_iter;
	    if (slot[0].length != EMPTY_SLOT)
	    {
	    	//slot_iter = &slot[0];
	    	i = 0;
	    }
	    else
	    {
	    	for (i = 1; i != slotCnt; ++i)
		{
			//j = data + (i - 1) * sizeof(slot_t);	//slot offset
		    	//slot_iter = (slot *)j;
		    	slot_iter = &(slot[i]);
		    	if (slot_iter->length != EMPTY_SLOT)
		    		break;
		}	
	    }
	    
	    firstRid.pageNo = curPage;
	    firstRid.slotNo = i;
	    return OK;
}

// **********************************************************
// returns RID of next record on the page
// returns DONE if no more records exist on the page; otherwise OK
Status HFPage::nextRecord (RID curRid, RID& nextRid)
{
	// fill in the body
	int curr_slot = curRid.slotNo;
	if (curr_slot == slotCnt - 1)
		return DONE;
	else if (curr_slot < 0 || curr_slot > slotCnt - 1)
		return FAIL;
		
	//iterate on subsequent slots
	int i, j;
	slot_t *slot_iter;
	for (i = curr_slot + 1; i < slotCnt; ++i)
	{
		//j = data + (i - 1) * sizeof(slot_t);	//slot offset
		//slot_iter = (slot *)j;
		slot_iter = &(slot[i]);
		if (slot_iter->length != EMPTY_SLOT)
			break;
	}
	
	if (i == slotCnt)
	{
		return DONE;
	}
	else
	{
		nextRid.pageNo = curPage;
		nextRid.slotNo = i;
	}
	return OK;
}

// **********************************************************
// returns length and copies out record with RID rid
Status HFPage::getRecord(RID rid, char* recPtr, int& recLen)
{
	// fill in the body
	//int rid_test = check_valid_rid(rid);
	int rid_test;
	if (rid.slotNo < 0 || rid.slotNo >= slotCnt)
	{
		rid_test = -1;
	}
	else
	{
		//slot_t *slot_ptr = (slot_t *)(data + (rid.slotNo - 1) * sizeof(slot_t));
		slot_t *slot_ptr = &(slot[rid.slotNo]);
		if (slot_ptr->length == EMPTY_SLOT)
			rid_test = 0;
		else
			rid_test = 1;
	}
	    
	if (rid_test == -1)
		return FAIL;
	if (rid_test == 0)
		return DONE;
	
	int sid = rid.slotNo;	
	//slot_t *slot_ptr = (slot_t *)(data + (sid - 1) * sizeof(slot_t));
	slot_t *slot_ptr = &(slot[sid]);
	char *rec_offset = data + slot_ptr->offset;
	memcpy(recPtr, rec_offset, slot_ptr->length);
	recLen = slot_ptr->length;
	
	return OK;
}

// **********************************************************
// returns length and pointer to record with RID rid.  The difference
// between this and getRecord is that getRecord copies out the record
// into recPtr, while this function returns a pointer to the record
// in recPtr.
Status HFPage::returnRecord(RID rid, char*& recPtr, int& recLen)
{
	// fill in the body
	//int rid_test = check_valid_rid(rid);
	int rid_test;
	if (rid.slotNo < 0 || rid.slotNo >= slotCnt)
	{
		rid_test = -1;
	}
	else
	{
		//slot_t *slot_ptr = (slot_t *)(data + (rid.slotNo - 1) * sizeof(slot_t));
		slot_t *slot_ptr = &(slot[rid.slotNo]);
		if (slot_ptr->length == EMPTY_SLOT)
			rid_test = 0;
		else
			rid_test = 1;
	}
	
	
	if (rid_test == -1)
		return FAIL;
	if (rid_test == 0)
		return DONE;
	
	int sid = rid.slotNo;	
	slot_t *slot_ptr = &(slot[sid]);//(slot_t *)(data + (sid - 1) * sizeof(slot_t));
	char *rec_offset = data + slot_ptr->offset;
	//memcpy(recPtr, rec_offset, slot_ptr->length);
	recPtr = rec_offset;
	recLen = slot_ptr->length;
	return OK;
}

// **********************************************************
// Returns the amount of available space on the heap file page
int HFPage::available_space(void)
{
	// fill in the body
	//if (slotCnt != 0)
		return freeSpace - sizeof(slot_t);
	//else
	//	return freeSpace;
	
	//return 0;
}

// **********************************************************
// Returns 1 if the HFPage is empty, and 0 otherwise.
// It scans the slot directory looking for a non-empty slot.
bool HFPage::empty(void)
{
	// fill in the body
	if (slotCnt == 0/* && slot[0].length == EMPTY_SLOT*/)
		return true;
	else
		return false;
	
}


