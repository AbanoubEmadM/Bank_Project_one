#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <fstream>

using namespace std;

const string FileName = "Users.txt";

struct strUser
{
	string AccountNumber;
	string Name;
	string Phone;
	string PIN;
	double AccountBalance;
	bool IsDeleted = false;
};

bool IsUserFound(strUser& User, string AccountNumber);

vector<string> SplitLine(string Line, string delim = "#//#")
{
	vector<string> vWords;
	short pos = 0;
	string sWord;
	while ((pos = Line.find(delim)) != string::npos)
	{
		sWord = Line.substr(0, pos);

		if(sWord != "")
		{
			vWords.push_back(sWord);
		}
		Line.erase(0, pos + delim.length());
	}
	if (Line != "")
	{
		vWords.push_back(Line);
	}
	return vWords;
}

strUser ConvertLineToRecord(vector<string> vWords)
{
	strUser User;
	User.AccountNumber = vWords[0];
	User.PIN = vWords[1];
	User.Name = vWords[2];
	User.Phone = vWords[3];
	User.AccountBalance = stoi(vWords[4]);
	return User;
}

void PrintClient(strUser User)
{
	cout << "Account Number: " << User.AccountNumber << endl;
	cout << "Name: " << User.Name << endl;
	cout << "Phone: " << User.Phone << endl;
	cout << "PIN: " << User.PIN << endl;
	cout << "Account Balance: " << User.AccountBalance << endl;
}

void ShowAllClients(vector<strUser> vUsers)
{
	for (strUser User : vUsers)
	{
		cout << "\n----------------------------------------------------\n";
		PrintClient(User);
		cout << "\n----------------------------------------------------\n";
	}
}

void ReadClientsFromFile(vector<strUser> &vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	strUser User;
	string Line;
	vector<string> vWords;

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			vWords = SplitLine(Line);
			User = ConvertLineToRecord(vWords);
			vUsers.push_back(User);
		}
	}
	MyFile.close();
}

strUser ReadClient(strUser &User)
{

	cin.ignore();
	cout << "Account Number: "; 
	getline(cin, User.AccountNumber);

	while (IsUserFound(User, User.AccountNumber))
	{
		cout << "This Number Already Exists\n";
		cout << "Account Number: ";
		getline(cin, User.AccountNumber);
	}

	cout << "Name : ";
	getline(cin, User.Name);

 	cout << "Phone: ";
	getline(cin, User.Phone);

	cout << "PIN: ";
	getline(cin, User.PIN);

	cout << "Account Balance: ";
	cin >> User.AccountBalance;

	return User;
}

void AddNewClient(vector<strUser>& vUsers, strUser& User)
{
	strUser U = ReadClient(User);
	vUsers.push_back(U);
}

bool DeleteClient(vector<strUser>& vUsers, string AccountNumber)
{
	for (strUser &User : vUsers)
	{
		if (AccountNumber == User.AccountNumber)
		{
			User.IsDeleted = true;
			return true;
		}
	}
	return false;
}

string ConvertRecordToLine(strUser User, string delim = "#//#")
{
	string Line;

	Line += User.AccountNumber + delim;
	Line += User.PIN + delim;
	Line += User.Name + delim;
	Line += User.Phone + delim;
	Line += to_string(User.AccountBalance);

	return Line;
}

void UpdateClientsAfterDeletion(vector<strUser>& vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (strUser User : vUsers)
		{
			if(!User.IsDeleted)
				MyFile << ConvertRecordToLine(User) << endl;
		}
	}
	MyFile.close();
}

void EditUser(strUser &User)
{
	cin.ignore();
	cout << "Account Number: ";
	getline(cin, User.AccountNumber);

	cout << "Name : ";
	getline(cin, User.Name);

	cout << "Phone: ";
	getline(cin, User.Phone);

	cout << "PIN: ";
	getline(cin, User.PIN);

	cout << "Account Balance: ";
	cin >> User.AccountBalance;
}

strUser FindUser(vector<strUser>& vUsers, string AccountNumber)
{
	for (strUser& User : vUsers)
	{
		if (AccountNumber == User.AccountNumber)
		{
			return User;
		}
	}

}

bool IsUserFound(strUser& User, string AccountNumber)
{
	if (AccountNumber == User.AccountNumber)
	{
		return true;
	}
	return false;
}

void UpdateClientInfo(strUser &User, vector<strUser>& vUsers, string AccountNumber)
{
	char Answer = 'n';

	User = FindUser(vUsers, AccountNumber);

	PrintClient(User);

	cin.ignore();
	cout << "Do you want to update this client info? [y/n]";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (strUser& User : vUsers)
		{
			if (IsUserFound(User, AccountNumber))
			{
				EditUser(User);
			}
		}
	}

}

void UpdateClientsInFile(vector<strUser>& vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (strUser User : vUsers)
		{
			MyFile << ConvertRecordToLine(User) << endl;
		}
	}
	MyFile.close();

}

string ReadAccountNumber()
{
	string AccNumber;
	cout << "Enter Account Number: ";
	cin >> AccNumber;

	return AccNumber;
}

void Deposit(vector<strUser>& vUsers, strUser &User)
{
	string AccountNumber = ReadAccountNumber();
	User = FindUser(vUsers, AccountNumber);
	int DepositValue;
	cout << "Enter Value you want to deposit: ";
	cin >> DepositValue;

	User.AccountBalance += DepositValue;
}

void Withdraw(vector<strUser>& vUsers, strUser& User)
{
	string AccountNumber = ReadAccountNumber();
	User = FindUser(vUsers, AccountNumber);
	int WithdrawValue;
	cout << "Enter Value you want to Withdraw: ";
	cin >> WithdrawValue;

	User.AccountBalance -= WithdrawValue;
}


char ShowMenu()
{
	char Answer;

	cout << "=====================================\n";
	cout << setw(20) << "Main Menu" << setw(20);
	cout << "\n=====================================\n";
	cout << "[1] Show Clients List.\n";
	cout << "[2] Add New Client.\n";
	cout << "[3] Delete Client.\n";
	cout << "[4] Update Client.\n";
	cout << "[5] Find Client.\n";
	cout << "[6] Exit. ";

	cin >> Answer;
	return Answer;
}

void ShowScreen(vector<strUser>& vUsers, strUser &User)
{
	char Ans = ShowMenu();
	switch (Ans)
	{
	case '1':
		ShowAllClients(vUsers);
		break;
	case '2':
		AddNewClient(vUsers, User);
		UpdateClientsInFile(vUsers);
		break;
	case '3':
		DeleteClient(vUsers, ReadAccountNumber());
		UpdateClientsAfterDeletion(vUsers);
		break;
	case '4':
		UpdateClientInfo(User, vUsers, ReadAccountNumber());
		UpdateClientsInFile(vUsers);
		break;
	case '5':
		PrintClient(FindUser(vUsers, ReadAccountNumber()));
		break;
	default:
		return;
	}
}

int main()
{
	vector<strUser> vUsers;
	strUser User;
	ReadClientsFromFile(vUsers);
	//ShowScreen(vUsers, User);
	Withdraw(vUsers, User);
}
