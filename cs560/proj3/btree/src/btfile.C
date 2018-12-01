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

  //  cout << "Openning B+tree index file: " << filename << endl;
  // status = MINIBASE_DB->get_file_entry(filename, headerPID);
  // if (status != OK) { // heapfile of name 'filename' doesn't exists in DB
  //   cout << "Open index file " << filename << "failed!" << endl;
  // }
  // // pin header page and keep it pinnned
  // Page *page_ptr;
  // returnStatus = MINIBASE_BM->pinPage(headerPID, page_ptr);
  // headerPage = (struct header_page *) page_ptr;
  // if (returnStatus != OK) {
  //   cout << "Pin header page failed!" << endl;
  // }

  // cout << "header PID: " << headerPID << endl;
  // cout << "Header page opened with root page: " << headerPage->root_pid
  //      << ", key type: " << headerPage->key_type << ", key size: "
  //      << headerPage->key_size << ", root node type: "
  //      << headerPage->root_type << endl;

  returnStatus = OK;
}

// create an index file with the given key type and size
BTreeFile::BTreeFile (Status& returnStatus, const char *filename, 
                      const AttrType keytype,
                      const int keysize)
{
  Status status;

  //  cout << "Creating B+tree index file: " << filename << endl;
  memcpy(fname, filename, strlen(filename));
  Page *page_ptr;
  MINIBASE_BM->newPage(headerPID, page_ptr);
  headerPage = (struct header_page *) page_ptr;
  headerPage->root_pid = INVALID_PAGE;
  headerPage->key_type = keytype;
  headerPage->key_size = keysize;

  PageId hpid;
  status = MINIBASE_DB->get_file_entry(filename, headerPID);
  if (status == OK) { // heapfile of name 'filename' doesn't exists in DB
    MINIBASE_DB->delete_file_entry(filename);
    status = MINIBASE_DB->add_file_entry(filename, headerPID);

    // MINIBASE_BM->pinPage(headerPID, page_ptr);
    // headerPage = (struct header_page *) page_ptr;
    // headerPage->root_pid = INVALID_PAGE;
    // headerPage->key_type = keytype;
    // headerPage->key_size = keysize;

    // cout << "ReOpen index file " << filename << endl;
  } else {

    status = MINIBASE_DB->add_file_entry(filename, headerPID);
  }

  /* create a blank new leaf page as root page in the beginning */
  PageId pid;
  Page *pptr;

  MINIBASE_BM->newPage(pid, pptr);
  BTLeafPage *new_leaf_page = (BTLeafPage *)pptr;
  new_leaf_page->init(pid);
  headerPage->root_pid = pid; // update root page id
  headerPage->root_type = LEAF;
  MINIBASE_BM->unpinPage(pid, TRUE);

  returnStatus = OK;
  // cout << "header PID: " << headerPID << endl;
  // cout << "Header page created with root page: " << headerPage->root_pid
  //      << ", key type: " << headerPage->key_type << ", key size: "
  //      << headerPage->key_size << ", root node type: "
  //      << headerPage->root_type << endl;

  // MINIBASE_BM->unpinPage(headerPID, TRUE);
}

