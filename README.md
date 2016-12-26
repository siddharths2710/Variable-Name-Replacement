# VARIABLE NAME REPLACEMENT IN C

## SYNOPSIS

Variables in a program are usually named based on the user’s preference or based
on their use , which is troublesome for other teams working on the project as
language or words may vary with programmers. The solution to this problem is our
program, which inputs a file and it lets the user replace each variable
declaration with name of their own choice. The change of name will help them
understand the program better and it also saves time . All he has to do is change
the names of the variables of the concerning program to another name that he so
desires.

##WORKING

* Input : File containing the code is opened in read mode.

* Create Symbol Table : For generating name and position links

* Acquiring new replacement names

* Check occurrences and addresses of the variables

* Check if the user has used a legal identifier name

* Replace the old name with the new name

## DETAILS
The symbol table that our program generates uses ​
**structures** and has the following three useful attributes:-

* Variable Name

* Primitive data type of variable

* Addresses of occurrences

Apart from this, various other cases were accounted for while generating an ideal
symbol table. For instance, the scope of the various variables. For dealing with the
three types of scopes of a typical C program ( local scope, global scope and function
scope) we have made use of a ​**stack** which implements the functioning of
parenthesis matching. Another problem to deal with was if any variable present in
the program was a substring of another variable (or even a function). In this case,
the output would consist of new variable names that the user has not even
prompted for. To deal with this, checks were made in the program to eliminate
such problems.
After this comes the prompt for replacing the old names with new ones. We first
check whether the names received from the user follow the rules of naming
identifiers of a typical C/C++ language. Once we obtain the new names from the
user which follows the required conventions we apply a sequence of various
**brute-force** checks and replacement techniques on the input file and generate a
new output file with the required names of the variables that is needed by the
user.

##Example

###### Testing Code for the main program ######
```

#include<stdio.h>
void foo(int,int);
int fact(int);
int power(int,int);
int main()
{
	int temp1=5,temp2=10;
	foo(temp2,temp1);
	foo(fact(temp2),pow(temp2,temp1));
}
void foo(int x,int y)
{
	printf("%d %d\n",x,y);
}
int fact(int n)
{
	if(n <=1)
		return 1;
	return n*fact(n-1);
}
int power(int m,int n)
{
	if(n==1)
	return m;
	return m*power(m,n-1);
}

```
###### Representation of the Symbol Table for the above code. ######

![alt tag](/SCREENS/2.png)

###### Representation of the Code after replacement ######

```

#include<stdio.h>
void foo(int,int);
int fact(int);
int power(int,int);
int main()
{
	int a=5,b=10;
	foo(b,a);
	foo(fact(b),pow(b,a));
}
void foo(int c,int d)
{
	printf("%d %d\n",c,d);
}
int fact(int e)
{
	if(e <=1)
		return 1;
	return e*fact(e-1);
}
int power(int f,int g)
{
	if(e==1)
	return f;
	return f*power(f,g-1);
}

```
