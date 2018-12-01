import java.math.BigInteger;
import java.security.SecureRandom;

public class RSAKey {
	
	private BigInteger e, d, n;
	
	public RSAKey(int bitsize) {
		SecureRandom sr = new SecureRandom();

		/* Two large primes */
		BigInteger p, q;
		while (true) {
			p = BigInteger.probablePrime(bitsize/2-1, sr);
			q = BigInteger.probablePrime(bitsize/2+1, sr);
			if (p.isProbablePrime(8) && q.isProbablePrime(8)) {
				//System.out.println("Both p and q are primes");
				break;
			}
		}

		this.n = p.multiply(q); // n = p * q
		BigInteger phi_n = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
		
		//this.e = new BigInteger(8, 100, sr);
		this.e = new BigInteger("3");
	    while (phi_n.gcd(this.e).intValue() > 1) {
	      this.e = this.e.add(new BigInteger("2"));
	    }
	    //System.out.println("e = " + this.e);
		this.d = this.e.modInverse(phi_n);
	}
	
	public void printKeyInHex() {
		String e_str = RSA.bigInteger2HexStr(this.e);
		String d_str = RSA.bigInteger2HexStr(this.d);
		String n_str = RSA.bigInteger2HexStr(this.n);
		
		System.out.println("---------Begin RSA Public Key---------");
		System.out.println("e:"+e_str);
		System.out.println("n:"+n_str);
		System.out.println("---------End RSA Public Key---------\n");
		
		System.out.println("---------Begin RSA Private Key---------");
		System.out.println("d:"+d_str);
		System.out.println("n:"+n_str);
		System.out.println("---------End RSA Private Key---------");
	}
	
	public BigInteger getN() {
		return this.n;
	}
	
	public BigInteger getE() {
		return this.e;
	}
	
	public BigInteger getD() {
		return this.d;
	}
}
