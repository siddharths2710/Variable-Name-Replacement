#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<ctype.h>
#define BUFFER_SIZE 1024
#define LINE_SIZE 100




typedef struct Node 			///For address part of variable in the symbol table given below
{
	int addr;
	struct Node* next;		//Do this afterwards
} Node;
typedef struct List
{
	Node * first;
} List;


typedef struct stack 		//For parenthesis matching
{
    char stk[BUFFER_SIZE];
    int top;
} STACK;
STACK st;

typedef struct row_of_sym_table		//Each row gonna have a name,type and list of addresses
{
	char name[50]; 		//variable name
	char type[15];
	List address; 		//This I'll do afterwards,since it will be line numbers only
} row;



int len_symtab = 0;		//Length For Symbol Table (As global variable for easy coding)

row* gen_sym_table(char**,int);
char** str_split(char* a_str, const char a_delim);
int match(char *t,char *p);
void removeSubstring(char *s,const char *toremove);
void removeUnwanted(char *s);
int no_of_lines(char *);
char** alloc_array2d(int r, int c);
void free_array2d(char **a, int r);
void addEntry(List* L, int addr);
void list_display(List* L);
void list_write(FILE *f,List* L);
char *rep(char *s,  char *old,char *new1);
void stack_push(char);
char  stack_pop();
void stack_display();

int main(int argc, char *argv[])		//give './a.out filename.c' after compiling    
{
	char **lines;  			//Store lines of input file
	char buffer[ BUFFER_SIZE ];	//Entire file as input
    	char *last_token;		//Traverse through file
	char *filename = argv[1];	//Command line argument => Hence give ./a.out 'filename'
	char **replacement;
	char **res;
	int i,j;
	int k=0;
	int n = no_of_lines(filename);
	int var_addr;
	Node *main_addr;
	row* z;
		
	FILE *ip,*op,*Table;	
	ip = fopen( filename,"r");
	
	lines = alloc_array2d((n+1),LINE_SIZE);	
	res = alloc_array2d((n+1),LINE_SIZE);	
	
	while( fgets(buffer, BUFFER_SIZE, ip) != NULL )		
	{

            last_token = strtok( buffer,  "\n" ); 	//Split by new line
            while( last_token != NULL )
	    {
                strcpy(lines[k],last_token);
		k++;
		last_token = strtok( NULL,  "\n" );
            }						
	
        }
        //Due to above code 'lines' has all lines of the input file 
       	
       	fclose(ip);
        
	
	assert(n>=k);
	
	/*for(j=0;j<k;j++)
	{
		printf("%s\n",lines[j]);	//All lines of FILE stored here
	}*/
		
	for(j=0;j<k;j++)
	{
		strcpy(res[j],lines[j]);	
	}
	
	
	z = gen_sym_table(lines,k); //Update indices of Symbol Table & Return Symbol Table
	
	replacement = alloc_array2d(len_symtab,50);
	
	Table = fopen("symbol_table.md","w");
	
	if(Table == NULL)
	{
		printf("Error opening file\n");
		return 0;
	}
	
	
	/*for(i=0;i<len_symtab;i++)
	{
		printf("%s  %s ",z[i].name,z[i].type);  //Display row of symbol table
		list_display(&z[i].address);
		printf("\n");
	}*/
	
	
	
	printf("Enter replacement name for the following variable(s)\n<Dont take names with initial variable names for replacement>\n");
	for(i=0;i<len_symtab;i++)
	{
		printf("%s of type %s :-\n",z[i].name,z[i].type);
		scanf("%s",replacement[i]);
	}
	for(i=0;i<len_symtab;i++)
	{
		for(j=0;j<len_symtab;j++)
		{
			if(strcmp(replacement[i],z[j].name) == 0)
			{
				printf("Conflicting names present\n");
				return 0;
			}
			
		}
	}
	
	
	
	for(i=0;i<len_symtab;i++)	//Check if proper variable naming rules are followed
	{
		if(match(replacement[i],";") != -1||match(replacement[i],"<") != -1||match(replacement[i],">") != -1||match(replacement[i],"(") != -1||match(replacement[i],")") != -1||match(replacement[i]," ") != -1 || match(replacement[i],"	" )!= -1 || match(replacement[i],",") != -1 || !(isalpha(replacement[i][0]) || replacement[i][0] == '_'))
		{
			printf("Wrong naming syntax somewhere ... please check \n");
			return -1;
		}
	}
		
	
	fprintf(Table,"Name\tType\tReplacement\t    Addresses_of_occurence\n");
	for(i=0;i<len_symtab;i++)
	{
		fprintf(Table,"%s\t%s\t%s\t     ",z[i].name,z[i].type,replacement[i]);
		list_write(Table,&z[i].address);
		fprintf(Table,"\n");
	}
	
	fclose(Table);
	
	
	
	for(i=0;i<len_symtab;i++)
	{
		main_addr = z[i].address.first;
		while(main_addr!=NULL)
		{	var_addr = main_addr->addr;
			strcpy(res[var_addr],rep(res[var_addr],z[i].name,replacement[i]));
			main_addr = main_addr->next;
		}
	}

	/*for(j=0;j<k;j++)
	{
		printf("%s\n",res[j]);
	}*/
	
	op = fopen("final.md","w");
	for(j=0;j<k;j++)
	{
		fprintf(op,"%s\n",res[j]);
	}
	
	
	printf("\n\nDone...\ncheck newly generated files symbol_table.md & final.md\n");
	fclose(op);
	free_array2d(lines,(n+1));
	free_array2d(res,(n+1));  
	free_array2d(replacement,len_symtab);
	return 0;
}

