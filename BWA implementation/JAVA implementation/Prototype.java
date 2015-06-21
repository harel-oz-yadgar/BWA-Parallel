import java.io.BufferedReader;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.*;
import java.util.concurrent.locks.ReentrantReadWriteLock.ReadLock;

public class Prototype {

	public static void main(String[] args) {
		int i,tmp;
		String str;
		Boolean change;
		Scanner scanner = new Scanner(System.in);
		BufferedReader dnaFile,readsFile;
		String X;

		try{
			dnaFile = new BufferedReader(new FileReader("C:\\Users\\Oz\\Desktop\\dna.txt"));
			readsFile = new BufferedReader(new FileReader("C:\\Users\\Oz\\Desktop\\reads.txt"));

			X=dnaFile.readLine();
		}catch(Exception e){
			System.err.println("Error cannot open file");
			return;
		}

		//---------------------------read X---------------------------
		//String X="CCATTCAGTTATAGATTGGTCCAGATCCGGTATAGACTACAGATCTCAGAGTCCCCGTTCATCCCACGGTTGTGGTATTTGACCGTTATACCGAATTATTCAGTTTGGGTCAGAGGTCTATACACCAACCGATACGCGCTGTCGAGAAGGGAGGACTAGAATTTTAGTCTCTCTAGCGCTCGTATAGGTTGGGATAGCTGGTGGGTCCGGCCGATCCCAGATCGTGCTTAATTGCACAGCAGCCCCATCGGTACGCTCAGAACCCCTTCGTCTGGTACAGCAACATTAGGATGAAGTATTGTTGGACCCGGGAAAACTACATCTCCCGGAAATGTACGCTCCCTAGGGCCGATTATAGTGGTGTACTGGTTCGTTGGGGTTCATCATGATTCTTAATCCCGCCTTAGCCATCCGTAATCCGGCTTAGCCTTCTATTTAGGATTGGTAGAGATCGCTTGGTTGGGGCCGCGCATCTTACAGAGCCGCCGCGGGGTTCCACTCCG";

		X+="$";

		//---------------------------BWT---------------------------
		String[] SA=suffixArray(X);

		int[] S=new int[X.length()];
		for(i=0;i<X.length();i++)
			S[i]=i;	

		/*System.out.println("Suffix array:");
		System.out.println("i\t| suffix");
		System.out.println("--------------------------");
		for(i=0;i<X.length();i++)
			System.out.println(i+"\t| "+SA[i]);
		System.out.println("\n\n");*/

		do{	//sort suffix array
			change=false;
			for(i=1;i<X.length();i++)
				if(SA[i].compareTo(SA[i-1])<0){
					str=SA[i];
					SA[i]=SA[i-1];
					SA[i-1]=str;
					tmp=S[i];
					S[i]=S[i-1];
					S[i-1]=tmp;
					change=true;
				}
		}while(change);

		/*System.out.println("Sorted suffix array:");
		System.out.println("i\t| S[i]\t| suffix");
		System.out.println("----------------------------------");
		for(i=0;i<X.length();i++)
			System.out.println(i+"\t| "+S[i]+"\t| "+SA[i]);
		System.out.println("\n\n");*/

		String B="";
		for(i=0;i<X.length();i++)
			B+=SA[i].charAt(X.length()-1);

		System.out.println("BWT:");
		System.out.println("B= "+B);
		System.out.print("S= (");
		for(i=0;i<X.length()-1;i++)
			System.out.print(S[i]+",");
		System.out.println(S[i]+")\n\n");

		//---------------------------B'---------------------------
		String Xt="";
		for(i=X.length()-1;i>=0;i--)
			Xt+=X.charAt(i);

		String[] SAt=new String[Xt.length()];
		for(i=0;i<Xt.length();i++)
			SAt[i]=Xt.substring(i)+Xt.substring(0,i);	

		do{	//sort suffix array
			change=false;
			for(i=1;i<Xt.length();i++)
				if(SAt[i].compareTo(SAt[i-1])<0){
					str=SAt[i];
					SAt[i]=SAt[i-1];
					SAt[i-1]=str;
					change=true;
				}
		}while(change);

		String Bt="";
		for(i=0;i<Xt.length();i++)
			Bt+=SAt[i].charAt(Xt.length()-1);

		PrintWriter writer;
		Group I;
		String W;
		int[] section;
		try {
			writer=new PrintWriter("C:\\Users\\Oz\\Desktop\\hits.txt", "UTF-8");
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}

		int reads=0;
		try{
			while( (W=readsFile.readLine()) !=null){
				I=InexactSearch(W,5,X,B,Bt);
				//System.out.println(I);
				writer.println(I.hits());
				for(i=0;i<I.length();i++){
					section=I.getSection(i);
					for(int j=section[0];j<=section[1];j++)
						writer.println(S[j]);//+" "+X.substring(S[j],S[j]+20));
				}
				//writer.println();
				reads++;
				System.out.println(reads+" reads has been processed");
			}

		}
		catch(Exception e){
		}

		writer.close();		
		System.out.println("finished");
	}



	private static String[] suffixArray(String X){
		String[] SA=new String[X.length()];	//suffix array
		for(int i=0;i<X.length();i++)
			SA[i]=X.substring(i)+X.substring(0,i);
		return SA;
	}

	private static int C(char a,String X){
		int num=0;
		for(int i=0;i<X.length()-1;i++)
			if(X.charAt(i)<a)
				num++;
		return num;
	}

	private static int O(char a,int i,String B){
		int occ=0;
		for(int j=0;j<=i;j++)
			if(a==B.charAt(j))
				occ++;
		return occ;		
	}

	private static int[] CalculateD(String W,String X,String Bt){
		int[] D=new int[W.length()];
		int k=1;
		int l=X.length()-1;
		int z=0;
		for(int i=0;i<W.length();i++){
			k=C(W.charAt(i),X)+O(W.charAt(i),k-1,Bt)+1;
			l=C(W.charAt(i),X)+O(W.charAt(i),l,Bt);
			if(k>l){
				k=1;
				l=X.length()-1;
				z++;
			}
			D[i]=z;
		}
		return D;
	}

	private static Group InexactSearch(String W,int z,String X,String B,String Bt){
		int[] D=CalculateD(W,X,Bt);
		return InexRecur(W,W.length()-1,z,0,X.length()-1,X,B,D);
		// should be InexRecur(W,W.length()-1,z,1,X.length()-1,X,B,D); but there is a problem
	}

	private static Group InexRecur(String W,int i,int z,int k,int l,String X,String B,int[] D){
		char[] letters={'A','C','G','T'};
		int tmp_k,tmp_l;

		if(z<0)
			return new Group();
		if(i<0){
			Group I=new Group();
			I.add(k,l);
			return I;
		}

		Group I=new Group();
		I.union(InexRecur(W,i-1,z-1,k,l,X,B,D));	//deletion
		for(char b:letters){
			tmp_k=C(b,X)+O(b,k-1,B)+1;
			tmp_l=C(b,X)+O(b,l,B);
			if(tmp_k<=tmp_l){
				I.union(InexRecur(W,i,z-1,tmp_k,tmp_l,X,B,D));	//insertion
				if(b==W.charAt(i))	//match
					I.union(InexRecur(W,i-1,z,tmp_k,tmp_l,X,B,D));
				else	//mismatch
					I.union(InexRecur(W,i-1,z-1,tmp_k,tmp_l,X,B,D));
			}
		}
		return I;
	}


}
