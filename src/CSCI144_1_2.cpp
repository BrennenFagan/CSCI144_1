//============================================================================
// Name        : CSCI144_1.cpp
// Author      : Brennen Fagan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */

using namespace std;

class Stock{
	string stock_name;
	int sharesnumber;
	int sharesowned;
	double price;

public:
	Stock(string n, int totalshares, double startprice)
	{
		stock_name=n;
		sharesnumber=totalshares;
		sharesowned=0;
		price=startprice;
	}
	double Buy(int sharesbought)
	{
		double revenue=0;
		if(sharesbought+sharesowned<=sharesnumber)
		{
			sharesowned+=sharesbought;
			revenue=sharesbought*price;
		}
		else //number of shares bought is less than available
		{
			//cannot buy. Later, can update with a wait to buy more shares.
			revenue=0;
		}
		return revenue;
	}
	double Sell(int sharessold)
	{
		double revenue=0;
		sharesowned-=sharessold;
		revenue=sharessold*price;
		return revenue;
	}
	string getName(){return stock_name;}
	int getTotal(){return sharesnumber;}
	int getOwned(){return sharesowned;}
	double getPrice(){return price;}
};

double CMD (string Input, vector<Stock> portfolio);
string makeCMD (vector<Stock> portfolio);
int EncodeStringtoInt(string Input);
string DecodeInttoString(int victim);

int main() {
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	cout<<"It is a five stock world: MSFT, APPL, LNUX, LFNT, DNKY."<<endl;
	vector <Stock> portfolio;
	Stock Msft ("MSFT",1000,35.2);
	Stock Appl ("APPL",250,43.1);
	Stock Lnux ("LNUX",200,2.74);
	Stock Lfnt ("LFNT",500,101);
	Stock Dnky ("DNKY",500,75.8);

	cout<<"Number of shares:         1000,  250,  200,  500,  500."<<endl;

	portfolio.push_back(Msft);//portfolio[0]=Msft;
	portfolio.push_back(Appl);//portfolio[1]=Appl;
	cout<<"Initial (no change) Price: 35.2, 43.1, 2.74, 101, 75.8."<<endl;
	portfolio.push_back(Lnux);//portfolio[2]=Lnux;
	portfolio.push_back(Lfnt);//portfolio[3]=Lfnt;
	cout<<"Randomly generating BUYs and SELs of random quantities."<<endl;
	portfolio.push_back(Dnky);//portfolio[4]=Dnky;


	cout<<"Assuming 100 shares are owned in each stock initially."<<endl;
	for(int i=0; i<5; i++)
	{
		portfolio[i].Buy(100);
		cout<<portfolio[i].getName()<<": "<<portfolio[i].getOwned()<<endl;
	}

	string example = makeCMD (portfolio);
	int encode =EncodeStringtoInt(example);
	cout<<"Example: "<<example<<", encoded: "<<encode<<", Revenue: "<<CMD(DecodeInttoString(encode),portfolio);

	//**********************************************************************

	//We Now initialize our pipe
	/*
	int thepipe[2];
	if ( pipe(thepipe)  )
    {
      fprintf (stderr, "Pipe failed.\n");
      return 1;
    }

	pid_t pidProducer;

	pidProducer=fork();

	//ERROR
	if(pidProducer<0)
	{
        perror("fork"); // display error message
        exit(1);
	}

	//CHILD
	else if(pidProducer==0)
	{

	}

	//PARENT
	else
	{
		pid_t pidConsumer;

		pidConsumer=fork();

	}

*/



	return 0;
}

double CMD (string Input, vector<Stock>portfolio)
{
	double revenue=0;
	string Action = Input.substr(0,3); Input.erase(Input.begin(),Input.begin()+4); //Buy or Sel
	string Victim = Input.substr(0,4); Input.erase(Input.begin(),Input.begin()+5); //Stock Name
	string Number = Input.substr(0,3); Input.erase(Input.begin(),Input.begin()+3); //Number of shares
	int numshares;
	stringstream convert(Number);
	if ( !(convert >> numshares))
		numshares = 0;

	if(Victim=="MSFT")
		if(Action=="BUY")
			revenue=portfolio[0].Buy(numshares);
		else
			revenue=portfolio[0].Sell(numshares);

	else if(Victim=="APPL")
		if(Action=="BUY")
			revenue=portfolio[1].Buy(numshares);
		else
			revenue=portfolio[1].Sell(numshares);

	else if(Victim=="LNUX")
		if(Action=="BUY")
			revenue=portfolio[2].Buy(numshares);
		else
			revenue=portfolio[2].Sell(numshares);

	else if(Victim=="LFNT")
		if(Action=="BUY")
			revenue=portfolio[3].Buy(numshares);
		else
			revenue=portfolio[3].Sell(numshares);

	else
		if(Action=="BUY")
			revenue=portfolio[4].Buy(numshares);
		else
			revenue=portfolio[4].Sell(numshares);

	return revenue;
}

