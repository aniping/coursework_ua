/*
 * sorted_page.C - implementation of class SortedPage
 *
 * Johannes Gehrke & Gideon Glass  951016  CS564  UW-Madison
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */

#include <stdlib.h>
#include <string.h>
#include "sorted_page.h"
#include "btindex_page.h"
#include "btleaf_page.h"

const char* SortedPage::Errors[SortedPage::NR_ERRORS] = {
  //OK,
  //Insert Record Failed (SortedPage::insertRecord),
  //Delete Record Failed (SortedPage::deleteRecord,
};


/*
 *  Status SortedPage::insertRecord(AttrType key_type, 
 *                                  char *recPtr,
 *                                    int recLen, RID& rid)
 *
 * Performs a sorted insertion of a record on an record page. The records are
 * sorted in increasing key order.
 * Only the  slot  directory is  rearranged.  The  data records remain in
 * the same positions on the  page.
 *  Parameters:
 *    o key_type - the type of the key.
 *    o recPtr points to the actual record that will be placed on the page
 *            (So, recPtr is the combination of the key and the other data
 *       value(s)).
 *    o recLen is the length of the record to be inserted.
 *    o rid is the record id of the record inserted.
 */

Status SortedPage::insertRecord (AttrType key_type,
                                 char * recPtr,
                                 int recLen,
                                 RID& rid)
{
	// put your code here
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
    	int i, j;
    	
    	int sid;
    	if (slotCnt == 0)
    	{
    		sid = 0;
    		i = 0;
    	}
    	else
    	{
    		//there should be no empty slot, so mark to assign a new slot later
    		//int i, j;
		slot_t *slot_iter;
		int int_key, new_int_key;
		char *str_key, *new_str_key;
		if (key_type == attrString)
		{
			for (i = 0; i != slotCnt; ++i)
			{
				//j = data + (i - 1) * sizeof(slot_t);
				//slot_iter = (slot_t *)j;
				slot_iter = &(slot[i]);
				str_key = (char *)(&data[slot_iter->offset]);
				new_str_key = (char *)recPtr;
				
				if (strcmp(str_key, new_str_key) > 0)
				{
					sid = i;	//the first slot larger than new record
					break;
				}
			}
			
			if (i == slotCnt)
				sid = -1;
		}
		else if (key_type == attrInteger)
		{
			for (i = 0; i != slotCnt; ++i)
			{
				//j = data + (i - 1) * sizeof(slot_t);
				//slot_iter = (slot_t *)j;
				slot_iter = &(slot[i]);
				int_key = *(int *)(&data[slot_iter->offset]);
				new_int_key = *(int *)recPtr;
				
				if (int_key > new_int_key)
				{
					sid = i;	//the first slot larger than new record
					break;
				}
			}
			
			if (i == slotCnt)
				sid = -1;
		}
    	}
    	
    	//move all slot after i 
    	memmove(&slot[i+1], &slot[i], sizeof(slot_t) * (slotCnt - i));
    	
    	//insert new slot
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
    			//if (slotCnt == 0)
    			{
    				//++slotCnt;
    				//freeSpace -= sizeof(slot_t);
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
    		//slotCnt++;
    		//freeSpace -= sizeof(slot_t);
    		sid = slotCnt - 1;
    	}
    	slotCnt++;
    	freeSpace -= sizeof(slot_t);
    	
    	rid.pageNo = curPage;
    	rid.slotNo = sid;
    	
    	//cout << "##slotCnt = " << slotCnt << endl;
	return OK;
	
	/*RID next_rid, curr_rid;
	Status sta;
	sta = firstRecord(curr_rid);
	if (sta == DONE)
	{
		HFPage::insertRecord(recPtr, recLen, rid);
		return OK;
	}
	else if (sta != OK)
	{
		return FAIL;
	}
	
	char *curr_recptr;
	int curr_reclen;
	sta = nextRecord(curr_rid, next_rid);
	while (sta == OK)
	{
		returnRecord(curr_rid, curr_recptr, curr_reclen);
		
	}*/
}


/*
 * Status SortedPage::deleteRecord (const RID& rid)
 *
 * Deletes a record from a sorted record page. It just calls
 * HFPage::deleteRecord().
 */

Status SortedPage::deleteRecord (const RID& rid)
{
	// put your code here
	// fill in the body
	    //int rid_test = check_valid_rid(rid);
	    //cout << "@@slotCnt before delete: " << slotCnt << endl;
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

            //cout << "slotCnt = " << slotCnt << ", sid = " << sid << endl;
	    	
	    	//remove slot if it is the last one
		/*if (sid == slotCnt - 1)
		{
			--slotCnt;
		    	freeSpace += sizeof(slot_t);
		}*/
	    }
	    rm_slot->offset = -1;	
	    rm_slot->length = EMPTY_SLOT;
	    //**************new****************
	    memmove(&slot[sid], &slot[sid + 1], sizeof(slot_t) * (slotCnt - sid - 1));
	    //*********************************
	    
	    //remove tail empty slots
	    int i, j, k;
	    k = slotCnt;
	    slot_t *slot_iter;
	    /*for (i = k - 1; i > 0; --i)
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
	    	freeSpace += sizeof(slot_t);*/
	    
	    //move all subsequent records
	    memmove(data + usedPtr + rm_length, data + usedPtr, rm_offset - usedPtr);
	    
	    
	    //update the slot.offset of all subsequent records
	    //int i, j;
	    //slot_t *slot_iter;
	    
	    slot[slotCnt-1].length = EMPTY_SLOT;
	    slot[slotCnt-1].offset = -1;
	    freeSpace += sizeof(slot_t);
	    slotCnt--;
	    
	    for (i = 0; i < slotCnt; ++i)	//slot[0] does not follow any slot
	    {
	    	//j = data + (i - 1) * sizeof(slot_t);	//slot offset
	    	//slot_iter = (slot *)j;
	    	slot_iter = &(slot[i]);
	    	if (slot_iter->length != EMPTY_SLOT && slot_iter->offset < rm_offset)
	    		slot_iter->offset += rm_length ;
	    }
	    
	    
	    usedPtr = usedPtr + rm_length;
	    freeSpace += rm_length;
	    
	    //cout << "@@slotCnt after delete: " << slotCnt << endl;
	    return OK;
}

int SortedPage::numberOfRecords()
{
	// put your code here
	return slotCnt;
}
