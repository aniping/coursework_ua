/**
 * CS466/566 Assignment 5: Part B - Public-Key Cryptography
 * 
 * Team members: Praharsh Srinivasula, Shuo Yang
 * 
 * Description: 
 * The program implements the RSA algorithm to generate key pairs, 
 * encrypt a number, and decrypt a number.
 */

import gnu.getopt.Getopt;
import java.math.BigInteger;

public class RSA {

	public static void main(String[] args){
		
		if (args.length == 0) {
			System.err.println("Please use -h option to see all available options.");
		}
		
		StringBuilder bitSizeStr = new StringBuilder();
		StringBuilder nStr = new StringBuilder();
		StringBuilder dStr = new StringBuilder();
		StringBuilder eStr = new StringBuilder();
		StringBuilder mStr = new StringBuilder();
		StringBuilder keyNeeded = new StringBuilder();
		RSAKey rsakey;
		
		// process command line options
		pcl(args, bitSizeStr, nStr, dStr, eStr,mStr, keyNeeded);
	
		/* Generate public/private key pair */
		if (keyNeeded.toString().equals("true")) {
			/* RSA key creation */
			if(!bitSizeStr.toString().equalsIgnoreCase("")){
				rsakey = new RSAKey(Integer.parseInt(bitSizeStr.toString()));
				rsakey.printKeyInHex();
			} else {
				rsakey = new RSAKey(1024);
				rsakey.printKeyInHex();
			}
		}
		
		/* Encryption */
		if(!eStr.toString().equalsIgnoreCase("")){
			RSAencrypt(mStr, nStr, eStr);
		}
		
		/* Decryption */
		if(!dStr.toString().equalsIgnoreCase("")){
			RSAdecrypt(mStr, nStr, dStr);
		}	
	}
	
	/**
	 * Encrypt mStr with public key <eStr, nStr>
	 * 
	 * @param mStr String representation of the message
	 * @param nStr String representation of value n in RSA
	 * @param eStr String representation of value e in RSA
	 */
	private static void RSAencrypt(StringBuilder mStr, StringBuilder nStr, StringBuilder eStr) {
		BigInteger msg = new BigInteger(mStr.toString().getBytes());
		
		byte[] e_bytes = RSA.hexStr2byteArray(eStr.toString());		
		byte[] n_bytes = RSA.hexStr2byteArray(nStr.toString());
		
		BigInteger n = new BigInteger(n_bytes);
		BigInteger e = new BigInteger(e_bytes);
		
		BigInteger encrypted_msg = msg.modPow(e, n);
		
		System.out.print("encrypted message:");
		System.out.println(RSA.bigInteger2HexStr(encrypted_msg));
	}

	/**
	 * Decrypt ciphertext cStr with private key <dStr, nStr>
	 * 
	 * @param cStr String representation of the ciphertext
	 * @param nStr String representation of value n in RSA
	 * @param dStr String representation of value d in RSA
	 */
	private static void RSAdecrypt(StringBuilder cStr, StringBuilder nStr, StringBuilder dStr){
		byte[] c_bytes = RSA.hexStr2byteArray(cStr.toString());		
		byte[] n_bytes = RSA.hexStr2byteArray(nStr.toString());
		byte[] d_bytes = RSA.hexStr2byteArray(dStr.toString());
		
		BigInteger encrypted_msg = new BigInteger(c_bytes);
		BigInteger n = new BigInteger(n_bytes);
		BigInteger d = new BigInteger(d_bytes);
		
		
		BigInteger decrypted_msg = encrypted_msg.modPow(d, n);
		System.out.print("derypted message:");
		System.out.println(new String(decrypted_msg.toByteArray()));
	}
	
	static void printHexStr(byte[] key) {
		StringBuilder key_str = new StringBuilder();
		for (int i = 0; i < key.length; i++) {
			key_str.append(String.format("%02x", key[i]));
		}
		System.out.println("0x" + key_str.toString());
	}
	
	public static byte[] hexStr2byteArray(String hexstr) {
		byte[] hexvals = new byte[hexstr.length()/2];
		
		for (int i = 0; i < hexstr.length()/2; i++) {
			int lval = Character.getNumericValue(hexstr.charAt(i*2));
			int rval = Character.getNumericValue(hexstr.charAt(i*2 + 1));
			hexvals[i] = (byte) ((lval << 4) + rval);
		}
		
		return hexvals;
	}
	
	public static String bigInteger2HexStr(BigInteger bigint) {
		StringBuilder bigint_str = new StringBuilder();
		byte[] bytes = bigint.toByteArray();
		for (int i = 0; i < bytes.length; i++) {
			bigint_str.append(String.format("%02x", bytes[i]));
		}
		return bigint_str.toString();
	}
	
	/**
	 * This function Processes the Command Line Arguments.
	 */
	private static void pcl(String[] args, StringBuilder bitSizeStr,
							StringBuilder nStr, StringBuilder dStr, StringBuilder eStr,
							StringBuilder m, StringBuilder keyNeeded) {
		/*
		 * http://www.urbanophile.com/arenn/hacking/getopt/gnu.getopt.Getopt.html
		 */	
		Getopt g = new Getopt("RSA Program", args, "hke:d:b:n:i:");
		int c;
		String arg;
		while ((c = g.getopt()) != -1){
		     switch(c){
		     	  case 'i':
		        	  arg = g.getOptarg();
		        	  m.append(arg);
		        	  break;
		          case 'e':
		        	  arg = g.getOptarg();
		        	  eStr.append(arg);
		        	  break;
		     	  case 'n':
		        	  arg = g.getOptarg();
		        	  nStr.append(arg);
		        	  break;
		     	  case 'd':
		        	  arg = g.getOptarg();
		        	  dStr.append(arg);
		        	  break;
		          case 'k':
		        	  keyNeeded.append("true");
		        	  break;
		     	  case 'b':
		        	  arg = g.getOptarg();
		        	  bitSizeStr.append(arg);
		        	  break;
		          case 'h':
		        	  callUsage(0);
		          case '?':
		            break; // getopt() already printed an error
		          default:
		        	  System.err.println("Wrong option!");
		              break;
		       }
		   }
	}
	
	private static void callUsage(int exitStatus) {

		String useage = "Usage: java RSA <options>\n" +
				"where possible options include:\n" +
				"  -h\t\t\t\t\t\t\t\tList out all the command line options\n" + 
				"  -k\t\t\t\t\t\t\t\tGenerate a RSA key, encoded in hex,"
				+ " printed on the command line\n" +
				"  -e <public_key> -n <modulus> -i <plaintext_value>\t\t" + 
				"Encrypt the integer <plaintext_value> using <public_key>\n"
				+ "\t\t\t\t\t\t\t\tand print the result (in hex) to stdout\n"
				+ "  -d <private_key> -n <modulus> -i <ciphertext value>\t\t"
				+ "Decrypt the <ciphertext_value> using <private_key>\n"
				+ "\t\t\t\t\t\t\t\tand print the result (in hex) to stdout\n";
		
		System.err.println(useage);
		System.exit(exitStatus);
		
	}
}
