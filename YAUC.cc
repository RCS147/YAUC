#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "Unit.h"
using namespace std;

/*
 * Pass in a unit conversion
 * YAUC <quantity> <unitFrom> <unitTo> 	- Output the quantity as the new unit
 * YAUC <quantity> <unitFrom> -A	- Output the quantity in every unit for the given measurement
 */

//Constants
static string fileName = "unitConstants.dat";

typedef std::list<Unit> UnitList;

//Prototypes
double convert(double quantity, Unit from, Unit to);
Unit unitLookup(UnitList* uList, string unitTag);
void showHelp();
string toLowercase(string & s);
bool generateUnitList(UnitList* uList, std::string file);

int main(int argc, char* argv[])
{
	/*
	cout << "argc = " << argc << endl;
	for (int i = 0; i < argc; i++) {

		cout << "argv[" << i << "] = " << argv[i] << endl;
	}
	*/

	//Read in the data file and generate the unit List.
	UnitList* uList = new UnitList();	
	if(!generateUnitList(uList, fileName)) {
		return EXIT_FAILURE;
	}

	double quantity;
	Unit unitFrom, unitTo;
	string s_unitFrom, s_unitTo;

	switch(argc) {
		default: //An unknown number of arguments were passed. Suggest the help file.
			cout << "Incorrect number of arguments, See --help for more information." << endl;
			return EXIT_FAILURE;
		break;
		
		case 2: //Only one argument was passed in. 
			//This should be something like -h. Parse accordingly
			if(argv[1] == "-h" || argv[1] == "--help");	
	
		break;

		case 3: //Two arguments were passed in.
			quantity = atof(argv[1]);
			if(quantity == 0 && !isdigit(argv[1][0])) { //This is a bit of a workaround to see if we are in fact looking at two units with no quantity
				quantity = 1.0;
			}

			//Parse Units
			try{
				s_unitFrom = argv[1];
				unitFrom = unitLookup(uList, argv[1]);
				s_unitTo = argv[2];
				unitTo = unitLookup(uList, argv[2]);
			} catch(string e) {
				cout << "Unit " << e << " could not be found." << endl;
				return EXIT_FAILURE;
			}
		break;

		case 4: //Three arguments were passed in. This is the typical number.
			try{
				quantity = atof(argv[1]);
			} catch (invalid_argument& e) { //If the first argument isn't a number we can't convert. 
				cout << "Invalid Quantity " << argv[1] << endl;
				return EXIT_FAILURE;
			}
			//Parse Units
			try{
				s_unitFrom = argv[2];
				unitFrom = unitLookup(uList, argv[2]);
				s_unitTo = argv[3];
				unitTo = unitLookup(uList, argv[3]);
			} catch(string e) {
				cout << "Unit " << e << " could not be found." << endl;
				return EXIT_FAILURE;
			}
		break;
	}

	try {
		cout << quantity << " " << s_unitFrom << " = " << convert(quantity, unitFrom, unitTo) << " " << s_unitTo << endl;
	} catch (int e) {
		cout << "Cannot convert from " << s_unitFrom << " to " << s_unitTo << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

double convert(double quantity, Unit from, Unit to)
{
	if(from.getMeasurement() != to.getMeasurement()){
		throw 1;
	}
	return quantity * (from.getRatio() / to.getRatio());
}

void showHelp();

string toLowercase(string & s){
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool generateUnitList(UnitList* uList, std::string file)
{
	//Open the file for reading.
	ifstream fin(file.c_str());
	
	if(!fin){
		cout << "Unit Constant File Not Found...ABORT" << endl;
		return false;
	}

	string curLine, measure;
	while(std::getline(fin, curLine)){ //Read the file, line by line
		if(curLine.length() == 0 || curLine.at(0) == '#') {
			continue;
		} else if(curLine.at(0) == '$') {
			//Identify the measurement type
			measure = curLine.substr(1, string::npos);
			continue;
		}

		//Grab the conversion ratio off the end of the string
		size_t pos = 0;
		pos = curLine.find('=');
		double ratio = atof(curLine.substr(pos+1, string::npos).c_str());
		curLine.erase(pos, string::npos); //Get rid of everything after the equals

		//Parse each tag within the string
		string token;
		while ((pos = curLine.find(',')) != string::npos) {
			token = curLine.substr(0, pos);
			//cout << token << "=" << ratio << endl;
			curLine.erase(0, pos + 1);
			
			Unit u(token, measure, ratio);
			
			uList->push_back(u);	//Associate each tag with it's ratio in the List.
		}
	}

	//Close the file.
	fin.close();

	return true;
}

Unit unitLookup(UnitList * uList, string unitTag)
{
	std::list<Unit>::iterator it;
	it = std::find(uList->begin(), uList->end(), unitTag);
	if( it == uList->end()) {
		throw unitTag;
	}
	return *it;	
}
