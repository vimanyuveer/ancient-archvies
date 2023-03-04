/* 	12/07/2019
	Vimanyu Veer
	PROJECT: DELIVERY LOGISTICS (TC3)
*/
// remove(), rename() in stdio.h
#include<iostream.h>
#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include<ctype.h>
#include<stdlib.h>
#include<iomanip.h>
#include<stdio.h>
void reg_parcel(int x);         //FUNCTION DELEREATIONS
void calc_fare();
void display();
char* ret_name();               //c-style string type
int ret_id();
void insert_data();
void show_data();
int track_ID_assign();
void search_data();
void modify_data();
void gen_headers();
int switchDriver(char y[6]);
//-------------------------------------------------------------------
class Parcel{
	int track_ID;
	float value;
	float fare;
	char deliver_to[16];
	char address[29];
public:
	void reg_parcel(int passed_ID = track_ID_assign()){ 	// parameter for modify fx, non-default case used in modify***
		cout<<endl<<"(AUTO ASSIGNED) Tracking ID--> "<<passed_ID;
		track_ID = passed_ID;
		cout<<endl<<endl<<"Enter Name of Addressee-->";
		cin.sync();				//flush input
		cin.getline(deliver_to, 16);
		cout<<endl<<"Enter Delivery Address-->";
		cin.getline(address, 29);
		cout<<endl<<"Enter Value of Parcel-->";
		cin>>value;
		calc_fare();
		cout<<endl<<"Fare is (10 %)-->"<<fare;
		getch();
	}
	void calc_fare(){
		fare = value / 10;			//fixed 10 perecent rate
	}
	void display(){
		cout<<endl<<"| ";
		cout.width(3); 		 	//sets field width
			cout<<track_ID;
			cout<<" | ";
		cout.width(15);
			cout<<deliver_to;
			cout<<" | ";
		cout.width(8);
			cout<<setprecision(2)<<value;	//in iomanip
			cout<<" | ";
		cout.width(8);
			cout<<setprecision(2)<<fare;
			cout<<" | ";
		cout.width(28);
			cout<<address;
			cout<<" |";
	}
	char* ret_name(){         	//char* type to return name
		return deliver_to;
	}
	int ret_id(){           	// helps with search
		return track_ID;
	}
}p_obj;		    				//object of Package
//-------------------------------------------------------------------------
//FILE HANDLING PART:
void insert_data(){
	ofstream rfile;                     	//declare rec file
	rfile.open("records.dat", ios::binary | ios::app);    	//open file
	p_obj.reg_parcel();	    		    	//user inputs data
	rfile.write((char*)&p_obj, sizeof(p_obj));  	//data gets written
	rfile.close();
}
void show_data(){
	fstream rfile("records.dat", ios::binary | ios::in);
       //	rfile.seekg(0, ios::beg);		//expl
	int counter = 0;            		//for number of records found
	while(rfile.read((char*)&p_obj, sizeof(p_obj)))
		{	p_obj.display();
			counter += 1;
		}
	if(counter != 0)
		cout<<"\n\n "<<counter<<" records found.";
	else
		cout<<"\n\n NO RECORDS FOUND.";
	rfile.close();
}
int track_ID_assign(){         		//auto assign track_ID
	fstream rec;
	rec.open("records.dat", ios::nocreate | ios::binary |ios::in);
	int flag = 0;
	while(rec.read((char*)&p_obj, sizeof(p_obj)));
	flag = p_obj.ret_id();      	//ret ID OF LAST object
	rec.close();
	return flag + 1;
/* 	This function counts and returns the number of records present in the
	file. Also, ios::nocreate is a mode in which a new file is not created,
	if one is not already found; this ensures that the data being tested
	is not modified.
*/
}
void search_data(){
	clrscr();			//as this function takes up a lot of screen sacpe
	cout<<"\n-----------------------------------";
	cout<<endl<<"\tSEARCH BY : "<<endl;
	cout<<"\t 1. name  - NAME OF ADDRESSEE"<<endl;
	cout<<"\t 2. id	   - TRACKING ID"<<endl;
	cout<<"ENTER SELECTION (num or text) --> ";
	int ch2 = 0;                		// for switch()
	char ch1[7];                      	// for user input
	gets(ch1);
	if(strcmpi(ch1, "name") == 0 || ch1[0] == '1')
		ch2 = 1;
	else if(strcmpi(ch1, "id") == 0 || ch1[0] == '2')
		ch2 = 2;
	else if(strcmpi(ch1, "cancel") == 0 || ch1[0] == '3')
	{
		ch2 = 3;
		cout<<"\nGoing Back...";
		delay(900);
	}
	if (ch2 == 0)
	{	cerr<<"ERROR! Input Error. Please Enter Again.";
			search_data();
	}                               	//re-promt entry
	else if(ch2 == 1 || ch2 == 2){
		switch(ch2){
			case 1: cout<<endl<<"Searching by name"<<endl;
				char term[16];
				cout<<"\tENTER SEARCH TERM--> ";
				cin.sync();		        //flush cin
				cin.getline(term, 16);
				int counter = 0;
				fstream rec;
				rec.open("records.dat", ios::binary | ios:: in);
					// p_obj obj of class parcel
				while(rec.read((char*)&p_obj, sizeof(p_obj)))
				{
					if(strcmpi(term, p_obj.ret_name()) == 0)
					{       if(counter == 0) 	//headers only appear once
							gen_headers();
						p_obj.display();
						counter += 1;
					}
				}
				if(counter == 0)
					cout<<"\n\n NO RECORDS FOUND";
				else
					cout<<"\n\n "<<counter<<" record(s) found. Press any key to continue. \n";
				rec.close();
				getch();
				break;

			case 2: 		//same as above, except for search parameter
				cout<<endl<<"Searching by TRACKING ID"<<endl;
				int term1;
				cout<<"\tENTER SEARCH TERM--> ";
				cin>>term1;
				int counter1 = 0;
				fstream rec1;
				rec1.open("records.dat", ios::binary | ios:: in);
					// p_obj obj of class parcel
				while(rec1.read((char*)&p_obj, sizeof(p_obj)))
				{
					if(term1 == p_obj.ret_id())
					{       if(counter1 == 0) 	//headers only appear once
							gen_headers();      //display the elements which have been delelted
						p_obj.display();
						counter1 += 1;
					}
				}
				if(counter1 == 0)
					cout<<"\n\n NO RECORDS FOUND";
				else
					cout<<"\n\n "<<counter1<<" record(s) found. Press any key to continue. \n";
				rec1.close();
				getch();
			break;
		}
	}
}
void modify_data(){
	clrscr();			     	//clear screen, display submenu
	cout<<"\n--------------------------\n";
	cout<<"\t MODIFYING DATA."<<endl;
	cout<<"\t 1. del    - Delete Data."<<endl;
	cout<<"\t 2. edit   - Change Data."<<endl;
	cout<<"\t 3. cancel - Go Back."<<endl;
	cout<<"ENTER YOUR CHOICE (text or num) --> ";
	char ch5[7] = "NULL";			// assignment as fx is calling itself, buggy otherwise
	cin.sync();		        		//flush cin 	***
	cin.getline(ch5, 7);
	int ch6 = 0;				    //temp var for switch()
	if(strcmpi(ch5, "del") == 0 || ch5[0] == '1')
		ch6 = 1;
	else if(strcmpi(ch5, "edit") == 0 || ch5[0] == '2')
		ch6 = 2;
	else if(strcmpi(ch5, "cancel") == 0 || ch5[0] == '3')
	{	cout<<"Going Back...";
		delay(900);
	}
	else if(strcmp(ch5, "NULL") != 0) 	//as fx calls itself
	{	cerr<<"\n ERROR! Command not recognised. Please try again.";
		delay(900);
		modify_data();
	}
	if(ch6 == 1 || ch6 == 2)		   	//only run favourable cases
	{
		switch(ch6){
			case 1: cout<<"\nDeleting Entry."<<endl;
				cout<<"Enter tracking ID of entry--> ";
				int ch7;
				cin>>ch7;
				fstream old_File("records.dat", ios::binary | ios:: in);
				fstream new_File("temp202.dat", ios::binary | ios::out);
				// opened two files to selectively copy non-delted entries
				int flag3 = 0;              //ensures table headers are generated once
				while(old_File.read((char*)&p_obj, sizeof(p_obj)))
				{
					if( ch7 != p_obj.ret_id())
						new_File.write((char*)&p_obj, sizeof(p_obj));
					else{
						if(flag3 == 0)
						{       cout<<"\n   DELETED ENTRY : "<<endl;
							gen_headers();
						}
						p_obj.display();    // display deleted enteries
						flag3 += 1;	        //increment deletion counter
					}
				}
				old_File.close();
				new_File.close();
				remove("records.dat");    	//complete swapping operation
				rename("temp202.dat", "records.dat");
				cout<<endl<<endl<<flag3<<" Record(s) Deleted. \n";
				cout<<"Press any key to continue.";
				getch();
				break;

			case 2: /* same as above, except the else case
				takes user input and changes the entry,
				instead of deleteing it. */
				cout<<"\nChanging Entry."<<endl;
				cout<<"Enter tracking ID of entry--> ";
				int ch8;
				cin>>ch8;
				fstream old_File1("records.dat", ios::binary | ios:: in);
				fstream new_File1("temp203.dat", ios::binary | ios::out);
				int flag4 = 0;
				while(old_File1.read((char*)&p_obj, sizeof(p_obj)))
				{
					if( ch8 != p_obj.ret_id())
						new_File1.write((char*)&p_obj, sizeof(p_obj));
					else{
						if(flag4 == 0)
						cout<<endl<<"Enter New Data--- \n";
						p_obj.reg_parcel(ch8);		//new entry assigned id = ch8, same as prev entry
						new_File1.write((char*)&p_obj, sizeof(p_obj));
						{       cout<<"\n ---------------------------------------";
							cout<<"\n\n   MODIFIED ENTRY : "<<endl;
							gen_headers();
						}
						p_obj.display();        // display deleted enteries
						flag4 += 1; 	        //increment deleteion counter, NOTE not really necescary
					}
				}
				old_File1.close();
				new_File1.close();
				remove("records.dat");        	//complete swapping operation
				rename("temp203.dat", "records.dat");
				cout<<endl<<endl<<flag4<<" Record(s) Changed. \n";
				cout<<"Press any key to continue.";
				getch();
				break;
		}
	}
}
//-------------------------------------------------------------------
int main(){
int mainLoop=1;  				        //to run main menu
char  mainChoice[16]; 	         		//to control flow from menu
while(mainLoop){                   		//main menu loop
clrscr();
cout<<"\n\t+-------------------------------------------------------+"<<endl;
cout<<"\t|	         VEER DELIVERY LOGISTICS    	   	|"<<endl;
cout<<"\t| Enter your choice, and press ENTER:			|"<<endl;
cout<<"\t| (text or integer input)-->				|"<<endl;
cout<<"\t| 1. reg	-Register package			|"<<endl;
cout<<"\t| 2. track    	-Track package				|"<<endl;
cout<<"\t| 3. alter    	-Alter Records (Modify/Delete)		|"<<endl;
cout<<"\t| 4. show     	-Show all Packages			|"<<endl;
cout<<"\t| 0. exit     	-EXIT the  program			|"<<endl;
cout<<"\t+-------------------------------------------------------+"<<endl;
cin.sync(); 				//*** flushing input, skipping getline otherwise
cin.getline(mainChoice, 6);             //choice gets inputted
switch(switchDriver(mainChoice)){     	//get integer to control flow
	case 0:
		cout<<"\nEXITING";
		int i = 15;
		while(i)                      	//animation while exiting
		{
			cout<<".";
			delay(25);
			i -=1;
		}
		mainLoop = 0;
		getch();
		break;
	case 1:
		insert_data();
		break;
	case 2:
		search_data();
		break;
	case 3:
		modify_data();
		break;
	case 4:
		gen_headers();      	    	//generate table headers
		show_data();
		cout<<endl<<endl<<"Press any key to continue.";
		getch();
		break;
	default:
		cerr<<"ERROR! Switching Error.";
		break;

	}		        		//end of switch
}		    	     		//end of main loop
return 0;
}                      		//end of main()
//-----------------------------------------------------------------------
void gen_headers(){     		//generate headers for displaying data table
cout<<endl;
cout<<"+ ID ";
cout<<" +    DELIVEREE   ";
cout<<" +   VALUE  ";
cout<<"+   FARE  ";
cout<<" +        ADDRESS\t\t     +";
}
int switchDriver(char input[6])       	//helps switch(), converts input to int value
{
		char list[5][10]; 		//5 words, 10 letters each
		strcpy(list[0], "exit");       	//initialising values
		strcpy(list[1], "reg");
		strcpy(list[2], "track");
		strcpy(list[3], "alter");
		strcpy(list[4], "show");

	 if(isdigit(input[0]))		//in ctype.h; called if int inputted
	   {	int y = input[0];	//store intial digit in y
		if(0 <= y <= 4)
			return y-48;
		/*NOTE-prev. atoi()
		As ascii is offset by 48 for numbers,
		ie, an input of '0' would give y= 48 in ASCII */
	   }

	else{
	      for(int x=0;  x < 5 ; x+=1){    	//search for command
		   if(strcmpi(input, list[x])==0)
			  return x;		//test and return answer
		 else if(x==4) break;  		//error case, for bypassing
	} }
	cerr<<"ERROR! Command not found. Press ENTER to continue.";
	getch();
	return -1;
}               				//END- switchDrive()
