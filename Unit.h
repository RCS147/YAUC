#include <string> 
using namespace std;

class Unit {
	public:
		Unit(string n = "", string m = "", double r = 0) {
			name = n;
			measurement = m;
			ratio = r;
		}

		Unit(const Unit &t){
			name = t.getName();
			measurement = t.getMeasurement();
			ratio = t.getRatio();
		}

		~Unit() {}

		string getName() const { return name; }
		string getMeasurement() const { return measurement; }
		double getRatio() const { return ratio; }

		inline bool operator==(const Unit& rhs) { return this->name == rhs.name; }
		inline bool operator!=(const Unit& rhs) { return !(*this == rhs); }

		Unit& operator=(const Unit & arg) {
			if(this == &arg) {
				return *this;
			}
	
			this->name = arg.getName();
			this->measurement = arg.getMeasurement();
			this->ratio = arg.getRatio();
		
			return *this;
		}

	private:
		string name;
		string measurement;
		double ratio;

};