string makeCMD (vector<Stock> portfolio)
{
	//Take Portfolio
	//Identify the number of entries
	//Pick a Random Number
	//Rand a < 50 => Buy
	//		 >=   => Sell
	// a mod 5 => Victim
	// Pick a random number of shares (001~100)
	// Retrieve Price

	string retcmd="";

	//int numstocks = portfolio.size(); //in general. Here only 5.
	int shares = rand() % 100+1;
	string numshares;
	stringstream convert; convert << shares; numshares = convert.str();
	if (shares<10)
		numshares="0" + numshares;
	if (shares<100)
		numshares="0" + numshares;

	int determine = rand() % 100;
	if(determine<50)
		retcmd="BUY ";
	else
		retcmd="SEL ";
	int choice = determine%5;

	double price = portfolio[choice].getPrice();
	string strprice;
	stringstream convert2;
	convert2 << price;
	strprice = convert2.str();

	retcmd=retcmd + portfolio[choice].getName() + " " + numshares + " " + strprice;

	return retcmd;
}


int EncodeStringtoInt(string Input)
{
	//			abcccddd.dd
	int encoded=10000000;
	//a) First digit = Dummy,
	//b) Next Digit = Buy >=5, Sel <5 and Stock encoded
	//c) Next Three Digits = Number of Shares (1 -> 100)
	//d) Remaining Digits = Price (2.74 -> 101)
	string Action = Input.substr(0,3); Input.erase(Input.begin(),Input.begin()+4); //Buy or Sel
	string Victim = Input.substr(0,4); Input.erase(Input.begin(),Input.begin()+5); //Stock Name
	string Number = Input.substr(0,3); Input.erase(Input.begin(),Input.begin()+3); //Number of shares

	int numshares;
	stringstream convert(Number);
	if ( !(convert >> numshares))
		numshares = 0;
	encoded+=numshares*1000;

	if(Victim=="MSFT")
	{
		encoded+=35.2;
		if(Action=="BUY")
			encoded+=1000000*5;
		else
			encoded+=1000000*0;
	}

	else if(Victim=="APPL")
	{
		encoded+=43.1;
		if(Action=="BUY")
			encoded+=1000000*6;
		else
			encoded+=1000000*1;
	}

	else if(Victim=="LNUX")
	{
		encoded+=2.74;
		if(Action=="BUY")
			encoded+=1000000*7;
		else
			encoded+=1000000*2;
	}

	else if(Victim=="LFNT")
	{
		encoded+=101;
		if(Action=="BUY")
			encoded+=1000000*8;
		else
			encoded+=1000000*3;
	}

	else
	{
		encoded+=75.8;
		if(Action=="BUY")
			encoded+=1000000*9;
		else
			encoded+=1000000*4;
	}

	return encoded;
}
string DecodeInttoString(int victim)
{
	//			abcccddd.dd
	//  encoded=10000000;
	//a) First digit = Dummy,
	//b) Next Digit = Buy >=5, Sel <5 and Stock encoded
	//c) Next Three Digits = Number of Shares (1 -> 100)
	//d) Remaining Digits = Price (2.74 -> 101)
	string decoded="";
	double encoded = double(victim);
	int Buy_Sel = int(trunc(encoded/1000000));
	int numshar = int(trunc(encoded/1000))%1000;

	if(Buy_Sel<5)
		decoded = decoded + "SEL ";
	else
		decoded = decoded + "BUY ";
	if(Buy_Sel%5==0)
		decoded = decoded + "MSFT ";
	else if (Buy_Sel%5 == 1)
		decoded = decoded + "APPL ";
	else if (Buy_Sel%5 == 2)
		decoded = decoded + "LNUX ";
	else if (Buy_Sel%5 == 3)
		decoded = decoded + "LFNT ";
	else
		decoded = decoded + "DNKY ";

	ostringstream convert;
	convert << numshar;
	decoded = decoded + convert.str() + " ";

	if(Buy_Sel%5==0)
		decoded = decoded + "35.2";
	else if (Buy_Sel%5 == 1)
		decoded = decoded + "43.1";
	else if (Buy_Sel%5 == 2)
		decoded = decoded + "2.74";
	else if (Buy_Sel%5 == 3)
		decoded = decoded + "101";
	else
		decoded = decoded + "75.8";


	/*
	 *Works only for integers apparently. Instead taking prices from predefineds.
	ostringstream convert2;
	convert2 << price;
	decoded = decoded + convert2.str() + " ";
	*/

	return decoded;
}
