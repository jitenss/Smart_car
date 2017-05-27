#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int registers[20]={0}, pc = 1,zero = 0,negative = 0;
char ir[20],ins[20],alu_sig[20]={'0','0','0'};
FILE *op;

int dist[10],dir[10],total_dist,inc_dist=0,inc_dir=0;

void input();
int string_to_int(char s[],int n);
void move_rr(char r1[],char r2[]);
void move_ri(char r[],char num[]);
void add(char r1[],char r2[]);
void addri(char r1[],char num[]);
void sub(char r1[],char r2[]);
void subri(char r1[],char num[]);
void mul(char r1[],char r2[]);
void divide(char r1[],char r2[]);
int jmp(char x[],char y[]);
int jeq(char x[],char y[],int val);
int jne(char x[],char y[],int val);
int jget(char x[],char y[],int val);
int jlt(char x[],char y[],int val);
int  compare_ri(char r[],char num[]);
int  compare_rr(char r1[],char r2[]);
void hlt();
void display_status();
void transfer_dist(char x[],char y[]);
void transfer_dir(char x[],char y[]);



//checking the instruction
void readIns()
{
	op = fopen("binary","r");

	char opcode[20],x[20],y[20];
	int i,k=0,val,temp=0;

	input();

	while(fscanf(op,"%s",ins)!=EOF)
	{
		pc++;
		strcpy(ir,ins);

		for(int i=0;i<=7;i++)
		{
			opcode[i] = ins[i];
		}
		opcode[8]='\0';

		k=0;
		for(i=8;i<12;i++)
		{
			x[k] = ins[i];
			k++;
		}
		x[k]='\0';

		k=0;
		for(i=12;i<16;i++)
		{
			y[k] = ins[i];
			k++;
		}
		y[k] = '\0';

		if(!strcmp(opcode,"11111111"))
		{
			hlt();
		}

		else if(!strcmp(opcode,"00000000"))   //move rr
		{
			move_rr(x,y);
		}
		else if(!strcmp(opcode,"00000001"))   //move ri
		{
			move_ri(x,y);
			//printf("r8 %d\n",registers[8]);
		}
		else if(!strcmp(opcode,"00000010"))		//compare rr
		{
			val = compare_rr(x,y);
			strcpy(alu_sig,"101");
		}
		else if(!strcmp(opcode,"00000011"))		//compare ri
		{
			val = compare_ri(x,y);
			strcpy(alu_sig,"101");
		}
		else if(!strcmp(opcode,"00000100"))		//add rr
		{
			add(x,y);
			strcpy(alu_sig,"001");
		}
		else if(!strcmp(opcode,"00010000"))		//add ri
		{
			addri(x,y);
			strcpy(alu_sig,"001");
		}
		else if(!strcmp(opcode,"00000101"))		//sub rr
		{
			sub(x,y);
			strcpy(alu_sig,"010");
		}
		else if(!strcmp(opcode,"00010001"))		//sub ri
		{
			subri(x,y);
			strcpy(alu_sig,"010");
		}
		else if(!strcmp(opcode,"00000110"))		//mul rr
		{
			mul(x,y);
			strcpy(alu_sig,"011");
		}
		else if(!strcmp(opcode,"00000111"))		//div rr
		{
			divide(x,y);
			strcpy(alu_sig,"100");
		}
		else if(!strcmp(opcode,"00001000"))		//jump
		{
			int z = jmp(x,y);
			pc = z;
		}
		else if(!strcmp(opcode,"00001001"))		//jump if equal
		{
			int z = jeq(x,y,val);
			pc = z;
		}
		else if(!strcmp(opcode,"00001010"))		//jump if not equal
		{
			int z = jne(x,y,val);
			pc = z;
		}
		else if(!strcmp(opcode,"00001101"))		//jump if less than
		{
			int z = jlt(x,y,val);
			pc = z+1;
		}
		else if(!strcmp(opcode,"00001110"))		//jump if greater than equal to
		{
			int z = jget(x,y,val);
			pc = z;
		}
		else if(!strcmp(opcode,"00001111"))		//ldb
		{

			if(temp==0)
			{
				transfer_dir(x,y);
				temp=1;
			}
			else
			{
				transfer_dist(x,y);
				temp=0;
			}
						
		}

		display_status();
		zero = 0;
		negative = 0;
		strcpy(alu_sig,"000");
		printf("\n");	
	}
	fclose(op);
}

void input()
{
	printf("Enter direction: 0 for straight; 1 for left; 2 for right;\n");

	int i;

	for(i=0;i<3;i++)
	{
		scanf("%d",&dir[i]);
	}

	printf("Enter distance\n");

	for(i=0;i<3;i++)
	{
		scanf("%d",&dist[i]);
	}
}

void transfer_dist(char x[],char y[])
{
	int a = string_to_int(x,4);
	int b = string_to_int(y,4);

	registers[b] = inc_dist;
	inc_dist++;

	registers[a] = dist[registers[b]];
}

