import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

public class Chat {
	static String host;
	static int port;
	static Socket s;
	static String username;
	
	static String privateKeyAlice;
	static String privateKeyBob;
	static String publicKeyAlice;
	static String publicKeyBob;
	static String aliceModulus;
	static String bobModulus;

	static String DESkey;
	
	public static void main(String[] args) throws IOException {

		if (args.length == 0 || args[0].equals("-h")) {
			callUsage(0);
		}
		
		@SuppressWarnings("resource")
		Scanner keyboard = new Scanner(System.in);
//		Process command line arguments
		pcl(args);
						
//		set up server, or join server
		setupServer();

//		Set up username
		System.out.println("User " + username + " logged in...");
		System.out.println("Welcome to (soon to be) encrypted chat program.\nChat starting below:");

		if (username.equals("alice")) {
//			System.out.println("I am Alice...");
//			System.out.println("Bob publice key: " + publicKeyBob);
//			System.out.println("Bob modulus: " + bobModulus);
//			System.out.println("Alice private key: " + privateKeyAlice);
//			System.out.println("Alice modulus: " + aliceModulus);
			
			aliceHandshake();
		} else if (username.equals("bob")) {
//			System.out.println("I am Bob...");
			privateKeyBob = publicKeyBob;
			publicKeyAlice = privateKeyAlice;
//			System.out.println("Bob priavate key: " + privateKeyBob);
//			System.out.println("Bob modulus: " + bobModulus);
//			System.out.println("Alice public key: " + publicKeyAlice);
//			System.out.println("Alice modulus: " + aliceModulus);
		
			String handshake_ret = bobHandshake();
			if (!handshake_ret.equals("OK")) {
				System.err.println("Hand shake failed!");
				System.exit(-1);
			}
		} else {
			System.err.println("Wrong username!");
			System.exit(-1);
		}
		
//		Make thread to print out incoming messages...
		ChatListenter chatListener = new ChatListenter();
		chatListener.start();

//		loop through sending and receiving messages
		PrintStream output = null;
		try {
			output = new PrintStream(s.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		} 
		String input = "";
		while(true){
			
			input = keyboard.nextLine();
			input = username + ": " + input;

			output.println(DES.encryptLine(new StringBuilder(DESkey), input));
			output.flush();	
		}
	}


	private static void aliceHandshake() {
		// 1. wait for Bob to send her DES key encrypted with her public key
		BufferedReader input = null;
		try {
			input = new BufferedReader(new InputStreamReader(s.getInputStream()));
		} catch (IOException e1) {
			e1.printStackTrace();
			System.err.println("System would not make buffer reader");
			System.exit(1);
		}
		String DESkey_encrypted;
		
		try {
			// Read lines off the scanner
			DESkey_encrypted = input.readLine();
//			System.out.println(DESkey_encrypted);

			if(DESkey_encrypted == null) {
				System.err.println("The other user has disconnected, closing program...");
				System.exit(1);
			}
			
			// 2. Decrypt the received DES key with Alice' private key
			DESkey = RSA.RSAdecrypt(new StringBuilder(DESkey_encrypted), 
					new StringBuilder(aliceModulus), new StringBuilder(privateKeyAlice));
//			System.out.println("Decrypted key: " + DESkey);
			
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
		
		// 3. Send encrypted "OK" message to Bob
		String encryptedOKmessage = RSA.RSAencrypt(new StringBuilder("OK"), 
				new StringBuilder(bobModulus), new StringBuilder(publicKeyBob));
//		System.out.println("Encrypted OK message: " + encryptedOKmessage);
		
		PrintStream output = null;
		try {
			output = new PrintStream(s.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		} 

		output.println(encryptedOKmessage);
		output.flush();	
	}
	
	
	private static String bobHandshake() {
		// 1. Generate a DES key
		DESkey = DES.genDESkey();
//		System.out.println("DES key: " + DESkey);
		
		// 2. Encrypt the key with Alice public key
		String encryptedDESkey = RSA.RSAencrypt(new StringBuilder(DESkey), 
				new StringBuilder(aliceModulus), new StringBuilder(publicKeyAlice));
//		System.out.println("Encrypted DES key: " + encryptedDESkey);
		
		// 3. Send the encrypted key to Alice
		PrintStream output = null;
		try {
			output = new PrintStream(s.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		} 

		output.println(encryptedDESkey);
		output.flush();	
		
		// 4. Wait for Alice' "OK" message
		BufferedReader input = null;
		try {
			input = new BufferedReader(new InputStreamReader(s.getInputStream()));
		} catch (IOException e1) {
			e1.printStackTrace();
			System.err.println("System would not make buffer reader");
			System.exit(1);
		}
		
		String OKmessage_encrypted;
		try {
			// Read lines off the scanner
			OKmessage_encrypted = input.readLine();
//			System.out.println(OKmessage_encrypted);

			if(OKmessage_encrypted == null) {
				System.err.println("The other user has disconnected, closing program...");
				System.exit(1);
			}
			
			// 2. Decrypt the received OK message with Bob' private key
			String OKmessage_decrypted = RSA.RSAdecrypt(new StringBuilder(OKmessage_encrypted), 
					new StringBuilder(bobModulus), new StringBuilder(privateKeyBob));
			System.out.println("Decrypted OK message: " + OKmessage_decrypted);
			return OKmessage_decrypted;
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
		return null;
	}
	
	/**
	 * Upon running this function it first tries to make a connection on 
	 * the given ip:port pairing. If it find another client, it will accept
	 * and leave function. 
	 * If there is no client found then it becomes the listener and waits for
	 * a new client to join on that ip:port pairing. 
	*/
	private static void setupServer() {
		try {
			// This line will catch if there isn't a waiting port
			s = new Socket(host, port);
			
		} catch (IOException e1) {
			System.out.println("There is no other client on this IP:port pairing, waiting for them to join.");
			
			try {
				ServerSocket listener = new ServerSocket(port);
				s = listener.accept();
				listener.close();
				
			} catch (IOException e) {
				e.printStackTrace();
				System.exit(1);
			}

		}
		System.out.println("Client Connected.");

	}

	/**
	 * This function Processes the Command Line Arguments.
	 * Right now the three accepted Arguments are:
	 * -p for the port number you are using
	 * -i for the IP address/host name of system
	 * -h for calling the usage statement.
	 */
	private static void pcl(String[] args) {
		/*
		 * http://www.urbanophile.com/arenn/hacking/getopt/gnu.getopt.Getopt.html
		*/
		LongOpt[] longopts = new LongOpt[2];
		longopts[0] = new LongOpt("alice", LongOpt.NO_ARGUMENT, null, 1);
		longopts[1] = new LongOpt("bob", LongOpt.NO_ARGUMENT, null, 2);
		Getopt g = new Getopt("Chat Program", args, "p:i:a:b:m:n:", longopts);
		int c;
		String arg;
		while ((c = g.getopt()) != -1){
		     switch(c){
		     	  case 1:
		     		  username = "alice";
		     		  break;
		     	  case 2:
		     		  username = "bob";
		     		  break;
		          case 'p':
		        	  arg = g.getOptarg();
		        	  port = Integer.parseInt(arg);
		        	  break;
		          case 'i':
		        	  arg = g.getOptarg();
		        	  host = arg;
		        	  break;
		          case 'a':
		        	  arg = g.getOptarg();
		        	  privateKeyAlice = arg;
		        	  break;
		          case 'm':
		        	  arg = g.getOptarg();
		        	  aliceModulus = arg;
		        	  break;
		          case 'b':
		        	  arg = g.getOptarg();
		        	  publicKeyBob = arg;
		        	  break;
		          case 'n':
		        	  arg = g.getOptarg();
		        	  bobModulus = arg;
		        	  break;
		          case 'h':
		        	  callUsage(0);
		          case '?':
		            break; // getopt() already printed an error
		            //
		          default:
		              break;
		       }
		   }
	}

	/**
	 * A helper function that prints out the useage help statement
	 * and exits with the given exitStatus
	 * @param exitStatus
	 */
	private static void callUsage(int exitStatus) {
		
		String useage = "Usage: java Chat <options>\n" +
				"where possible options include:\n" +
				"  -h\tList out all the command line options\n" + 
				"  --alice -a <private key alice> -m <alice modulus> -b <public key bob>" +
				" -n <bob modulus> -p <port> -i <ip address>\t" + 
				"Alice runs\n" +
				"  --bob -b <private key bob> -n <bob modulus> -a <public key alice>" + 
				" -m <alice modulus> -p <port> -i <ip address>\t" + 
				"Bob runs\n";
		
		System.err.println(useage);
		System.exit(exitStatus);
		
	}

	/**
	 * A private class which runs as a thread listening to the other 
	 * client. It prints out the message on screen.
	 */
	static private class ChatListenter implements Runnable {
		private Thread t;
		ChatListenter(){
		}
		
		@Override
		public void run() {
			System.out.println("Hand shake succeed!");

			BufferedReader input = null;
			try {
				input = new BufferedReader(new InputStreamReader(s.getInputStream()));
			} catch (IOException e1) {
				e1.printStackTrace();
				System.err.println("System would not make buffer reader");
				System.exit(1);
			}
			String inputStr;
			while(true){
				try {
//					Read lines off the scanner
					inputStr = input.readLine();
					
					if(inputStr == null){
						System.err.println("The other user has disconnected, closing program...");
						System.exit(1);
					}
					System.out.println(DES.decryptLine(new StringBuilder(DESkey), inputStr));
					
				} catch (IOException e) {
					e.printStackTrace();
					System.exit(1);
				}
			}
		}
		   
		public void start(){
			if (t == null){
				t = new Thread(this);
				t.start();
			}
		}
	}
}