BTreeFile::~BTreeFile ()
{
  // put your code here
  // cout << "In ~BTreeFile: header PID: " << headerPID << endl;
  // cout << "Header page created with root page: " << headerPage->root_pid
  //      << ", key type: " << headerPage->key_type << ", key size: "
  //      << headerPage->key_size << ", root node type: "
  //      << headerPage->root_type << endl;

  MINIBASE_BM->unpinPage(headerPID, TRUE);
  //MINIBASE_DB->delete_file_entry(fname);
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

void BTreeFile::splitIndexNode(BTIndexPage *index_ptr, _childentry*& newchild_entry)
{
  Status status;

  /* create a new leaf page to hold the last half records */
  PageId new_pid;
  Page *new_pptr;

  MINIBASE_BM->newPage(new_pid, new_pptr);
  BTIndexPage *new_index_ptr = (BTIndexPage *)new_pptr;
  new_index_ptr->init(new_pid);

  /* redistribue entries */
  int num_records = index_ptr->numberOfRecords();
  RID entry2move, new_entry_rid, entry2delete;
  PageId pid2move, left_most_pid2move;
  void *key2move;
  int int_key;
  char str_key[MAX_KEY_SIZE1];
  char mid_key[MAX_KEY_SIZE1];
  if (headerPage->key_type == attrInteger) {
    key2move = &int_key;
  } else {
    key2move = str_key;
  }

  // advance the current page to the midpoint
  index_ptr->get_first(entry2move, key2move, pid2move);
  for (int i = 1; i < num_records/2; ++i) {
    index_ptr->get_next(entry2move, key2move, pid2move);
  }
  left_most_pid2move = pid2move;

  // insert the last half records into the new page
  for (int i = 0; i < (num_records - num_records/2); ++i) {
    index_ptr->get_next(entry2move, key2move, pid2move);
    new_index_ptr->insertKey(key2move, headerPage->key_type, pid2move, new_entry_rid);
    if (i == 0)
      memcpy(mid_key, key2move, MAX_KEY_SIZE1);
  }
  new_index_ptr->setLeftLink(left_most_pid2move);

  // delete the last half records of the current page
  for (int i = 0; i < (num_records - num_records/2); ++i) {
    index_ptr->get_first(entry2move, key2move, pid2move);
    for (int j = 1; j < num_records/2; ++j) {
      index_ptr->get_next(entry2move, key2move, pid2move);
    }
    index_ptr->get_next(entry2move, key2move, pid2move);
    index_ptr->deleteRecord(entry2move);
  }

  // printChildEntry(newchild_entry);
  // if (headerPage->key_type == attrString)
  //   cout << "Mid key: " << (char *)mid_key << endl;
  if (keyCompare(newchild_entry->key, mid_key, headerPage->key_type) < 0) {
    index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                         newchild_entry->pid, new_entry_rid);
  } else {
    new_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                             newchild_entry->pid, new_entry_rid);
  }
  // printIndexNode(index_ptr);
  // printIndexNode(new_index_ptr);

  //exit(-1);

  /* fill in new child entry */
  RID entry_rid;
  PageId data_pid;
  _childentry *child_entry = (_childentry *)malloc(sizeof(_childentry));
  if (headerPage->key_type == attrInteger) {
    child_entry->key = malloc(sizeof(int));
  } else {
    child_entry->key = malloc(MAX_KEY_SIZE1 * sizeof(char));
  }
  // get the smallest key on the new page
  new_index_ptr->get_first(entry_rid, child_entry->key, data_pid);
  child_entry->pid = new_pid;
  newchild_entry = child_entry;

  MINIBASE_BM->unpinPage(new_pid, TRUE);

  //exit(-1);
}

void BTreeFile::splitLeafNode(BTLeafPage *leaf_ptr, _childentry*& newchild_entry)
{
  Status status;

  /* create a new leaf page to hold the last half records */
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
  }

  // insert the last half records into the new page
  for (int i = 0; i < (num_records - num_records/2); ++i) {
    leaf_ptr->get_next(entry2move, key2move, data2move);
    //cout << "Inserting key " << *(int *)key2move << " to new leaf page " << new_pid << endl;
    new_leaf_ptr->insertRec(key2move, headerPage->key_type, data2move, new_entry_rid);
  }

  // delete the last half records of the current page
  for (int i = 0; i < (num_records - num_records/2); ++i) {
    leaf_ptr->get_first(entry2move, key2move, data2move);
    for (int j = 1; j < num_records/2; ++j) {
      leaf_ptr->get_next(entry2move, key2move, data2move);
    }
    leaf_ptr->get_next(entry2move, key2move, data2move);
    leaf_ptr->deleteRecord(entry2move);
  }

  // cout << "number of records: " << leaf_ptr->numberOfRecords() << endl;
  // cout << "new number of records: " << new_leaf_ptr->numberOfRecords() << endl;

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

  // cout << "After splitting page " << leaf_ptr->page_no() << endl;
  // printLeafNode(leaf_ptr, FALSE);
  // printLeafNode(new_leaf_ptr, FALSE);

  MINIBASE_BM->unpinPage(new_pid, TRUE);
}

