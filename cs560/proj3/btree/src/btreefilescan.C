/*
 * btreefilescan.C - function members of class BTreeFileScan
 *
 * Spring 14 CS560 Database Systems Implementation
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 
 */

#include "minirel.h"
#include "buf.h"
#include "db.h"
#include "new_error.h"
#include "btfile.h"
#include "btreefilescan.h"

/*
 * Note: BTreeFileScan uses the same errors as BTREE since its code basically 
 * BTREE things (traversing trees).
 */

BTreeFileScan::~BTreeFileScan()
{
  // put your code here
}


Status BTreeFileScan::get_next(RID & rid, void* keyptr)
{
  Status status;
  Page *pptr;

  if (done == TRUE) return DONE;

  if (first_key) {
    if (key_lo && key_type == attrString) {
      // if (strcmp((char *)first_key, "adrift") == 0) {
      //   MINIBASE_BM->pinPage(current_pid, pptr);
      //   BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;

      //   //leaf_ptr->get_first(cur_rid, keyptr, rid);
      //   leaf_ptr->get_next(cur_rid, keyptr, rid);
      //   MINIBASE_BM->unpinPage(current_pid);

      //   // exit(-1);
      // }
    }
    if (key_hi && keyCompare(first_key, key_hi, key_type) > 0) {
      return DONE;
    }
    memcpy(keyptr, first_key, key_size);
    if (key_type == attrString) {
      rec_retrieved_in_cur_page = 0;
    }
    memcpy(cur_key, first_key, key_size);
    rid = first_rid;
    //cur_rid = first_rid;
    first_key = NULL;
    return OK;
  }

  if (current_pid == INVALID_PAGE) return DONE;

  MINIBASE_BM->pinPage(current_pid, pptr);
  BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;

  if (rec_retrieved_in_cur_page == 0) {
    status = leaf_ptr->get_first(cur_rid, keyptr, rid);
    if (key_lo && keyCompare(key_lo, keyptr, key_type) > 0) {
      return DONE;
    }

    if (key_hi && keyCompare(keyptr, key_hi, key_type) > 0) {
      MINIBASE_BM->unpinPage(current_pid);
      return DONE;
    }
    memcpy(cur_key, keyptr, key_size);
    MINIBASE_BM->unpinPage(current_pid);
    if (status == OK) rec_retrieved_in_cur_page++;
    else return DONE;
  } else {
    status = leaf_ptr->get_next(cur_rid, keyptr, rid);
    // if (key_lo && key_type == attrString) {
    //   if (strcmp((char *)keyptr, "adrift") == 0) {
    //     //exit(-1);
    //   }
    // }
    if (key_hi && keyCompare(keyptr, key_hi, key_type) > 0) {
      MINIBASE_BM->unpinPage(current_pid);
      return DONE;
    }
    memcpy(cur_key, keyptr, key_size);
    // cout << "current key retrieved: " << *(int *)cur_key << endl;
    //exit(-1);
    //if (status != OK) {
    if (rec_retrieved_in_cur_page == leaf_ptr->numberOfRecords()-1) {
      // if (key_lo && key_type == attrString) {
      //   cout << "TODO: go to the next page!\n";
      //   exit(-1);
      // }
      PageId tmp_pid = current_pid;
      current_pid = leaf_ptr->getNextPage();
      rec_retrieved_in_cur_page = 0;
      MINIBASE_BM->unpinPage(tmp_pid);
      return OK;
    } else {
      MINIBASE_BM->unpinPage(current_pid);
      rec_retrieved_in_cur_page++;
      return OK;
    }
  }

  return OK;
}

void BTreeFileScan::init()
{
  char keyptr[MAX_KEY_SIZE1];
  if (key_lo) {
    if (key_type == attrString) {
      //cout << "Low key: " << (char *)key_lo << endl;
      //exit(-1);
    }
    Status status;
    Page *pptr;
    RID rid;

    first_key = NULL;
    while (TRUE) {
      MINIBASE_BM->pinPage(current_pid, pptr);
      BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;

      if (rec_retrieved_in_cur_page == 0) {
        status = leaf_ptr->get_first(cur_rid, keyptr, rid);
        rec_retrieved_in_cur_page++;
        //cout << "key pre-scan: " << *(int *)keyptr << endl;
        if (keyCompare(keyptr, key_lo, key_type) >= 0) {
          first_key = (char *)malloc(MAX_KEY_SIZE1);
          memcpy(first_key, keyptr, MAX_KEY_SIZE1);
          first_rid = rid;
          MINIBASE_BM->unpinPage(current_pid);
          break;
        }
        MINIBASE_BM->unpinPage(current_pid);
      } else {
        status = leaf_ptr->get_next(cur_rid, keyptr, rid);
        //cout << "key pre-scan: " << *(int *)keyptr << endl;
        if (keyCompare(keyptr, key_lo, key_type) >= 0) {
          first_key = (char *)malloc(MAX_KEY_SIZE1);
          memcpy(first_key, keyptr, MAX_KEY_SIZE1);
          first_rid = rid;
          rec_retrieved_in_cur_page++;
          break;
        }
        if (rec_retrieved_in_cur_page == leaf_ptr->numberOfRecords()-1) {
          //cout << "TODO: go to the next page!\n";
          PageId tmp_pid = current_pid;
          current_pid = leaf_ptr->getNextPage();
          rec_retrieved_in_cur_page = 0;
          MINIBASE_BM->unpinPage(tmp_pid);
          if (current_pid == INVALID_PAGE) {
            done = TRUE;
            return;
          }
        } else {
          MINIBASE_BM->unpinPage(current_pid);
          rec_retrieved_in_cur_page++;
        }
      }
    }
  }
}

Status BTreeFileScan::delete_current()
{
  // put your code here
  // Page *pptr;
  // MINIBASE_BM->pinPage(current_pid, pptr);
  // BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;

  // // //cout << "# of records: " << leaf_ptr->numberOfRecords() << endl;
  // RID rid_tmp = cur_rid;
  // bf->printLeafNode(leaf_ptr, TRUE);
  // leaf_ptr->deleteRecord(cur_rid);
  // // //cout << "Delete record: " << cur_rid.pageNo << ", " << cur_rid.slotNo << endl;
  // // //cout << "# of records: " << leaf_ptr->numberOfRecords() << endl;
  // // //  exit(-1);
  // //printLeafNode(leaf_ptr, TRUE);
  // MINIBASE_BM->unpinPage(current_pid, TRUE);
  // cout << "current key: " << *(int *)cur_key << endl;
  // //bf->Delete(cur_key, cur_rid);
  // exit(-1);
  return OK;
}


int BTreeFileScan::keysize() 
{
  return key_size;
}
