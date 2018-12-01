/**
 * Routines related to DES encryption.
 */

import java.util.ArrayList;
import java.util.BitSet;

public class DESEncrypt {

	/*
	 * Split the input which is a bitset into 64-bit blocks.
	 */
	static ArrayList<BitSet> splitInput2bitBlocks(BitSet inputBits, int len) {
		int num_blocks;
		if (len % 64 == 0) {
			num_blocks = len / 64;
		} else {
			num_blocks = len/64 + 1;
		}
		
		ArrayList<BitSet> blocks = new ArrayList<BitSet>(num_blocks);
		int i;
		for (i = 0; i < num_blocks-1; i++) {
			BitSet block = inputBits.get(i*64, i*64 + 64);
			blocks.add(block);
		}
		
		/* Pad last block with 0 if needed */
		BitSet last_block = new BitSet(64);
		for (int j = 0; j < 64; j++) {
			if (inputBits.get(i*64 + j)) {
				last_block.set(j);
			}
		}
		blocks.add(last_block);
		
		return blocks;
	}
	
	/*
	 * Initial permutation table.
	 */
	static int[] IPTable() {
		int[] IP = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,
				64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,
				45,37,29,21,13,5,63,55,47,39,31,23,15,7};
		
		return IP;
	}
	
	/*
	 * Apply initial permutation.
	 */
	static BitSet initialPermutation(BitSet msgBlock) {
		BitSet msgBlockIP = new BitSet(64);
		int[] IP = IPTable();
		
		for (int i = 0; i < IP.length; i++) {
			if (msgBlock.get(IP[i]-1))
				msgBlockIP.set(i);
		}
		
		return msgBlockIP;
	}
	
	/*
	 * Bit selection table.
	 */
	static int[] bitSelTable() {
		int[] bitSel = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,
				16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
		
		return bitSel;
	}
	
	/*
	 * Expand 32bit bitset 'R' into a 48bit bitset.
	 */
	static BitSet expandRto48bits(BitSet R) {
		BitSet RE = new BitSet(48);
		int[] bitSel = bitSelTable();
		
		for (int i = 0; i < bitSel.length; i++) {
			if (R.get(bitSel[i]-1))
				RE.set(i);
		}
		
		return RE;
	}
	
	/*
	 * Get Sbox given sbox number.
	 */
	static ArrayList<byte[]> getSbox(int i) {
		switch(i) {
		case 1:
			ArrayList<byte[]> S1 = new ArrayList<byte[]>(4);
			byte[] S1_row1 = {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
			byte[] S1_row2 = {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8};
			byte[] S1_row3 = {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0};
			byte[] S1_row4 = {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13};
			S1.add(S1_row1);
			S1.add(S1_row2);
			S1.add(S1_row3);
			S1.add(S1_row4);
			return S1;
		case 2:
			ArrayList<byte[]> S2 = new ArrayList<byte[]>(4);
			byte[] S2_row1 = {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10};
			byte[] S2_row2 = {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5};
			byte[] S2_row3 = {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15};
			byte[] S2_row4 = {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9};
			S2.add(S2_row1);
			S2.add(S2_row2);
			S2.add(S2_row3);
			S2.add(S2_row4);
			return S2;
		case 3:
			ArrayList<byte[]> S3 = new ArrayList<byte[]>(4);
			byte[] S3_row1 = {10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4, 2,  8};
			byte[] S3_row2 = {13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1};
			byte[] S3_row3 = {13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7};
			byte[] S3_row4 = {1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12};
			S3.add(S3_row1);
			S3.add(S3_row2);
			S3.add(S3_row3);
			S3.add(S3_row4);
			return S3;
		case 4:
			ArrayList<byte[]> S4 = new ArrayList<byte[]>(4);
			byte[] S4_row1 = {7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15};
			byte[] S4_row2 = {13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9};
			byte[] S4_row3 = {10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4};
			byte[] S4_row4 = {3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14};
			S4.add(S4_row1);
			S4.add(S4_row2);
			S4.add(S4_row3);
			S4.add(S4_row4);
			return S4;
		case 5:
			ArrayList<byte[]> S5 = new ArrayList<byte[]>(4);
			byte[] S5_row1 = {2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9};
			byte[] S5_row2 = {14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6};
			byte[] S5_row3 = {4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14};
			byte[] S5_row4 = {11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3};
			S5.add(S5_row1);
			S5.add(S5_row2);
			S5.add(S5_row3);
			S5.add(S5_row4);
			return S5;
		case 6:
			ArrayList<byte[]> S6 = new ArrayList<byte[]>(4);
			byte[] S6_row1 = {12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4, 14,  7,   5, 11};
			byte[] S6_row2 = {10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8};
			byte[] S6_row3 = {9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6};
			byte[] S6_row4 = {4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13};
			S6.add(S6_row1);
			S6.add(S6_row2);
			S6.add(S6_row3);
			S6.add(S6_row4);
			return S6;
		case 7:
			ArrayList<byte[]> S7 = new ArrayList<byte[]>(4);
			byte[] S7_row1 = {4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1};
			byte[] S7_row2 = {13,  0, 11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6};
			byte[] S7_row3 = {1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2};
			byte[] S7_row4 = {6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12};
			S7.add(S7_row1);
			S7.add(S7_row2);
			S7.add(S7_row3);
			S7.add(S7_row4);
			return S7;
		case 8:
			ArrayList<byte[]> S8 = new ArrayList<byte[]>(4);
			byte[] S8_row1 = {13,  2,   8,  4,  6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7};
			byte[] S8_row2 = {1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2};
			byte[] S8_row3 = {7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8};
			byte[] S8_row4 = {2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11};
			S8.add(S8_row1);
			S8.add(S8_row2);
			S8.add(S8_row3);
			S8.add(S8_row4);
			return S8;
		}
		
		return null;
	}
	
	/*
	 * Apply SBox-'sno' to the bitset 'sixbit' and return a bitset with 4bits.
	 */
	static BitSet sboxProcess(BitSet sixbit, int sno) {
		byte[] values = sixbit.get(2, 6).toByteArray();
		int row, col;
		
		if (sixbit.get(0)) {
			if (sixbit.get(1)) { // 11
				row = 3;
			} else { // 10
				row = 2;
			}
		} else {
			if (sixbit.get(1)) { // 01
				row = 1;
			} else { // 00
				row = 0;
			}
		}
		
		if (values.length > 0) {
			col = values[0];
		} else {
			col = 0;
		}
		
		ArrayList<byte[]> sbox = getSbox(sno);
		byte[] val = new byte[1];
		val[0] = sbox.get(row)[col];
		BitSet fourbit = BitSet.valueOf(val);
		
		return fourbit;
	}
	
	/*
	 * Selection routine of DES.
	 */
	static BitSet selection(BitSet RE48bit) {
		BitSet R32bit = new BitSet(32);
		for (int i = 0; i < 8; i++) {
			BitSet fourbit = sboxProcess(RE48bit.get(i*4, i*4 + 4), i+1);
			for (int j = 0; j < 4; j++) {
				if (fourbit.get(j)) {
					R32bit.set(i*4 + j);
				}
			}
		}
		return R32bit;
	}
	
	/*
	 * Initial permutation table.
	 */
	static int[] PTable() {
		int[] ptable = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,
				32,27,3,9,19,13,30,6,22,11,4,25};
		
		return ptable;
	}
	
	/*
	 * Final permutation table.
	 */
	static int[] FPTable() {
		int[] fptable = {40,     8,   48,    16,    56,   24,    64,   32,
				39,     7,   47,    15,    55,   23,    63,  31,
	            38,     6,   46,    14,    54,   22,    62,   30,
	            37,     5,   45,    13,    53,   21,    61,   29,
	            36,     4,   44,    12,    52,   20,    60,   28,
	            35,     3,   43,    11,    51,   19,    59,   27,
	            34,     2,   42,    10,    50,   18,    58,   26,
	            33,     1,   41,     9,    49,   17,    57,   25};
		
		return fptable;
	}
	
	/*
	 * The F function of DES.
	 */
	static BitSet func(BitSet R, BitSet key) {
		BitSet RE = expandRto48bits(R);

		RE.xor(key);
		
		BitSet R32bit = selection(RE);
		
		BitSet final32bit = new BitSet(32);
		int[] ptable = PTable();
		
		for (int i = 0; i < ptable.length; i++) {
			if (R32bit.get(ptable[i]-1))
				final32bit.set(i);
		}
		return final32bit;
	}
}
