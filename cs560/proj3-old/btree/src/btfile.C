/*
 * btfile.C - function members of class BTreeFile 
 * 
 * Johannes Gehrke & Gideon Glass  951022  CS564  UW-Madison
 * Edited by Young-K. Suh (yksuh@cs.arizona.edu) 03/27/14 CS560 Database Systems Implementation 
 */

#include "minirel.h"
#include "buf.h"
#include "db.h"
#include "new_error.h"
#include "btfile.h"
#include "btreefilescan.h"

// Define your error message here
const char* BtreeErrorMsgs[] = {
  // Possible error messages
  // _OK
  // CANT_FIND_HEADER
  // CANT_PIN_HEADER,
  // CANT_ALLOC_HEADER
  // CANT_ADD_FILE_ENTRY
  // CANT_UNPIN_HEADER
  // CANT_PIN_PAGE
  // CANT_UNPIN_PAGE
  // INVALID_SCAN
  // SORTED_PAGE_DELETE_CURRENT_FAILED
  // CANT_DELETE_FILE_ENTRY
  // CANT_FREE_PAGE,
  // CANT_DELETE_SUBTREE,
  // KEY_TOO_LONG
  // INSERT_FAILED
  // COULD_NOT_CREATE_ROOT
  // DELETE_DATAENTRY_FAILED
  // DATA_ENTRY_NOT_FOUND
  // CANT_GET_PAGE_NO
  // CANT_ALLOCATE_NEW_PAGE
  // CANT_SPLIT_LEAF_PAGE
  // CANT_SPLIT_INDEX_PAGE
};

static error_string_table btree_table( BTREE, BtreeErrorMsgs);

// open an existing index file
BTreeFile::BTreeFile (Status& returnStatus, const char *filename)
{
  Status status;

  cout << "Openning B+tree index file: " << filename << endl;
  status = MINIBASE_DB->get_file_entry(filename, headerPID);
  if (status != OK) { // heapfile of name 'filename' doesn't exists in DB
    cout << "Open index file " << filename << "failed!" << endl;
  }
  // pin header page and keep it pinnned
  Page *page_ptr;
  status = MINIBASE_BM->pinPage(headerPID, page_ptr);
  headerPage = (struct header_page *) page_ptr;
  if (status != OK) {
    cout << "Pin header page failed!" << endl;
  }
}

// create an index file with the given key type and size
BTreeFile::BTreeFile (Status& returnStatus, const char *filename, 
                      const AttrType keytype,
                      const int keysize)
{
  Status status;

  cout << "Creating B+tree index file: " << filename << endl;
  Page *page_ptr;
  MINIBASE_BM->newPage(headerPID, page_ptr);
  headerPage = (struct header_page *) page_ptr;
  headerPage->root_pid = INVALID_PAGE;
  headerPage->key_type = keytype;
  headerPage->key_size = keysize;
  status = MINIBASE_DB->add_file_entry(filename, headerPID);

  cout << "Header page created with root page: " << headerPage->root_pid
       << ", key type: " << headerPage->key_type << ", key size: "
       << headerPage->key_size << endl;
}

BTreeFile::~BTreeFile ()
{
  // put your code here
}

Status BTreeFile::destroyFile ()
{
  // put your code here
  return OK;
}

int rec_count = 1;
Status BTreeFile::insert(const void *key, const RID rid)
{
  if (headerPage->root_pid == INVALID_PAGE) { // empty btree file
    PageId pid;
    Page *pptr;

    /* create a blank new leaf page */
    MINIBASE_BM->newPage(pid, pptr);
    BTLeafPage *new_leaf_page = (BTLeafPage*)pptr;
    new_leaf_page->init(pid);
    cout << "new leaf page inited\n";
    headerPage->root_pid = pid; // update root page id

    // insert the first record
    RID leaf_entry_rid;
    //cout << "before insertion " << headerPage->key_type << endl;
    new_leaf_page->insertRec(key, headerPage->key_type, rid, leaf_entry_rid);
    //cout << "after insertion\n";
    MINIBASE_BM->unpinPage(pid, TRUE);
    cout << "record " << rec_count++ << " inserted to root leaf page\n";
    cout << "Free space remaining: " << new_leaf_page->free_space() << endl;
  } else {
    Page *pptr;
    MINIBASE_BM->pinPage(headerPage->root_pid, pptr);
    BTLeafPage *root_leaf_page = (BTLeafPage*)pptr;
    if (root_leaf_page->free_space() >= 16) {
      RID leaf_entry_rid;
      Status status = root_leaf_page->insertRec(key, headerPage->key_type, rid, leaf_entry_rid);
      MINIBASE_BM->unpinPage(headerPage->root_pid, TRUE);
      if (status == DONE) {
        cout << "ERROR: cannot insert more records to root leaf node!\n";
        exit(-1);
      }
      cout << "record " << rec_count++ << " inserted to root leaf page\n";
      cout << "Free space remaining: " << root_leaf_page->free_space() << endl;
    } else {
      cout << "TODO: time to split the root leaf node!\n";
      cout << "Free space remaining: " << root_leaf_page->free_space() << endl;
      printLeafNode(root_leaf_page);
      MINIBASE_BM->unpinPage(headerPage->root_pid);

      exit(-1);
    }
  }

  return OK;
}

Status BTreeFile::Delete(const void *key, const RID rid) {
  // put your code here
  return OK;
}
    
IndexFileScan *BTreeFile::new_scan(const void *lo_key, const void *hi_key) {
  // put your code here
  return NULL;
}

int BTreeFile::keysize()
{
  // put your code here
  return headerPage->key_size;
}

void BTreeFile::printLeafNode(BTLeafPage *leaf_ptr)
{
  RID entry_rid, data_rid;
  int int_key;
  char str_key[MAX_KEY_SIZE1];

  if (headerPage->key_type == attrInteger) {
    leaf_ptr->get_first(entry_rid, &int_key, data_rid);
    cout << "first key: " << int_key << ", data rid: "
         << data_rid.pageNo << ", " << data_rid.slotNo << endl;

    while (TRUE) {
      leaf_ptr->get_next(entry_rid, &int_key, data_rid);
      cout << "key: " << int_key << ", data rid: "
           << data_rid.pageNo << ", " << data_rid.slotNo << endl;
    }
  }
}
