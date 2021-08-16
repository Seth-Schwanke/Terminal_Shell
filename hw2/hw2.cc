#include <iostream>
#include <string>
#include <cctype>
#include <sys/types.h>	
#include <unistd.h>
#include <cstdlib>	
#include <wait.h>	
#include <vector>	
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
string program_name; 

//Code provided by Jack Applin CS253 CSU spring 2021
bool execute(vector<string> command) {
    // Convert vector of C++ strings to an array of C strings for exectp():
    vector<char *> argv;		// vector of C string pointers
    for (auto &v : command)		// for each C++ string:
	argv.push_back(v.data());	//    add address of C-string inside it
    argv.push_back(nullptr);		// argv ends with a null pointer

    // Duplicate ourself: -1:error, 0:child, >0:parent
    const auto pid = fork();		// step #1: copy ourself
    if (pid < 0)
	return false;			// fork failure?
    if (pid == 0) {			// child?
	execvp(argv[0], argv.data());	// step #2: replace child with new prog
	exit(0xff);			// only got here if execvp() failed.
    }
    int status;
    wait(&status);			// step #3: wait for child to finish
    return status != 0xff00;		// true iff we succeeded
}



//End of provided code
void printer(vector<string> input)
{	
	
	vector<string> done; 
	string hold = "";
	for(size_t i = 0; i < input.size(); i++){
		hold = input[i];
		if(input[i] != ""){
		done.push_back(input[i]);
		}
	} 

	if(!execute(done)){
		string error = "";
		for(size_t k = 0; k < done.size(); k++){
			if(k!=0){
			error+= " ";
			}
			if(k==1){
			error+= "(";
			error+= done[k];
			}
			else{
			error+= done[k];
			}			
		} 
		error+= ")"; 
	cerr << program_name << " can't run: " << error << "\n";
	exit (EXIT_FAILURE);
	}
/*
	cout << "Elements: ";
	for(size_t z = 0; z < done.size(); z++){
	cout << done[z] << ",";
	}
	*/
}


void cmd_process(string input){
string temp = "";
vector<string> cmd;
	for(size_t i = 0; i < input.size(); i++){
				

		if(input[i] == '\\'){
		temp += input[i+1];
		i++;
		} 
		
		else if(isspace(input[i])){
		cmd.push_back(temp);
		temp.clear();
		//continue;
		}
	
		else if(input[i] == '('){
		cmd.push_back(temp);
		temp.clear();
		
		}
		
		else if(input[i] == ')'){
		cmd.push_back(temp);
		temp.clear();
		printer(cmd);
		cmd.clear();
		}
			
		else{
			if(isspace(input[i])){
			continue;
			}
	        	temp += input[i];
		}
	
	}
    printer(cmd);
	
}


void analyze(istream &in)
{
string line;
string full = "";
	while(getline(in,line))
	{
	full += line;
	line.clear();
	}
	cmd_process(full);
	//cout << "inside analyze\n";
}

int main(int argc, char *argv[])
{

program_name = argv[0];

if (argc > 1){
	for(int i = 1; i < argc; i++){ //go through argv for files
	ifstream in(argv[i]); //create f stream
	if(!in.is_open()){ //was it able to open the file?
	cerr << program_name << ": " << argv[i] << " can not be opened. Exiting.\n"; //std error msg
	exit (EXIT_FAILURE); //exit the program
	}
	string s;
	string full= "";
		while(getline(in,s)){
		full += s;
		s.clear();
		}
	//cout << "This is full: " << full << "\n";
	cmd_process(full);
	}

}
	else
	analyze(cin);

return 0;
}
