import java.io.*;
import java.util.*;

/**
 * Represents a tree node object which is the basic component of
 * a directory tree.
 */
class TreeNode {
	private int level;
	private char digit;
	private ArrayList<TreeNode> children = null;
	private HashBucket bucket = null;
	private int bucketNo;
	private BucketDirectoryTree tree;
	
	TreeNode() {}
	
	TreeNode(int level, char digit, BucketDirectoryTree tree) {
		this.level = level;
		this.digit = digit;
		this.tree = tree;
		bucket = tree.allocateBucket();
	}
	
	TreeNode(int level, char digit, TreeNode parent, BucketDirectoryTree tree) {
		this.level = level;
		this.digit = digit;
		this.tree = tree;
		bucket = parent.getBucket();
	}
	
	void insert(String prefix, String suffix, int record_offset) {
		if (children == null) { // fill buckets only at the leaf nodes
			if (!bucket.isFull()) {
				bucket.insertSlot(prefix+suffix, record_offset);
			} else {
				split();
				char next_digit = suffix.charAt(0);
				prefix += suffix.charAt(0);
				suffix = suffix.substring(1);
				this.children.get(next_digit-'0').insert(prefix, suffix, record_offset);
			}
		} else { // traversal to the leaf nodes to fill the buckets
			char next_digit = suffix.charAt(0);
			prefix += suffix.charAt(0);
			suffix = suffix.substring(1);
			this.children.get(next_digit-'0').insert(prefix, suffix, record_offset);
		}
	}
	
	private void split() {
		TreeNode child;
		children = new ArrayList<TreeNode>(10);
		child = new TreeNode(this.level+1, '0', this, this.tree);
		children.add(child);
		for (char c = '1'; c <= '9' ; c++) {
			child = new TreeNode(this.level+1, c, this.tree);
			children.add(child);
		}
		
		// reallocate bucket slots from parent to children
		for (Iterator<Slot> iterator = this.bucket.getSlots().iterator(); iterator.hasNext() ;) {
		//for (Slot slot: this.bucket.getSlots()) {
			Slot slot = iterator.next();
			char next_digit = slot.getKey().charAt(this.level+1);
			if (next_digit != '0') {
				this.children.get(next_digit-'0').getBucket().insertSlot(slot.getKey(), 
						slot.getOffset2DBFile());
				slot.setKey(HashBucket.emptyKey); // fill the removed slot with empty key
				slot.setOffset2DBFile(HashBucket.emptyOffset); // fill the removed slot with empty offset
			
				iterator.remove(); // remove the redistributed slot
			}
		}
	}
	
	public int getLevel() {
		return this.level;
	}
	
	public char getDigit() {
		return this.digit;
	}
	
	public HashBucket getBucket() {
		return this.bucket;
	}
	
	public void printNode() {
		System.out.println("Node "+this.digit+" (level "+this.level+", bucket "+this.bucketNo+")");
		if (this.children != null) {
			for (TreeNode child : this.children) {
				child.printNode();
			}
		}
	}
	
	public void fetchBucketFromDisk(RandomAccessFile bucketStream,
			ArrayList<HashBucket> buckets) {
		if (this.children != null) {
			for (TreeNode child : this.children) {
				child.fetchBucketFromDisk(bucketStream, buckets);
			}
		} else {
			if (this.bucket == null) {
				try {
					bucketStream.seek(this.bucketNo * HashBucket.BUCKET_LENGTH);
					this.bucket = new HashBucket();
					this.bucket.fetchBucket(bucketStream);
					buckets.add(this.bucket);
				} catch (IOException e) {
					System.out.println("I/O ERROR: cannot seek the bucket stream."+e);
					System.exit(-1);
				}
			} else {
				buckets.add(this.bucket);
			}
		}
	}
	
	/**
	 * Perform query at the subtree starts with the current tree node.
	 * @param key
	 * @param bucketStream
	 * @param key_len
	 * @param buckets
	 */
	public void query(String key, RandomAccessFile bucketStream, 
			int key_len, ArrayList<HashBucket> buckets) {
		if (this.children != null) {
			if (key_len > 1) {
				//System.out.println("Looking up the child of node "+this.digit+" at level "+this.level);
				this.children.get(key.charAt(level+1)-'0').query(key, bucketStream, 
					key_len-1, buckets); // locate the child
			} else {
				for (TreeNode child : this.children) {
					child.fetchBucketFromDisk(bucketStream, buckets);
				}
			}
		} else {
			this.fetchBucketFromDisk(bucketStream, buckets);
		}
	}
	