row* gen_sym_table(char **l,int num)
{
	row* res = (row*)malloc((num)*sizeof(row));
	char **vars;
	char a;
	int i,j,k,x,y,z;
	char * temp;
	char * type = malloc(15);
	for(i=0;i<num;i++)
	{	
		while(st.top != -1)
		{
			a = stack_pop();
		}
		res[len_symtab].address.first=0;
		
		/*For each int,float,double,char
		store variable name and type in symbol table
		I did 'match(l[i],"(") == -1' so that function names aren't included in the symbol Table
		However because of above statement even function parameters are not there
		*/
		
		if(match(l[i],"int") != -1 && match(l[i],";") != -1 && match(l[i],"(") == -1) //Ex. int a=5,b,c; or simply int a;
		{
			vars = str_split(l[i],',');
			
			for (k = 0; *(vars + k); k++)
			{
			    strcpy(type,"int");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"int ");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			
		}
		
		if(match(l[i],"int") != -1 && match(l[i],";") == -1 && match(l[i],"(") != -1 && match(l[i+1],"{") != -1 )
		{
			temp = (char*)malloc(100);
			x=0;
			y=match(l[i],"(") + 1;
			while(l[i][y] != ')')
			{
				temp[x] = l[i][y];
				x++;y++;
			}
			temp[x] = '\0';
			
			//printf("%s\n",temp);
			vars = str_split(temp,',');
			
			for (k = 0; *(vars + k); k++)
			{
			    strcpy(type,"int");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    
			    removeSubstring(*(vars + k),"int ");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,"int");
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			free(temp);
		}
		
		if(match(l[i],"char") != -1 && match(l[i],";") != -1 && match(l[i],"(") == -1)  	//NOT else-if <For bad-indented input>
		{
			vars = str_split(l[i],',');
			
			for (k = 0; *(vars + k); k++)
			{
			
			   strcpy(type,"char");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"char ");
			    removeUnwanted(*(vars + k));
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    		
			    			break;
			    		
			    	}
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		if(x == 0 && !isalpha(x + strlen(*(vars + k))))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    len_symtab++;	
			    free(*(vars + k));
			}
			free(vars);
			
		}
		
		if(match(l[i],"char") != -1 && match(l[i],";") == -1 && match(l[i],"(") != -1 && match(l[i+1],"{") != -1 )
		{
			temp = (char*)malloc(100);
			x=0;
			y=match(l[i],"(") + 1;
			while(l[i][y] != ')')
			{
				temp[x] = l[i][y];
				x++;y++;
			}
			temp[x] = '\0';
			
			//printf("%s\n",temp);
			vars = str_split(temp,',');
			
			for (k = 0; *(vars + k); k++)
			{
			
			    strcpy(type,"char");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
				
			    removeSubstring(*(vars + k),"char");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			free(temp);
		}
		
		
		if(match(l[i],"float") != -1 && match(l[i],";") != -1 && match(l[i],"(") == -1)
		{
			vars = str_split(l[i],',');
			
			for (k = 0; *(vars + k); k++)
			{
			
			     strcpy(type,"float");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"float ");
			    removeUnwanted(*(vars + k));
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		if(x == 0 && !isalpha(x + strlen(*(vars + k))))
			    		addEntry(&res[len_symtab].address,j);
			    		if((x-1 >= 0) && !isalpha(l[j][x-1]))
			    		{
			    			y = strlen(*(vars + k));
			    			
			    			if((x+y  ) < strlen(l[i]))
			    			{
			    				if(!isalpha(l[j][x + y]))
			    					addEntry(&res[len_symtab].address,j);
			    			}
			    			else	
			    				addEntry(&res[len_symtab].address,j);
			    		}
			    		
			    	}
			    }
			    len_symtab++;	
			    free(*(vars + k));
			}
			free(vars);
		}
		
		
		if(match(l[i],"float") != -1 && match(l[i],";") == -1 && match(l[i],"(") != -1 && match(l[i+1],"{") != -1 )
		{
			temp = (char*)malloc(100);
			x=0;
			y=match(l[i],"(") + 1;
			while(l[i][y] != ')')
			{
				temp[x] = l[i][y];
				x++;y++;
			}
			temp[x] = '\0';
			
			//printf("%s\n",temp);
			vars = str_split(temp,',');
			
			for (k = 0; *(vars + k); k++)
			{
			     strcpy(type,"float");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"float");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			free(temp);
		}
		
		
		if(match(l[i],"double") != -1 && match(l[i],";") != -1 && match(l[i],"(") == -1) 
		{
			vars = str_split(l[i],',');
			
			for (k = 0; *(vars + k); k++)
			{
				 strcpy(type,"double");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"double ");
			    removeUnwanted(*(vars + k));
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		if(x == 0 && !isalpha(x + strlen(*(vars + k))))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    		
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;	
			}
			free(vars);
		}
		
		
		if(match(l[i],"double") != -1 && match(l[i],";") == -1 && match(l[i],"(") != -1 && match(l[i+1],"{") != -1 )
		{
			temp = (char*)malloc(100);
			x=0;
			y=match(l[i],"(") + 1;
			while(l[i][y] != ')')
			{
				temp[x] = l[i][y];
				x++;y++;
			}
			temp[x] = '\0';
			
			//printf("%s\n",temp);
			vars = str_split(temp,',');
			
			for (k = 0; *(vars + k); k++)
			{
				 strcpy(type,"double");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"double ");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			free(temp);
		}
		
		if(match(l[i],"bool") != -1 && match(l[i],";") != -1 && match(l[i],"(") == -1)
		{
			vars = str_split(l[i],',');
			
			for (k = 0; *(vars + k); k++)
			{
			 strcpy(type,"bool");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"bool ");
			    removeUnwanted(*(vars + k));
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    
			    	
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
					if(x == 0 && !isalpha(x + strlen(*(vars + k))))
			    		addEntry(&res[len_symtab].address,j);
				    		else
				    		{
				    		if((x-1 >= 0) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    		
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;	
			}
			free(vars);
		}
		
		if(match(l[i],"bool") != -1 && match(l[i],";") == -1 && match(l[i],"(") != -1 && match(l[i+1],"{") != -1 )
		{
			temp = (char*)malloc(100);
			x=0;
			y=match(l[i],"(") + 1;
			while(l[i][y] != ')')
			{
				temp[x] = l[i][y];
				x++;y++;
			}
			temp[x] = '\0';
			
			//printf("%s\n",temp);
			vars = str_split(temp,',');
			
			for (k = 0; *(vars + k); k++)
			{
				 strcpy(type,"bool");
			    if(match(*(vars + k),"*") != -1)
			    strcat(type," ptr");
			    removeSubstring(*(vars + k),"bool ");
			    removeUnwanted(*(vars + k));
			    
			    strcpy(res[len_symtab].name, *(vars + k));
			    strcpy(res[len_symtab].type,type);
			    addEntry(&res[len_symtab].address,i);
			    for(j = i+1;j<num;j++)
			    {
			    	if(match(l[j],"{") != -1)
			    	{
			    		stack_push('{');
			    	}
			    	else if(match(l[j],"}") != -1)
			    	{
			    		if(stack_pop() == '.' && match(l[j],";") == -1)
			    			break;
			    		
			    	}
			    	
			    	x = match(l[j],*(vars + k));
			    	if(x != -1)
			    	{
			    		
			    		if(x == 0 && !isalpha(l[j][x + strlen(*(vars + k))]))
			    		addEntry(&res[len_symtab].address,j);
			    		else
			    		{
				    		if((x >= 1) && !isalpha(l[j][x-1]))
				    		{
				    			y = strlen(*(vars + k));
				    			
				    			if((x+y  ) < strlen(l[i]))
				    			{
				    				if(!isalpha(l[j][x + y]))
				    					addEntry(&res[len_symtab].address,j);
				    			}
				    			else	
				    				addEntry(&res[len_symtab].address,j);
				    		}
			    		}
			    	}
			    }
			    free(*(vars + k));
			    len_symtab++;
			}
			free(vars);
			free(temp);
		}
		
		
	}
	return res;
}


