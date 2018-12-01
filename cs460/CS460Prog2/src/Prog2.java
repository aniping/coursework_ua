/**
 * Dynamic hashing implementation.
 * This program takes a text file of records of people born in Texas
 * in 1950 as input, creates a binary file for these records. 
 * 
 * Then creates a Dynamic Hash index for these binary records and use it to satisfy
 * a simple keywords-based query. In particular, we are to index on the compound key 
 * formed from the concatenation of the two-digit month and the two-digit day of each 
 * person’s birthdate. Thus, the key for a person born on August 14 would
 * be 0814, and the key for a birthdate of November 3 would be 1103.
 * 
 * Compile: javac *.java
 * Run: java Prog2 (if input TexasBirths1950.txt is under the same path as the source code)
 *      java Prog2 path/to/input
 * 
 * Author: Shuo Yang
 */

import java.io.*;
import java.util.Scanner;

public class Prog2 {
	
	public static void main (String [] args)
	{
		RandomAccessFile recordStream = null;   // stream for reading records from binary database file
		RandomAccessFile bucketStream = null;   // stream for write buckets to hash bucket file
		RandomAccessFile treeStream = null;   // stream for write directory tree to file
		
		DataRecord record = new DataRecord(); // hold each record read from binary database file
		String month, day;
		BucketDirectoryTree tree = null;
		int offset; // offset of record in binary database file
		File bucketFile, treeFile, recordFile; // hash bucket file and tree file
		long numRecord;
		
		bucketFile = new File("hash_buckets.bin");
		treeFile = new File("directory_tree.bin");
		recordFile = new File("objects.bin");
		try {
			if (!recordFile.exists()) { // create binary DB file if not exist
				if (args.length == 1) {
					BinaryDBFile.create(args[0]);
				} else { // default option
					BinaryDBFile.create("TexasBirths1950.txt");
				}
			}
			
			/* Create bucket file and tree file if they do not exist yet. */
			if (!bucketFile.exists() || !treeFile.exists()) {
				recordStream = new RandomAccessFile("objects.bin", "r");
				bucketStream = new RandomAccessFile(bucketFile, "rw");
				treeStream = new RandomAccessFile(treeFile, "rw");
				
				numRecord = recordStream.length() / DataRecord.RECORD_LENGTH;			
				tree = new BucketDirectoryTree();
				offset = 0;
				// fill first 2 buckets for testing
				for (int i = 0; i < numRecord; i++) {
					record.fetchObject(recordStream);
					month = Integer.toString(record.getBirthMonth());
					day = Integer.toString(record.getBirthDay());
					if (month.length() == 1) month = '0' + month;
					if (day.length() == 1) day = '0' + day;
					tree.insertKey2bucket(month, day, offset);
					offset += DataRecord.RECORD_LENGTH;
				}
				//tree.printBuckets();
				tree.dumpBuckets(bucketStream);
				tree.flattenTree(treeStream);

				recordStream.close();
			} 
			recordStream = new RandomAccessFile("objects.bin", "r");
			bucketStream = new RandomAccessFile(bucketFile, "rw");
			treeStream = new RandomAccessFile(treeFile, "rw");

			tree = new BucketDirectoryTree(true);
			tree.fetchTree(treeStream);
			Scanner scanner = new Scanner(System.in);
			while (true) {
				System.out.println("\nPlease enter a search key in the format of " +
						"mmdd (month, day) or mm (month only) or mmd (month and a prefix of day)");
				System.out.println("Enter Q to exit");
				System.out.print(">>> ");
				String key = scanner.next();
				if (key.equals("Q")) break;
				// TODO: need to perform key validity check, otherwise key like 0634 will
				// return the same results as 0630 because there is only one bucket for 063*.
				tree.query(key, bucketStream, recordStream);
			}
			scanner.close();
		} catch (IOException e) {
			System.out.println("I/O ERROR: Something went wrong with the "
                    + "opening of the binary database file using RandomAccessFile.");
            System.exit(-1);
		}
		
		try {
			bucketStream.close();
			treeStream.close();
			recordStream.close();
		} catch (IOException e) {
			System.out.println("I/O ERROR: couldn't close data stream.");
			System.exit(-1);
		}
	}
}
