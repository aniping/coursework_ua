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

  /* create a blank new leaf page as root page in the beginning */
  PageId pid;
  Page *pptr;

  MINIBASE_BM->newPage(pid, pptr);
  BTLeafPage *new_leaf_page = (BTLeafPage *)pptr;
  new_leaf_page->init(pid);
  headerPage->root_pid = pid; // update root page id
  headerPage->root_type = LEAF;
  MINIBASE_BM->unpinPage(pid, TRUE);

  cout << "Header page created with root page: " << headerPage->root_pid
       << ", key type: " << headerPage->key_type << ", key size: "
       << headerPage->key_size << ", root node type: "
       << headerPage->root_type << endl;
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

void BTreeFile::advance2midpoint(Page *pptr, nodetype nt)
{
  void *key;
  int int_key;
  char str_key[MAX_KEY_SIZE1];
  if (headerPage->key_type == attrInteger) {
    key = &int_key;
  } else {
    key = str_key;
  }

  if (nt == LEAF) {
    BTLeafPage *leaf_ptr;
    leaf_ptr = (BTLeafPage *)pptr;

    RID entry_rid, data_rid;
    leaf_ptr->get_first(entry_rid, key, data_rid);

    // advance to the middle of the page
    for (int i = 0; i < leaf_ptr->numberOfRecords()/2-1; ++i) {
      leaf_ptr->get_next(entry_rid, key, data_rid);
      cout << "key: " << *((int *)key)<< endl;
    }
  }
}

int split_cnt = 0;
void BTreeFile::splitLeafNode(BTLeafPage *leaf_ptr, _childentry*& newchild_entry)
{
  split_cnt++;
  Status status;
  /* create a new leaf page */
  PageId new_pid;
  Page *new_pptr;

  MINIBASE_BM->newPage(new_pid, new_pptr);
  BTLeafPage *new_leaf_ptr = (BTLeafPage *)new_pptr;
  new_leaf_ptr->init(new_pid);

  /* redistribue entries */
  int num_records = leaf_ptr->numberOfRecords();
  RID entry2move, data2move, new_entry_rid, entry2delete;
  void *key2move;
  int int_key;
  char str_key[MAX_KEY_SIZE1];
  if (headerPage->key_type == attrInteger) {
    key2move = &int_key;
  } else {
    key2move = str_key;
  }

  // advance the current page to the midpoint
  leaf_ptr->get_first(entry2move, key2move, data2move);
  for (int i = 1; i < num_records/2; ++i) {
    leaf_ptr->get_next(entry2move, key2move, data2move);
    cout << "Advance to key " << *(int *)key2move << endl;
  }

  // insert the last half records into the new page
  for (int i = 0; i < (num_records - num_records/2); ++i) {
    leaf_ptr->get_next(entry2move, key2move, data2move);
    cout << "Inserting key " << *(int *)key2move << " to new leaf page " << new_pid << endl;
    new_leaf_ptr->insertRec(key2move, headerPage->key_type, data2move, new_entry_rid);
  }

  //cout << "before deletion...\n";
  //leaf_ptr->dumpPage();
  // delete the last half records of the current page
  // for (int i = 0; i < (num_records - num_records/2); ++i) {
  //   leaf_ptr->get_first(entry2move, key2move, data2move);
  //   for (int j = 1; j < num_records/2; ++j) {
  //     leaf_ptr->get_next(entry2move, key2move, data2move);
  //   }
  //   leaf_ptr->get_next(entry2move, key2move, data2move);
  //   //cout << "delete rid: " << entry2move.pageNo << ", " << entry2move.slotNo << endl;
  //   leaf_ptr->deleteRecord(entry2move);
  //   //cout << "delete key " << *(int *)key2move << " from leaf page "
  //   //   << leaf_ptr->page_no() << endl;
  // }

  /* delete all entries, save the first half and then insert them back,
     a workaround solution... */
  _leafentry *saved_entries = new _leafentry[num_records/2];
  cout << "before deletion...\n";
  printLeafNode(leaf_ptr);
  RID saved_entry_rid;
  if (split_cnt >= 2) {
    for (int i = 0; i < num_records-1; ++i) {
      leaf_ptr->get_first(entry2move, key2move, data2move);
      leaf_ptr->get_next(entry2move, key2move, data2move);
      if (i < num_records/2 - 1) {
        memcpy(saved_entries[i].key, key2move, headerPage->key_size);
        saved_entries[i].rid = data2move;
        cout << "key " << *(int *)(saved_entries[i].key) << " saved\n";
      }
      cout << "retrieve key " << *(int *)key2move << endl;
      leaf_ptr->deleteRecord(entry2move);
    }
    leaf_ptr->get_first(entry2move, key2move, data2move);
    cout << "retrieve first key " << *(int *)key2move << endl;
    leaf_ptr->dumpPage();
    //leaf_ptr->deleteRecord(entry2move);

    for (int i = 0; i < num_records/2 - 1; ++i) {
      leaf_ptr->insertRec(saved_entries[i].key, headerPage->key_type,
                          saved_entries[i].rid, saved_entry_rid);
      cout << "key " << *(int *)(saved_entries[i].key) << " inserted\n";
    }
    cout << "after insertion...\n";
    leaf_ptr->dumpPage();
    printLeafNode(leaf_ptr);
    printLeafNode(new_leaf_ptr);

    //exit(-1);
  } else {
    for (int i = 0; i < num_records; ++i) {
      status = leaf_ptr->get_first(entry2move, key2move, data2move);
      if (status != OK) break;
      if (i < num_records/2) {
        memcpy(saved_entries[i].key, key2move, headerPage->key_size);
        saved_entries[i].rid = data2move;
        cout << "key " << *(int *)(saved_entries[i].key) << " saved\n";
      }

      leaf_ptr->deleteRecord(entry2move);

      cout << "delete key " << *(int *)key2move << " from leaf page "
           << leaf_ptr->page_no() << endl;
      cout << "# of records: " << leaf_ptr->numberOfRecords() << endl;
      //leaf_ptr->dumpPage();
    }
    cout << "after deletion...\n";
    leaf_ptr->dumpPage();

    for (int i = 0; i < num_records/2; ++i) {
      leaf_ptr->insertRec(saved_entries[i].key, headerPage->key_type,
                          saved_entries[i].rid, saved_entry_rid);
      cout << "key " << *(int *)(saved_entries[i].key) << " inserted\n";
    }
    cout << "after insertion...\n";
    leaf_ptr->dumpPage();
  }
  //exit(-1);

  cout << "number of records: " << leaf_ptr->numberOfRecords() << endl;
  cout << "new number of records: " << new_leaf_ptr->numberOfRecords() << endl;

  /* fill in new child entry */
  RID entry_rid, data_rid;
  _childentry *child_entry = (_childentry *)malloc(sizeof(_childentry));
  if (headerPage->key_type == attrInteger) {
    child_entry->key = malloc(sizeof(int));
  } else {
    child_entry->key = malloc(MAX_KEY_SIZE1 * sizeof(char));
  }
  // get the smallest key on the new page
  new_leaf_ptr->get_first(entry_rid, child_entry->key, data_rid);
  child_entry->pid = new_pid;
  newchild_entry = child_entry;

  /* Link two leaf pages */
  new_leaf_ptr->setNextPage(leaf_ptr->getNextPage());
  new_leaf_ptr->setPrevPage(leaf_ptr->page_no());
  leaf_ptr->setNextPage(new_pid);

  //printLeafNode(leaf_ptr);
  //printLeafNode(new_leaf_ptr);
  MINIBASE_BM->unpinPage(new_pid, TRUE);
}

