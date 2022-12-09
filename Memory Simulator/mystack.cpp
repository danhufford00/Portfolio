#include "mystack.h"

/********************************************
Function: mystack()

Use: Creates a mystack object with default
     values

Arguments: None

Returns: None

Notes: None
********************************************/

mystack::mystack(){
	stack_array = nullptr;//Does not point anything
	stackCapacity = 0;
	stackSize = 0;
}

/********************************************
Function: mystack(const mystack& obj)

Use: Creates a mystack object with the same
     values as another mystack object

Arguments: const mystack& obj - a reference
           to a constant mystack object (the
	   object whose values get copied)

Returns: None

Notes: None
********************************************/

mystack::mystack(const mystack& obj){

	stackSize = obj.stackSize;//We must specify which stackSize is which, thus we state it is the variable belong to the object obj
	stackCapacity = obj.stackCapacity;

	if (stackCapacity > 0)//If there are elements to point to
		stack_array = new char[stackCapacity];
	else//If the stackCapacity is 0, or there are no elements to point to
		stack_array = nullptr;

	for (size_t x = 0; x < stackSize; x++)
		stack_array[x] = obj.stack_array[x];
}

/********************************************
Function: ~mystack()

Use: Resets the value of the stack_array
     pointer, so that there is no array to
     be used (because none is pointed to)

Arguments: None

Returns: None

Notes: None
********************************************/

mystack::~mystack(){
	delete[] stack_array;//the pointer variable named stack_array does not point to anything
}

/********************************************
Function: mystack& operator=(const mystack& obj)

Use: Sets the value of one mystack object equal
     to the value of another

Arguments: const mystack& obj - a reference to
	   a constant mystack object (the values
	   of the object calling the function are
	   set to the values of this object)

Returns: A reference to a mystack object,
	 specifically the one that called the
	 function

Notes: None
********************************************/

mystack& mystack::operator=(const mystack& obj){
	if (this != &obj){//If the object is set as equal to itself (the memory addresses are the same)

		delete[] stack_array;//The pointer variable no longer points to the previous array

		stackCapacity = obj.stackCapacity;//The new stackCapacity and stackSize are the values of right operand
		stackSize = obj.stackSize;

		if (stackCapacity > 0)//If there is something to point to
			stack_array = new char[stackCapacity];//Redeclaring the length of the array, and having the pointer point to that
		else
			stack_array = nullptr;

		for (size_t x = 0; x < stackSize; x++)
			stack_array[x] = obj.stack_array[x];//The values of the array belonging to the object obj are passed to the array of the calling object (in the same locations)
	}
	return *this;//Returning the address of the calling object
}

/********************************************
Function: size_t capacity() const

Use: Gives the stackCapacity of the mystack
     object in question

Arguments: None

Returns: A size_t value representing the
	 capacity of the stack

Notes: None
********************************************/

size_t mystack::capacity() const{
	return stackCapacity;
}

/********************************************
Function: size_t size() const

Use: Gives the stackSize of the mystack
     object in question

Arguments: None

Returns: A size_t variable representing the
	 size of the stack

Notes: None
********************************************/

size_t mystack::size() const{
	return stackSize;
}

/********************************************
Function: bool empty() const

Use: Checks if the stack is empty

Arguments: None

Returns: A boolean value representing if the
	 stack is empty (true) or not (false)

Notes: None
********************************************/

bool mystack::empty() const{
	if (stackSize == 0)//If the stack's size is 0, the stack is empty
		return true;
	else
		return false;
}

/********************************************
Function: void clear()

Use: Clears the values of the stack

Arguments: None

Returns: None

Notes: None
********************************************/

void mystack::clear(){
	stackSize = 0;//when stackSize is 0, the stack will never show values
}

/********************************************
Function: void reserve(size_t n)

Use: Increases the stackCapacity of a stack

Arguments: size_t n - a size_t variable
		      representing the new
		      stack capcity

Returns: None

Notes: None
********************************************/

void mystack::reserve(size_t n){
	if (n < stackSize || n == stackCapacity)//If you try to make the capcity smaller than the size or not change the value of the stack capcity, it returns
		return;
	stackCapacity = n;//Setting the new value of stackCapacity
	char* temp = new char[stackCapacity];//Declaring a new pointer to a char, and pointing it towards a character array of size stackCapacity
	for (size_t x = 0; x < stackSize; x++)
		temp[x] = stack_array[x];//The for loop moves the values from the current stack to the new stack
	delete[] stack_array;//the variable stack_array no longer points to the original stack array
	stack_array = temp;//The variable stack_array now points to the newly created character array
}

/********************************************
Function: char top() const

Use: Returns the top of the stack array

Arguments: None

Returns: A character - the top of the stack
	 array

Notes: None
********************************************/

const char& mystack::top() const{
	return stack_array[stackSize - 1];//Because a stack is LIFO (Last In First Out), the top of the array is the last element
}

/********************************************
Function: void push(char v)

Use: Appends a new character value to the top
     of the stack array

Arguments: char v - The value to be pushed to
	   the top of the stack array

Returns: None

Notes: None
********************************************/

void mystack::push(char v){
	if (stackSize == stackCapacity){//If there is no room for a new value
		if (stackCapacity == 0)
			reserve(1);//Increasese the size of the array by 1
		else
			reserve(stackCapacity * 2);
	}
	stack_array[stackSize] = v;//v is pushed to the top of the stack
	stackSize++;//The stack size is incremented by one
}

/********************************************
Function: void pop()

Use: Removes the top value of the stack array

Arguments: None

Returns: None

Notes: None
********************************************/

void mystack::pop(){
	stackSize--;//By decrementing the size of the stack by 1, the previous top value will no longer be displayed
}
