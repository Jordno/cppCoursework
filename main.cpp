#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Person{
protected:
	string forename, surname, email;

public:
	Person(){}
	Person(string s, string f) :surname(s), forename(f){}

	string get_surname() const{
		return surname;
	}

	string get_forename() const{
		return forename;
	}

	virtual bool has_telephone() const{ return false; }

	virtual bool has_email() const { return false; }

	virtual string get_email() const { return ""; };
	virtual string get_telephone() const { return ""; };

	virtual void type() const {
		cout << "person" << endl;
	};

};

class Person_with_telephone : public virtual Person{
protected:
	string telephone_number;

public:
	Person_with_telephone(){}
	Person_with_telephone(string s, string f, string t) : Person(s, f), telephone_number(t){}

	bool has_telephone() const {
		return !telephone_number.empty();
	}

	virtual string get_telephone() const {
		return telephone_number;
	}

	void set_telephone(string &t){
		telephone_number = t;
	}

	virtual void type() const {
		cout << "person w/ t" << endl;
	};
};

class Person_with_email : public virtual Person{
protected:
	string email;

public:
	Person_with_email(){}
	Person_with_email(string s, string f, string e) :Person(s, f), email(e){}

	bool has_email() const {
		return !email.empty();
	}

	virtual string get_email() const {
		return email;
	}

	void set_email(string &e){
		email = e;
	}

	virtual void type() const {
		cout << "person w/ e" << endl;
	};
};

class Person_with_telephone_email : public Person_with_telephone, public Person_with_email{
public:
	Person_with_telephone_email(){}
	Person_with_telephone_email(string s, string f, string t, string e) : Person(s, f), Person_with_telephone(s, f, t), Person_with_email(s, f, e) {}

	virtual void type() const {
		cout << "person w/ t + e" << endl;
	};

};

ostream& operator<<(ostream &os, Person &p){
	if (p.has_email() && p.has_telephone()){
		cout << "<person" << " S " << p.get_surname() << " F " << p.get_forename() << " E " << p.get_email() << " T " << p.get_telephone() << " >";
	}
	else if (p.has_email()){
		cout << "<person" << " S " << p.get_surname() << " F " << p.get_forename() << " E " << p.get_email() << " >";
	}
	else if (p.has_telephone()){
		cout << "<person" << " S " << p.get_surname() << " F " << p.get_forename() << " T " << p.get_telephone() << " >";
	}
	else {
		cout << "<person" << " S " << p.get_surname() << " F " << p.get_forename() << " >";
	}
	return os;
}

istream& operator>>(istream &is, Person &p){
	string opening, s, surname, f, forename, closing;

	if (
		(is >> opening >> s >> surname >> f >> forename >> closing)
		&&
		((opening == "<person") && (s == "S") && (f == "F") && (closing == ">"))
		)
	{
		p = Person(surname, forename);
	}
	return is;
}

istream& operator>>(istream &is, Person_with_email &p){
	string opening, s, surname, f, forename, e, email, closing;

	if (
		(is >> opening >> s >> surname >> f >> forename >> e >> email >> closing) &&
		((opening == "<person") && (s == "S") && (f == "F") && (e == "E") && (closing == ">"))){
		p = Person_with_email(surname, forename, email);
	}
	return is;
}

istream& operator>>(istream &is, Person_with_telephone &p){
	string opening, s, surname, f, forename, t, telephone, closing;
	if (
		(is >> opening >> s >> surname >> f >> forename >> t >> telephone >> closing) &&
		((opening == "<person") &&
		(s == "S") &&
		(f == "F") &&
		(t == "T") &&
		(closing == ">"))
		)
	{
		p = Person_with_telephone(surname, forename, telephone);
	}
	return is;
}

istream& operator>>(istream &is, Person_with_telephone_email &p){
	string opening, s, surname, f, forename, t, telephone, e, email, closing;

	if (
		(is >> opening >> s >> surname >> f >> forename >> t >> telephone >> e >> email >> closing) &&
		((opening == "<person") &&
		(s == "S") &&
		(f == "F") &&
		(t == "T") &&
		(e == "E") &&
		(closing == ">"))
		)
	{
		p = Person_with_telephone_email(surname, forename, telephone, email);
	}
	return is;
}

Person* make_person(istream &stream){
	Person *person_pointer = new Person;
	stream >> *person_pointer;
	return person_pointer;
}

Person_with_email* make_person_email(istream &stream){
	Person_with_email *person_pointer = new Person_with_email;
	stream >> *person_pointer;
	return person_pointer;
}

Person_with_telephone* make_person_telephone(istream &stream){
	Person_with_telephone *person_pointer = new Person_with_telephone;
	stream >> *person_pointer;
	return person_pointer;
}

Person_with_telephone_email* make_person_telephone_email(istream &stream){
	Person_with_telephone_email *person_pointer = new Person_with_telephone_email;
	stream >> *person_pointer;
	return person_pointer;
}

istream & read_person(istream &in, Person * & p){
	string input;
	getline(in, input);
	stringstream stream(input);
	string e = " E ";
	string t = " T ";
	bool has_e = false;
	bool has_t = false;

	if (input.find(e) != std::string::npos){ has_e = true; }
	if (input.find(t) != string::npos){ has_t = true; }

	if (has_e && has_t) { 
		cout << "e + t\n";
		Person_with_telephone_email *person_pointer = make_person_telephone_email(stream);
		p = person_pointer;
	}
	else if (has_e){
		cout << "e\n";
		Person_with_email *person_pointer = make_person_email(stream);
		p = person_pointer;
	}
	else if (has_t){ 
		cout << "t \n";
		Person_with_telephone *person_pointer = make_person_telephone(stream);
		p = person_pointer;
	}
	else{ 
		cout << "is person \n";
		Person *person_pointer = make_person(stream);
		p = person_pointer;
	}
	return in;
}

template <typename Contact> class Contacts {
	
	int num_contacts; //the number of contacts stored in the array
	int array_size;   //the size of the array that will be holding the contacts
	Contact *array; 

public:
	Contacts() : num_contacts(0), array_size(1), array(new Contact[array_size]) {}

	int size() const { return num_contacts; } //return the number of contacts stored

	Contact & operator[](int i) { return array[i]; }

	void pop_back() { num_contacts--; } //change the count so that the next new contact overwrites a 'deleted' one

	void push_back(const Contact & x) {
		
		if (num_contacts == array_size) { //if the array is full, then increase the capacity
			
			array_size *= 2; 							  //double the size of the array
			Contact *new_array = new Contact[array_size]; //create an array of double size of the original
	
			for (int i = 0; i < num_contacts; ++i) {
				new_array[i] = array[i]; //copy the original array in to the new one
			}
	
			delete[] array;    //destroy the original array
			array = new_array; //store the new array in the original pointer
		
		}
		
		array[num_contacts] = x; //put the new contact at the end of the existing contacts
		num_contacts++;          //add to the number of objects
	
	}

};

int main(){

	Person *p = 0;

	string ss = "<person S Smith F Tom >\n<person S Smith F Dick T +49.921.1434 >";
	stringstream iss(ss);
	while (read_person(iss, p) && p)
		cout << *p << endl;


}