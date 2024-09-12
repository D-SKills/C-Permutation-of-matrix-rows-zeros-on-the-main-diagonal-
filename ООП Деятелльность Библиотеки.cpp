//������ � ����� ������������� ����. �������
//������_�.� ������ 1990 2
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<iomanip>

using namespace std;

//����� �����
class Book
{
public:
	char author[255];//������� + �������� ������
	char name[255];//��������
	int year;//��� �������
	int number;//���-�� �����������

public:
	Book() { }
	//���������� ���������� � ����� � ����������
	Book initKL()
	{
		char str_author[255];
		cout << "������� ������� � �������� ������" << endl;
		cin >> str_author;
		strcpy(author,str_author);

		char str_name[255];
		cout << "������� �������� �����" << endl;
		cin >> str_name;
		strcpy(name, str_name);

		cout << "������� ��� �������" << endl;
		cin >> year;

		cout << "������� ���-�� �����������" << endl;
		cin >> number;
		return *this;
	}
	//���������� ���������� � �����
	Book init(char* _Author, char* _name, int _year, int _number)
	{
		strcpy(author,_Author);
		strcpy(name, _name);
		year = _year;
		number = _number;
		return *this;
	}
	//������ �����
	void PrintBook()
	{
		cout << "______________________________________________" << endl;
		cout << "|"<<setw(15) << left<< author << "|" << setw(15) << left << name << "|" << setw(5) << left << year << "|" << setw(6) << left << number <<"|"<< endl;
		cout << "______________________________________________" << endl;
	}

};

//����� ����������
class Bibliotek
{
	vector <Book> Books;//������ - ���� ���� ���� � ����������
	int size=0;//������ ����������

public:

	//������ �������� � ������ �� �����
	int CreateBaz( const char* f_name)//�������� � ���������� ����
	{
		FILE* f;

		if ((f = fopen(f_name, "r")) == NULL)
		{
			cout << "Cann't open";
			return -1;
		}
		else
		{
			while (!feof(f))
			{
				char a[255];
				char n[255];
				int year;
				int num;
				
				fscanf(f, "%s", a);
				fscanf(f, "%s",n);
				fscanf(f, "%d", &year);
				fscanf(f, "%d", &num);
				Book ekz;
				ekz.init(a, n, year, num);
				Books.push_back(ekz);
				size++;
			}
			fclose(f);
			return 1;
		}
	}

	//������ �������� � ������ � ����
	int FileOutput(const char* f_name)
	{
		FILE* f;

		if ((f = fopen(f_name, "w")) == NULL)
		{
			cout << "Cann't open";
			return -1;
		}
		else
		{
			for (auto it=Books.begin(); it!= Books.end(); it++)
			{
				fputs(it->author, f);
				fputs(" ", f);
				fputs(it->name, f);
				fputs(" ", f);
				fprintf(f, "%d",it->year);
				fputs(" ", f);
				fprintf(f, "%d", it->number);
				fputs("\n", f);
			}
			fclose(f);
			return 1;
		}
	}

	//����� ���� ���� ������ ������
	void PrintBooksOneA(string autor)
	{

		cout << "______________________________________________" << endl;
		cout << "|     �����     |   ��������    | ��� |���-��|" << endl;
		for (auto it = Books.begin(); it!= Books.end(); it++)//��������
		{
			if (it->author == autor)
			{
				it->PrintBook();
			}
		}
	}
	
	//�������� ���� ���� � ���������� �������
	void DeleteBookOneA(string autor)
	{
		int s = 0;
		string names;
		for (auto it = Books.begin(); it != Books.end(); it++)
		{
			if (it->author == autor)
			{
				names = it->name;
				s++;
			}

		}
		while (s > 0)
		{
			(*this).DeleteBook(autor, names);
			s--;
		}
	}
	
	//���������� ���� �� ���� ������������ ������� ������� ������
	void sortBooksYear(vector<Book>& Books, int size)
	{

		for (int i = 0; i < size; i++)
		{
			int j_min = i;
			for (int j = i; j < size; j++)
			{
				if (Books[j_min].year > Books[j].year)
				{
					j_min = j;
				}
			}
			Book temp;
			temp = Books[i];
			Books[i] = Books[j_min];
			Books[j_min] = temp;
		}
	}

	//������� �����(������� �� ���������� �� ����� - ��� ������ � �������� �����)
	void DeleteBook(string key, string key_name)
	{
		//�������� ����� �� ����� - ��� ������ � �������
		auto  it = Books.begin();
		for (it; it->author != key && it->name != key_name; it++) {}
		Books.erase(it);
		size--;

	}

	//������� ���������� � ����� �������� �� ���������
	void InfoBook(string key)
	{
		//�������� ����� �� ����� - ��� ������ � �������
		auto  it = Books.begin();
		cout << "______________________________________________" << endl;
		cout << "|     �����     |   ��������    | ��� |���-��|" << endl;
		for (it;  it->name != key; it++) {}
		it->PrintBook();
		size--;

	}

