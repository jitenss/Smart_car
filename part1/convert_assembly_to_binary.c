#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct table
{
	char ins[20];
	char binary[20];
};

struct table opcodes[50];
struct table registers[50];
struct table labels[50];

int num_opcodes=0,num_registers=0,num_labels=0;

//function to make table of opcodes
void get_opcodes()
{
	FILE *op = fopen("opcodes","r");

	if(op==NULL)
	{
		printf("File not found\n");
	}
	else
	{
		while(fscanf(op,"%s %s",opcodes[num_opcodes].ins,opcodes[num_opcodes].binary)!=EOF)
		{
			num_opcodes++;
		}
	}
	fclose(op);
}

//function to make table for registers
void get_registers()
{
	FILE *op = fopen("registers","r");

	if(op==NULL)
	{
		printf("File not found");
	}
	else
	{
		while(fscanf(op,"%s %s",registers[num_registers].ins,registers[num_registers].binary)!=EOF)
		{
			num_registers++;
		}
	}
	fclose(op);
}


//convert numbers to binary
void convert_numbers_to_binary(int num,char arr[],int l)
{
	int temp,i=0;

	while(num!=0)
	{
		temp = num%2;
		arr[i]=temp + '0';
		num/=2;
		i++;
	}
	for(;i<l;i++)
	{
		arr[i]='0';
	}
	char ch;
	int j=l-1;

	for(i=0;i<l/2;i++)
	{
		ch = arr[i];
		arr[i]= arr[j];
		arr[j] = ch;
		j--;
	}
	arr[l]='\0';
}

//function to find all the labels in the assembly language i.e. pass 1 of the assembler
void find_labels()
{
	FILE *assembly = fopen("assembly","r");
	FILE *labels = fopen("labels","w");

	char s[20];
	int l,lines=0,label_number=0,num_labels_read=0,j=0;
	char label_number_string[20];


	while(fgets(s,20,assembly)!=NULL)
	{
		l = strlen(s);
		lines++;

		char temp[20],temp1[20];

		if(s[0]=='L' && (s[1]>=48 && s[1]<=57))
		{
			if(label_number==0)
				label_number = lines;
			else
				label_number += 1;

			
			int q;

			for(q=0;q<l;q++)
			{
				if(s[q]!=':')
				{
					temp1[q] = s[q];
				}
			}
			temp1[q-2] = '\0';

			convert_numbers_to_binary(label_number,temp,8);
			fprintf(labels,"%s %s\n",temp1,temp);
			num_labels++;
		}
	}
	fclose(assembly);
	fclose(labels);
}

//function to make table for labels
void get_labels()
{
	FILE *op = fopen("labels","r");

	if(op==NULL)
	{
		printf("File not found");
	}
	else
	{
		while(fscanf(op,"%s %s",labels[num_labels].ins,labels[num_labels].binary)!=EOF)
		{
			num_labels++;
		}
	}
	fclose(op);
}

//function to convert assembly language to binary
void convert_to_binary()
{
	FILE *binary = fopen("binary","w");
	FILE *assembly = fopen("assembly","r");

	char s[50],temp[10];

	int l,i,l_temp,j,k=0;

	while(fgets(s,20,assembly)!=NULL)
	{
		l = strlen(s);
		
		for(i=0;i<l-1;i++)
		{
			if(s[i]!=' ' && s[i]!='#' && s[i]!='$' && s[i]!=',' && s[i]!=':')
			{
				temp[k] = s[i];
				k++;
			}
			if(s[i]==' ')
			{
				temp[k]='\0';
				l_temp = strlen(temp);

				for(j=0;j<num_opcodes;j++)
				{
					if(strcmp(temp,opcodes[j].ins)==0)
					{
						fprintf(binary,"%s",opcodes[j].binary);
					}
				}
				
				k=0;
			}
			if(s[i]==',' || i==l-2)
			{
				temp[k]='\0';
				k=0;

				if(temp[0]>=48 && temp[0]<=57)
				{
					int num=0,q;

					for(q=0;q<strlen(temp);q++)
					{
						num = (num*10) + temp[q];
					}
					convert_numbers_to_binary(num,temp,4);
					fprintf(binary, "%s",temp);
				}

				if(temp[0]=='R')
				{
					for(j=0;j<num_registers;j++)
					{
						if(strcmp(temp,registers[j].ins)==0)
						{
							fprintf(binary, "%s",registers[j].binary);
						}
					}
				}
				else if(temp[0]=='L')
				{
					for(j=0;j<num_labels;j++)
					{
						if(strcmp(temp,labels[j].ins)==0)
						{
							fprintf(binary, "%s",labels[j].binary);
						}
					}
				}
				else if(temp[0]=='H')
				{
					for(j=0;j<num_opcodes;j++)
					{
						if(strcmp(temp,opcodes[j].ins)==0)
						{
							fprintf(binary,"%s",opcodes[j].binary);
						}
					}
				}
			}
		}
		fprintf(binary,"\n");
	}
	fclose(binary);
	fclose(assembly);
}


//function for printing the opcodes table
void print_opcodes()
{
	int i;

	for(i=0;i<num_opcodes;i++)
	{
		printf("%s %s\n",opcodes[i].ins,opcodes[i].binary);
	}
}

//function for printing the registers table
void print_registers()
{
	int i;

	for(i=0;i<num_registers;i++)
	{
		printf("%s %s\n",registers[i].ins,registers[i].binary);
	}
}

//function for printing the labels table
void print_labels()
{
	int i;

	for(i=0;i<num_labels;i++)
	{
		printf("%s %s\n",labels[i].ins,labels[i].binary);
	}
}

void print_machine()
{
	FILE *bin=fopen("binary","r");

    char s[50];
    
    while(fgets(s,50,bin)!=NULL)
    {
        printf("%s",s);
    }
    fclose(bin);
}

void main()
{
	int ch=0;

	get_opcodes();
	get_registers();

	while(ch!=6)
	{
		printf("\nEnter your choice\n");
		printf("1. Print Opcode Table\n");
		printf("2. Print Register Table\n");
		printf("3. Convert assembly to binary\n");
		printf("4. Print Symbol Table\n");
		printf("5. Print Machine Code\n");
		printf("6. Exit\n");
		
		scanf("%d",&ch);

		switch(ch)
		{
			
			case 1:	
				print_opcodes();
				break;
			case 2:
			
				print_registers();
				break;
			case 3:
				find_labels();
				get_labels();
				convert_to_binary();
				printf("Successfully converted\n");
				break;
			case 4:
				print_labels();
				break;
			case 5:
				print_machine();
				break;
			default:
				if(ch!=6)
				{	
					printf("Wrong choice....enter again");
				} 
		}
	}
}
