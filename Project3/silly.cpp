#include <iostream>
#include <string>
#include <sstream>
#include <cmath> 
#include <set>
#include <stack> 
#include <utility>
#include <deque>
#include <queue>
#include <vector>
#include <getopt.h>
#include <fstream>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "TableEntry.h"

using namespace std;

bool quiet = false;


class greaterComparison {
public:

	greaterComparison(TableEntry val_in, uint32_t col_in)
		: value(val_in), col(col_in) {}

	bool operator() (vector<TableEntry>& row) {
		return (row[col] > value);
	}
	

private:

	TableEntry value;
	uint32_t col;

};

class lessComparison {
public:
	lessComparison(TableEntry val_in, uint32_t col_in)
		: value(val_in), col(col_in) {}

	bool operator() (vector<TableEntry>& row) {
		return (row[col] < value);
	}

private:

	TableEntry value;
	uint32_t col;

};

class equalComparison {
public:
	equalComparison(TableEntry val_in, uint32_t col_in)
		: value(val_in), col(col_in) {}

	bool operator() (vector<TableEntry>& row) {
		return (row[col] == value);
	}

private:

	TableEntry value;
	uint32_t col;

};

class Table {
public:

	enum class Type { String, Double, Int, Bool };
	enum class Operation { Less, Greater, Equal};

	uint32_t columns = 0;
	uint32_t size = 0;
	string tableName;

	//Actual data in table
	vector<vector<TableEntry>> data;
	vector<string> colNames;
	vector<Type> types;

	unordered_map<TableEntry, vector<size_t>> hash;
	map<TableEntry, vector<size_t>> bst;
	string hashType = "NULL";
	size_t colHashed = 0;

	void deleteWithOp(char op, TableEntry value, uint32_t index) {
		switch (op) {

		case '<': {
			lessComparison comp(value, index);

			data.erase(remove_if(data.begin(), data.end(), comp), data.end());

			break;
		}

		case '>': {
			greaterComparison comp(value, index);
			data.erase(remove_if(data.begin(), data.end(), comp), data.end());

			break;
		}

		case '=': {
			equalComparison comp(value, index);

			data.erase(remove_if(data.begin(), data.end(), comp), data.end());

			break;
		}

		}
	}

	void outputRow(size_t rowIndex, vector<uint32_t> colsToPrint) {

		if (!quiet) {
			for (size_t j = 0; j < colsToPrint.size(); ++j) {
				cout << data[rowIndex][colsToPrint[j]] << " ";
			}
			cout << "\n";
		}

	}

	size_t printWithOp(char op, TableEntry value, uint32_t index, vector<uint32_t> colsToPrint) {

		size_t counter = 0;
		if (colHashed == index && hashType == "bst") {
			for (auto it = bst.begin(); it != bst.end(); ++it) {

				switch (op) {

				case '<': {
					if (it->first < value) {
						//output all rows contained in vector
						for (size_t k = 0; k < it->second.size(); ++k) {
							outputRow(it->second[k], colsToPrint);
						}
						counter += it->second.size();
					}
			
					break;
				}

				case '>': {
					if (it->first > value) {
						for (size_t k = 0; k < it->second.size(); ++k) {
							outputRow(it->second[k], colsToPrint);
						}
						counter += it->second.size();
					}
					break;
				}

				case '=': {
					if (it->first == value) {
						for (size_t k = 0; k < it->second.size(); ++k) {
							outputRow(it->second[k], colsToPrint);
						}
						counter += it->second.size();
					}
					break;
				}

				}
			}
		}
		
		else {
			for (size_t j = 0; j < data.size(); ++j) {
				switch (op) {

				case '<': {

					if (data[j][index] < value) {
						outputRow(j, colsToPrint);
						counter++;
					}

					break;
				}

				case '>': {
					if (data[j][index] > value) {
						outputRow(j, colsToPrint);
						counter++;
					}
					break;
				}

				case '=': {
					if (data[j][index] == value) {
						outputRow(j, colsToPrint);
						counter++;
					}
					break;
				}

				}
			}
		}
		return counter;
	}

	void tablePrintAll(vector<uint32_t> ind) {

			//loop through how many rows of data there are
			for (size_t j = 0; j < this->data.size(); ++j) {
				//loop through each column you want data from
				for (uint32_t k = 0; k < ind.size(); ++k) {
					cout << this->data[j][ind[k]] << " ";
				}
				cout << "\n";
			}
		
			
	}