void BTreeFile::insertEntry2IndexNode(PageId pid, const void *key, const RID rid,
                                      _childentry*& newchild_entry)
{
  Status status;
  RID entry_rid;

  // TODO: if newchild_entry is not NULL, then add it to the current index page
  // if (newchild_entry) {
  //   cout << "TODO: need to add an new entry to the index page: " << pid << endl;
  //   //printChildEntry(newchild_entry);
  //   exit(-1);
  // }

  /* pin the given index page */
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTIndexPage *index_ptr = (BTIndexPage*)pptr;

  PageId next_pid;
  index_ptr->get_page_no(key, headerPage->key_type, next_pid);
  // printKey("next key to insert: ", key);
  // cout << "next page ID to look for: " << next_pid << endl;

  /* pin the next page to lookup */
  Page *next_pptr;
  MINIBASE_BM->pinPage(next_pid, next_pptr);
  SortedPage *spptr = (SortedPage *)next_pptr;
  if (spptr->get_type() == LEAF) {
    MINIBASE_BM->unpinPage(next_pid);
    insertEntry2LeafNode(next_pid, key, rid, newchild_entry);

    if (newchild_entry != NULL) { // a leaf node split into 2 nodes before insertion
      //printChildEntry(newchild_entry);
      // TODO: check if there are enough space for insertion!!!
      if (index_ptr->free_space() > getIndexEntrySize()) { // there are space for insertion
        index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                             newchild_entry->pid, entry_rid);
        //cout << "Free space remaining: " << index_ptr->free_space() << endl;
        //printIndexNode(index_ptr);
        MINIBASE_BM->unpinPage(pid, TRUE);
        newchild_entry = NULL;
        insertEntry2IndexNode(pid, key, rid, newchild_entry);
      } else {
        //cout << "TODO: need to split an index page!\n";
        if (headerPage->key_type == attrString) {
          //cout << "Before splitting an index page " << pid << endl;
          // printChildEntry(newchild_entry);
          //printTree(headerPage->root_pid);
        }
        splitIndexNode(index_ptr, newchild_entry);
        if (headerPage->key_type == attrString) {
          //cout << "After splitting an index page!\n";
          //printChildEntry(newchild_entry);
          //printTree(headerPage->root_pid);
        }

        // if (pid == headerPage->root_pid) {
        //   cout << "@@@@@@@@@@@@@The index page splitted is the ROOT page\n";

        //   /* create a new index page */
        //   PageId new_pid;
        //   Page *new_pptr;

        //   MINIBASE_BM->newPage(new_pid, new_pptr);
        //   BTIndexPage *new_index_ptr = (BTIndexPage *)new_pptr;
        //   new_index_ptr->init(new_pid);

        //   /* insert the new child entry to the index page */
        //   new_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
        //                            newchild_entry->pid, entry_rid);
        //   new_index_ptr->setLeftLink(headerPage->root_pid); // link to left-most leaf page

        //   /* update header page info since the root page has changed
        //      from a leaf node to a index node. */
        //   headerPage->root_pid = new_pid; // update root page id
        //   headerPage->root_type = INDEX; // index page

        //   // MINIBASE_BM->unpinPage(headerPID, TRUE);
        //   //printIndexNode(new_index_ptr);
        //   MINIBASE_BM->unpinPage(new_pid, TRUE);

        //   newchild_entry = NULL;

        //   if (headerPage->key_type == attrString) {
        //     printTree(headerPage->root_pid);
        //     //exit(-1);
        //   }

        //   insertEntry2IndexNode(new_pid, key, rid, newchild_entry);
        // }
        //printChildEntry(newchild_entry);
        MINIBASE_BM->unpinPage(pid, TRUE);
      }
    } else {
      MINIBASE_BM->unpinPage(pid);
    }
  } else { // INDEX page
    //cout << "TODO: next level is also index page!!!\n";
    MINIBASE_BM->unpinPage(next_pid);
    MINIBASE_BM->unpinPage(pid);

    insertEntry2IndexNode(next_pid, key, rid, newchild_entry);
    //exit(-1);
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
    // if (headerPage->key_type == attrString)
    //   cout << "key: " << (char *)key << " has been insert into page " << pid << endl;
    newchild_entry = NULL; // indicate no child page created due to this insertion
  } else { // split the current leaf page
    //cout << "key: " << *(int *)key << " failed to insert into page " << pid << endl;
    //if (headerPage->key_type == attrString)
      //cout << "Splitting leaf page " << pid << endl;
    splitLeafNode(leaf_ptr, newchild_entry);
  }
  MINIBASE_BM->unpinPage(pid, TRUE);
}

