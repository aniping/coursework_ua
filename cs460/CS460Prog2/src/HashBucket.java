import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.*;

/**
 * Represents a slot inside a hash bucket.
 * @author shuoyang
 *
 */
class Slot {
	private String key;
	private int offset2DBFile;
	
	public void setKey(String key) {
		this.key = key;
	}
	
	public void setOffset2DBFile(int offset) {
		this.offset2DBFile = offset;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public int getOffset2DBFile() {
		return this.offset2DBFile;
	}
	
	public String toString() {
		return '(' + this.key + ", " + this.offset2DBFile + ')';
	}
}

/**
 * Represents a hash bucket.
 * 
 * @author shuoyang
 *
 */
public class HashBucket {
	
	public static final int NUM_SLOTS = 750;
	public static final int KEY_LENGTH = 4;  // "MMDD"
	public static final int OFFSET_LENGTH = 4; // integer offset
	public static final int BUCKET_LENGTH = (KEY_LENGTH+OFFSET_LENGTH) * NUM_SLOTS; // how many bytes
	public static final String emptyKey = "0000"; // indicate an empty key which should be ignored
	public static final int emptyOffset = -1; // indicate an empty offset which should be ignored
	
	private ArrayList<Slot> slots;
	private int offsetInBucketFile;
	private int bucketNo;
	
	HashBucket () {
		slots = new ArrayList<Slot>(NUM_SLOTS);
	}
	HashBucket (int bucketNo) {
		slots = new ArrayList<Slot>(NUM_SLOTS);
		offsetInBucketFile = bucketNo * BUCKET_LENGTH;
		this.bucketNo = bucketNo;
	}
	
	public int getOffsetInBucketFile() {
		return this.offsetInBucketFile;
	}
	
	public int getBucketNo() {
		return this.bucketNo;
	}
	
	public ArrayList<Slot> getSlots() {
		return this.slots;
	}
	public void deleteSlot() {
		
	}
	
	public boolean isFull() {
		if (slots.size() >= NUM_SLOTS) return true;
		else return false;
	}
	
	public void insertSlot(String key, int record_offset) {
		Slot slot = new Slot();
		slot.setKey(key);
		slot.setOffset2DBFile(record_offset);
		slots.add(slot);
		//System.out.println("Inserting slot to bucket "+bucketNo);
	}
	
	public void printSlots() {
		for (int i = 0; i < slots.size(); ++i) {
			System.out.println(slots.get(i).toString());
		}
	}
	
	public void getRecords(ArrayList<DataRecord> records, RandomAccessFile recordStream) {
		for (int i = 0; i < slots.size(); ++i) {
			if (slots.get(i).getOffset2DBFile() != -1) {
				try {
					DataRecord record = new DataRecord();
					recordStream.seek(slots.get(i).getOffset2DBFile());
					record.fetchObject(recordStream);
					records.add(record);
				} catch (IOException e) {
					System.out.println("I/O ERROR: Couldn't seek the record stream!\n");
					System.exit(-1);
				}
			}
		}
	}
	
	public void printFirstSlot() {
		System.out.println(slots.get(0).toString());
	}
	
	public void dumpBucket(RandomAccessFile stream)
	{
		try {
			int i;
			for (i = 0; i < slots.size(); ++i) {
				stream.writeBytes(slots.get(i).getKey());
				stream.writeInt(slots.get(i).getOffset2DBFile());
			}
			
			// fill the remaining slots in bucket with empty key and offset.
			for ( ; i < HashBucket.NUM_SLOTS; ++i) {
				stream.writeBytes(HashBucket.emptyKey);
				stream.writeInt(HashBucket.emptyOffset);
			}
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't dump the buckt!\n");
			System.exit(-1);
		}
	}
	
	public void fetchBucket(RandomAccessFile stream)
	{
		try {
			byte[] key = new byte[HashBucket.KEY_LENGTH];
			int offset;
			for (int i = 0; i < HashBucket.NUM_SLOTS; ++i) {
				stream.readFully(key);
				offset = stream.readInt();
				Slot slot = new Slot();
				slot.setKey(new String(key));
				slot.setOffset2DBFile(offset);
				this.slots.add(slot);
			}
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't read from the stream;\n\t"
					+ "perhaps the file system is full?");
			System.exit(-1);
		}
	}
}
