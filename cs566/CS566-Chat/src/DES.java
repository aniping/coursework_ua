/**
 * CS466/566 Assignment 5: Part A - Symmetric Key Cryptography (DES).
 * 
 * Team members: Praharsh Srinivasula, Shuo Yang
 * 
 * Description: 
 * The program implements the DES algorithm with CBC mode to encrypt/decrypt a file.
 * The program takes an ASCII file as input, given a 64 bit key in hex format,
 * encrypts it into an cipher text file. Then given the cipher text file as input,
 * the program will decrypt it back to the original plain text file.
 */

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Collections;
import java.security.SecureRandom;
import gnu.getopt.Getopt;

public class DES {

	public static void main(String[] args) {

		StringBuilder inputFile = new StringBuilder();
		StringBuilder outputFile = new StringBuilder();
		StringBuilder keyStr = new StringBuilder();
		StringBuilder encrypt = new StringBuilder();

		if (args.length == 0) {
			System.err.println("Please use -h option to see all available options.");
		}
		
		// process command line options
		pcl(args, inputFile, outputFile, keyStr, encrypt);

		if(keyStr.toString() != "" && encrypt.toString().equals("e")){
			encrypt(keyStr, inputFile, outputFile);
		} else if(keyStr.toString() != "" && encrypt.toString().equals("d")){
			decrypt(keyStr, inputFile, outputFile);
		}
	}