Status BTreeFile::insert(const void *key, const RID rid)
{
  if (headerPage->root_type == LEAF) { // root is a leaf page (btree has only one node)
    _childentry *newchild_entry;
    insertEntry2LeafNode(headerPage->root_pid, key, rid, newchild_entry);

    if (newchild_entry != NULL) { // special case: split root leaf page
      //printChildEntry(newchild_entry);
      /* create a new index page */
      PageId new_pid;
      Page *new_pptr;

      MINIBASE_BM->newPage(new_pid, new_pptr);
      BTIndexPage *new_index_ptr = (BTIndexPage *)new_pptr;
      new_index_ptr->init(new_pid);

      /* insert the new child entry to the index page */
      RID entry_rid;
      new_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                               newchild_entry->pid, entry_rid);
      new_index_ptr->setLeftLink(headerPage->root_pid); // link to left-most leaf page

      /* update header page info since the root page has changed
         from a leaf node to a index node. */
      // Page *page_ptr;
      // MINIBASE_BM->pinPage(headerPID, page_ptr);
      // headerPage = (struct header_page *) page_ptr;

      headerPage->root_pid = new_pid; // update root page id
      headerPage->root_type = INDEX; // index page

      // MINIBASE_BM->unpinPage(headerPID, TRUE);

      //printIndexNode(new_index_ptr);
      MINIBASE_BM->unpinPage(new_pid, TRUE);

      // after split, recursively insert the <key, rid>
      insert(key, rid);
    }
  } else { // root page is index page
    _childentry *newchild_entry = NULL;
    insertEntry2IndexNode(headerPage->root_pid, key, rid, newchild_entry);

    // TODO: if newchild_entry is not NULL, then create a new root page
    // to insert the new entry
    if (newchild_entry) {
      //printChildEntry(newchild_entry);
      RID entry_rid;

      /* pin root page, comment this block out if not working */
      // Page *root_pptr;
      // MINIBASE_BM->pinPage(headerPage->root_pid, root_pptr);
      // BTIndexPage *root_index_ptr = (BTIndexPage *)root_pptr;
      // if (root_index_ptr->free_space() >= getIndexEntrySize()) { // there are space for insertion
      //   cout << "inserting new entry to ROOT page " << headerPage->root_pid << endl;
      //   root_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
      //                            newchild_entry->pid, entry_rid);
      //   MINIBASE_BM->unpinPage(headerPage->root_pid, TRUE);
      //   return OK;
      // }

      //cout << "TODO: need to split root page again!\n";
      //exit(-1);

      /* create a new index page */
      PageId new_pid;
      Page *new_pptr;

      MINIBASE_BM->newPage(new_pid, new_pptr);
      BTIndexPage *new_index_ptr = (BTIndexPage *)new_pptr;
      new_index_ptr->init(new_pid);

      /* insert the new child entry to the index page */
      new_index_ptr->insertKey(newchild_entry->key, headerPage->key_type,
                               newchild_entry->pid, entry_rid);
      new_index_ptr->setLeftLink(headerPage->root_pid); // link to left-most leaf page

      /* update header page info since the root page has changed
         from a leaf node to a index node. */
      // Page *page_ptr;
      // cout << "before pin header page\n";
      // MINIBASE_BM->pinPage(headerPID, page_ptr);
      // headerPage = (struct header_page *) page_ptr;

      headerPage->root_pid = new_pid; // update root page id
      headerPage->root_type = INDEX; // index page

      // MINIBASE_BM->unpinPage(headerPID, TRUE);
      // cout << "after pin header page\n";

      //printIndexNode(new_index_ptr);
      MINIBASE_BM->unpinPage(new_pid, TRUE);

      newchild_entry = NULL;
      // after split, recursively insert the <key, rid>
      insert(key, rid);
      // exit(-1);
    }
  }
}

