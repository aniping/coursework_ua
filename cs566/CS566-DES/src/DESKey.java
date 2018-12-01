/**
 * Routines related to DES key generataion.
 */

import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.BitSet;

public class DESKey {

	/*
	 * A list of DES weak keys or semi-weak keys. Used for weak key detection.
	 * 
	 * Credit: http://www.umich.edu/~x509/ssleay/des-weak.html
	 */
	static String[] weakKeyList() {
		String[] weakKeys = {"0101010101010101", "fefefefefefefefe", "1f1f1f1f1f1f1f1f",
				"e0e0e0e0e0e0e0e0", "01fe01fe01fe01fe", "1fe01fe01fe01fe0", "01e001e001e001e0",
				"1ffe1ffe1ffe1ffe", "011f011f011f011f", "e0fee0fee0fee0fe", "fe01fe01fe01fe01",
				"e01fe01fe01fe01f", "e001e001e001e001", "fe1ffe1ffe1ffe1f", "1f011f011f011f01",
				"fee0fee0fee0fee0", "e0e0e0e0f1f1f1f1", "1f1f1f1f0e0e0e0e"};
		
		return weakKeys;
	}
	
	/*
	 * Perform weak key check against the hardcoded weak key list.
	 */
	static boolean weakkeyCheck(String key) {
		String[] weakkeys = weakKeyList();
		for (int i = 0; i < weakkeys.length; i++) {
			if (key.equals(weakkeys[i])) {
				System.err.println(key + " is a weak key or semi-weak key!\n" +
						"Please provide a non-weak 64-bit key!");
				return false; // a weak key found
			}
		}
		return true;
	}
	
	/*
	 * Perform format check on the given input key with string format
	 * and return the key with correct format.
	 */
	static String keyFormatCheck(StringBuilder keyStr) {
		String key = keyStr.toString().toLowerCase(); // convert to lower case
		
		if (key.length() == 16) { // correct format 1: string with 16 hex character
			return key;
		}
		if (key.length() == 18 && key.substring(0, 2).equals("0x")) {
			// correct format 2: string with 16 hex character prepended with 0x
			return key.substring(2, key.length()); 
		}
		
		System.err.println("The given does not have the correct format!\n"+
				"Correct format 1 (case insensitive): 0123456789abcdef\n" + 
				"Correct format 2 (case insensitive): 0x0123456789abcdef");
		return null;
	}
	
