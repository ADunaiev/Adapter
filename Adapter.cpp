#include<iostream>
using namespace std;

enum System{celsius, fahrenheit };

string print_syst(System s)
{
    if ((int)s == 0)
        return "celsius";
    else if ((int)s == 1)
        return "fahrenheit";
}

struct Temperature
{
    int degree;
    System syst;
public:
    Temperature(int degreeP, System systemP);
    friend ostream& operator<<(ostream& out, const Temperature& object);
    void set_degree(int degreeP);
    void set_syst(System systP);
};
Temperature::Temperature(int degreeP, System systemP):
    degree{degreeP}, syst{systemP}{}
ostream& operator<<(ostream& out, const Temperature& object)
{
    out << object.degree << " " << print_syst(object.syst);
    return out;
}
void Temperature::set_degree(int degreeP)
{
    degree = degreeP;
}
void Temperature::set_syst(System systP)
{
    syst = systP;
}


class Celsius {
protected:
    Temperature* tempCels;
public:
    Celsius(int tempP);
    Celsius();
    virtual ~Celsius() = default;

    virtual Temperature* Celsius_Temp_Request() const;
    void set_temp(int tempP);
};
Celsius::Celsius(int tempP)
{
    tempCels = new Temperature(tempP, celsius);
}
Celsius::Celsius()
{
    tempCels = new Temperature(0, celsius);
}
Temperature* Celsius::Celsius_Temp_Request() const {
    cout << "Celsius: For example temperature is: ";
    return tempCels;
}
void Celsius::set_temp(int tempP)
{
    (* tempCels).set_degree(tempP);
}

class Fahrenheit {
    Temperature* tempFahr;
public:
    Fahrenheit(int degrF);
    Fahrenheit();
    Temperature* Fahrenheit_Temp_Request() const;
};
Fahrenheit::Fahrenheit(int degrF)
{
    tempFahr = new Temperature(degrF, fahrenheit);
}
Fahrenheit::Fahrenheit() : Fahrenheit(0){}
Temperature* Fahrenheit::Fahrenheit_Temp_Request() const {
    cout << "Fahrenheit: The temperature is " << *tempFahr << endl;
    return tempFahr;
}

class Adapter : public Celsius {
private:
    Fahrenheit* fahrenheit_;

public:
    Adapter(Fahrenheit* fahrenheit) : fahrenheit_(fahrenheit) {}
    Temperature* Celsius_Temp_Request() const override {
        Temperature* to_adapt = fahrenheit_->Fahrenheit_Temp_Request();
        int temp1 = ((*to_adapt).degree - 32) * 5 / 9;
        (*to_adapt).set_degree(temp1);
        (*to_adapt).set_syst(celsius);
        cout << "Is the same as ";
        return to_adapt;
    }
};

void ClientCode(const Celsius* target) {
    std::cout << *(target->Celsius_Temp_Request());
}

int main() {
    std::cout << "I can work just fine with the Celsius temp:\n";
    Celsius* target = new Celsius(10);
    ClientCode(target);
    std::cout << "\n\n";
    Fahrenheit* adaptee = new Fahrenheit(23);
    std::cout << "The Fahrenheit class has a different system. See, I don't understand it:\n";
    adaptee->Fahrenheit_Temp_Request();
    std::cout << "\n";
    std::cout << "Celsius: But I can work with it via the Adapter:\n";
    Adapter* adapter = new Adapter(adaptee);
    ClientCode(adapter);
    std::cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;

    return 0;
}