Status BTreeFile::deleteEntryFromIndexNode(PageId pid, const void *key, const RID rid)
{
  Status status;
  RID entry_rid;

  /* pin the given index page */
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTIndexPage *index_ptr = (BTIndexPage*)pptr;

  //cout << "Trying to delete key " << *(int *)key << " from page " << pid << endl;
  PageId next_pid;
  index_ptr->get_page_no(key, headerPage->key_type, next_pid);
  //cout << "next page to look up: " << next_pid << endl;
  //printIndexNode(index_ptr);
  //exit(-1);

  /* pin the next page to lookup */
  Page *next_pptr;
  MINIBASE_BM->pinPage(next_pid, next_pptr);
  SortedPage *spptr = (SortedPage *)next_pptr;
  if (spptr->get_type() == LEAF) {
    MINIBASE_BM->unpinPage(next_pid);
    deleteEntryFromLeafNode(next_pid, key, rid);
  } else { // INDEX page
    MINIBASE_BM->unpinPage(next_pid);
    deleteEntryFromIndexNode(next_pid, key, rid);
  }
  MINIBASE_BM->unpinPage(pid);
}

Status BTreeFile::deleteEntryFromLeafNode(PageId pid, const void *key, const RID rid)
{
  Status status;
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTLeafPage *leaf_ptr = (BTLeafPage*)pptr;

  //printLeafNode(leaf_ptr, TRUE);

  RID entry_rid, data_rid, rid_delete;
  char key2[MAX_KEY_SIZE1];
  leaf_ptr->get_first(entry_rid, key2, data_rid);
  if (keyCompare(key, key2, headerPage->key_type) == 0 && rid == data_rid) {
    rid_delete = entry_rid;
  } else {
    while (leaf_ptr->get_next(entry_rid, key2, data_rid) == OK) {
      if (keyCompare(key, key2, headerPage->key_type) == 0 && rid == data_rid) {
        //cout << "Found entry to delete!\n";
        rid_delete = entry_rid;
        break;
      }
    }
  }

  status = leaf_ptr->deleteRecord(rid_delete);
  //cout << "Deleting key " << *(int *)key << " from page " << pid << endl;
  //if (*(int *)key >= 430 && *(int *)key <= 450)
  //printLeafNode(leaf_ptr, TRUE);
  MINIBASE_BM->unpinPage(pid, TRUE);

  return status;
}