void BTreeFile::insertEntry2IndexNode(PageId pid, const void *key, const RID rid,
                                      _childentry*& newchild_entry)
{
  Status status;

  /* pin the given index page */
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTIndexPage *index_ptr = (BTIndexPage*)pptr;

  PageId next_pid;
  index_ptr->get_page_no(key, headerPage->key_type, next_pid);
  //  printKey("next key to insert: ", key);
  //  cout << "next page ID to look for: " << next_pid << endl;

  /* pin the next page to lookup */
  Page *next_pptr;
  MINIBASE_BM->pinPage(next_pid, next_pptr);
  SortedPage *spptr = (SortedPage *)next_pptr;
  if (spptr->get_type() == LEAF) {
    MINIBASE_BM->unpinPage(next_pid);
    cout << "inserting key: " << *(int *)key << " to page " << next_pid << endl;
    insertEntry2LeafNode(next_pid, key, rid, newchild_entry);
    //printLeafNode(leaf_ptr);

    if (newchild_entry != NULL) {
      cout << "TODO: need to insert new key into the index page " << pid << endl;
      printChildEntry(newchild_entry);
      exit(-1);
    }
  } else { // INDEX page
    cout << "TODO: next level is also index page!!!\n";
    //MINIBASE_BM->unpinPage(next_pid);
    exit(-1);
  }
}

void BTreeFile::insertEntry2LeafNode2(PageId pid, const void *key, const RID rid,
                                      _childentry*& newchild_entry)
{
  Status status;
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTLeafPage *leaf_ptr = (BTLeafPage*)pptr;

  if (leaf_ptr->free_space() >= 2*getLeafEntrySize()) { // there are space for insertion
    RID entry_rid;

    leaf_ptr->insertRec(key, headerPage->key_type, rid, entry_rid);

    newchild_entry = NULL;
    MINIBASE_BM->unpinPage(pid, TRUE);
  } else { // split the current leaf page
    cout << "Before split leaf node\n";
    printLeafNode(leaf_ptr);
    splitLeafNode(leaf_ptr, newchild_entry);

    MINIBASE_BM->unpinPage(pid, TRUE);
    //MINIBASE_BM->unpinPage(new_pid, TRUE);
  }
}