	size_t tablePrintWhere(vector<uint32_t> indexToPrint, vector<string> names) {
		string colName;
		cin >> colName;
		bool found = false;
		char op;
		Type colType;
		size_t count = 0;
		uint32_t indexToCompare = 0;
		cin >> op;

		for (uint32_t j = 0; j < colNames.size(); ++j) {
			if (colName == colNames[j]) {
				colType = types[j];
				indexToCompare = j;
				found = true;
			}
		}

		if (!found) {
			cout << "Error during PRINT: " << colName <<
				" does not name a column in " << this->tableName << "\n";
			getline(cin, colName);
			return 99999;
		}

		//Print first line
		if (!quiet) {
			for (uint32_t j = 0; j < names.size(); ++j) {
				cout << names[j] << " ";
			}
			cout << "\n";
		}
		switch (colType) {

			case Type::Int: {
				int curVal;
				cin >> curVal;
				TableEntry testVal{ curVal };

				count = printWithOp(op, testVal, indexToCompare, indexToPrint);
				break;
			}

			case Type::Double: {
				double curVal;
				cin >> curVal;
				TableEntry testVal{ curVal };

				count = printWithOp(op, testVal, indexToCompare, indexToPrint);
				break;
			}

			case Type::String: {
				string curVal;
				cin >> curVal;
				TableEntry testVal{ curVal };

				count = printWithOp(op, testVal, indexToCompare, indexToPrint);
				break;
			}

			case Type::Bool: {
				bool curVal;
				cin >> curVal;
				TableEntry testVal{ curVal };

				count = printWithOp(op, testVal, indexToCompare, indexToPrint);
				break;
			}
		}
		return count;
	}

	void tableDelete() {
		string colName;
		cin >> colName >> colName;
		char op;
		Type colType = Type::Bool;
		bool found = false;
		uint32_t index = 0;
		size_t  sizeBefore = data.size();
		cin >> op;

		for (uint32_t j = 0; j < colNames.size(); ++j) {
			if (colName == colNames[j]) {
				colType = types[j];
				index = j;
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Error during DELETE: " << colName << " does not name a column in " << this->tableName << "\n";
			getline(cin, colName);
			return;
		}

			switch (colType) {

				case Type::Int: {
					int curVal;
					cin >> curVal;
					TableEntry testVal{ curVal };

					deleteWithOp(op, testVal, index);
					break;
				}

				case Type::Double: {
					double curVal;
					cin >> curVal;
					TableEntry testVal{ curVal };

					deleteWithOp(op, testVal, index);
					break;
				}

				case Type::String: {
					string curVal;
					cin >> curVal;
					TableEntry testVal{ curVal };

					deleteWithOp(op, testVal, index);
					break;
				}

				case Type::Bool: {
					bool curVal;
					cin >> curVal;
					TableEntry testVal{ curVal };

					deleteWithOp(op, testVal, index);
					break;
				}
			}
			cout << "Deleted " << sizeBefore - data.size() << " rows from " << this->tableName << "\n";
		
	}

	void tablePrint() {
		uint32_t numberCols = 0;
		vector<string> colName;
		vector<uint32_t> index;
		size_t numPrinted = 99999;

		cin >> numberCols;
		bool foundName = false;
		string curName = "";
		string printType = "";


		for (uint32_t j = 0; j < numberCols; ++j) {
			cin >> curName;
			colName.push_back(curName);
			foundName = false;

			//Find indicies of said columns
			for (uint32_t k = 0; k < this->columns; ++k) {
				if (this->colNames[k] == curName) {
					index.push_back(k);
					foundName = true;
					break;
				}
			}
			if (!foundName) {
				cout << "Error during PRINT: " << curName << " does not name a column in " 
					<< this->tableName << "\n";
				getline(cin, curName);
				return;
			}

		}

		cin >> printType;



		if (printType == "ALL") {
			if (!quiet) {
				for (uint32_t j = 0; j < colName.size(); ++j) {
					cout << colName[j] << " ";
				}
				cout << "\n";
				tablePrintAll(index);
			}

			
			numPrinted = data.size();
		}

		if (printType == "WHERE") {
			numPrinted = tablePrintWhere(index, colName);
		}

		if (numPrinted == 99999)
			return;

		cout << "Printed " << numPrinted << " matching rows from " << this->tableName << "\n";
	}

	void generate_new_ind() {

		bst.clear();
		hash.clear();

		if (hashType == "bst") {
			for (size_t j = 0; j < data.size(); ++j) {
				TableEntry curHash(data[j][colHashed]);
				bst[curHash].push_back(j);
			}
		}
		else {
			for (size_t j = 0; j < data.size(); ++j) {
				TableEntry curHash(data[j][colHashed]);
				hash[curHash].push_back(j);
			}
		}
	}

private:

};

class MasterTable {

public:
	void create() {
		string curTable;
		uint32_t numCol;

		cin >> curTable >> numCol;
		Table newTable;


		//Condition to check if table already exists
		if (allTables.find(curTable) != allTables.end()) {
			cout << "Error during CREATE: Cannot create already existing table " << curTable << "\n";
			getline(cin, curTable);
			return;
		}

		newTable.tableName = curTable;

		for (uint32_t j = 0; j < numCol; ++j) {
			//assign colTypes
			string curType;
			cin >> curType;

			if (curType == "string")
				newTable.types.push_back(Table::Type::String);
			else if (curType == "double")
				newTable.types.push_back(Table::Type::Double);
			else if (curType == "int")
				newTable.types.push_back(Table::Type::Int);
			else if (curType == "bool")
				newTable.types.push_back(Table::Type::Bool);
		}

		cout << "New table " << curTable << " with column(s)";

		for (uint32_t j = 0; j < numCol; ++j) {
			//assign colNames
			string curName;
			cin >> curName;

			newTable.colNames.push_back(curName);

			cout << " " << curName;
		}

		cout << " created" << "\n";

		newTable.data.reserve(numCol);
		newTable.columns = numCol;

		allTables.emplace(curTable, newTable);


	}

