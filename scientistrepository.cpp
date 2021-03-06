#include "scientistrepository.h"

const char delim  = ';';
const char* DATABASE = "database.txt";

//Constructor that makes a new vector of Scientists and reads into it from a file
ScientistRepository::ScientistRepository(){
    scientistVector = vector<Scientist>();
    read();
}
//Writes one instance of scientist to a file
void ScientistRepository::write(Scientist s){
    ofstream write;
    write.open(DATABASE, ios::out | ios::app);
    write << s.firstName << delim
          << s.lastName << delim
          << s.gender << delim
          << s.birthdate << delim
          << s.deathdate << delim
          << s.nationality << endl;
    write.close();
}
//Overwrites the database file with the vector
void ScientistRepository::save(){
    ofstream write;
    write.open(DATABASE, ios::out);

    for(unsigned int i = 0; i < scientistVector.size() ; i++){
        write << scientistVector[i].firstName << delim
              << scientistVector[i].lastName << delim
              << scientistVector[i].gender << delim
              << scientistVector[i].birthdate << delim
              << scientistVector[i].deathdate << delim
              << scientistVector[i].nationality << endl;
    }

    write.close();

}
//Adds an instance of scientist to the vector and writes it to a file
void ScientistRepository::add(Scientist s){
    scientistVector.push_back(s);
    write(s);
}


void ScientistRepository::update(Scientist &s, Scientist &replace){
    //Searches for the name and removes it from the vector.
    for(unsigned int i = 0; i < scientistVector.size() ; i++){
        if(scientistVector[i] == s){
            scientistVector[i] = replace;
            break;
        }
    }

    //Overwrites the database.txt with the new vector.
    save();
}

//Removes one instance of scientist from the vector
void ScientistRepository::remove(Scientist s){

    //Searches for the name and removes it from the vector.
    for(unsigned int i = 0; i < scientistVector.size() ; i++){
        if(scientistVector[i] == s){
            scientistVector.erase(scientistVector.begin() + i);
            break;
        }
    }

    //Overwrites the database.txt with the new vector.
    save();
}

//Reads all scientist from a file
void ScientistRepository::read(){
    ifstream read;
    read.open("database.txt");
    string s;

    while(read >> s){
        scientistVector.push_back(Scientist::fromString(s,delim));
    }
    read.close();
}

//Sorts Scientists by selected field and order
vector<Scientist> ScientistRepository::list(ScientistSort::Field field, ScientistSort::Order order){
    vector<Scientist> ret(scientistVector);
    // SELECT * FROM scientists ORDER BY field,order
    auto cmp = ScientistSort::Comparer(field, order);
    stable_sort(ret.begin(), ret.end(), cmp);
    return ret;
}

//Searches for default amount of Scientists (1)
vector<Scientist> ScientistRepository::search(ScientistSort::Field field, bool fuzzy, string query){
    return search(field, fuzzy, 1, query);
}

//Searches for Scientists after the parameters selected
//If fuzzy is true then it finds everything that is within 2 Levenshtein distance from the original query
vector<Scientist> ScientistRepository::search(ScientistSort::Field field, bool fuzzy, size_t rows, string query){

    vector<Scientist> ret;

    for(auto it = scientistVector.begin(); it != scientistVector.end(); it++){
        switch(field){

            case ScientistSort::FIRST_NAME:
                if(fuzzy && levenshtein_distance<string>((*it).firstName,query) < 3)
                    ret.push_back((*it));
                else if((*it).firstName == query)
                    ret.push_back((*it));
                break;

            case ScientistSort::LAST_NAME:
                if(fuzzy && levenshtein_distance<string>((*it).lastName,query) < 3)
                    ret.push_back((*it));
                else if((*it).lastName == query)
                    ret.push_back((*it));
                break;

            case ScientistSort::GENDER:
                if((*it).gender == query[0])
                    ret.push_back((*it));
                break;

            case ScientistSort::BIRTH_DATE:
                if(fuzzy && levenshtein_distance<string>((*it).birthdate.toDateString(),query) < 3)
                    ret.push_back((*it));
                else if((*it).birthdate == Date::fromString(query))
                    ret.push_back((*it));
                break;

            case ScientistSort::DEATH_DATE:
                if(fuzzy && levenshtein_distance<string>((*it).deathdate.toDateString(),query) < 3)
                    ret.push_back((*it));
                else if((*it).deathdate == Date::fromString(query))
                    ret.push_back((*it));
                break;

            case ScientistSort::NATIONALITY:
                if(fuzzy && levenshtein_distance<string>((*it).nationality,query) < 3)
                    ret.push_back((*it));
                else if((*it).nationality == query)
                    ret.push_back((*it));
                break;

            default:
                if((*it).firstName == query)
                    ret.push_back((*it));
                break;
        }
        if(ret.size() > rows)
            break;
    }
    return ret;
}