void transfer_dir(char x[],char y[])
{
	int a = string_to_int(x,4);
	int b = string_to_int(y,4);

	registers[b] = inc_dir;
	inc_dir++;

	registers[a] = dir[registers[b]];
}

int power(int n)
{
	int result=1,i;
	for(i=0;i<n;i++)
	{
		result=result*2;
	}
	//printf("%d\n",result);
	return result;
}

//convert string to integer
int string_to_int(char s[],int n)
{
	int i,a=0,k=0;
	for(i=n-1;i>=0;i--)
	{
		a+=(s[i]-'0')*power(k);
		k++;
	}
	//printf("%s %d",s,a);
	return a;
}

//moving values in registers
void move_rr(char r1[],char r2[])
{	

	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	registers[a] = registers[b];
}

//moving values in registers
void move_ri(char r[],char num[])
{

	int a = string_to_int(r,4);
	int b = string_to_int(num,4);

	registers[a] = b;
}

void add(char r1[],char r2[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	registers[a] = registers[a] + registers[b];
}
void addri(char r1[],char num[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(num,4);

	registers[a] = registers[a] + b;
}

void sub(char r1[],char r2[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	registers[a] = registers[a] - registers[b];
}

void subri(char r1[],char num[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(num,4);

	registers[a] = registers[a] - b;
}

void mul(char r1[],char r2[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	registers[a] = registers[a] * registers[b];
}

void divide(char r1[],char r2[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	registers[a] = registers[a] / registers[b];
}

int jmp(char x[],char y[])
{
	char label[20];
	int i,k=0;

	for(i=0;i<4;i++)
	{
		label[k] = x[i]; 
		k++;
	}
	for(i=0;i<4;i++)
	{
		label[k] = y[i]; 
		k++;
	}

	int a = string_to_int(label,8);
	
	fseek(op,a*17,SEEK_SET);

	return a+1;
}

int jeq(char x[],char y[],int val)
{
	char label[20];
	int i,k=0;

	for(i=0;i<4;i++)
	{
		label[k] = x[i]; 
		k++;
	}
	for(i=0;i<4;i++)
	{
		label[k] = y[i]; 
		k++;
	}

	int a = string_to_int(label,8);

	if(val==0)
	{
		fseek(op,a*17,SEEK_SET);
		return a+1;
	}

	return pc;
}

int jne(char x[],char y[],int val)
{
	char label[20];
	int i,k=0;

	for(i=0;i<4;i++)
	{
		label[k] = x[i]; 
		k++;
	}
	for(i=0;i<4;i++)
	{
		label[k] = y[i]; 
		k++;
	}

	int a = string_to_int(label,8);

	if(val!=0)
	{
		fseek(op,a*17,SEEK_SET);
		return a+1;
	}
	return pc;
}

int jlt(char x[],char y[],int val)
{
	char label[20];
	int i,k=0;

	for(i=0;i<4;i++)
	{
		label[k] = x[i]; 
		k++;
	}
	for(i=0;i<4;i++)
	{
		label[k] = y[i]; 
		k++;
	}

	int a = string_to_int(label,8);

	if(val==1)
	{
		fseek(op,a*17,SEEK_SET);
		return a+1;
	}
	return pc;
}

int jget(char x[],char y[],int val)
{
	char label[20];
	int i,k=0;

	for(i=0;i<4;i++)
	{
		label[k] = x[i]; 
		k++;
	}
	for(i=0;i<4;i++)
	{
		label[k] = y[i]; 
		k++;
	}

	int a = string_to_int(label,8);

	if(val==0 || val==2)
	{
		fseek(op,a*17,SEEK_SET);
		return a+1;
	}
	return pc;
}

int  compare_ri(char r[],char num[])
{
	int a = string_to_int(r,4);
	int b = string_to_int(num,4);

	int val;

	if(registers[a]==b)
	{
		zero = 1;
		val=0;
	}
	else if(registers[a]<b)
	{
		negative = 1;
		val = 1;
	}
	else
	{
		val = 2;
	}

	return val;
}

int  compare_rr(char r1[],char r2[])
{
	int a = string_to_int(r1,4);
	int b = string_to_int(r2,4);

	int val;

	if(registers[a] == registers[b])
	{
		zero = 1;
		val=0;
	}
	else if(registers[a] < registers[b])
	{
		negative = 1;
		val = 1;
	}
	else
	{
		val = 2;
	}

	return val;
}

void hlt()
{
	exit(0);
}

void display_status()
{

	printf("Instruction register: %s\n",ir);
	printf("Program Counter: %d\n",pc);

	printf("Status of general purpose registers:\n");

	for(int i=0;i<=15;i++)
	{
		printf("R%d: %d\n",i,registers[i]);
	}

	printf("Status of flag registers:\n");
	printf("Zero: %d\n",zero);
	printf("Negative %d\n",negative);

	printf("ALU Control Signal: %s\n", alu_sig);
}

void main()
{
	int td;

	printf("Enter total distance to reach the destination\n");
	scanf("%d",&td);

	registers[7] = td;

	readIns();
}