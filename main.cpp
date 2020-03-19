#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct MonthError{
    string err;
};

struct DayError{
    string err;
};

struct DateError{
    string err;
};


class Date {
    int year;
    int month;
    int day;
public:
    
    Date(int Year, int Month, int Day){
        year = Year;
        month = Month;
        day = Day;
    }
    
    int GetYear() const{
        return year;
    }
    int GetMonth() const{
        return month;
    }
    int GetDay() const{
        return day;
    }
    
};

bool operator< (const Date& lhs, const Date& rhs){
    if(lhs.GetYear() < rhs.GetYear()){
        return true;
    } else if(lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth()){
        return true;
    } else if(lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() < rhs.GetDay()){
        return true;
    }else {
        return false;
    }
}


void EnsureNextSymbolAndSkip(stringstream& stream) {
    if (stream.peek() != '-') {
        DateError a;
        a.err = stream.str();
        throw a;
    }
    stream.ignore(1);
}

Date ParseDate(const string& s){
    stringstream stream(s);
    int year, month, day;

    
    stream >> year;
    EnsureNextSymbolAndSkip(stream);
    stream >> month;
    EnsureNextSymbolAndSkip(stream);
    stream >> day;
   
    if(!stream.eof() || stream.fail()) {
           DateError a;
           a.err = stream.str();
           throw a;
       }
    
    if(month < 1 || month > 12){
        MonthError a;
        a.err = to_string(month);
        throw a;
    }
    if(day < 1 || day > 31){
        DayError a;
        a.err = to_string(day);
        throw a;
    }
    
    return Date(year, month, day);
}

class Database {
    map< Date, vector<string> > DB;
public:
    
    void AddEvent(const Date& date, const string& event){
        int flag = 0;
        for(const string n : DB[date]){
            if(n == event) flag = 1;
        }
        if(!flag){
            DB[date].push_back(event);
            sort(DB[date].begin(), DB[date].end());
        }
    }
    
    bool DeleteEvent(const Date& date, const string& event){
        int num=0;
        int flag = 0;
        for(const string n : DB[date]){
            num++;
            if(n == event){
                flag = 1;
                break;
            }
        }
        if(flag){
            DB[date].erase(DB[date].begin() + num - 1);
        }
        return flag;
        
    }
        
    int DeleteDate(const Date& date){
        int size = 0;
        for(const string n : DB[date]){
            size++;
        }
        DB[date].clear();
        return size;
    }

    void Find(const Date& date) const{
        for(const auto n : DB.at(date)){
            cout << n << endl;
        }
    }
  
    void Print() const{
        for(const auto m : DB){
            for(const string n : m.second){
                cout << setw(4) << setfill('0') << m.first.GetYear() << "-";
                cout << setw(2) << setfill('0') << m.first.GetMonth() << "-";
                cout << setw(2) << setfill('0') << m.first.GetDay() << " ";
                cout << n << endl;
            }
        }
    }
};

int main() {
    Database db;
    string command;
    
    while (getline(cin, command)){
        string s1, s2, s3;
        stringstream stream(command);
        stream >> s1 >> s2 >> s3;
        
        try{
            if(s1.empty()) ignore;
            else if(s1 == "Add") db.AddEvent(ParseDate(s2), s3);
            else if(s1 == "Del"){
                if(s3.empty()){
                    int r = db.DeleteDate(ParseDate(s2));
                    cout << "Deleted " << r << " events" << endl;}
                else{
                    if(db.DeleteEvent(ParseDate(s2), s3))
                        cout << "Deleted successfully" << endl;
                    else cout << "Event not found" << endl;
                }
            }
            else if(s1 == "Find") db.Find(ParseDate(s2));
            else if(s1 == "Print") db.Print();
            else cout << "Unknown command: " << s1 << endl;
            
        } catch(DateError& a){
            cout << "Wrong date format: " << a.err << endl;
//            return 0;
        } catch(MonthError& a){
            cout << "Month value is invalid: " << a.err << endl;
//            return 0;
        } catch(DayError& a){
            cout << "Day value is invalid: " << a.err << endl;
//            return 0;
        } catch(exception){
//            return 0;
        }
    }

    return 0;
}

