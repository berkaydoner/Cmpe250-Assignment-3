/*
Student Name: Berkay Döner
Student Number: 2017400117
Project Number: 3
Compile Status: [SUCCESS]
Running Status: [SUCCESS]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/
#include "SeatOperations.h"

using namespace std;

SeatOperations::SeatOperations(int N, int M){
    this->N=N;//initializing N
    this->M=M;//initializing M
    lines[0] = vector<Person>(N);//initializing the line A
    lines[1] = vector<Person>(M);
}

void SeatOperations::addNewPerson(int personType, const string& ticketInfo){
    int line = ticketInfo[0]-'A';//finding line using ASCII table
    int ticketNum= stoi(ticketInfo.substr(1));//ticketNum is the remaining part
    Person prev;//This will hold the person we should move
    int previndex;//This will hold the index of the seat that person sits.
    if(line==0){//If line is A
        if(ticketNum%N!=0){//If ticket number is not divisible by N
            prev =lines[0][ticketNum%N-1];//hold the person we should move
            lines[0][ticketNum%N-1] = Person{personType, line,ticketNum,-1};//place the new person
            previndex=ticketNum%N-1;//update the index
        }
        else{
            prev =lines[0][N-1];//the person should be placed at the end
            lines[0][N-1] = Person{personType, line,ticketNum,-1};
            previndex = N-1;
        }
    }
    else{//If line is B
        if(ticketNum%M!=0){//If the seat number is not divisible by M
            prev =lines[1][ticketNum%M-1];//hold the person who should move
            lines[1][ticketNum%M-1] = Person{personType,line,ticketNum,-1};//place the person
            previndex= N+ticketNum%M-1;//we add N to the index to show that this person sits in the line B
        }
        else{
            prev =lines[1][M-1];//the person should be at the end
            lines[1][M-1] = Person{personType,line,ticketNum,-1};
            previndex = N + M - 1;//we add N to the index again
        }
    }
    if(prev.type==0){//If the prev person has no type, then there is no such person and we don't need to move
        return;
    }
    processPrevPerson(prev,previndex);//the prev person should move, the function will do that
}
void SeatOperations::processPrevPerson(Person& person,int index){
    //here, the index represent the seat the person is currently sitting, it is not necessarily the initial seat.
    Person prev;//this will hold the person that will move when we place the current person
    int prevIndex;//this will hold the index of the seat of the prev
    if(index<N){//if index<N, then the current line is A
        int seat = index;
        if(person.type==1){//If the person is type 1
            if(person.seatNumber%M!=0){//If the person shouldn't be placed at the end of the line B
                prev = lines[1][person.seatNumber % M - 1];//hold the person
                prevIndex = N + person.seatNumber % M - 1;//N is added to the seat as the person will move to B
                lines[1][person.seatNumber%M-1] = person;//place the person
            }
            else {//If the person should be placed at the end
                prev = lines[1][M - 1];
                prevIndex = N + M - 1;//again N is added to represent that the person will move to B
                lines[1][M - 1] = person;
            }
        }
        else if(person.type==2){//If the type of the person is 2
            if(seat==N-1){//If the person is at the end
                prev = lines[1][0];//place them to the beginning of line B
                lines[1][0] = person;
                prevIndex = N;
            }
            else{//If the person is not at the end
                prev = lines[0][seat + 1];//just place them at the next seat
                lines[0][seat+1] = person;
                prevIndex = seat + 1;
            }
        }
        else if(person.type==3){//If the type of the person is 3
            /*prevOp holds the previous shift, as the type 3 moves with the pattern 1,3,5,7,9... we can add 2 to
             previous value to find the seat the person should currently sit, rather than adding 1,4,9,.. to the
             initial seat number.*/
            int prevOp = person.type3LastOperation;
            int shifted = prevOp+2;//now the person should shift 2 seats more
            int sum = seat+shifted;//the person should sit at this index
            person.type3LastOperation=prevOp+2;//update the value of the last operation
            if(sum<N) {//No need to change lines, just shift the seat
                prev = lines[0][sum];
                lines[0][sum] = person;
                prevIndex = sum;
            }
            else{
                int finalline = 1;//the person should change lines
                while(true){
                    sum-=N;//we start by N as the person was in line A
                    if(sum<M){//If the person now can be placed in line B
                        finalline=1;//the person will sit at the line B
                        break;
                    }
                    sum-=M;//we then subtract M, as we have started with line A
                    if(sum<N){//If the person now can be placed in line A
                        finalline=0;//the person will sit at line A
                        break;
                    }
                }
                prev = lines[finalline][sum];//this is the person who should move next
                lines[finalline][sum]=person;//this person should be placed
                prevIndex = finalline * N + sum;//this will hold the current index of this person
            }
        }
        //We now processed all types for line A, then we should call this function again only if the prev person exists
        if(prev.type == 0){//the person doesn't exist, just finish the method
            return;
        }
        processPrevPerson(prev, prevIndex);//the prev person should move too.
    }
    else {//this will process the people who are currently sitting in line B
        int seat = index-N;//we subtract N as we have stored the indexes of people on B with additional N
        if(person.type==1){//If the person is type 1
            if(person.seatNumber%N!=0){//If they shouldn't be at the end of the line A
                prev = lines[0][person.seatNumber % N - 1];//just place them at the corresponding position
                prevIndex = person.seatNumber % N - 1;// we don't add N, since the person moves to line A
                lines[0][person.seatNumber%N-1] = person;
            }
            else{//If the person should be at the end
                prev = lines[0][N - 1];// place them at the end of line A
                prevIndex = N - 1;
                lines[0][N-1] = person;
            }
        }
        else if(person.type==2){//If the person is type 2
            if(seat==M-1){//If they are at the end
                prev = lines[0][0];// place them at the beginning of line A
                lines[0][0] = person;
                prevIndex = 0;//no additional N is used.
            }
            else{//If they are not at the end
                prev = lines[1][seat + 1];// place them at the next seat
                lines[1][seat+1] = person;
                prevIndex = N + seat + 1;// they stay in the line B, additional N is needed.
            }
        }
        else if(person.type==3){// If the person is type 3
            int prevOp = person.type3LastOperation;//hold the previous addition
            int shifted = prevOp+2;//we need to move the person 2 more seats
            int sum = seat+shifted;//this will hold the index the person should move to
            person.type3LastOperation=prevOp+2;//update the operation
            if(sum<M) {//If the person doesn't need to change lines
                prev = lines[1][sum];//just place them at the shifted position
                lines[1][sum] = person;
                prevIndex = N + sum;
            }
            else{//the person should change lines
                int finalline = 1;//this will hold the line that the person should be in
                while(true){
                    sum-=M;//we subtract M first since the person was in line B
                    if(sum<N){//If they can be seated in line A
                        finalline=0;//update the line
                        break;
                    }
                    sum-=N;// we subtract N now as we have started in line B
                    if(sum<M){//If they can be seated in line B
                        finalline=1;//update the line
                        break;
                    }
                }
                prev = lines[finalline][sum];//holds the person should move
                lines[finalline][sum]=person;
                prevIndex = finalline * N + sum;//this will hold the index of the person
            }
        }
        if(prev.type == 0){//If the person who should move next doesn't exist, just finish the method
            return;
        }
        processPrevPerson(prev, prevIndex);//else, the person prev should be processed next.
    }
}
void SeatOperations::printAllSeats(ofstream& outFile){//this method prints the lines' content to the file
    for(int i =0;i< N;i++){//We first print the line A
        if(lines[0][i].type==0){//If the person doesn't exist
            outFile<< 0<<endl;//just write 0
            continue;
        }
        string ticket;// this holds the information of the ticket
        if(lines[0][i].line==0){//if the person's initial seat was in line A
            ticket = "A";
        }
        else {
            ticket ="B";//if the person's initial seat was in line B
        }
        ticket.append(std::to_string(lines[0][i].seatNumber));//we append the seat number to the letter
        outFile << lines[0][i].type << " " << ticket << endl;//write the ticket to the file
    }
    for(int i= 0;i<M; i++){//we then will write the content of line B
        if(lines[1][i].type==0){//if the person doesn't exist
            outFile<< 0<<endl;//just write 0
            continue;
        }
        string ticket;//holds the information
        if(lines[1][i].line==0){//if the initial seat of the person was in line A
            ticket ="A";
        }
        else{
            ticket ="B";//if the initial seat of the person was in line B
        }
        ticket.append(std::to_string(lines[1][i].seatNumber));//append the seat number to the letter
        outFile << lines[1][i].type << " " << ticket <<endl;//write the ticket information to the file
    }

}

// YOU CAN ADD YOUR HELPER FUNCTIONS