void BTreeFile::insertEntry2LeafNode(PageId pid, const void *key, const RID rid,
                                     _childentry*& newchild_entry)
{
  Status status;
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTLeafPage *leaf_ptr = (BTLeafPage*)pptr;

  if (leaf_ptr->free_space() >= getLeafEntrySize()) { // there are space for insertion
    RID entry_rid;

    leaf_ptr->insertRec(key, headerPage->key_type, rid, entry_rid);
    // leaf_ptr->dumpPage();

    newchild_entry = NULL;
    MINIBASE_BM->unpinPage(pid, TRUE);
  } else { // split the current leaf page
    cout << "Before split leaf node\n";
    splitLeafNode(leaf_ptr, newchild_entry);

    MINIBASE_BM->unpinPage(pid, TRUE);
    //MINIBASE_BM->unpinPage(new_pid, TRUE);
  }
}

Status BTreeFile::insert(const void *key, const RID rid)
{
  if (headerPage->root_type == LEAF) { // root is a leaf page (btree has only one node)
    _childentry *newchild_entry;
    insertEntry2LeafNode(headerPage->root_pid, key, rid, newchild_entry);

    if (newchild_entry != NULL) {
      printChildEntry(newchild_entry);

      /* create a new leaf page */
      PageId new_pid;
      Page *new_pptr;

      MINIBASE_BM->newPage(new_pid, new_pptr);
      BTIndexPage *new_index_ptr = (BTIndexPage *)new_pptr;
      new_index_ptr->init(new_pid);

      RID entry_rid;
      new_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                               newchild_entry->pid, entry_rid);
      new_index_ptr->setLeftLink(headerPage->root_pid);

      /* update header page info since the root page has changed
         from a leaf node to a index node. */
      headerPage->root_pid = new_pid; // update root page id
      headerPage->root_type = INDEX;

      printIndexNode(new_index_ptr);
      MINIBASE_BM->unpinPage(new_pid, TRUE);

      // after split, recursively call itself to insert the <key, rid>
      insert(key, rid);
    }
  } else { // root page is index page
    _childentry *newchild_entry;
    insertEntry2IndexNode(headerPage->root_pid, key, rid, newchild_entry);
  }
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

/********************
 * Helper functions
 ********************/
int BTreeFile::getLeafEntrySize()
{
  if (headerPage->key_type == attrInteger)
    return 2 * headerPage->key_size + sizeof(RID);
  else
    return headerPage->key_size + sizeof(RID);
}

void BTreeFile::printKey(const char *prefix, const void *key)
{
  if (headerPage->key_type == attrInteger) {
    cout << prefix << *(int *)key << endl;
  } else if (headerPage->key_type == attrString) {
    cout << prefix << (char *)key << endl;
  }
}

/********************
 * Debug functions
 ********************/
void BTreeFile::printChildEntry(_childentry *entry)
{
  if (headerPage->key_type == attrInteger)
    cout << "new child starts with key: " << *(int *)(entry->key)
         << ", page ID: " << entry->pid << endl;
  else
    cout << "new child starts with key: " << (char *)(entry->key)
         << ", page ID: " << entry->pid << endl;
}

void BTreeFile::printIndexNode(BTIndexPage *index_ptr)
{
  RID entry_rid;
  PageId pid, leftmost_pid;
  int int_key;
  char str_key[MAX_KEY_SIZE1];

  cout << "Index page " << index_ptr->page_no() << " :(# records: "
       << index_ptr->numberOfRecords() << ")" << endl;
  if (headerPage->key_type == attrInteger) {
    index_ptr->get_first(entry_rid, &int_key, pid);
    cout << "<" << index_ptr->getLeftLink() << "," << int_key << "," << pid << ">\n";

    Status status;
    while (TRUE) {
      status = index_ptr->get_next(entry_rid, &int_key, pid);
      if (status != OK) {
        return;
      }
      cout << "<" << int_key << "," << pid << ">\n";
    }
  }
}

void BTreeFile::printLeafNode(BTLeafPage *leaf_ptr)
{
  Status status;
  RID entry_rid, data_rid;
  int int_key;
  char str_key[MAX_KEY_SIZE1];

  if (headerPage->key_type == attrInteger) {
    leaf_ptr->get_first(entry_rid, &int_key, data_rid);
    cout << "Leaf page ID: " << leaf_ptr->page_no() << ", Number of records: "
         << leaf_ptr->numberOfRecords();
    cout << ", first key: " << int_key << ", entry rid: "
         << entry_rid.pageNo << ", " << entry_rid.slotNo << endl;

    while (leaf_ptr->get_next(entry_rid, &int_key, data_rid) == OK) {
      cout << "key: " << int_key << ", entry rid: "
           << entry_rid.pageNo << ", " << entry_rid.slotNo << endl;
    }

    // while (TRUE) {
    //   status = leaf_ptr->get_next(entry_rid, &int_key, data_rid);
    //   if (status != OK) {
    //     //cout << ", last key: " << int_key << endl;
    //     return;
    //   }
    //   cout << "key: " << int_key << ", entry rid: "
    //        << entry_rid.pageNo << ", " << entry_rid.slotNo << endl;
    // }
  }
}
