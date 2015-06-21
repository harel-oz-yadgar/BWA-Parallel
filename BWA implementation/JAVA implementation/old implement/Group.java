import java.util.ArrayList;
import javax.lang.model.element.Element;

public class Group {
	private ArrayList<Integer> elements = new ArrayList<Integer>();

	public void add(int element){
		if(elements.indexOf(element)!=-1)
			return;
		elements.add(element);
		sort();
	}

	public void add(int from,int to){
		while(from<=to){
			if(elements.indexOf(from)==-1)
				elements.add(from);
			from++;
		}
		sort();
	}


	public void union(Group other){
		for(int i=0;i<other.elements.size();i++)
			if(elements.indexOf(other.elements.get(i))==-1)
				elements.add(other.elements.get(i));
		sort();
	}


	private void sort(){
		int min,location;
		ArrayList<Integer> tmp = new ArrayList<Integer>();
		while(elements.size()>0){
			min=elements.get(0);
			location=0;
			for(int i=1;i<elements.size();i++)
				if(elements.get(i)<min){
					min=elements.get(i);
					location=i;
				}
			elements.remove(location);
			tmp.add(min);
		}
		elements=tmp;	
	}

	public String toString(){
		String str="{";
		if(elements.size()>=1)
			str+=elements.get(0);
		for(int i=1;i<elements.size();i++){
			str+=", "+elements.get(i);
		}
		return str+"}";
	}

}
