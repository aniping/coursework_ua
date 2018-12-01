/*
 * btleaf_page.C - implementation of class BTLeafPage
 *
 * Johannes Gehrke & Gideon Glass  951016  CS564  UW-Madison
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */

#include <stdlib.h>
#include <string.h>
#include "btleaf_page.h"

const char* BTLeafErrorMsgs[] = {
// OK,
// Insert Record Failed,
};
static error_string_table btree_table(BTLEAFPAGE, BTLeafErrorMsgs);
   
/*
 * Status BTLeafPage::insertRec(const void *key,
 *                             AttrType key_type,
 *                             RID dataRid,
 *                             RID& rid)
 *
 * Inserts a key, rid value into the leaf node. This is
 * accomplished by a call to SortedPage::insertRecord()
 * The function also sets up the recPtr field for the call
 * to SortedPage::insertRecord() 
 * 
 * Parameters:
 *   o key - the key value of the data record.
 *
 *   o key_type - the type of the key.
 * 
 *   o dataRid - the rid of the data record. This is
 *               stored on the leaf page along with the
 *               corresponding key value.
 *
 *   o rid - the rid of the inserted leaf record data entry.
 */

Status BTLeafPage::insertRec(const void *key,
                              AttrType key_type,
                              RID dataRid,
                              RID& rid)
{
	// put your code here
	int len;
	KeyDataEntry *kde = (KeyDataEntry *)malloc(sizeof(KeyDataEntry));
	Datatype dt;
	dt.rid = dataRid;

	// cout << "size of keydataentry: " << sizeof(KeyDataEntry) << endl;
	// cout << "size of int: " << sizeof(int) << endl;
	// cout << "size of kde: " << sizeof(kde) << endl;
	// cout << "size of RID: " << sizeof(RID) << endl;
	
	if (key_type == attrString || key_type == attrInteger)
		make_entry(kde, key_type, key, LEAF, dt, &len);
	else
		return FAIL;

	// cout <<	"before sorted page insertion\n";
	//SortedPage::insertRecord(key_type, (char *)kde, sizeof(kde), rid);
	SortedPage::insertRecord(key_type, (char *)kde, len, rid);
	//printf("@@key = %d, rid = (%d, %d)\n", *(int *)key, rid.pageNo, rid.slotNo);
	// cout <<	"after sorted page insertion\n";

	return OK;
}

/*
 *
 * Status BTLeafPage::get_data_rid(const void *key,
 *                                 AttrType key_type,
 *                                 RID & dataRid)
 *
 * This function performs a binary search to look for the
 * rid of the data record. (dataRid contains the RID of
 * the DATA record, NOT the rid of the data entry!)
 */

Status BTLeafPage::get_data_rid(void *key,
                                AttrType key_type,
                                RID & dataRid)
{
	// put your code here
	int min, max, mid;
	min = 0;
	max = slotCnt;
	//slot_t *slot_mid;
	RID rid_mid;
	char *rec_ptr = (char *)malloc(sizeof(KeyDataEntry));
	int rec_len;
	Status sta;
	char *key_mid = (char *)malloc(MAX_KEY_SIZE1 > sizeof(int) ? MAX_KEY_SIZE1 : sizeof(int));
	RID data_mid;
	Datatype dt;

	while (max >= min)
	{
		mid = (min + max) / 2;
		//slot_mid = &(slot[mid]);
		rid_mid.pageNo = curPage;
		rid_mid.slotNo = mid;
		sta = HFPage::getRecord(rid_mid, rec_ptr, rec_len);
		dt.rid = data_mid;
		get_key_data(key_mid, &dt, (KeyDataEntry *)rec_ptr, rec_len, LEAF);
		
		if (keyCompare(key_mid, key, key_type) < 0)
		{
			min = mid + 1;
		}
		else if (keyCompare(key_mid, key, key_type) > 0)
		{
			max = mid - 1;
		}
		else if (keyCompare(key_mid, key, key_type) == 0)
		{
			dataRid = data_mid;
			break;
		}
	}
	
	return OK;
}

/* 
 * Status BTLeafPage::get_first (const void *key, RID & dataRid)
 * Status BTLeafPage::get_next (const void *key, RID & dataRid)
 * 
 * These functions provide an
 * iterator interface to the records on a BTLeafPage.
 * get_first returns the first key, RID from the page,
 * while get_next returns the next key on the page.
 * These functions make calls to RecordPage::get_first() and
 * RecordPage::get_next(), and break the flat record into its
 * two components: namely, the key and datarid. 
 */
Status BTLeafPage::get_first (RID& rid,
                              void *key,
                              RID & dataRid)
{ 
  // put your code here
  	Status sta;
	sta = HFPage::firstRecord(rid);
	
	char *rec_ptr = (char *)malloc(sizeof(KeyDataEntry));
	int rec_len;
	sta = HFPage::getRecord(rid, rec_ptr, rec_len);
	if (sta != OK) return sta;
	//cout << "entry length: " << rec_len << endl;
	Datatype dt;
	//dt.rid = dataRid;
	get_key_data(key, &dt, (KeyDataEntry *)rec_ptr, rec_len, LEAF);
	//cout << "get_first: " << dt.rid.pageNo << ", " << dt.rid.slotNo << endl;
	dataRid = dt.rid;

	return OK;
}

Status BTLeafPage::get_next (RID& rid,
                             void *key,
                             RID & dataRid)
{
	// put your code here
	Status sta;
	RID next;
	sta = HFPage::nextRecord(rid, next);
	if (sta != OK) { // done
		return sta;
	}

	rid = next;
	
	char *rec_ptr = (char *)malloc(sizeof(KeyDataEntry));
	int rec_len;
	sta = HFPage::getRecord(rid, rec_ptr, rec_len);
	if (sta != OK) {
		//cout << sta << endl;
		//cout << "No more record in the current page. Status: " << sta << endl;
		exit(-1);
	}
	Datatype dt;
	//dt.rid = dataRid;
	get_key_data(key, &dt, (KeyDataEntry *)rec_ptr, rec_len, LEAF);
	dataRid = dt.rid;
	
	//cout << "get_next: " << dt.rid.pageNo << ", " << dt.rid.slotNo << endl;
	
	return OK;
}