	void remove() {
		string tableName;
		cin >> tableName;

		auto it = allTables.find(tableName);

		if (it == allTables.end()) {
			cout << "Error during REMOVE: " << tableName <<
				 " does not name a table in the database\n";
			getline(cin, tableName);
		}
		else {
			allTables.erase(it);
			cout << "Table " << tableName << " " << "deleted\n";
		}

	}

	void insert() {
		string tableName;
		cin >> tableName >> tableName;
		size_t numberNewRows;
		size_t sizeBeforeChange;
		string junk;

		cin >> numberNewRows;
		cin >> junk;

		//If table name doesn't exist, throw error, clear out rest of insert,
		if (allTables.find(tableName) == allTables.end()) {

			cout << "Error during INSERT: " << tableName << " does not name a table in the database\n";

			getline(cin, tableName);
			return;
		}

		sizeBeforeChange = allTables[tableName].data.size();


		//Allocate a new number of rows that can fit
		allTables[tableName].data.reserve(sizeBeforeChange + numberNewRows);


		for (size_t j = sizeBeforeChange; j < sizeBeforeChange + numberNewRows; ++j) {

			//Allocate the number of required columns for each row
			//allTables[tableName].data[j].reserve(allTables[tableName].columns);
			vector<TableEntry> testVector;
			testVector.reserve(allTables[tableName].columns);

			for (size_t k = 0; k < allTables[tableName].columns; ++k) {
				
				Table::Type curType = allTables[tableName].types[k];

				//Switch case for each possible type the value could be
				switch (curType) {
					case(Table::Type::String):
					{
						string newVal;
						cin >> newVal;
						TableEntry val2{ newVal };

						testVector.emplace_back(val2);
						//allTables[tableName].data[j].emplace_back(newVal);

						break;
					}

					case(Table::Type::Double):
					{
						double newVal;
						cin >> newVal;

						TableEntry val2{ newVal };

						testVector.emplace_back(val2);

						break;
					}

					case(Table::Type::Int):
					{
						int newVal;
						cin >> newVal;
						TableEntry val2{ newVal };

						testVector.emplace_back(val2);

						break;
					}

					case(Table::Type::Bool):
					{
						bool newVal;
						cin >> newVal;

						testVector.emplace_back(newVal);

						break;
					}
				}
			}
			allTables[tableName].data.emplace_back(testVector);

		}

		if(allTables[tableName].hashType != "NULL")
			allTables[tableName].generate_new_ind();
		
		cout << "Added " << numberNewRows << " rows to " << tableName << " from position "
			<< sizeBeforeChange << " to " << sizeBeforeChange + numberNewRows - 1 << "\n";
		 
	}

	void print() {
		string tableName = "";
		cin >> tableName >> tableName;
		if (allTables.find(tableName) == allTables.end()) {
			cout << "Error during PRINT: " << tableName << " does not name a table in the database\n";
			getline(cin, tableName);
			return;
		}

		allTables[tableName].tablePrint();

	}