	/*
	 * Decryption routine 
	 */
	private static void decrypt(StringBuilder keyStr, StringBuilder inputFile,
			StringBuilder outputFile) {
		String key = DESKey.keyFormatCheck(keyStr);
		if (key == null) {
			System.exit(-1);
		}

		if (!DESKey.weakkeyCheck(key)) {
			System.exit(-1);
		}
		
		// Convert the key from the string to bitset (64 bits)
		BitSet key_64bit = DESKey.produceKeyFromHexStr(key);
		if (!DESKey.notWeakKey(key_64bit)) {
			System.err.println("This is a weak key! Please provide a non-weak 64-bit key!");
			System.exit(0);
		}
		// Generate 16 subkeys, each of 48 bits.
		ArrayList<BitSet> subkeys = DESKey.produceSubkeys(key_64bit);
		
		try {
			PrintWriter writer = new PrintWriter(outputFile.toString(), "UTF-8");
			List<String> lines = Files.readAllLines(Paths.get(inputFile.toString()), 
					Charset.defaultCharset());
			
			// Put the input data into a bitset.
			BitSet outBits = new BitSet(lines.size() * 64);
			for (int i = 0; i < lines.size(); i++) {
				for (int j = 0; j < 64; j++) {
					if (lines.get(i).charAt(j) == '1') {
						outBits.set(i*64 + j);
					}
				}
			}
			
			// Split into 64-bit blocks
			ArrayList<BitSet> outblocks = DESEncrypt.splitInput2bitBlocks(outBits, 
					lines.size() * 64);
			
			Collections.reverse(subkeys); // keys should be applied in reverse order
			BitSet IV = outblocks.get(0); // first block is IV (initiliztion vector)
			
			/* Apply CBD mode to decrypt */
			BitSet prevEecrypted = IV;
			ArrayList <BitSet> decryptedBlocks = new ArrayList <BitSet>();
			for (int i = 1; i < outblocks.size(); i++) {
				BitSet decryptedBlock = DES_encrypt(outblocks.get(i), subkeys);
				decryptedBlock.xor(prevEecrypted);
				prevEecrypted = outblocks.get(i);
				decryptedBlocks.add(decryptedBlock);
			}
			
			/* Convert the decrypted bit blocks into string */
			StringBuilder decryptedText = new StringBuilder();
			for (int i = 0; i < decryptedBlocks.size(); i++) {
				byte[] decrypted_byte = decryptedBlocks.get(i).toByteArray();
				decryptedText.append(new String(decrypted_byte));
			}
			
			System.out.print(decryptedText.toString());
			writer.print(decryptedText.toString());
			writer.flush();
			writer.close();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/*
	 * Encryption routine
	 */
	private static void encrypt(StringBuilder keyStr, StringBuilder inputFile,
			StringBuilder outputFile) {
		String key = DESKey.keyFormatCheck(keyStr);
		if (key == null) {
			System.exit(-1);
		}

		if (!DESKey.weakkeyCheck(key)) {
			System.exit(-1);
		}
		
		// Convert the key from the string to bitset (64 bits)
		BitSet key_64bit = DESKey.produceKeyFromHexStr(key);
		
		// Generate 16 subkeys, each of 48 bits.
		ArrayList<BitSet> subkeys = DESKey.produceSubkeys(key_64bit);
		
		try {
			PrintWriter writer = new PrintWriter(outputFile.toString(), "UTF-8");

			/* Convert input into bits */
			byte[] inputBytes = Files.readAllBytes(Paths.get(inputFile.toString()));
			BitSet inputBits = BitSet.valueOf(inputBytes);
			ArrayList<BitSet> blocks = DESEncrypt.splitInput2bitBlocks(inputBits, inputBytes.length * 8);
			
			/* CBC mode of operation */
			SecureRandom random = new SecureRandom();
			byte rand_bytes[] = new byte[8]; // 64 bits IV
			random.nextBytes(rand_bytes);
			BitSet IV64bit = BitSet.valueOf(rand_bytes);
			
			BitSet encryptedBit64;
			BitSet prevEncryptedBlock = IV64bit;
			ArrayList<BitSet> encryptedBlocks = new ArrayList<BitSet>();
			encryptedBlocks.add(IV64bit); // prepend IV at the beginning
			for (int i = 0; i < blocks.size(); i++) {
				blocks.get(i).xor(prevEncryptedBlock);
				encryptedBit64 = DES_encrypt(blocks.get(i), subkeys);
				prevEncryptedBlock = encryptedBit64;
				encryptedBlocks.add(encryptedBit64);
			}
			
			/* Print the enrypted message as ascii hex string. */
			System.out.println("Encrypted message as ascii hex string:\n");
			for (int i = 1; i < encryptedBlocks.size(); i++) {
				byte[] vals = encryptedBlocks.get(i).toByteArray();
				
				StringBuilder key_str = new StringBuilder();
				for (int j = 0; j < vals.length; j++) {
					key_str.append(String.format("%02x", vals[j]));
				}
				System.out.println(key_str.toString());
			}
			
			/* Write the encrypted data as bit strings. */
			for (int i = 0; i < encryptedBlocks.size(); i++) {
				StringBuilder encryptedText = new StringBuilder();
				for (int j = 0; j < 64; j++) {
					if (encryptedBlocks.get(i).get(j)) {
						encryptedText.append('1');
					} else {
						encryptedText.append('0');
					}
				}
				writer.print(encryptedText.toString() + "\n");
			}
			
			writer.flush();
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static String encryptLine(StringBuilder keyStr, String line) {
		String key = keyStr.toString();

		// Convert the key from the string to bitset (64 bits)
		BitSet key_64bit = DESKey.produceKeyFromHexStr(key);

		// Generate 16 subkeys, each of 48 bits.
		ArrayList<BitSet> subkeys = DESKey.produceSubkeys(key_64bit);

		/* Convert input into bits */
		byte[] inputBytes = line.getBytes();
		BitSet inputBits = BitSet.valueOf(inputBytes);
		ArrayList<BitSet> blocks = DESEncrypt.splitInput2bitBlocks(inputBits, inputBytes.length * 8);

		/* CBC mode of operation */
		SecureRandom random = new SecureRandom();
		byte rand_bytes[] = new byte[8]; // 64 bits IV
		random.nextBytes(rand_bytes);
		BitSet IV64bit = BitSet.valueOf(rand_bytes);

		BitSet encryptedBit64;
		BitSet prevEncryptedBlock = IV64bit;
		ArrayList<BitSet> encryptedBlocks = new ArrayList<BitSet>();
		encryptedBlocks.add(IV64bit); // prepend IV at the beginning
		for (int i = 0; i < blocks.size(); i++) {
			blocks.get(i).xor(prevEncryptedBlock);
			encryptedBit64 = DES_encrypt(blocks.get(i), subkeys);
			prevEncryptedBlock = encryptedBit64;
			encryptedBlocks.add(encryptedBit64);
		}

		StringBuilder output = new StringBuilder();
		/* Write the encrypted data as bit strings. */
		for (int i = 0; i < encryptedBlocks.size(); i++) {
			StringBuilder encryptedText = new StringBuilder();
			for (int j = 0; j < 64; j++) {
				if (encryptedBlocks.get(i).get(j)) {
					encryptedText.append('1');
				} else {
					encryptedText.append('0');
				}
			}
			output.append(encryptedText.toString());
		}
		
		return output.toString();
	}
	
	public static String decryptLine(StringBuilder keyStr, String line) {
		String key = keyStr.toString();
		
		// Convert the key from the string to bitset (64 bits)
		BitSet key_64bit = DESKey.produceKeyFromHexStr(key);
		
		// Generate 16 subkeys, each of 48 bits.
		ArrayList<BitSet> subkeys = DESKey.produceSubkeys(key_64bit);
		
		ArrayList<String> lines = new ArrayList<String>();
		for (int i = 0; i < line.length() / 64; i++) {
			lines.add(line.substring(i*64, i*64 + 64));
		}

		// Put the input data into a bitset.
		BitSet outBits = new BitSet(lines.size() * 64);
		for (int i = 0; i < lines.size(); i++) {
			for (int j = 0; j < 64; j++) {
				if (lines.get(i).charAt(j) == '1') {
					outBits.set(i*64 + j);
				}
			}
		}

		// Split into 64-bit blocks
		ArrayList<BitSet> outblocks = DESEncrypt.splitInput2bitBlocks(outBits, 
				lines.size() * 64);

		Collections.reverse(subkeys); // keys should be applied in reverse order
		BitSet IV = outblocks.get(0); // first block is IV (initiliztion vector)

		/* Apply CBD mode to decrypt */
		BitSet prevEecrypted = IV;
		ArrayList <BitSet> decryptedBlocks = new ArrayList <BitSet>();
		for (int i = 1; i < outblocks.size(); i++) {
			BitSet decryptedBlock = DES_encrypt(outblocks.get(i), subkeys);
			decryptedBlock.xor(prevEecrypted);
			prevEecrypted = outblocks.get(i);
			decryptedBlocks.add(decryptedBlock);
		}

		/* Convert the decrypted bit blocks into string */
		StringBuilder decryptedText = new StringBuilder();
		for (int i = 0; i < decryptedBlocks.size(); i++) {
			byte[] decrypted_byte = decryptedBlocks.get(i).toByteArray();
			decryptedText.append(new String(decrypted_byte));
		}

//		System.out.print(decryptedText.toString());
		return decryptedText.toString();
	}
	
	/**
	 * Core implementation of DES. 
	 * It actually implements a balanced Feistel network described by DES.
	 * @param block 64-bit message block
	 * @param subkeys a list of 16 subkeys, each is 48 bits long
	 * @return encrypted data as a 64-bit bitset. 
	 */
	private static BitSet DES_encrypt(BitSet block, ArrayList<BitSet> subkeys) {
		/* Apply initial permutation */
		BitSet blockIP = DESEncrypt.initialPermutation(block);
		
		/*-- Apply Feistel function (F function) of DES --*/
		
		BitSet L0 = blockIP.get(0, 32);
		BitSet R0 = blockIP.get(32, 64);

		BitSet Lprev = L0;
		BitSet Rprev = R0;
		BitSet Lcurr = Rprev, Rcurr = Lprev;

		for (int iter = 0; iter < 16; iter++) {
			Lcurr = Rprev;
			Rcurr = Lprev; 
			Rcurr.xor(DESEncrypt.func(Rprev, subkeys.get(iter)));
			Lprev = Lcurr;
			Rprev = Rcurr;
		}
		
		/*-- Apply Feistel function (F function) of DES --*/
		
		/* Apply final permutation */
		
		int[] fptable = DESEncrypt.FPTable();
		BitSet encryptedBit64 = new BitSet(64);

		for (int i = 0; i < fptable.length; i++) {
			if (fptable[i] > 32) {
				if (Lcurr.get(fptable[i]-32-1)) {
					encryptedBit64.set(i);
				}
			} else {
				if (Rcurr.get(fptable[i]-1)) {
					encryptedBit64.set(i);
				}
			}
		}

		return encryptedBit64;
	}

	/*
	 * Generate a non-weak 64-bit key.
	 */
	static String genDESkey(){
		BitSet key_64bit;
		
		while (true) {
			key_64bit = DESKey.produceKey();
//			DESKey.printKeyHex(key_64bit.toByteArray());
			
			if (DESKey.notWeakKey(key_64bit)) {
				break;
			} else {
				continue;
			}
		}

		StringBuilder key_str = new StringBuilder();
		for (int i = 0; i < key_64bit.toByteArray().length; i++) {
			key_str.append(String.format("%02x", key_64bit.toByteArray()[i]));
		}
		
		return key_str.toString();
	}


	/**
	 * This function Processes the Command Line Arguments.
	 * -p for the port number you are using
	 * -h for the host name of system
	 */
	private static void pcl(String[] args, StringBuilder inputFile,
			StringBuilder outputFile, StringBuilder keyString,
			StringBuilder encrypt) {
		/*
		 * http://www.urbanophile.com/arenn/hacking/getopt/gnu.getopt.Getopt.html
		 */	
		Getopt g = new Getopt("DES cipher", args, "hke:d:i:o:");
		int c;
		String arg;
		while ((c = g.getopt()) != -1){
			switch(c){
			case 'o':
				arg = g.getOptarg();
				outputFile.append(arg);
				break;
			case 'i':
				arg = g.getOptarg();
				inputFile.append(arg);
				break;
			case 'e':
				arg = g.getOptarg();
				keyString.append(arg);
				encrypt.append("e");
				break;
			case 'd':
				arg = g.getOptarg();
				keyString.append(arg);
				encrypt.append("d");
				break;
			case 'k':
				genDESkey();
				break;
			case 'h':
				callUseage(0);
			case '?':
				break; // getopt() already printed an error
				//
			default:
				break;
			}
		}

	}

	private static void callUseage(int exitStatus) {

		String useage = "Usage: java DES <options>\n" +
				"where possible options include:\n" +
				"  -h\t\t\t\t\t\t\t\tPrint out help information\n" + 
				"  -k\t\t\t\t\t\t\t\tGenerate a DES key, encoded in hex,"
				+ " printed on the command line\n" +
				"  -e <64_bit_key_in_hex> -i <input_file> -o <output_file>\t" + 
				"Encrypt the file <input_file> using <64_bit_key_in_hex>\n"
				+ "\t\t\t\t\t\t\t\twith the enrypted data written to <output_file>\n"
				+ "  -d <64_bit_key_in_hex> -i <input_file> -o <output_file>\t"
				+ "Decrypt the file <input_file> using <64_bit_key_in_hex>\n"
				+ "\t\t\t\t\t\t\t\twith the decrypted data stored in <output_file>\n";

		System.err.println(useage);
		System.exit(exitStatus);

	}

}