	public void flatten(RandomAccessFile stream) {
		if (this.children != null) { // write non-leaf nodes to stream
			try {
				stream.writeInt(this.digit);
				stream.writeInt(this.level);
				stream.writeInt(-1); // use -1 to indicate that non-leaf node does not point to a bucket 
				for (TreeNode child : this.children) {
					child.flatten(stream);
				}
			} catch (IOException e) {
				System.out.println("I/O ERROR: Couldn't flatten the tree");
				System.exit(-1);
			}
		} else { // write leaf nodes to stream
			try {
				stream.writeInt(this.digit);
				stream.writeInt(this.level);
				stream.writeInt(this.bucket.getBucketNo());
			} catch (IOException e) {
				System.out.println("I/O ERROR: Couldn't flatten the tree");
				System.exit(-1);
			}
		}
	}
	
	public void fetchNodes(RandomAccessFile stream) {
		try {
			this.digit = (char) stream.readInt();
			this.level = stream.readInt();
			this.bucketNo = stream.readInt();
			if (this.bucketNo == -1) { // not a leaf node
				children = new ArrayList<TreeNode>(10);
				for (int i = 0; i < 10; i++) {
					TreeNode child = new TreeNode();
					child.fetchNodes(stream);
					children.add(child);
				}
			}
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't fetch the tree");
			System.exit(-1);
		}
	}
}

/**
 * Represents a bucket directory tree.
 */
public class BucketDirectoryTree {

	/* Two root nodes, 0*** and 1*** */
	TreeNode rootNode0;
	TreeNode rootNode1;
	
	// use a bucket pool to manage allocation of buckets in a central place.
	private ArrayList<HashBucket> bucketPool = null;
	
	BucketDirectoryTree() {
		bucketPool = new ArrayList<HashBucket>();
		rootNode0 = new TreeNode(0, '0', this);
		rootNode1 = new TreeNode(0, '1', this);
	}
	
	BucketDirectoryTree(boolean noBucketsInMemory) {
		rootNode0 = new TreeNode();
		rootNode1 = new TreeNode();
	}
	
	public HashBucket allocateBucket() {
		HashBucket bucket = new HashBucket(bucketPool.size());
		bucketPool.add(bucket);
		
		return bucket;
	}
	
	public void insertKey2bucket(String month, String day, int record_offset) {
		if (month.charAt(0) == '1') { // key is: '1***'
			rootNode1.insert("1", month.charAt(1)+day, record_offset);
		} else { // key is: '0***'
			rootNode0.insert("0", month.charAt(1)+day, record_offset);
		}
	}
	
	public void dumpBuckets(RandomAccessFile stream) {
		for (int i = 0; i < bucketPool.size(); i++) {
			bucketPool.get(i).dumpBucket(stream);
		}
	}
	
	public void printBuckets() {
		for (int i = 0; i < bucketPool.size(); i++) {
			bucketPool.get(i).printSlots();
		}
	}
	
	public void printFirstSlot(RandomAccessFile stream) {
		for (int i = 0; i < bucketPool.size(); i++) {
			HashBucket bucket = new HashBucket();
			bucket.fetchBucket(stream);
			bucket.printFirstSlot();
		}
	}
	
	public void printTree() {
		System.out.println("Tree starts at root 0:");
		rootNode0.printNode();
		System.out.println("Tree starts at root 1:");
		rootNode1.printNode();
	}
	
	/**
	 * Perform query against the given key.
	 * @param key the search key
	 * @param bucketStream stream for hash bucket file
	 * @param recordStream stream for records from binary database file
	 */
	public void query(String key, RandomAccessFile bucketStream,
			RandomAccessFile recordStream) {
		ArrayList<HashBucket> buckets = new ArrayList<HashBucket>();
		if (key.charAt(0) == '0') {
			rootNode0.query(key, bucketStream, key.length(), buckets);
		} else if (key.charAt(0) == '1') {
			rootNode1.query(key, bucketStream, key.length(), buckets);
		}
		//System.out.println("Number of buckets fetched: "+buckets.size());
		
		ArrayList<DataRecord> records = new ArrayList<DataRecord>();
		for (HashBucket bucket : buckets) {
			bucket.getRecords(records, recordStream);
		}
		
		for (DataRecord record: records) {
			System.out.println(record.getLastName()+", "+record.getFirstName()+", "
        			+record.getMiddleName()+", "+record.getSuffix()+", "
					+record.getBirthMonth()+", "+record.getBirthDay()+", "
        			+record.getBirthYear()+", "+record.getGender()+", "
					+record.getBirthCity());
		}
		
		System.out.println("Number of matching records: "+records.size());
	}
	
	/*
	 * Write the tree structure from 'stream' to disk.
	 */
	public void flattenTree(RandomAccessFile stream) {
		rootNode0.flatten(stream);
		rootNode1.flatten(stream);
		this.bucketPool.get(2).printSlots();
	}
	
	/*
	 * Fetch the tree structure from disk into 'stream'.
	 */
	public void fetchTree(RandomAccessFile stream) {
		rootNode0.fetchNodes(stream);
		rootNode1.fetchNodes(stream);
	}
}
