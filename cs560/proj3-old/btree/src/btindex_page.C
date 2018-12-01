/*
 * btindex_page.C - implementation of class BTIndexPage
 *
 * Johannes Gehrke & Gideon Glass  951016  CS564  UW-Madison
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */

#include <stdlib.h>
#include <string.h>
#include "btindex_page.h"

// Define your Error Messge here
const char* BTIndexErrorMsgs[] = {
  //Possbile error messages,
  //OK,
  //Record Insertion Failure,
};

static error_string_table btree_table(BTINDEXPAGE, BTIndexErrorMsgs);

Status BTIndexPage::insertKey (const void *key,
                               AttrType key_type,
                               PageId pageNo,
                               RID& rid)
{
	  // put your code here
	  KeyDataEntry *kde = (KeyDataEntry *)malloc(sizeof(KeyDataEntry));
	  if (key_type == attrString)
	  	strcpy(kde->key.charkey, (char *)key);
	  else if (key_type == attrInteger)
	  	kde->key.intkey = *((int *)key);
	  else
	  	return FAIL;
	  	
	  kde->data.pageNo = pageNo;
	  SortedPage::insertRecord(key_type, (char *)kde, sizeof(kde), rid);
	  
	  return OK;
}

Status BTIndexPage::deleteKey (const void *key, AttrType key_type, RID& curRid)
{
	// put your code here
	//find rid of the key
	int i, j, sid;
	slot_t *slot_iter;
	Keytype kv, kv_iter;
	if (key_type == attrInteger)
		kv.intkey = *((int *)key);
	else if (key_type == attrString)
		//kv = *((char *)key);
		strcpy(kv.charkey, (char *)key);
	
	sid = -1;	
	for (i = 0; i != slotCnt; ++i)
	{
		slot_iter = &(slot[i]);
		if (key_type == attrInteger)
			kv_iter.intkey = *((int *)(&data[slot_iter->offset]));
		else if (key_type == attrString)
			strcpy(kv_iter.charkey, ((char *)(&data[slot_iter->offset])));
			
		if (keyCompare(&kv_iter, &kv, key_type) == 0)
		{
			sid = i;
			break;
		}
	}
	
	if (sid == -1)
		return FAIL;
	
	curRid.pageNo = curPage;
	curRid.slotNo = sid;
	
	return OK;
}

Status BTIndexPage::get_page_no(const void *key,
                                AttrType key_type,
                                PageId & pageNo)
{
	// put your code here
	Status sta;
	RID rid;
	char *key_iter = (char *)malloc(MAX_KEY_SIZE1 > sizeof(int) ? MAX_KEY_SIZE1 : sizeof(int));
	PageId last_right, right_pid;
	Keytype kt, kt_iter;
	
	sta = get_first(rid, key_iter, right_pid);
	last_right = getLeftLink();
	while (sta == OK)
	{
		if (key_type == attrString || key_type == attrInteger)
		{
			if (keyCompare(key_iter, key, key_type) >= 0)
				break;
		}
		else
		{
			return FAIL;
		}
		
		last_right = right_pid;;
		sta = get_next(rid, key_iter, right_pid);
	}
	
	if (sta == OK)
	{
		pageNo = last_right;
		return OK;
	}
	else
	{
		return FAIL;
	}
	
}

    
Status BTIndexPage::get_first(RID& rid,
                              void *key,
                              PageId & pageNo)
{
	// put your code here
	Status sta;
	sta = HFPage::firstRecord(rid);
	
	char *rec_ptr = (char *)malloc(sizeof(KeyDataEntry));
	int rec_len;
	sta = HFPage::getRecord(rid, rec_ptr, rec_len);
	Datatype dt;
	dt.pageNo = pageNo;
	get_key_data(key, &dt, (KeyDataEntry *)rec_ptr, rec_len, INDEX);
	
	return OK;
}

Status BTIndexPage::get_next(RID& rid, void *key, PageId & pageNo)
{
	// put your code here
	Status sta;
	RID next;
	sta = HFPage::nextRecord(rid, next);
	rid = next;
	
	char *rec_ptr = (char *)malloc(sizeof(KeyDataEntry));
	int rec_len;
	sta = HFPage::getRecord(rid, rec_ptr, rec_len);
	Datatype dt;
	dt.pageNo = pageNo;
	get_key_data(key, &dt, (KeyDataEntry *)rec_ptr, rec_len, INDEX);
	
	return OK;
}