	/**
	 * Return the table of permuted choice 1.
	 */
	static int[] permutedChoice1() {
		int[] pc1 = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,
				10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,
				47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53, 
				45,37,29,21,13,5,28,20,12,4};
		return pc1;
	}
	
	static int[] permutedChoice2() {
		int[] pc2 = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,
				16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,
				44,49,39,56,34,53,46,42,50,36,29,32};
		
		return pc2;
	}
	
	/**
	 * Print out the key as a bit string.
	 * @param key as a bit set
	 * @param len length of the bit set
	 */
	static void printKeyBinary(BitSet key, int len) {
		for (int i = 0; i < len; ) {
			if (key.get(i))
				System.out.print("1");
			else
				System.out.print("0");
			i++;
			if ((len % 32 == 0) && i % 8 == 0) {
				System.out.print("  ");
			} else if (len % 28 == 0 && i % 7 == 0) {
				System.out.print("  ");
			} else if (len == 48 && i % 6 == 0) {
				System.out.print("  ");
			}
		}
		System.out.println();
	}
	
	/**
	 * Print out the key as a hex string.
	 * @param key as a byte array
	 */
	static void printKeyHex(byte[] key) {
		StringBuilder key_str = new StringBuilder();
		for (int i = 0; i < key.length; i++) {
			key_str.append(String.format("%02x", key[i]));
		}
		System.out.println("0x" + key_str.toString());
	}

	/**
	 * Given the key as a byte array, flip every 8th bit of
	 * each byte to make sure it is odd parity. Store and return the 
	 * key as a bit set. 
	 * @param key
	 * @return key as a bit set (64-bit)
	 */
	static BitSet convert2BitsWithOddParity(byte[] key) {
		BitSet bs = BitSet.valueOf(key);

		int i;
		for (i = 0; i < bs.size(); i += 8) {
			int j, bit_count = 0;
			for (j = 0; j < 8; j++) {
				if (bs.get(i+j)) {
					bit_count++;
				}
			}
			if (bit_count % 2 == 0) {
				bs.flip(i+j-1);
			}
		}

		return bs;
	}

	/*
	 * Apply permutation choice 1 to the key.
	 */
	static BitSet keyPermutePC1(BitSet key_64bit) {
		int[] pc1 = permutedChoice1();
		
		BitSet key_56bit = new BitSet(56);
		for (int i = 0; i < pc1.length; i++) {
			if (key_64bit.get(pc1[i]-1))
				key_56bit.set(i);
		}
		
		return key_56bit;
	}
	
	/*
	 * Generate 16 subkeys, each with 48 bits.
	 */
	static void generateSubkeys(ArrayList<BitSet> subkeys, 
			ArrayList<BitSet> CList, ArrayList<BitSet> DList) {
		/*
		 * Apply permutation choice 2 to the subkeys.
		 */
		int[] pc2 = permutedChoice2();
		for (int i = 0; i < 16; i++) {
			BitSet subkey = new BitSet(48);
			for (int j = 0; j < pc2.length; j++) {
				if (pc2[j] <= 28) { // use CList
					if (CList.get(i+1).get(pc2[j]-1)) {
						subkey.set(j);
					}
				} else {  // use DList
					if (DList.get(i+1).get(pc2[j]-28-1)) {
						subkey.set(j);
					}
				}
			}
			subkeys.add(subkey);
		}
	}
	
	/*
	 * Split key into blocks.
	 */
	static void splitKey2Blocks(ArrayList<BitSet> CList,
			ArrayList<BitSet> DList, BitSet C0, BitSet D0) {
		
		CList.add(C0);
		DList.add(D0);
		
		for (int i = 1; i <= 16; i++) {
			int num_left_shift;
			if (i == 1 || i == 2 || i == 9 || i == 16) {
				num_left_shift = 1;
			} else {
				num_left_shift = 2;
			}
			BitSet Ctmp = new BitSet(28);
			BitSet Dtmp = new BitSet(28);
			
			BitSet Cprev = CList.get(i-1);
			BitSet Dprev = DList.get(i-1);
			for (int j = 0; j < 28; j++) {
				if ((j + num_left_shift) < 28) {
					if (Cprev.get(j+num_left_shift)) {
						Ctmp.set(j);
					}
					if (Dprev.get(j+num_left_shift)) {
						Dtmp.set(j);
					}
				} else { // wrap around
					if (Cprev.get(j+num_left_shift-28)) {
						Ctmp.set(j);
					}
					if (Dprev.get(j+num_left_shift-28)) {
						Dtmp.set(j);
					}
				}
			}
			CList.add(Ctmp);
			DList.add(Dtmp);
		}
	}
	
	/*
	 * Generate a DES key as a bitset.
	 */
	static BitSet produceKey() {
		SecureRandom random = new SecureRandom();
		byte raw_key[] = new byte[8]; // 64 bits key
		random.nextBytes(raw_key);
		
		BitSet key_64bit = DESKey.convert2BitsWithOddParity(raw_key);
		return key_64bit;
	}
	
	/*
	 * Convert a hex number to a bitset.
	 */
	static void hex2bitset(char hex, BitSet bs, int start) {
		switch(hex) {
		case '1':
			bs.set(start+3); break;
		case '2':
			bs.set(start+2); break;
		case '3':
			bs.set(start+3);
			bs.set(start+2); break;
		case '4':
			bs.set(start+1); break;
		case '5':
			bs.set(start+1);
			bs.set(start+3); break;
		case '6':
			bs.set(start+2);
			bs.set(start+1); break;
		case '7':
			bs.set(start+3);
			bs.set(start+1);
			bs.set(start+2); break;
		case '8':
			bs.set(start); break;
		case '9':
			bs.set(start);
			bs.set(start+3); break;
		case 'a':
			bs.set(start);
			bs.set(start+2); break;
		case 'b':
			bs.set(start);
			bs.set(start+2);
			bs.set(start+3); break;
		case 'c':
			bs.set(start);
			bs.set(start+1); break;
		case 'd':
			bs.set(start);
			bs.set(start+1);
			bs.set(start+3); break;
		case 'e':
			bs.set(start);
			bs.set(start+1);
			bs.set(start+2); break;
		case 'f':
			bs.set(start);
			bs.set(start+1);
			bs.set(start+2);
			bs.set(start+3); break;
		}
	}
	
	/*
	 * Generate a DES key given the hex string format of the key.
	 */
	static BitSet produceKeyFromHexStr(String key_str) {
		BitSet key_64bit = new BitSet(64);
		
		for (int i = 0; i < key_str.length(); i++) {
			hex2bitset(key_str.charAt(i), key_64bit, i*4);
		}
		
		return key_64bit;
	}
	
	/*
	 * Check if the given key is a weak key or not.
	 */
	static boolean notWeakKey(BitSet key_64bit) {
		ArrayList<BitSet> subkeys_48bit = DESKey.produceSubkeys(key_64bit);
		
		for (int i = 0; i < 15; i++) {
			if (subkeys_48bit.get(i).equals(subkeys_48bit.get(i+1))) {
				continue;
			} else {
				return true;
			}
		}
		
		return false;
	}
	
	/*
	 * Generate 16 48-bit subkeys given the 64-bit key.
	 */
	static ArrayList<BitSet> produceSubkeys(BitSet key_64bit) {
		BitSet key_56bit = DESKey.keyPermutePC1(key_64bit);
		
		ArrayList<BitSet> CList = new ArrayList<BitSet>(17);
		ArrayList<BitSet> DList = new ArrayList<BitSet>(17);
		BitSet C0 = key_56bit.get(0, 28);
		BitSet D0 = key_56bit.get(28, 56);

		DESKey.splitKey2Blocks(CList, DList, C0, D0);
		ArrayList<BitSet> subkeys = new ArrayList<BitSet>(16);
		DESKey.generateSubkeys(subkeys, CList, DList);
		return subkeys;
	}
}