	void deletes() {
		string tableName;
		cin >> tableName >> tableName;

		if (allTables.find(tableName) == allTables.end()) {
			cout << "Error during DELETE: " << tableName <<
				" does not name a table in the database\n";
			getline(cin, tableName);
			return;
		}

		allTables[tableName].tableDelete();
		
		if (allTables[tableName].hashType != "NULL")
			allTables[tableName].generate_new_ind();

	}

	void join() {
		string junk, curString;
		string firstTable, secondTable;
		string col1, col2;
		size_t numberToPrint, curIndex, indexTable1, indexTable2;
		vector<string> namesToPrint;
		vector<size_t> indexToPrint;
		vector<size_t> tableToPrint;
		size_t count = 0;

		//Get input values and names/indices to print
		// tablename1 AND tablename2
		cin >> firstTable >> junk >> secondTable;

		if (allTables.find(firstTable) == allTables.end()) {
			cout << "Error during JOIN: " << firstTable <<
				" does not name a table in the database\n";
			getline(cin, firstTable);
			return;
		}

		if (allTables.find(secondTable) == allTables.end()) {
			cout << "Error during JOIN: " << secondTable <<
				" does not name a table in the database\n";
			getline(cin, secondTable);
			return;
		}

		//WHERE colname1 = colname2
		cin >> junk >> col1 >> junk >> col2;
		//AND PRINT n
		cin >> junk >> junk >> numberToPrint;

		bool found = false;
		//Find the indices of the columns that need to be printed
		for (size_t j = 0; j < allTables[firstTable].columns; ++j) {
			if (allTables[firstTable].colNames[j] == col1) {
				indexTable1 = j;
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Error during JOIN: " << col1 << " does not name a column in " << firstTable << "\n";
			getline(cin, firstTable);
			return;
		}

		found = false;
		for (size_t j = 0; j < allTables[secondTable].columns; ++j) {
			if (allTables[secondTable].colNames[j] == col2) {
				indexTable2 = j;
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Error during JOIN: " << col2 << " does not name a column in " << secondTable << "\n";
			getline(cin, secondTable);
			return;
		}

		namesToPrint.reserve(numberToPrint);
		tableToPrint.reserve(numberToPrint);
		for (uint32_t j = 0; j < numberToPrint; ++j) {

			cin >> curString;
			namesToPrint.emplace_back(curString);
			cin >> curIndex;
			tableToPrint.emplace_back(curIndex);

			if (curIndex == 1) {
				auto it = find(allTables[firstTable].colNames.begin(), allTables[firstTable].colNames.end(), curString);

				//If it is in the table
				if (it != allTables[firstTable].colNames.end()) {
					indexToPrint.push_back(it - allTables[firstTable].colNames.begin());
				}
				else {
					cout << "Error during JOIN: " << curString << " does not name a column in " << firstTable << "\n";
					getline(cin, firstTable);
					return;
				}
			}
			else if (curIndex == 2) {
				auto it = find(allTables[secondTable].colNames.begin(), allTables[secondTable].colNames.end(), curString);
				//If it is in the table
				if (it != allTables[secondTable].colNames.end()) {
					indexToPrint.push_back(it - allTables[secondTable].colNames.begin());
				}
				else {
					cout << "Error during JOIN: " << curString << " does not name a column in " << secondTable << "\n";
					getline(cin, secondTable);
					return;
				}
			}
		}

		
		
		string col1CurName, col2CurName;

		//First line of output
		if (!quiet) {
			for (uint32_t j = 0; j < numberToPrint; ++j) {
				cout << namesToPrint[j] << " ";
			}
			cout << "\n";
		}

		unordered_map<TableEntry, vector<int>> temporaryMap;
		

		//Need to make an index if there isn't one already
		//Goes through each row, as you find a value, add the index of what row it was to
		//the hash of that vlaue
		for (size_t j = 0; j < allTables[secondTable].data.size(); ++j) {
			temporaryMap[allTables[secondTable].data[j][indexTable2]].emplace_back(j);
		}
		
		//For every row in the first table
		for (uint32_t j = 0; j < allTables[firstTable].data.size(); ++j) {
			TableEntry curHash(allTables[firstTable].data[j][indexTable1]);

			if (!quiet) {
				for (size_t k = 0; k < temporaryMap[curHash].size(); ++k) {
					//Output row
					uint32_t tempAddress;

					for (size_t i = 0; i < numberToPrint; ++i) {
						if (tableToPrint[i] == 1) {
							cout << allTables[firstTable].data[j][indexToPrint[i]] << " ";
						}
						else if (tableToPrint[i] == 2) {
							//cout << "IND 2 ";
							tempAddress = temporaryMap[curHash][k];
							cout << allTables[secondTable].data[tempAddress][indexToPrint[i]] << " ";
						}
					}
					cout << "\n";
				}
			}
			count += temporaryMap[curHash].size();
		}

		cout << "Printed " << count << " rows from joining " << firstTable << " to " << secondTable << "\n";
		

	}

	void generate() {
		string junk, tableName, indType, colName;
		size_t colIndex;
		cin >> junk >> tableName >> indType >> junk >> junk >> colName;
		

		//Error checks if table actually exists
		if(allTables.find(tableName) == allTables.end()) {
			cout << "Error during GENERATE: " << tableName <<
				" does not name a table in the database\n";
				getline(cin, tableName);
				return;
		}

		//Error checks if column exists, and finds index of it if it does
		auto it = find(allTables[tableName].colNames.begin(), allTables[tableName].colNames.end(), colName);
		if (it == allTables[tableName].colNames.end()) {
			cout << "Error during GENERATE: " << colName << " does not name a column in " << tableName << "\n";
			getline(cin, tableName);
			return;
		}

		//Clears indices if they already exist
		if (allTables[tableName].hashType != "NULL") {
			allTables[tableName].bst.clear();
			allTables[tableName].hash.clear();
		}

		colIndex = it - allTables[tableName].colNames.begin();
		allTables[tableName].colHashed = colIndex;
		//Loop over current table for every row
		//Loop for bst tree
		//Make 2 seperate for loops to only check condition once, more lines but more efficient
		if (indType == "bst") {
			allTables[tableName].hashType = "bst";
			for (size_t j = 0; j < allTables[tableName].data.size(); ++j) {
				TableEntry curHash(allTables[tableName].data[j][colIndex]);
				allTables[tableName].bst[curHash].push_back(j);
			}
			cout << "Created bst index for table " << tableName << " on column " << colName << "\n";
		}
		else {
			allTables[tableName].hashType = "hash";
			for (size_t j = 0; j < allTables[tableName].data.size(); ++j) {
				TableEntry curHash(allTables[tableName].data[j][colIndex]);
				allTables[tableName].hash[curHash].push_back(j);
			}
			cout << "Created hash index for table " << tableName << " on column " << colName << "\n";
		}

	}

private:
	//The main table of tables
	unordered_map<string, Table> allTables;
	int numberOfTables = 0;

};

MasterTable mainTable;

void getOpts(int argc, char* argv[]) {
	// bool modeSpecified = false;

	opterr = false;
	int choice;
	int option_index = 0;
	option long_options[] = {
		{ "quiet", no_argument,  nullptr, 'q'},
		{ "help",  no_argument,  nullptr, 'h'},
		{ nullptr, 0, nullptr, '\0'}
	};

	//  Fill in the double quotes, to match the mode and help options.
	while ((choice = getopt_long(argc, argv, "qh",
		long_options, &option_index)) != -1) {

		switch (choice) {
		case 'q':
			quiet = true;
			break;

		case 'h':
			cout << "This program recreates SQL in a simplified version called SillyQl\n";
			exit(0);

		default:
			cerr << "Unkown command line option";
			exit(1);
		}
	}
}

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	cin >> std::boolalpha;
	cout << std::boolalpha;

	getOpts(argc, argv);
	//cout << quiet << "\n";
	string cmd;
	string junk;
	do {
		cout << "% ";
		cin >> cmd;
		if (cmd[0] == '#')
			getline(cin, cmd);

		else if (cmd == "CREATE")
			mainTable.create();

		else if (cmd == "QUIT") {
			cout << "Thanks for being silly!\n";
		}

		else if (cmd == "REMOVE")
			mainTable.remove();

		else if (cmd == "INSERT")
			mainTable.insert();

		else if (cmd == "PRINT")
			mainTable.print();

		else if (cmd == "DELETE")
			mainTable.deletes();

		else if (cmd == "JOIN")
			mainTable.join();

		else if (cmd == "GENERATE")
			mainTable.generate();

		else {
			cout << "Error: unrecognized command\n";
			getline(cin, junk);
		}

		//Process command
	} while (cmd != "QUIT");

	//May need to manually clean memory here, not sure

}