Status BTreeFile::Delete(const void *key, const RID rid)
{
  // printTree(headerPage->root_pid);
  // Page *pptr;
  // MINIBASE_BM->pinPage(70, pptr);
  // BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;
  // printLeafNode(leaf_ptr, TRUE);
  //exit(-1);

  // if (headerPage->key_type == attrString) {
  //   cout << "\nprint tree....\n";
  //   printTree(headerPage->root_pid);
  //   exit(-1);
  // }

  if (headerPage->root_type == LEAF) { // root is a leaf page (btree has only one node)
    return deleteEntryFromLeafNode(headerPage->root_pid, key, rid);
  } else { // root page is index page
    return deleteEntryFromIndexNode(headerPage->root_pid, key, rid);
  }

  return OK;
}

PageId BTreeFile::getLeftMostLeafPage()
{
  if (headerPage->root_type == LEAF) { // root is a leaf page (btree has only one node)
    return headerPage->root_pid;
  } else { // root page is index page
    PageId pid = headerPage->root_pid;

    while (TRUE) {
      Page *pptr;
      MINIBASE_BM->pinPage(pid, pptr);
      BTIndexPage *index_ptr = (BTIndexPage*)pptr;
      PageId left_most_pid = index_ptr->getLeftLink();
      MINIBASE_BM->unpinPage(pid);

      MINIBASE_BM->pinPage(left_most_pid, pptr);
      SortedPage *sptr = (SortedPage *)pptr;
      if (sptr->get_type() == LEAF) {
        MINIBASE_BM->unpinPage(left_most_pid);
        return left_most_pid;
      } else { // next level is index page, go deep
        MINIBASE_BM->unpinPage(left_most_pid);
        pid = left_most_pid;
      }
    }
  }
}

IndexFileScan *BTreeFile::new_scan(const void *lo_key, const void *hi_key)
{
  BTreeFileScan *scan_ptr = new BTreeFileScan();

  // Page *pptr;

  // MINIBASE_BM->pinPage(getLeftMostLeafPage(), pptr);
  // BTLeafPage *leaf_ptr = (BTLeafPage *)pptr;
  // printLeafNode(leaf_ptr, TRUE);
  // exit(-1);

  scan_ptr->key_lo = (void *)lo_key;
  scan_ptr->key_hi = (void *)hi_key;
  scan_ptr->done = FALSE;

  //cout << "left most page: " << getLeftMostLeafPage() << endl;
  scan_ptr->current_pid = getLeftMostLeafPage();
  scan_ptr->key_size = headerPage->key_size;
  scan_ptr->key_type = headerPage->key_type;
  scan_ptr->rec_retrieved_in_cur_page = 0;
  scan_ptr->first_key = NULL;
  //scan_ptr->bf = this;

  if (lo_key && hi_key && keyCompare(lo_key, hi_key, headerPage->key_type) > 0) {
    scan_ptr->done = TRUE;
    return scan_ptr;
  }

  if (lo_key) {
    scan_ptr->init();
  }

  return scan_ptr;
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

int BTreeFile::getIndexEntrySize()
{
  if (headerPage->key_type == attrInteger)
    return 2 * headerPage->key_size + sizeof(int);
  else
    return headerPage->key_size + sizeof(int);
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
    cout << "<" << index_ptr->getLeftLink() << "," << int_key << "," << pid << "> ";

    Status status;
    PageId next_pid;
    while (TRUE) {
      status = index_ptr->get_next(entry_rid, &int_key, next_pid);
      if (status != OK) {
        break;
      }
      cout << "<" << int_key << "," << next_pid << "> ";
    }
    cout << "\n";
  } else {
    index_ptr->get_first(entry_rid, str_key, pid);
    cout << "<" << index_ptr->getLeftLink() << "," << str_key << "," << pid << "> ";

    Status status;
    PageId next_pid;
    while (TRUE) {
      status = index_ptr->get_next(entry_rid, str_key, next_pid);
      if (status != OK) {
        break;
      }
      cout << "<" << str_key << "," << next_pid << "> ";
    }
    cout << "\n";
  }
}