	//��������� ����� ����� �� ���������� �����  
	void Set(string key, string key_name)
	{
		//��������� ������ � ����� �� ����� - ��� ������ � �������
		int n;
		cout << "����� ���� �� ������ ��������?" << endl;
		cout << "1. �����" << endl;
		cout << "2. ��������" << endl;
		cout << "3. ���" << endl;
		cout << "4. ���-��" << endl;
		cin >> n;
		auto it = Books.begin();
		for (it; it->author != key && it->name != key_name; it++) {}
		switch (n)
		{
		case 1:
		{
			char key_a[255];
			cin >> key_a;
			strcpy(it->author, key_a);
			break;
		}
		case 2:
		{
			char key_n[255];
			cin >> key_n;
			strcpy(it->name, key_n);
			break;
		}
		case 3:
		{
			int key_y;
			cin >> key_y;
			it->year=key_y;
			break;
		}
		case 4:
		{
			int key_n;
			cin >> key_n;
			it->number= key_n;
			break;
		}
		default:
			break;
		}
		
	
	}

	//���������� ���������� ������� � ����������
	void ZapolnBibliotek()
	{
		cout << "������� ���-�� ���� ������� ����� �������� � ����������" << endl;
		int n;
		cin >> n;
		Book B; //����a
		for (int i = 0; i < n; i++)//��������� �����
		{
			size++;
			B.initKL();
			Books.push_back(B);
		}
	}

	//������ ���� ���� ������������� �� ���� ������� 
	void PrintBooksYear()
	{
	
		sortBooksYear(Books, size);//��������� 
		cout << "______________________________________________" << endl;
		cout << "|     �����     |   ��������    | ��� |���-��|" << endl;
		for (int i = 0; i < size; i++)//��������
		{
			Books[i].PrintBook();
		}
	}

	//����
	void Menu()
	{
		bool f = 0;
		while (f != 1)
		{
			cout << "1. �������� ����� � ����������\n";
			cout << "2. �������� ����� � ����\n";
			cout << "3. ������� ����� �� �����\n";
			cout << "4. ������� ��������� ����\n";
			cout << "5. ������ ������ ����, ������������� �� ����� �������\n";
			cout << "6. �������� ���������� � ����� �� �� �����\n";
			cout << "7. ������� ��� ����� ��������� ������\n";
			cout << "8. ������� ��� ����� ��������� ������\n";
			cout << "9. ������� ���������� � ����� �� �� ��������\n"; 
			cout << "10. �����\n";
			int c;
			cin >> c;
			switch (c)
			{
			case 1:
			{
				(*this).ZapolnBibliotek();
				cout << endl;
				break;
			}
			case 2://������ � ����
			{
				char filename[255];
				int r;
				cout << "������� ��� �����: " << endl;
				cin >> filename;
				r = FileOutput(filename);
				if (r == 1) cout << "������� ��������" << endl;
				else cout << "������" << endl;

				break;
			}
			case 3://���������� �� �����
			{
				char filename[255];
				int r;
				cout << "������� ��� �����: " << endl;
				scanf("%s",filename);
				r = CreateBaz(filename);
				if (r == 1) cout << "������� �������" << endl;
				else cout << "������" << endl;
				break;
			}
			case 4:
			{
				string autor;
				string namebook;
				int k;
				cout << "������� ���� ����� �������?" << endl;
				cin >> k;
			while( k>0 )
				{
					cout << "������� ������� � �������� ������:" << endl;
					cin >> autor;
					cout << "������� �������� �����:" << endl;
					cin >> namebook;
					(*this).DeleteBook(autor, namebook);
					cout << "true" << endl;
					k--;
				}
				cout << endl;
				break;
			}
			case 5:
			{
				if (size != 0)
				{
					cout << endl << "������ ���� ������������� �� ���� �������:" << endl;
					(*this).PrintBooksYear();
				}
				else cout << "���������� �����!" << endl;
				cout << endl;
				break;
			}
			case 6:
			{
				string a_key, n_key;
				cout << "������� ������� � �������� ������" << endl;
				cin >> a_key;
				cout << "������� �������� �����" << endl;
				cin >> n_key;
				(*this).Set(a_key, n_key);
				cout << endl;
				break;
			}
			case 7:
			{	string autor;
				cout << "������� ������� � �������� ������:" << endl;
				cin >> autor;
				DeleteBookOneA(autor);
				cout << endl;
			break;
			}
			case 8:
			{
				string autor;
				cout << "������� ������� � �������� ������:" << endl;
				cin >> autor;
				if (size != 0)
				{
					cout << endl << "����� " <<autor<<":"<< endl;
					(*this).PrintBooksOneA(autor);
				}
				else cout << endl << "���� " << autor << " ��� � ���������� !" << endl;
				cout << endl;
				break;
			}
			case 9:
			{
				string name_book;
				cout << "������� �������� �����:" << endl;
				cin >> name_book;
				InfoBook(name_book);
				cout << endl;
				break;
			}
			case 10:
			{
				f = 1;
				break;
			}
			default:cout << "�� ������ ����! ���������� ��� ���.\n";
			}
		}
		cin.sync();
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Bibliotek Bibl;
	Bibl.Menu();
	
}