#include <iostream>
#include <cctype>
#include <string>
#include "mystack.h"

using std::string;//As stated in inpost.h, because the convert function returns a string, the file needs to know what a string is

/*************************************
Function: int prec(char c)

Use: Gives the precedence of any given
     operator

Arguments: A character representing the
	   operator in question

Returns: An integer representing the
	 precedence of the character c

Notes: None
*************************************/

int prec(char c){
	if(c == '~' || c == '^')
		return 3;
	else if (c == '*' || c == '/')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return 0;
}

/*************************************
Function: string convert(const string& infix)

Use: Converts a string in infix notation
     to postfix notation

Arguments: const string& infix - a
	   reference to a constant
	   string (to be converted to
	   postfix)

Returns: A string giving the postfix
	 version of the infix expression

Notes: None
*************************************/

string convert(const string& infix){

	mystack opstack;
	opstack.push('$');//Sets the top of the stack to a $ so we know when the stack is empty
	string postfix = "";//The string to be returned
	size_t x = 0;//X will go byte by byte through the string, because a character is only 1 byte long

	while(x < infix.length()){//Goes through each character of the string infix

		if (islower(infix[x])){//If the character is a lowercase letter
			postfix += ' ';
			postfix += infix[x];//The infix character is added to the postfix string
			x++;//Because we are using a while loop, the size_t variable x must be incremented
		}
		else if (isdigit(infix[x])){//If the character is a number
			while (isdigit(infix[x])){//In case there are more numbers after (like a double or triple digit number) we use a while loop
				if (isdigit(infix[x - 1]) == false)//If the character before the current character is an integer, we don't want to add a space (because it's all one number)
					postfix += ' ';
				postfix += infix[x];
				x++;
			}
		}
		else if (isspace(infix[x]))//If the character is a space, we skip over it
			x++;
		else if(infix[x] == '('){//If the character is an open parenthesis
			opstack.push(infix[x]);//The open parentehsis is pushed to the top of the stack
			x++;
		}
		else if (infix[x] == ')'){//If the character is a close parenthesis
			while (opstack.top() != '$' && opstack.top() != '('){//While the opstack array is not empty and we are not yet done with the expression within the parenthesis
				postfix += ' ';
				postfix += opstack.top();
				opstack.pop();//Once the character is added to the top of the postfix string, we can remove it from the opstack
			}

			if (opstack.top() != '$')
				opstack.pop();//Remove the top value
			x++;
		}
		else{//If the value is an operator
			while (opstack.top() != '$' && prec(infix[x]) <= prec(opstack.top())){//While the stack is not empty and the precedence is correct
				postfix += ' ';
				postfix += opstack.top();
				opstack.pop();
			}
			opstack.push(infix[x]);//At the end, we push the value onto the opstack
			x++;
		}
	}

	while(opstack.top() != '$'){
		postfix += ' ';
		postfix += opstack.top();
		opstack.pop();
	}

	for (size_t x = 0; x < postfix.size(); x++)//everything is moved over one so there is no space at the start
		postfix[x] = postfix[x + 1];

	postfix.resize(postfix.size() - 1);

	return postfix;//Returning the postfix expression

}//function