void BTreeFile::printLeafNode(BTLeafPage *leaf_ptr, int print_all)
{
  Status status;
  RID entry_rid, data_rid;
  int int_key;
  char str_key[MAX_KEY_SIZE1];

  if (headerPage->key_type == attrInteger) {
    leaf_ptr->get_first(entry_rid, &int_key, data_rid);
    cout << "Leaf page " << leaf_ptr->page_no() << ": (Number of records: "
         << leaf_ptr->numberOfRecords() << ")";
    cout << " (key range: " << int_key << " --->";

    while (leaf_ptr->get_next(entry_rid, &int_key, data_rid) == OK) {
      if (print_all == TRUE)
        cout << "key: " << int_key << ", entry rid: "
             << entry_rid.pageNo << ", " << entry_rid.slotNo << endl;
    }
    if (!print_all)
      cout << " " << int_key << ")\n";
  } else {
    leaf_ptr->get_first(entry_rid, str_key, data_rid);
    cout << "Leaf page " << leaf_ptr->page_no() << ": (Number of records: "
         << leaf_ptr->numberOfRecords() << ")";
    cout << " (key range: " << str_key << " --->";

    while (leaf_ptr->get_next(entry_rid, str_key, data_rid) == OK) {
      if (print_all == TRUE)
        cout << "key: " << str_key << ", entry rid: "
             << entry_rid.pageNo << ", " << entry_rid.slotNo << endl;
    }
    if (!print_all)
      cout << " " << str_key << ")\n";
  }
}

void BTreeFile::printTree(PageId pid)
{
  Page *pptr;
  MINIBASE_BM->pinPage(pid, pptr);
  BTIndexPage *index_ptr = (BTIndexPage*)pptr;
  RID entry_rid;
  PageId next_pid;
  int int_key;
  char str_key[MAX_KEY_SIZE1];

  printIndexNode(index_ptr);

  /* pin the next page to lookup */
  Page *next_pptr;
  MINIBASE_BM->pinPage(index_ptr->getLeftLink(), next_pptr);
  SortedPage *spptr = (SortedPage *)next_pptr;
  if (spptr->get_type() == INDEX) {
    MINIBASE_BM->unpinPage(index_ptr->getLeftLink());


    if (headerPage->key_type == attrInteger) {
      index_ptr->get_first(entry_rid, &int_key, next_pid);
      printTree(index_ptr->getLeftLink());
      printTree(next_pid);

      Status status;
      while (TRUE) {
        status = index_ptr->get_next(entry_rid, &int_key, next_pid);
        if (status != OK) {
          break;
        }
        printTree(next_pid);
      }
    } else {
      index_ptr->get_first(entry_rid, str_key, next_pid);
      printTree(index_ptr->getLeftLink());
      printTree(next_pid);

      Status status;
      while (TRUE) {
        status = index_ptr->get_next(entry_rid, str_key, next_pid);
        if (status != OK) {
          break;
        }
        printTree(next_pid);
      }
    }

    MINIBASE_BM->unpinPage(pid);
  } else {
    //MINIBASE_BM->unpinPage(index_ptr->getLeftLink());
    // print leaf nodes
    if (headerPage->key_type == attrString) {
      BTLeafPage *leaf_ptr = (BTLeafPage *)spptr;
      printLeafNode(leaf_ptr, TRUE);

      index_ptr->get_first(entry_rid, str_key, next_pid);
      MINIBASE_BM->pinPage(next_pid, pptr);
      leaf_ptr = (BTLeafPage*)pptr;
      printLeafNode(leaf_ptr, TRUE);

      Status status;
      while (TRUE) {
        status = index_ptr->get_next(entry_rid, str_key, next_pid);
        if (status != OK) {
          break;
        }
        MINIBASE_BM->pinPage(next_pid, pptr);
        leaf_ptr = (BTLeafPage*)pptr;
        printLeafNode(leaf_ptr, TRUE);
      }
    }

    MINIBASE_BM->unpinPage(pid);
  }
}
