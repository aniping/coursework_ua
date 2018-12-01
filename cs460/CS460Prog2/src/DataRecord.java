/*
 * DataRecord.java -- Used for dumping and fetching data record 
 * to and from a RandomAccessFile data stream.
 *
 * Author:  Shuo Yang (2015/09/14)
 * 
 * Code was adapted from Dr. McCann's BinaryIO.java.
 */

import java.io.*;

/*
 * DataRecord class -- Holds a record from the input file with
 * each string field padded to its maximum length such that all
 * records are of the same length. It also provides methods for
 * dumping and fetching data record to and from a RandomAccessFile 
 * data stream.
 */
class DataRecord
{
	// Maximum length for each string field in the record
	
	public static final int MAX_LNAME_LENGTH = 77;
	public static final int MAX_FNAME_LENGTH = 17;
	public static final int MAX_MNAME_LENGTH = 17;
	public static final int MAX_SUFFIX_LENGTH = 111;
	public static final int MAX_BIRTH_CITY_LENGTH = 14;

	// 3 ints (12 bytes) + 5 strs + 1 char (2 bytes) = 250 bytes
	// Note that allocating 2 bytes for a char is becasue of 
	// RandomAccessFile writes/reads 2 bytes when its writeChar()
	// and readChar() methods are called.
	public static final int RECORD_LENGTH = 250; 

	// The data fields that comprise a record of our file

	private    int birthMonth;    // month of birth
	private    int birthDay;      // day of birth
	private    int birthYear;     // year of birth
	private String lastName;      // last name
	private String firstName;     // first name
	private String middleName;    // middle name
	private String suffix;        // suffix
	private String birthCity;     // city of birth
	private   char gender;        // gender

	// 'Getters' for the data field values

	public     int getBirthMonth() { return this.birthMonth; }
	public     int getBirthDay()   { return this.birthDay; }
	public     int getBirthYear()  { return this.birthYear; }
	public  String getLastName()   { return this.lastName; }
	public  String getFirstName()  { return this.firstName; }
	public  String getMiddleName() { return this.middleName; }
	public  String getSuffix()     { return this.suffix; }
	public  String getBirthCity()  { return this.birthCity; }
	public    char getGender()     { return this.gender; }

	// 'Setters' for the data field values

	public  void setBirthMonth(int month)        { this.birthMonth = month; }
	public  void setBirthDay(int day)            { this.birthDay = day; }
	public  void setBirthYear(int year)          { this.birthYear = year; }
	public  void setLastName(String lname)       { this.lastName = lname; }
	public  void setFirstName(String fname)      { this.firstName = fname; }
	public  void setMiddleName(String mname)     { this.middleName = mname; }
	public  void setSuffix(String suffix)        { this.suffix = suffix; }
	public  void setBirthCity(String birthcity)  { this.birthCity = birthcity; }
	public  void setGender(char gender)          { this.gender = gender; }

	/* dumpObject(stream) -- write the content of the object's fields
	 * to the file represented by the given RandomAccessFile object
	 * reference.  Primitive types (e.g., int) are written directly.
	 * Non-fixed-size values (e.g., strings) are converted to the
	 * maximum allowed size before being written.  The result is a
	 * file of uniformly-sized records.  Also note that text is
	 * written with just one byte per character, meaning that we are
	 * not supporting Unicode text.
	 */

	public void dumpObject(RandomAccessFile stream)
	{
		StringBuffer lname = new StringBuffer(this.lastName);  // paddable last name
		StringBuffer fname = new StringBuffer(this.firstName);
		StringBuffer mname = new StringBuffer(this.middleName);
		StringBuffer suffix = new StringBuffer(this.suffix);
		StringBuffer birthcity = new StringBuffer(this.birthCity);

		try {
			stream.writeInt(this.birthMonth);
			stream.writeInt(this.birthDay);
			stream.writeInt(this.birthYear);
			stream.writeChar(this.gender);
			
			lname.setLength(DataRecord.MAX_LNAME_LENGTH); // padding last name
			stream.writeBytes(lname.toString());
			
			fname.setLength(DataRecord.MAX_FNAME_LENGTH);
			stream.writeBytes(fname.toString());
			
			mname.setLength(DataRecord.MAX_MNAME_LENGTH);
			stream.writeBytes(mname.toString());
			
			suffix.setLength(DataRecord.MAX_SUFFIX_LENGTH);
			stream.writeBytes(suffix.toString());
			
			birthcity.setLength(DataRecord.MAX_BIRTH_CITY_LENGTH);
			stream.writeBytes(birthcity.toString());
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't write to the file;\n\t"
					+ "perhaps the file system is full?");
			System.exit(-1);
		}
	}

	/* fetchObject(stream) -- read the content of the object's fields
	 * from the file represented by the given RandomAccessFile object
	 * reference, starting at the current file position.  Primitive
	 * types (e.g., int) are read directly.  To create Strings containing
	 * the text, because the file records have text stored with one byte
	 * per character, we can read a text field into an array of bytes and
	 * use that array as a parameter to a String constructor.
	 */

	public void fetchObject(RandomAccessFile stream)
	{
		byte[] lname = new byte[DataRecord.MAX_LNAME_LENGTH];  // file -> byte[] -> String
		byte[] fname = new byte[DataRecord.MAX_FNAME_LENGTH];
		byte[] mname = new byte[DataRecord.MAX_MNAME_LENGTH];
		byte[] suffix = new byte[DataRecord.MAX_SUFFIX_LENGTH];
		byte[] birthcity = new byte[DataRecord.MAX_BIRTH_CITY_LENGTH];

		try {
			this.birthMonth = stream.readInt();
			this.birthDay = stream.readInt();
			this.birthYear = stream.readInt();
			this.gender = stream.readChar();
			
			stream.readFully(lname);
			this.lastName = new String(lname);
			
			stream.readFully(fname);
			this.firstName = new String(fname);
			
			stream.readFully(mname);
			this.middleName = new String(mname);
			
			stream.readFully(suffix);
			this.suffix = new String(suffix);
			
			stream.readFully(birthcity);
			this.birthCity = new String(birthcity);
		} catch (IOException e) {
			System.out.println("I/O ERROR: Couldn't read from the stream;\n\t"
					+ "perhaps the file system is full?");
			System.exit(-1);
		}
	}
}

