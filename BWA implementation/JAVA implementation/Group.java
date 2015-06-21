import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import javax.lang.model.element.Element;

public class Group {
	private ArrayList<int[]> elements = new ArrayList<int[]>();

	public void add(int k,int l){
		int[] section={k,l};
		int i;

		if(elements.size()==0){
			elements.add(section);
			return;
		}

		if(elements.get(0)[0]>k){	//new first k,l
			elements.add(0,section);
			rebalance(0);
			return;
		}
		
		//insert in sorted manner using k
		for(i=0;i<elements.size();i++){
			if(elements.get(i)[0]<k)	//didnt locate the spot yet
				continue;

			if(elements.get(i)[0]==k){	//locate the spot
				if(elements.get(i)[1]>=l)	//if k,l is already in the spot return
					return;
				//l[i]<l than take over the spot and rebalance from the spot 
				//g->elements[i][1]=l;
				section=elements.remove(i);
				section[1]=l;
				elements.add(i,section);
				rebalance(i);
				return;
			}

			if(elements.get(i)[0]>k){	//we passed the wanted spot
				elements.add(i,section);
				rebalance(i-1);
				return;	
			}
		}

		//if we got here k>all k[i]
		i=elements.size();
		elements.add(i,section);
		rebalance(i-1);	
	}


	void shiftRight(int index){//push all the elements starting from <index> to the right
		for(int i=elements.size();i>index;i--)
			elements.set(i, elements.get(i-1));
	}

	void shiftLeft(int index){//push all the elements starting from <index>+1 to the left
		elements.remove(index);
	}

	void rebalance(int index){
		int i;

		for(i=index;i<elements.size()-1;i++){
			if(elements.get(i)[1]==elements.get(i+1)[0]-1){	//k[i],l[i],l[i]+1=k[i+1],l[i+1] => k[i],l[i+1]
				elements.get(i)[1]=elements.get(i+1)[1];
				shiftLeft(i+1);
				rebalance(i);
				return;
			}
			if(elements.get(i)[1]<elements.get(i+1)[0]){	//k[i],l[i],k[i+1],l[i+1]
				rebalance(i+1);
				return;
			}

			if(elements.get(i)[1]==elements.get(i+1)[0]){ //k[i],l[i]=k[i+1],l[i+1] => k[i],l[i+1]
				elements.get(i)[1]=elements.get(i+1)[1];
				shiftLeft(i+1);
				rebalance(i);	
				return;
			}

			//its certain that k[i+1]<l[i]
			if(elements.get(i)[1]<=elements.get(i+1)[1]){	//k[i],k[i+1],l[i],l[i+1] or k[i],k[i+1],l[i]=l[i+1] => k[i],l[i+1]
				elements.get(i)[1]=elements.get(i+1)[1];
				shiftLeft(i+1);
				rebalance(i);
				return;
			}

			//k[i],k[i+1],l[i+1],l[i]

			shiftLeft(i+1);
			i--;	
		}
	}

	public void union(Group other){
		for(int i=0;i<other.elements.size();i++)
			this.add(other.elements.get(i)[0],other.elements.get(i)[1]);
	}

	public String toString(){
		String str="";
		for(int i=0;i<elements.size();i++)
			str+="["+elements.get(i)[0]+","+elements.get(i)[1]+"] ";
		return str;
	}

	public String printToC(){
		String str="hits: "+elements.size()+"\n";
		if(elements.size()>=1)
			str+=elements.get(0)[0]+","+elements.get(0)[1]+" ";
		for(int i=1;i<elements.size();i++)
			str+=elements.get(i)[0]+","+elements.get(i)[1]+" ";
		return str+"\n";
	}

	int length(){
		return elements.size();
	}
	
	int hits(){
		int hits=0;
		for(int i=0;i<elements.size();i++)
			hits+=elements.get(i)[1]-elements.get(i)[0]+1;
		return hits;
	}
	
	int[] getSection(int i){
		return elements.get(i);
	}
}
