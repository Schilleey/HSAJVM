//--Stack---------------------------------------------------

template<class T>
class List
{
public:
	T elem;
	List<T>* next;
		
	List(T newelem, List<T>* newnext)
	{
		elem = newelem;
		next = newnext;
	};
};

template<class T>
class Stack
{
private:
	List<T>* top;
	List<T>* firstelem;
	
public:
	Stack()
	{
		firstelem = new List<T>(0, top);
		top = firstelem;
	}
	
	virtual ~Stack()
	{
		if(firstelem)
			delete firstelem;
			
		if(top)
			delete top;
	}
	
	void push(T elem)
	{
		top = new List<T>(elem, top);
	}
	
	T pop()
	{
		List<T>* popelem = top;
		top = top->next;
		return popelem->elem;
	}
	
	bool isempty()
	{
		return top == firstelem;
	}
};
