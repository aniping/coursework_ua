import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;

/**
 * Create binary database file given the input text file.
 * 
 */

public class BinaryDBFile {
	// Index number for each field in the record from the input file

	public static final int LNAME_INDEX      = 0;
	public static final int FNAME_INDEX      = 1;
	public static final int MNAME_INDEX      = 2;
	public static final int SUFFIX_INDEX     = 3;
	public static final int MONTH_INDEX      = 4;
	public static final int DAY_INDEX        = 5;
	public static final int YEAR_INDEX 		 = 6;
	public static final int GENDER_INDEX 	 = 7;
	public static final int BIRTH_CITY_INDEX = 8;
	
	public static void create (String path2input)
	{
		File             fileRef;             // used to create the file
		RandomAccessFile dataStream = null;   // specializes the file I/O
		long             numberOfRecords = 0; // loop counter for reading file
		FileInputStream  fstream;
		BufferedReader br;
		
		/* Create a File object to represent the file and a
         * RandomAccessFile (RAF) object to supply appropriate
         * file access methods.  Note that there is a constructor
         * available for creating RAFs directly (w/o needing a
         * File object first), but having access to File object
         * methods is often handy.
         */
		fileRef = new File("objects.bin");
		try {
            dataStream = new RandomAccessFile(fileRef, "rw");
        } catch (IOException e) {
            System.out.println("I/O ERROR: Something went wrong with the "
                             + "creation of the RandomAccessFile object.");
            System.exit(-1);
        }
		
		// Open the input file
		try {
			fstream = new FileInputStream(path2input);
			br = new BufferedReader(new InputStreamReader(fstream));
			
			String line;
			String[] fields;
			
			String lname, fname, mname, suffix, birthcity;
			int month, day, year;
			char gender = 0;
			
			DataRecord record = new DataRecord();
			int numLines = 0;
			while ((line = br.readLine()) != null) {
				fields = line.split("\t");
				
				// assign initial values
				lname = "NONE"; fname = "NONE"; mname = "NONE"; 
				suffix = "NONE"; birthcity = "NONE";
				month = 0; day = 0; year = 0; gender = 0;
				
				if (fields.length == 0) {
					continue;	
				}		
								
				// incrementally fill each field
				
				if (fields.length > BinaryDBFile.LNAME_INDEX) { 
					lname = fields[BinaryDBFile.LNAME_INDEX]; 
				}
				if (fields.length > BinaryDBFile.FNAME_INDEX) { 
					fname = fields[BinaryDBFile.FNAME_INDEX]; 
				}
				if (fields.length > BinaryDBFile.MNAME_INDEX) { 
					mname = fields[BinaryDBFile.MNAME_INDEX]; 
				}
				if (fields.length > BinaryDBFile.SUFFIX_INDEX) { 
					suffix = fields[BinaryDBFile.SUFFIX_INDEX]; 
				}
				if (fields.length > BinaryDBFile.MONTH_INDEX) { 
					month = Integer.parseInt(fields[BinaryDBFile.MONTH_INDEX]); 
				}
				if (fields.length > BinaryDBFile.DAY_INDEX) { 
					day = Integer.parseInt(fields[BinaryDBFile.DAY_INDEX]); 
				}
				if (fields.length > BinaryDBFile.YEAR_INDEX) { 
					year = Integer.parseInt(fields[BinaryDBFile.YEAR_INDEX]); 
				}
				if (fields.length > BinaryDBFile.GENDER_INDEX) { 
					gender = fields[BinaryDBFile.GENDER_INDEX].charAt(0); 
				}
				if (fields.length > BinaryDBFile.BIRTH_CITY_INDEX) { 
					birthcity = fields[BinaryDBFile.BIRTH_CITY_INDEX];
				}
				
				record.setLastName(lname);
				record.setFirstName(fname);
				record.setMiddleName(mname);
				record.setSuffix(suffix);
				record.setBirthCity(birthcity);
				record.setBirthMonth(month);
				record.setBirthDay(day);
				record.setBirthYear(year);
				record.setGender(gender);
				
				record.dumpObject(dataStream);
				
				numLines++;
			}
			System.out.println("Num of lines: " + numLines);
			fstream.close();
			
			try {
	            dataStream.setLength(numLines * DataRecord.RECORD_LENGTH);
	        } catch (IOException e) {
	            System.out.println("I/O ERROR: Couldn't set the desired length for data stream.");
	            System.exit(-1);
	        }
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't open the input file. "
					+ "Please check the path of the given file.");
			System.exit(-1);
		}
				
		try {
			dataStream.seek(0);
		} catch (IOException e) {
			System.out.println("I/O ERROR: Seems we can't reset the file "
					+ "pointer to the start of the file.");
			System.exit(-1);
		}
		
		try {
			System.out.println("File length: " + dataStream.length());
            numberOfRecords = dataStream.length() / DataRecord.RECORD_LENGTH;
        } catch (IOException e) {
            System.out.println("I/O ERROR: Couldn't get the file's length.");
            System.exit(-1);
        }

        System.out.println("\nThere are " + numberOfRecords
                         + " records in the file.\n");
        
        // Print out the first 5 records 
        
		DataRecord record = new DataRecord();
		System.out.println("First 5 records:\n");
        for (int i = 0; i < 5; i++) {
        	record.fetchObject(dataStream);
        	System.out.println(record.getLastName()+", "+record.getFirstName()+", "
        			+record.getMiddleName()+", "+record.getSuffix()+", "
					+record.getBirthMonth()+", "+record.getBirthDay()+", "
        			+record.getBirthYear()+", "+record.getGender()+", "
					+record.getBirthCity());
        }
        
        // Print out the last 5 records 

        // Position the file pointer to the start of the last 5 records
        try {
			dataStream.seek((numberOfRecords-5) * DataRecord.RECORD_LENGTH);
		} catch (IOException e) {
			System.out.println("I/O ERROR: Seems we can't reset the file "
					+ "pointer to the start of the file.");
			System.exit(-1);
		}
		System.out.println("\nLast 5 records:\n");
        for (int i = 0; i < 5; i++) {
        	record.fetchObject(dataStream);
        	System.out.println(record.getLastName()+", "+record.getFirstName()+", "
        			+record.getMiddleName()+", "+record.getSuffix()+", "
					+record.getBirthMonth()+", "+record.getBirthDay()+", "
        			+record.getBirthYear()+", "+record.getGender()+", "
					+record.getBirthCity());
        }
        
        // Close data stream
        try {
        	dataStream.close();
        } catch (IOException e) {
        	System.out.println("I/O ERROR: Cannot close the data stream.");
			System.exit(-1);
        }
	}
}
