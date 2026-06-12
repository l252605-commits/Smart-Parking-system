#include<iostream>
#include<string>
#include<cmath>
#include<fstream>
using namespace std;
class ParkingBlock {
private:
	string** slots;
	int* slotsperrow;
	int totalrows;
	float xcoordinate, ycoordinate;
	void clearmemory();
public:
	// default construcor to initialize
	ParkingBlock() : slots(nullptr), slotsperrow(nullptr), totalrows(0),
		xcoordinate(0.0), ycoordinate(0.0) {
	}
	//declare friend function
	friend ostream& operator<<(ostream& out, const ParkingBlock& obj);
	friend istream& operator>>(istream& in, ParkingBlock& obj);
	ParkingBlock(int numrows) {
		totalrows = numrows;
		slots = new string * [numrows];
		slotsperrow = new int[numrows];
		for (int i = 0; i < numrows; i++) {
			slots[i] = nullptr;
			slotsperrow[i] = 0;
		}
		xcoordinate = 0;
		ycoordinate = 0;
	}
	void addrow(int n) {
		int newtotal = totalrows + n;
		string** newslots = new string * [newtotal];
		int* newslotsperrow = new int[newtotal];
		for (int i = 0; i < totalrows; i++) {
			newslots[i] = slots[i];
			newslotsperrow[i] = slotsperrow[i];
		}
		for (int i = totalrows; i < newtotal; i++) {
			newslots[i] = nullptr;
			newslotsperrow[i] = 0;
		}
		delete[] slots;
		delete[] slotsperrow;
		slots = newslots;
		slotsperrow = newslotsperrow;
		totalrows = newtotal;
	}
	void addslots(int row, int n) {
		if (row < 0 || row >= totalrows) {
			cout << "Invalid row index " << endl;
			return;
		}
		string* oldRow = slots[row];
		int oldN = slotsperrow[row];

		slots[row] = new string[n];
		slotsperrow[row] = n;

		// Copy old cars 
		for (int i = 0; i < n; i++) {
			if (oldRow != nullptr && i < oldN) {
				slots[row][i] = oldRow[i];
			}
			else {
				slots[row][i] = "Empty";
			}
		}
		if (oldRow != nullptr) {
			delete[] oldRow;
		}

	}
	//copy constructor
	ParkingBlock(const ParkingBlock& obj) {
		totalrows = obj.totalrows;
		xcoordinate = obj.xcoordinate;
		ycoordinate = obj.ycoordinate;
		if (obj.slots == nullptr)
		{
			slots = nullptr;
			slotsperrow = nullptr;
			return;
		}
		slotsperrow = new int[totalrows];
		slots = new string * [totalrows];
		for (int i = 0; i < totalrows; i++) {
			slotsperrow[i] = obj.slotsperrow[i];
			if (obj.slots[i] != nullptr) {
				slots[i] = new string[slotsperrow[i]];
				for (int j = 0; j < slotsperrow[i]; j++) {
					slots[i][j] = obj.slots[i][j];
				}
			}
			else {
				slots[i] = nullptr;
			}
		}
	}
	//move constructor
	ParkingBlock(ParkingBlock&& obj) {

		totalrows = obj.totalrows;
		xcoordinate = obj.xcoordinate;
		ycoordinate = obj.ycoordinate;
		slotsperrow = obj.slotsperrow;
		slots = obj.slots;
		//deallocate the object
		obj.slots = nullptr;
		obj.slotsperrow = nullptr;
		obj.totalrows = 0;
		obj.xcoordinate = 0;
		obj.ycoordinate = 0;
	}
	//move assignment
	ParkingBlock& operator=(ParkingBlock&& obj) {
		if (this != &obj) {
			clearmemory(); //since the object has already allocated so to deallocate it call the function
			totalrows = obj.totalrows;
			xcoordinate = obj.xcoordinate;
			ycoordinate = obj.ycoordinate;
			slotsperrow = obj.slotsperrow;
			slots = obj.slots;
			//deallocate the object
			obj.slots = nullptr;
			obj.slotsperrow = nullptr;
			obj.totalrows = 0;
			obj.xcoordinate = 0;
			obj.ycoordinate = 0;
		}
		return*this;
	}
	//copy assignment
	ParkingBlock& operator=(const ParkingBlock& obj) {
		if (this == &obj) {
			return*this;
		}
		clearmemory();
		totalrows = obj.totalrows;
		xcoordinate = obj.xcoordinate;
		ycoordinate = obj.ycoordinate;
		if (obj.slots != nullptr) {
			slots = new string * [totalrows];
			slotsperrow = new int[totalrows];
			for (int i = 0; i < totalrows; i++) {
				slotsperrow[i] = obj.slotsperrow[i];
				if (obj.slots[i] != nullptr) {
					slots[i] = new string[slotsperrow[i]];
					for (int j = 0; j < slotsperrow[i]; j++) {
						slots[i][j] = obj.slots[i][j];
					}
				}
				else {
					this->slots[i] = nullptr;
				}
			}
			return*this;
		}
		else {
			slots = nullptr;
			slotsperrow = nullptr;
			totalrows = 0;
		}
		return*this;
	}
	void ParkVehicle(int r, int s, string id) {
		r--;
		s--;
		if (r >= 0 && r < totalrows && s >= 0 && s < slotsperrow[r]) {
			if (slots[r][s] == "Empty") {
				slots[r][s] = id;
				cout << "Vehicle: " << slots[r][s] << " is Parked successfully " << endl;
			}
			else {
				cout << "Slot is aleady occupied by " << slots[r][s] << endl;
			}
		}
		else {
			cout << "Invalid index of row or Slot" << endl;
		}
	}
	//Remove Vehicle
	void removeCar(int r, int s) {
		r--;
		s--;
		if (r >= 0 && r < totalrows && s >= 0 && s < slotsperrow[r]) {
			if (slots[r][s] != "Empty") {
				string id = slots[r][s];
				slots[r][s] = "Empty"; // Reset the slot
				cout << "vehicle: " << id << " is removed successfully " << endl;
			}
			else {
				cout << "The slot is already empty." << endl;
			}
		}
		else {
			cout << "Invalid row or slot index." << endl;
		}

	}
	//search vehicle
	int* searchVehicle(string id) {
		for (int i = 0; i < totalrows; i++) {
			for (int j = 0; j < slotsperrow[i]; j++) {
				if (slots[i][j] == id) {  //check match
					int* result = new int[2]; //if found then alocate for result
					result[0] = i; // Row index
					result[1] = j; // Slot index
					return result;
				}
			}
		}
		return nullptr;
	}
	//display
	void displayBlock(int blockIndex) const { //const means cannot do any change
		cout << "\n----- Block: " << blockIndex + 1 << " -----" << endl;
		cout << "Location: (" << xcoordinate << ", " << ycoordinate << ")" << endl;
		cout << "Total Rows: " << totalrows << endl;
		for (int i = 0; i < totalrows; i++) {
			cout << "Row " << i + 1 << " [" << slotsperrow[i] << " slots]: ";
			if (slots[i] == nullptr) {
				cout << "(Row not initialized)";
			}
			else {
				for (int j = 0; j < slotsperrow[i]; j++) {
					cout << "[" << slots[i][j] << "] ";
				}
			}
			cout << endl;
		}
		cout << "------------------------------" << endl;
	}
	//Resize Array
	void resizeSlots(int row, int newSize) {
		row--;
		if (row < 0 || row >= totalrows) {
			cout << "Invalid row index." << endl;
			return;
		}
		//Create the new row
		string* newRow = new string[newSize];
		//Copy data
		for (int i = 0; i < newSize; i++) {
			if (i < slotsperrow[row]) {
				newRow[i] = slots[row][i];
			}
			else {
				newRow[i] = "Empty";
			}
		}
		delete[] slots[row];
		slots[row] = newRow;
		slotsperrow[row] = newSize;
		cout << "Row " << row + 1 << " successfully resized to " << newSize << " slots." << endl;
	}
	//Empty Row
	void emptyRow(int row) {
		row--;
		if (row < 0 || row >= totalrows) {
			cout << "Error: Invalid row index." << endl;
			return;
		}
		if (slots[row] != nullptr) {
			for (int j = 0; j < slotsperrow[row]; j++) {
				slots[row][j] = "Empty";
			}
			cout << "Row " << row + 1 << " has been cleared. All slots are now Empty." << endl;
		}
		else {
			cout << "Row " << row + 1 << " is already uniinitialized." << endl;
		}
	}
	//empty block
	void emptyBlock() {
		if (slots == nullptr) {
			cout << "Block is already uninitialized." << endl;
			return;
		}

		for (int i = 0; i < totalrows; i++) {
			emptyRow(i + 1);
		}
		cout << "All rows in this block have been reset to Empty." << endl;
	}
	//Transfer Vehicle
	void transferVehicles(ParkingBlock& source) {
		// Calling the assignment operator
		*this = source;
		source.emptyBlock();
		/* copy assignment is the most appropriate appproach here becuse it exactly done all the
		* requiremnts that the function need. it clear previous memory and reallocate according to
		* object which we are passing and also perform deep copy which is requird
		*/
	}
	void setcoordinates(float x, float y) {
		xcoordinate = x, ycoordinate = y;
	}
	float getx() {
		return xcoordinate;
	}
	float gety() {
		return ycoordinate;
	}
	int getrows() {
		return totalrows;
	}
	int getslots(int r) {
		if (r >= 0 && r < totalrows) {
			return slotsperrow[r];
		}
		return 0;
	}
	string getslotstatus(int r, int s) {


		if (r >= 0 && r < totalrows && s >= 0 && s < slotsperrow[r]) {
			return slots[r][s];
		}
		return "invalid";
	}
	//Subscript operator overload
	string* operator[](int idx) {
		if (idx < 0 || idx >= totalrows) {
			cout << "Invalid row index" << endl;
			return nullptr;
		}
		return slots[idx];
	}
	//equal operator overlod
	bool operator==(const ParkingBlock& obj)const {
		if (totalrows != obj.totalrows || xcoordinate != obj.xcoordinate || ycoordinate != obj.ycoordinate) {
			return false;
		}
		for (int i = 0;i < totalrows;i++) {
			if (slotsperrow[i] != obj.slotsperrow[i]) {
				return false;
			}
			for (int j = 0;j < slotsperrow[i];j++) {
				if (slots[i][j] != obj.slots[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	//get occupaid slots
	int getoccupaid() const {
		int count = 0;
		for (int i = 0;i < totalrows;i++) {
			for (int j = 0;j < slotsperrow[i];j++) {
				if (slots[i][j] != "Empty") {
					count++;
				}
			}
		}
		return count;
	}
	//greater and smaller than operator overload
	bool operator>(const ParkingBlock& obj) const {
		return getoccupaid() > obj.getoccupaid();
	}
	bool operator<(const ParkingBlock& obj) const {
		return getoccupaid() < obj.getoccupaid();
	}
	//+ operator overload to merge two blocks
	ParkingBlock operator+(const ParkingBlock& obj) const {
		ParkingBlock res;
		res.totalrows = totalrows + obj.totalrows;
		res.slots = new string * [res.totalrows];
		res.slotsperrow = new int[res.totalrows];
		for (int i = 0;i < totalrows;i++) {
			res.slotsperrow[i] = slotsperrow[i];
			res.slots[i] = new string[slotsperrow[i]];
			for (int j = 0;j < slotsperrow[i];j++) {
				res.slots[i][j] = slots[i][j];
			}
		}
		for (int i = 0;i < obj.totalrows;i++) {
			res.slotsperrow[i + totalrows] = obj.slotsperrow[i];
			res.slots[i + totalrows] = new string[obj.slotsperrow[i]];
			for (int j = 0;j < obj.slotsperrow[i];j++) {
				res.slots[i + totalrows][j] = obj.slots[i][j];
			}
		}
		return res;
	}
	//difference operator
	ParkingBlock operator-(const ParkingBlock& obj) const {
		ParkingBlock res;
		res.totalrows = totalrows;
		res.slots = new string * [res.totalrows];
		res.slotsperrow = new int[res.totalrows];
		for (int i = 0;i < totalrows;i++) {
			res.slotsperrow[i] = slotsperrow[i];
			res.slots[i] = new string[slotsperrow[i]];
		}
		for (int i = 0;i < totalrows;i++) {
			for (int j = 0;j < slotsperrow[i];j++) {
				string currentvehicle = slots[i][j];
				//store empty in res block if current is empty
				if (currentvehicle == "Empty") {
					res.slots[i][j] = "Empty";
				}
				else {
					bool found = false;
					for (int r = 0;r < obj.totalrows;r++) {
						for (int s = 0;s < obj.slotsperrow[r];s++) {
							if (obj.slots[r][s] == currentvehicle) {
								found = true;
								break;
							}
						}
						if (found) {
							break;
						}
					}
					//store 
					if (!found) {
						res.slots[i][j] = slots[i][j];
					}
					else {
						res.slots[i][j] = "Empty";
					}
				}

			}
		}
		return res;
	}
	//add vehicle using operator overloading
	ParkingBlock& operator+=(const string& Vehicleid) {
		float mindist = 1e9;
		int bestslot = -1, bestrow = -1;
		for (int i = 0;i < totalrows;i++) {
			for (int j = 0;j < slotsperrow[i];j++) {
				if (slots[i][j] == "Empty") {
					float slotx = xcoordinate + j * 8;
					float sloty = ycoordinate + i * 16;
					float dist = sqrt((slotx - xcoordinate) * (slotx - xcoordinate) + (sloty - ycoordinate) * (sloty - ycoordinate));
					//update distance
					if (dist < mindist) {
						mindist = dist;
						bestrow = i;
						bestslot = j;
					}
				}
			}
		}
		if (bestrow != -1) {
			slots[bestrow][bestslot] = Vehicleid;
			cout << "Vehicle " << Vehicleid << " smart-parked at Row " << bestrow + 1 << ", Slot " << bestslot + 1 << endl;
		}
		else {
			cout << "No empty slots available!" << endl;
		}
		return *this; // Return reference for chaining
	}
	ParkingBlock& operator-=(const ParkingBlock& obj) {
		for (int i = 0;i < totalrows;i++) {
			for (int j = 0;j < slotsperrow[i];j++) {
				if (slots[i][j] == "Empty") {
					continue;
				}
				else {
					string occupaid = slots[i][j];
					bool found = false;
					for (int r = 0;r < obj.totalrows;r++) {
						for (int s = 0;s < obj.slotsperrow[i];s++) {
							if (obj.slots[r][s] == occupaid) {
								found = true;
								break;
							}
						}
						if (found) {
							break;
						}
					}
					if (found) {
						slots[i][j] = "Empty";
					}
				}
			}
		}
		return *this;
	}
	//remove specific vehicle using operator overloading
	ParkingBlock& operator-=(const string& vehicleid) {
		for (int i = 0;i < totalrows;i++) {
			for (int j = 0;j < slotsperrow[i];j++) {
				if (slots[i][j] == vehicleid) {
					slots[i][j] = "Empty";
					return *this; //return early if found
				}
			}
		}
		cout << "Vehicle not found " << endl;
		return *this;
	}
	//prefix increment
	ParkingBlock operator++() {

		ParkingBlock temp = *this;
		temp.addrow(1);
		int newidx = totalrows - 1; //store after update
		temp.addslots(newidx, 4);
		return temp;
	}
	//postfix increment
	ParkingBlock operator++(int) {
		ParkingBlock temp = *this;
		this->addrow(1);
		int newidx = totalrows - 1; //store after update
		this->addslots(newidx, 4);
		return temp;
	}
	//pre decrement
	ParkingBlock operator--() {
		if (totalrows > 0) {
			// Delete last row
			delete[] slots[totalrows - 1];

			int newtotal = totalrows - 1;
			string** newslots = new string * [newtotal];
			int* newslotsperrow = new int[newtotal];

			for (int i = 0; i < newtotal; i++) {
				newslots[i] = slots[i];
				newslotsperrow[i] = slotsperrow[i];
			}
			//delete old memory
			delete[] slots;
			delete[] slotsperrow;
			// update memory
			slots = newslots;
			slotsperrow = newslotsperrow;
			totalrows = newtotal;
		}
		return *this;
	}
	//post decrement
	ParkingBlock operator--(int) {
		ParkingBlock temp = *this;
		if (totalrows > 0) {
			// Delete last row
			delete[] slots[totalrows - 1];

			int newtotal = totalrows - 1;
			string** newslots = new string * [newtotal];
			int* newslotsperrow = new int[newtotal];

			for (int i = 0; i < newtotal; i++) {
				newslots[i] = slots[i];
				newslotsperrow[i] = slotsperrow[i];
			}
			//delete old memory
			delete[] slots;
			delete[] slotsperrow;
			// update memory
			slots = newslots;
			slotsperrow = newslotsperrow;
			totalrows = newtotal;
		}
		return temp;
	}
	// Multiplication
	ParkingBlock operator*(int n) const {
		if (n <= 0) {
			cout << "Multiplier must be positive." << endl;
			return *this;
		}
		ParkingBlock res(totalrows);
		res.xcoordinate = xcoordinate;
		res.ycoordinate = ycoordinate;
		for (int i = 0; i < totalrows; i++) {
			res.addslots(i, slotsperrow[i] * n);
		}
		return res;
	}

	//Reverse Multiplication:
	friend ParkingBlock operator*(int n, const ParkingBlock& obj) {
		return obj * n;
	}

	//Division
	ParkingBlock operator/(int n) const {
		if (n <= 0) {
			cout << "Divisor must be positive." << endl;
			return *this;
		}
		ParkingBlock res(totalrows);
		res.xcoordinate = this->xcoordinate;
		res.ycoordinate = this->ycoordinate;
		for (int i = 0; i < totalrows; i++) {
			int newSize = this->slotsperrow[i] / n;
			if (newSize == 0) {
				newSize = 1; //atleast one slot explicitly
			}
			res.addslots(i, newSize);
		}
		return res;

	}
	//merge vehicle
	void mergeVehicles(ParkingBlock& source) {
		if (source.totalrows > totalrows) { //check if des has less rows then add more till source
			addrow(source.totalrows - totalrows);
		}
		for (int i = 0; i < source.totalrows; i++) {
			for (int j = 0; j < source.slotsperrow[i]; j++) {  //loop in source slot 
				if (source.slots[i][j] != "Empty") {
					string vehicleToMove = source.slots[i][j];
					bool foundSpot = false; //bool initial false
					for (int k = 0; k < slotsperrow[i]; k++) { // loop in destinetion slop
						if (slots[i][k] == "Empty") {
							slots[i][k] = vehicleToMove;
							foundSpot = true;
							break; // Move to the next source vehicle
						}
					}
					// if no empty spot was found, expand the row
					if (!foundSpot) {
						int currentSize = slotsperrow[i];
						// Expand by 1
						resizeSlots(i, currentSize + 1);
						// Place the vehicle in the newly created last index
						slots[i][currentSize] = vehicleToMove;
					}
				}
			}
		}
		cout << "Merge completed successfully." << endl;
	}
	~ParkingBlock() {
		clearmemory();
	}
};
void ParkingBlock::clearmemory() {
	if (slots != nullptr) {
		for (int i = 0; i < totalrows; i++) {
			if (slots[i] != nullptr) { // Only delete if addslots was called
				delete[] slots[i];
			}
		}
		delete[] slots;
	}
	delete[] slotsperrow;
	slots = nullptr;
	slotsperrow = nullptr;
	totalrows = 0;
}
void loadlayout(ParkingBlock*& lot, int& totalblock, string filename) {
	ifstream fin(filename);
	if (!fin) {
		cout << "File " << filename << " not found " << endl;
		return;
	}
	if (lot != nullptr) {
		delete[] lot;
	}
	fin >> totalblock;
	lot = new ParkingBlock[totalblock];
	for (int i = 0; i < totalblock; i++) {
		int rows;
		fin >> rows;
		lot[i] = ParkingBlock(rows);
		for (int j = 0; j < rows; j++) {
			int slots;
			fin >> slots;
			lot[i].addslots(j, slots);
		}
	}
	fin.close();
}
void smartallocation(ParkingBlock*& lot, int& totalblock, int& desb, string id) {
	int idx = desb - 1;
	int bestblock, bestrow, bestslot;
	double dis = 0.0;
	double mindis = 1e9; //max value
	float x, y;
	for (int i = 0; i < totalblock; i++) {
		cout << "Enter X and Y Coordinates for Block " << i + 1 << endl;
		cin >> x >> y;
		lot[i].setcoordinates(x, y);
	}
	if (idx < 0 || idx >= totalblock) {
		cout << "Invalid Desired Block " << endl;
		return;
	}
	float desx = lot[idx].getx();
	float desy = lot[idx].gety();
	bool found = false;
	for (int i = 0; i < totalblock; i++) {
		float basex = lot[i].getx();
		float basey = lot[i].gety();
		for (int j = 0; j < lot[i].getrows(); j++) {
			for (int k = 0; k < lot[i].getslots(j); k++) {
				if (lot[i].getslotstatus(j, k) == "Empty") {
					float slotX = basex + (k * 8);
					float slotY = basey + (j * 16);
					dis = sqrt((slotX - desx) * (slotX - desx) + (slotY - desy) * (slotY - desy));
					if (dis < mindis) {
						mindis = dis; //update best
						bestblock = i;
						bestrow = j;
						bestslot = k;
						found = true;
					}
				}
			}
		}
		if (found) {
			cout << "Best slot at Block: " << bestblock + 1 << endl;
			cout << "Row: " << bestrow + 1 << endl;
			cout << "Slot: " << bestslot + 1 << endl;
			cout << "Minimum Distance is: " << mindis << endl;
			cout << "Do you want to park the car here then enter 1 " << endl;
			int choice;
			cin >> choice;
			if (choice == 1) {
				int btemp = bestblock + 1; // update bcz it decremnt by 1 in add func
				int rtemp = bestrow + 1;
				int stemp = bestslot + 1;

				lot[bestblock].ParkVehicle(rtemp, stemp, id);
				break;
			}
		}
		else {
			cout << "Parking is full!" << endl;
		}
	}
}
void loadBlockCoordinates(string filename, ParkingBlock*& lot, int& totalBlocks) {
	ifstream fin(filename);
	if (!fin) {
		cout << "Coordinate file not found " << endl;
		return;
	}
	float x, y;
	for (int i = 0; i < totalBlocks; i++) {
		// Read the next two numbers from the file
		if (fin >> x >> y) {
			// call setter function
			lot[i].setcoordinates(x, y);
			// display
			cout << "Block " << i + 1 << " updated: ";
			lot[i].displayBlock(i);
		}
		else {
			cout << "File ended unexpectedly at block " << i + 1 << endl;
			break;
		}
	}
	fin.close();
}
ostream& operator<<(ostream& out, const ParkingBlock& obj) {
	out << "Location: (" << obj.xcoordinate << ", " << obj.ycoordinate << ")" << endl;
	out << "Total Rows: " << obj.totalrows << endl;

	for (int i = 0; i < obj.totalrows; i++) {
		out << "Row " << i + 1 << " [" << obj.slotsperrow[i] << " slots]: ";

		if (obj.slots[i] == nullptr) {
			out << "(Row not initialized)";
		}
		else {
			for (int j = 0; j < obj.slotsperrow[i]; j++) {
				out << "[" << obj.slots[i][j] << "], ";
			}
		}
		out << endl;
	}
	out << "------------------------------" << endl;
	return out;
}
//take input using overloaded operator
istream& operator>>(istream& in, ParkingBlock& obj) {
	cout << "Enter X and Y coordinates for the block: " << endl;
	in >> obj.xcoordinate >> obj.ycoordinate;
	cout << "Enter total number of rows: " << endl;
	in >> obj.totalrows;
	obj.slots = new string * [obj.totalrows];
	obj.slotsperrow = new int[obj.totalrows];
	for (int i = 0;i < obj.totalrows;i++) {
		cout << "Enter number of slots for row " << i + 1 << endl;
		cin >> obj.slotsperrow[i];
		obj.slots[i] = new string[obj.slotsperrow[i]];
		for (int j = 0;j < obj.slotsperrow[i];j++) {
			obj.slots[i][j] = "Empty";
		}
	}
	return in;
}
int main() {
	ParkingBlock* lot = nullptr;
	int totalblock = 0;
	int choice;
	do {
		cout << "Enter 1  to initize the block " << endl;
		cout << "Enter 2  for Park Car " << endl;
		cout << "Enter 3  for Remove Car " << endl;
		cout << "Enter 4  for Search Vehicle " << endl;
		cout << "Enter 5  for display parking stand " << endl;
		cout << "Enter 6  for Resize parking stand " << endl;
		cout << "Enter 7  to  empty a specific row of specific block " << endl;
		cout << "Enter 8  to  empty a specific block by caling emprtrow function " << endl;
		cout << "Enter 9  to transfer vehicle from 1 block to another " << endl;
		cout << "Enter 10 to load Layout of the Lot from the file " << endl;
		cout << "Enter 11 for Smart Allocation " << endl;
		cout << "Enter 12 to load coordinates of blocks from the file " << endl;
		cout << "Enter 13 to merge vehicles from one block to another " << endl;
		cout << "Enter 14 to make one block equal to other or transfer vehicle " << endl;
		cout << "Enter 15 to display the full parking lot using stream operator " << endl;
		cout << "Enter 16 for Input (>>) and Output (<<) operator" << endl;
		cout << "Enter 17 for subscript ([]) operator " << endl;
		cout << "Enter 18 for Equality (==) operator " << endl;
		cout << "Enter 19 for Comparison (>, <) operator " << endl;
		cout << "Enter 20 for Merge (+) operator " << endl;
		cout << "Enter 21 for Set Subtraction (-) operator " << endl;
		cout << "Enter 22 for Smart Park (+=) operator " << endl;
		cout << "Enter 23 for Compound Subtract (-=) operator " << endl;
		cout << "Enter 24 for Increment & decrement (++ / --) operator " << endl;
		cout << "Enter 25 for multiplication with n " << endl;
		cout << "Enter 26 for Division with n " << endl;
		cout << "Enter 27 to exit " << endl;
		cin >> choice;
		if (choice < 0) {
			cout << "Invalid Choice " << endl;
		}
		switch (choice) {
		case 1:
		{
			if (lot != nullptr) {
				delete[] lot;
			}
			cout << "Enter number of Blocks for the Parking Lot: " << endl;
			cin >> totalblock;
			lot = new ParkingBlock[totalblock];
			int rows;
			for (int i = 0; i < totalblock; i++) {
				cout << "Enter number of Rows for Block " << i + 1 << endl;
				cin >> rows;
				lot[i] = ParkingBlock(rows);
				int slots;
				for (int j = 0; j < rows; j++) {
					cout << "Enter number of slots for Block: " << i + 1 << " and Row: " << j + 1 << endl;
					cin >> slots;
					lot[i].addslots(j, slots);
				}

			}
			break;
		}
		case 2:
		{
			if (lot == nullptr || totalblock == 0) {
				cout << "Please Initialise the parking lot first " << endl;
				break;
			}
			else {
				int b, r, s;
				string id;
				cout << "Enter block where you want to park the vehicle " << endl;
				cin >> b;
				b--;
				if (b >= 0 && b < totalblock) {
					cout << "Enter row number of block " << b + 1 << " where you want to park the vehicle" << endl;
					cin >> r;
					cout << "Enter slot of row " << r + 1 << " where you want to park the vehicle" << endl;
					cin >> s;
					cout << "Enter Vehicle id " << endl;
					cin.ignore(1000, '\n');
					getline(cin, id);
					lot[b].ParkVehicle(r, s, id);
				}
				else {
					cout << "Block not exist " << endl;
				}
				break;
			}
		}
		case 3: { // Remove a Vehicle
			if (lot == nullptr || totalblock == 0) {
				cout << "Error: System not initialized." << endl;
				break;
			}
			int b, r, s;
			cout << "Enter Block Number: " << endl;
			cin >> b;
			b--;
			if (b >= 0 && b < totalblock) {
				cout << "Enter Row Number: " << endl;;
				cin >> r;
				cout << "Enter Slot Number: " << endl;
				cin >> s;
				lot[b].removeCar(r, s);
			}
			else {
				cout << "Invalid Block Number." << endl;
			}
			break;
		}
		case 4: { //search
			if (lot == nullptr || totalblock == 0) {
				cout << "Initialize the lot first!" << endl;
				break;
			}
			cout << "Enter Vehicle ID to search: ";
			string searchid;
			cin.ignore(1000, '\n');
			getline(cin, searchid);
			bool foundGlobal = false;
			for (int i = 0; i < totalblock; i++) {
				int* location = lot[i].searchVehicle(searchid);
				if (location != nullptr) {
					cout << "---------Vehicle Found--------- " << endl;
					cout << "Block: " << i + 1 << endl;
					cout << "Row: " << location[0] + 1 << endl;
					cout << "Slot: " << location[1] + 1 << endl;

					delete[] location;  //delete location
					foundGlobal = true;
					break; //stop searching
				}
			}
			if (!foundGlobal) {
				cout << "Vehicle with ID " << searchid << " not found in any block." << endl;
			}
			break;
		}
		case 5:
		{
			if (lot == nullptr || totalblock == 0) {
				cout << "Please initialize Parking lot " << endl;
				break;
			}
			else {
				cout << "===== FULL PARKING LOT STATUS =====" << endl;
				for (int i = 0; i < totalblock; i++) {
					cout << "Block " << i + 1 << ":" << endl;
					cout << lot[i]; // using overloaded operator to display block status
				}
			}
			break;
		}
		case 6: { // Resize Slots in a Row
			if (lot == nullptr || totalblock == 0) {
				cout << "Initialize the lot first " << endl;
				break;
			}
			int b, r, s;
			cout << "Enter Block Number: " << endl;
			cin >> b;
			b--;
			if (b >= 0 && b < totalblock) {
				cout << "Enter Row Number: " << endl;;
				cin >> r;
				cout << "Enter New Number of Slots: " << endl;
				cin >> s;

				lot[b].resizeSlots(r, s);
			}
			else {
				cout << "Invalid Block." << endl;
			}
			break;
		}
		case 7:
		{ // Empty a Row
			if (lot == nullptr || totalblock == 0) {
				cout << "Initialize the lot first!" << endl;
				break;
			}
			int b, r;
			cout << "Enter Block Number: " << endl;
			cin >> b;
			b--;
			if (b >= 0 && b < totalblock) {
				cout << "Enter Row Number to empty: " << endl;
				cin >> r;
				lot[b].emptyRow(r);
			}
			else {
				cout << "Invalid Block." << endl;
			}
			break;
		}
		case 8:
		{
			if (lot == nullptr || totalblock == 0) {
				cout << "Error: System not initialized." << endl;
				break;
			}
			int b;
			cout << "Enter Block Number to empty: ";
			cin >> b;
			b--;
			if (b >= 0 && b < totalblock) {
				lot[b].emptyBlock();
			}
			else {
				cout << "Invalid Block Number." << endl;
			}
			break;
		}
		case 9:
		{
			int sourceidx;
			int destidx;
			cout << "Enter source block " << endl;
			cin >> sourceidx;
			cout << "Enter destinetion block " << endl;
			cin >> destidx;
			if ((sourceidx >= 1 && sourceidx <= totalblock && destidx >= 1 && destidx <= totalblock)) {
				if (sourceidx == destidx) {
					cout << "\nSource and destineion cannot be equal" << endl;
				}
				else {
					sourceidx--;
					destidx--;
					lot[destidx].transferVehicles(lot[sourceidx]);
					cout << "Vehicle transfered successfully " << endl;
				}
			}
			else {
				cout << "Invalid Block index " << endl;
			}
			break;
		}
		case 10:
		{
			string fname;
			cout << "Enter file name " << endl;
			cin.ignore(1000, '\n');
			getline(cin, fname);
			loadlayout(lot, totalblock, fname);
			break;
		}
		case 11:
		{
			if (lot == nullptr || totalblock == 0) {
				cout << "Please Initialise the parking lot first " << endl;
				break;
			}
			else {
				int desb;
				string id;
				cout << "Enter Desired Block where you want to park the vehicle " << endl;
				cin >> desb;
				cout << "Enter Vehicle id " << endl;
				cin.ignore(1000, '\n');
				getline(cin, id);
				smartallocation(lot, totalblock, desb, id);
				break;
			}
		}
		case 12:
		{
			string file;
			cout << "Enter File Name " << endl;
			cin.ignore(1000, '\n');
			getline(cin, file);
			loadBlockCoordinates(file, lot, totalblock);
			break;
		}
		//merge blocks
		case 13:
		{
			int srcidx, destidx;

			cout << "--- Merge Blocks ---" << endl;
			cout << "Enter Source Block Number : ";
			cin >> srcidx;
			cout << "Enter Destination Block Number : ";
			cin >> destidx;
			if (srcidx > 0 && srcidx <= totalblock && destidx > 0 && destidx <= totalblock) {

				if (srcidx == destidx) {
					cout << "Source and Destination cannot be the same block " << endl;
				}
				else {
					lot[destidx - 1].mergeVehicles(lot[srcidx - 1]);

					cout << "Vehicles merged from Block " << srcidx << " into Block " << destidx << "." << endl;
				}
			}
			else {
				cout << "Invalid Block Number. Please enter values between 1 and " << totalblock << "." << endl;
			}
			break;
		}
		//copy assignmant and move
		case 14:
		{
			int ch, src, dest;
			cout << "Enter 1 to copy data of one block to another block " << endl;
			cout << "Enter 2 to transfer data from one block to another block " << endl;
			cin >> ch;
			if (ch == 1) {
				cout << "Enter Source block " << endl;
				cin >> src;
				cout << "Enter Destination block " << endl;
				cin >> dest;
				if (src > 0 && src <= totalblock && dest > 0 && dest <= totalblock) {
					lot[dest - 1] = lot[src - 1];
					cout << "Source and Destination blocks are equal " << endl;
				}
			}
			else if (ch == 2) {
				cout << "Enter Source block " << endl;
				cin >> src;
				cout << "Enter Destination block " << endl;
				cin >> dest;
				if (src > 0 && src <= totalblock && dest > 0 && dest <= totalblock) {
					lot[dest - 1] = std::move(lot[src - 1]);
					cout << "Source and Destination blocks are equal " << endl;
				}
			}
			else {
				cout << "Invalid choice " << endl;
			}
			break;
		}
		//display full lot 
		case 15:
		{
			if (lot == nullptr || totalblock == 0) {
				cout << "Please initialize Parking lot " << endl;
			}
			else {
				cout << "===== FULL PARKING LOT STATUS =====" << endl;
				for (int i = 0; i < totalblock; i++) {
					cout << "--- Block " << i + 1 << " ---" << endl;
					cout << lot[i];
				}
			}
			break;
		}
		//stream input and output operator test
		case 16:
		{
			ParkingBlock b3;
			cin >> b3;
			cout << "=====The Block data is===== " << endl;
			cout << b3 << endl;
			break;
		}
		//subscript operator to access specific row
		case 17:
		{
			if (lot == nullptr) {
				cout << "Parking lot is not initialized " << endl;
			}
			else {
				int b, r;
				cout << "Enter Block number: " << endl;
				cin >> b;
				cout << "Enter Row number: " << endl;
				cin >> r;
				if (b > 0 && b <= totalblock) {
					string* selectedRow = lot[b - 1][r - 1]; //store the returning row
					if (selectedRow != nullptr) {
						cout << "Accessing Row " << r << " of Block " << b << ":" << endl;

						int numSlots = lot[b - 1].getslots(r - 1); //get no of slots of that row
						for (int i = 0; i < numSlots; i++) {
							cout << "  Slot " << i + 1 << ": " << selectedRow[i] << endl;
						}
					}
					else {
						cout << "row is empty." << endl;
					}
				}
				else {
					cout << "Invalid block index " << endl;
				}
				break;
			}
		}
		//Equality check
		case 18:
		{
			int b1, b2;
			cout << "Enter first Block index (1-" << totalblock << "): ";
			cin >> b1;
			cout << "Enter second Block index (1-" << totalblock << "): ";
			cin >> b2;

			if (b1 > 0 && b1 <= totalblock && b2 > 0 && b2 <= totalblock) {
				if (lot[b1 - 1] == lot[b2 - 1]) {
					cout << "The blocks are Equal" << endl;
				}
				else {
					cout << "The blocks are not Equal" << endl;
				}
			}
			else {
				cout << "Invalid block " << endl;
			}
			break;
		}
		//for comparision operator
		case 19:
		{
			int b1, b2;
			cout << "Enter first Block index (1-" << totalblock << "): " << endl;
			cin >> b1;
			cout << "Enter second Block index (1-" << totalblock << "): " << endl;
			cin >> b2;
			if (b1 > 0 && b1 <= totalblock && b2 > 0 && b2 <= totalblock) {
				if (lot[b1 - 1] > lot[b2 - 1]) {
					cout << "The block " << b1 << " is greater than block " << b2 << endl;
				}
				else {
					cout << "The block " << b1 << " is less than block " << b2 << endl;
				}
			}
			else {
				cout << "Invalid block " << endl;
			}
			break;
		}
		//merged the blocks
		case 20:
		{
			int b1, b2;
			cout << "Enter first Block index (1-" << totalblock << "): " << endl;
			cin >> b1;
			cout << "Enter second Block index (1-" << totalblock << "): " << endl;
			cin >> b2;
			if (b1 > 0 && b1 <= totalblock && b2 > 0 && b2 <= totalblock) {
				ParkingBlock b3 = lot[b1 - 1] + lot[b2 - 1];
				cout << "=====The merged Blocked is===== " << endl;
				cout << b3 << endl;
			}
			else {
				cout << "Invalid Block index " << endl;
			}
			break;
		}
		//Differnece of blocks 
		case 21:
		{
			int b1, b2;
			cout << "Enter first Block index (1-" << totalblock << "): " << endl;
			cin >> b1;
			cout << "Enter second Block index (1-" << totalblock << "): " << endl;
			cin >> b2;
			if (b1 > 0 && b1 <= totalblock && b2 > 0 && b2 <= totalblock) {
				ParkingBlock b3 = lot[b1 - 1] - lot[b2 - 1];
				cout << "=====The subtracted Blocked is===== " << endl;
				cout << b3 << endl;
			}
			else {
				cout << "Invalid Block index " << endl;
			}
			break;
		}
		//Smart park vehicle
		case 22:
		{
			int b;
			string vehicleid;
			cout << "Enter block number where you want to park vehicle samrtly " << endl;
			cin >> b;
			cout << "Enter vehicle id which you want to park " << endl;
			cin.ignore(1000, '\n');
			getline(cin, vehicleid);
			if (b > 0 && b <= totalblock) {
				lot[b - 1] += vehicleid;
			}
			else {
				cout << "Invalid Block index " << endl;
			}
			break;
		}
		case 23:
		{
			int b;
			string vehicleid;
			cout << "Enter block number where you want to remove the vehicle " << endl;
			cin >> b;
			cout << "Enter vehicle id which you want to remove " << endl;
			cin.ignore(1000, '\n');
			getline(cin, vehicleid);
			if (b > 0 && b <= totalblock) {
				lot[b - 1] -= vehicleid;
			}
			else {
				cout << "Invalid Block index " << endl;
			}
			break;
		}
		case 24:
		{
			int b;
			cout << "Enter block index (1 to " << totalblock << "): " << endl;
			cin >> b;
			if (b > 0 && b <= totalblock) {
				int choice;
				do {
					cout << "Enter 1 for pre increment " << endl;
					cout << "Enter 2 for post increment " << endl;
					cout << "Enter 3 for pre decrement " << endl;
					cout << "Enter 4 for post decrement " << endl;
					cout << "Enter 5 for exit " << endl;
					cin >> choice;
					switch (choice) {
					case 1:
						++lot[b - 1];
						cout << "Pre-increment performed." << endl;
						break;
					case 2:
						lot[b - 1]++;
						cout << "Post-increment performed." << endl;
						break;
					case 3:
						--lot[b - 1];
						cout << "Pre-decrement performed." << endl;
						break;
					case 4:
						lot[b - 1]--;
						cout << "Post-decrement performed." << endl;
						break;
					case 5:
						cout << "Exiting increment/decrement menu." << endl;
						break;
					default:
						cout << "Invalid choice." << endl;
					}
				} while (choice != 5);
			}
			else {
				cout << "Invalid block index!" << endl;
			}
			break;
		}
		case 25: { // Multiplication
			int b, n;
			cout << "Enter block index and multiplier: \n";
			cin >> b >> n;
			if (b > 0 && b <= totalblock) {
				ParkingBlock res1 = lot[b - 1] * n;
				ParkingBlock res2 = n * lot[b - 1]; // friend operator
				cout << "Block * n result:" << endl << res1;
				cout << "n * Block result:" << endl << res2;
			}
			else {
				cout << "Invalid block " << endl;
			}
			break;
		}
		case 26: { // Division
			int b, n;
			cout << "Enter block index and divisor: \n";
			cin >> b >> n;
			if (b > 0 && b <= totalblock) {
				ParkingBlock res = lot[b - 1] / n;
				cout << "Block / n result:" << endl << res;
			}
			else {
				cout << "Invalid block " << endl;
			}
			break;
		}
		}
	} while (choice != 27);

	return 0;
}