char *rep( char *s, char *old, char *new1)
{
    char *ret;
    int i, count = 0;
    int newlen = strlen(new1);
    int oldlen = strlen(old);
    int temp;
    for (i = 0; s[i] != '\0'; i++)    
    {
        if (strstr(&s[i], old) == &s[i]) 
        {
            count++;
            i += oldlen - 1;
        }
    }
    ret = (char *)malloc(i + count * (newlen - oldlen));
    if (ret == NULL)
    {	
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (*s)
    {
        if (strstr(s, old) == s) //compare the substring with the newstring
        {
           temp = match(s,old);
           if((temp + oldlen < strlen(s))&&(!isalpha(s[temp + oldlen])))
            {strcpy(&ret[i], new1);
            i += newlen; //adding newlength to the new string
            s += oldlen;//adding the same old length the old string
            }
        	else
        ret[i++] = *s++;	
        }
        else
        ret[i++] = *s++;
    }
    ret[i] = '\0';
    return ret;
}


int no_of_lines(char *filename)		//Counts line numbers
{
	char c;	
	int count = 0;
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
    	{
		return 0;	
	}	
	for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') 
            count = count + 1;
	fclose(fp);
	return count;
}



void addEntry(List* L, int addr) //Add address of variable occurence in Symbol Table
{
	Node* t = malloc(sizeof(Node));
	t->addr = addr;
	t->next = NULL;
	Node * temp = L->first;
	while (temp != NULL && temp->next != 0)
	{
		temp = temp->next;
	}
	if (temp == NULL)
	{
		L->first = t;
	}
	else
	temp->next = t;
}

void list_display(List* L)
{
	Node * temp = L->first;
	while (temp != NULL)
	{
		printf("%d ", temp->addr);
		temp = temp->next;
	}
}

void list_write(FILE *f,List* L)
{
	Node * temp = L->first;
	while (temp != NULL)
	{
		fprintf(f,"%d ", temp->addr);
		temp = temp->next;
	}
}


int match(char *t,char *p)			//string matching
{
	int n = strlen(t) , m = strlen(p),i,j;
	for(i=0;i<n-m+1;i++)
	{
		j=0;
		while(j<m && t[i+j] == p[j])
			j++;
		if(j == m)
		{
			return i;
		}
	}
	
	return -1;
}

void stack_push (char a)
{
    if (st.top == (BUFFER_SIZE - 1))
    {
        return;
    }
    else
    {
        st.top = st.top + 1;
        st.stk[st.top] = a;
    }
    return;
}
char stack_pop ()
{
    char num;
    if (st.top == - 1)
    {
       // printf ("Stack is Empty\n");
        return '.';
    }
    else
    {
        num = st.stk[st.top];
        st.top = st.top - 1;
    }
    return(num);
}

char** str_split(char* a_str, const char a_delim)  //Split string into list(array) of strings 
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


void removeSubstring(char *s,const char *toremove)
{
  int i=0,j,k=0;
  char * temp;
  if(strcmp(toremove,"[]") == 0)
  {
  	temp = malloc(30*sizeof(char));
  	while(s[i])
  	{
  		if(s[i] == '[')
  		{
  			temp[k] = '[';
  			k++;i++;
  			while(s[i]!= ']')
  			{
  				temp[k] = s[i];
  				k++;i++;
  			}
  			temp[k] = ']';	
  			temp[k+1]='\0';
  			removeSubstring(s,temp);
  		}
  		i++;
  	}	
  	
  	free(temp);
  }
  else if(strcmp(toremove,"=") == 0)
  {
  	temp = malloc(100*sizeof(char));
  	while(s[i])
  	{
  		if(s[i] == '=')
  		{
  			temp[k] = '=';
  			k++;i++;
  			while(s[i])
  			{
  				temp[k] = s[i];
  				k++;i++;
  			}
  			temp[k] = '\0';
  			//printf("%s\n",temp);
  			removeSubstring(s,temp);
  		}
  		i++;
  	}	
  	
  	free(temp);
  }
  else if(strcmp(toremove,"//") == 0 || strcmp(toremove,"/*") == 0)
  {
  	temp = malloc(100*sizeof(char));
  	while(s[i])
  	{
  		if(s[i] == '/')
  		{
  			temp[k] = '/';
  			k++;i++;
  			while(s[i])
  			{
  				temp[k] = s[i];
  				k++;i++;
  			}
  			temp[k] = '\0';
  			removeSubstring(s,temp);
  		}
  		i++;
  	}	
  	
  	free(temp);
  }
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
  
}

void removeUnwanted(char *s)
{
removeSubstring(s,"\n");
removeSubstring(s,"/*");
removeSubstring(s,"//");
removeSubstring(s,"/");
removeSubstring(s,";");
removeSubstring(s,"*");
removeSubstring(s,"=");
removeSubstring(s," ");
removeSubstring(s,"	");
removeSubstring(s,"[]");
removeSubstring(s,"{");
removeSubstring(s,"}");

}


char** alloc_array2d(int r, int c) { 
char **a; 
int i; 
a = (char **) malloc(r * sizeof(char *));
for(i = 0; i < r; i++) 
{ 
	a[i]=(char*)malloc(c*sizeof(char));
	a[i][c-1] = '\0';   
} 
return a; 
}

void free_array2d(char **a, int r) 
{
        int i; 
        for(i = 0; i < r; i++) 
        { 
	        free(a[i]); 
        } 
        free(a); 
} 
