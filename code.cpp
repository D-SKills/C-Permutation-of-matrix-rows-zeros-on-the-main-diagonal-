#include<iostream>
#include<fstream>
#include<list>
#include <Windows.h>>


using namespace std;

enum SymbolicTokenClass { Alpha, Digit, AR_OP, Equal, LT_sk, RT_sk, TochZap, Space , EndStr, EndOF, Error };

enum States { A1, A2, A3, A4, A5, A10, A17, A18, A19, Exit1, Exit2, Exit3, Stop};


const int state_number = 12;	 // Число состояний (без s_Stop).
const int class_number = 11;	// Число символьных классов


//класс Ячейка памяти
class YPamiati
{
public:
	string name_perem;	//имя переменной
	int znachenie=-1;	//значение переменой

	int local=0;		//область видимости
public:
	// конструктор по умолчанию
	YPamiati()
	{
		name_perem = "";
	}
	// конструктор с параметрами
	YPamiati(string name, int value, int loc)
	{
		name_perem = name;
		znachenie = value;
		local = loc;
	}
	// метод - печать информации о ячейке памяти
	void Print()
	{
		cout << name_perem << "	" << znachenie << "	" << local << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////////////
// структура - лексема
struct SymbolicToken
{

	char ch;		//символ прочитанный из файла
	string val;		//переменная
	int zn;			//значение подсчитанного выражения
	string per;		//переменная для присваивания
	char oper=0;	//операция 
	string tecych;	//текущая переменная
	SymbolicTokenClass token_class_;
};

// структура Таблица переменных
struct VarTable
{
public:
	list<YPamiati> var;//список ячеек памяти

	// добавляет значение переменной если она еще не встречалась 
	void zamen(string per, int zn)
	{
		auto it = var.begin();
		for (it; it != var.end(); it++)
		{
			if (it->name_perem == per && it->znachenie == -1 )
			{
				it->znachenie = zn;
			}
		}
	}
	// проверяет есть ли переменная в памяти в текущий момент (возвращает  0 - нет 1 - да)
	int find(string per)
	{
		auto it = var.begin();
		for (it; it != var.end(); it++)
		{
			if (it->name_perem == per)
			{
				return 1;
			}
		}
		return 0;
	}

	// добавляет значение переменной если она уже встречалась 
	int dobl(string per, int zn)
	{
		auto ik = var.begin();
	
		for (ik; ik != var.end(); ik++)
		{
		
				if (ik->name_perem == per)
				{
					ik->znachenie = zn;
					return 1;
				}
		}
		return 0;
	}

	//возвращает значение переменной
	int getzn(string per)
	{
		auto it = var.begin();
		for (it; it != var.end(); it++)
		{
			if (it->name_perem == per )
			{
				return it->znachenie;
				break;
			}
		}
		return -1;
	}

	// вычисление арифметических выражений
	int operation( int znpr, char op,int zn)
	{
		int n;
		if (op == '/')
		{
			
			n = znpr/zn;
		}

		else if (op == '*')
		{

			n =  znpr*zn;
		}

		else if (op == '%')
		{

			n = znpr%zn;
		}

		else if (op == '+')
		{

			n = znpr +zn;
		}
		else if (op == '-')
		{

			n = znpr -zn;
		}
		return n;
	}

	//удаление локальных переменных
	void deldobl(int l)
	{
		for (auto it = var.begin(); it != var.end(); it++)
		{
			if (it->local == l)
			{
				var.erase(it);
				break;
			}

		}
	}

	//возвращает уровень доступа
	int getlev(string per)
	{
		auto it = var.begin();
		for (it; it != var.end(); it++)
		{
			if (it->name_perem == per)
			{
				return it->local;
				break;
			}
		}
		return -1;
	}
};

// транслитератор
static SymbolicTokenClass transliterator(char ch)
{
	if ((ch >= 192 && ch <= 255)) { return Alpha; }
	else if (isalpha(ch)) { return Alpha; }
	else if (isdigit(ch)) { return Digit; }
	else if (ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '*') { return AR_OP; }
	else if (ch == '=' ) { return Equal; }
	else if (ch == '{') { return LT_sk; }
	else if (ch == '}') { return RT_sk; }
	else if (ch == ' ') { return Space; }
	else if (ch == '\n') { return EndStr; }
	else if (ch == ';') { return TochZap; }
	else if (ch == EOF) { return EndOF; }
	else { return Error; }
}

//класс - парсер
class Parser
{

private:
	SymbolicToken s;

	VarTable vt;
	int zn=0;
	int l=0;


	typedef int (Parser::* function_pointer)();
	function_pointer table[state_number][class_number];
public:



	//ошибка
	int Error()
	{
		cout << "Ошибка!";
		return Stop;
	}
	// переход на новую строку
	int a1()
	{
			return A1;
	}
	// переход считываения с новой строки переменой
	int a2()
	{
		s.val = s.ch;
		return A2;
	}

	//добавляем переменную
	int a3()
	{
		s.per = s.val;
		s.tecych = s.per;
		if (vt.dobl(s.val, -1) == 0)
		{
			YPamiati yp(s.val, -1, l);
			vt.var.push_back(yp);
		};


		return A3;
	}

	// формируем первую букву переменной
	int a4()
	{
		s.val = s.ch;
		return A4;
	}

	// прочитано ключевое слово-команда - print
	int a5()
	{
		if (s.val == "print" && s.ch==' ')
		{
			s.val.clear();
			return A5;

		}
		else if ((s.val == "print")&& s.ch==';')
		{
			if (vt.find(s.tecych)==1)
			{
				cout << "Вывод: ";
				cout << s.tecych<< "	" << vt.getzn(s.tecych) << "	"  << endl;
			}
			else
			{
				cout << "Переменная " << s.tecych << " недоступна в этой области!" << endl;
				return Error();
			}
		}

	}

	// просмотр имени переменной для печати
	int pr()
	{
		s.val += s.ch;
		return A5;
	}

	// печать на экран конструкций  вида print x;
	int c()
	{
	
		if (vt.find(s.val)==1)
		{
			cout << "Вывод: ";
			cout << s.val << "	" << vt.getzn(s.val) <<"	" << endl;
			s.val.clear();
			return A1;
		}
		else
		{
			cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
			return Error();
		}
	}

	// формируем первую цифру числа
	int a10()
	{
		s.zn = s.ch - '0';
		return A10 ;
	}

	// подсчитываем значения перед ;
	int a17()
	{
		
			// для выражений z = 10 + a
			if ((s.oper != 0) && (s.val.size() > 0) && (zn != 0))
			{
				if (vt.find(s.val) == 0)
				{
					cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
					return Error();
				}

				zn = vt.operation(zn, s.oper, vt.getzn(s.val));
				vt.dobl(s.per, zn);
				
			
			}
			// для выражений z = x / y ;
			else if ((s.oper != 0) && (s.val.size() > 0) && (s.zn == -1))
			{
				if (vt.find(s.val) == 0)
				{
					cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
					return Error();
				}
				zn = vt.operation(vt.getzn(s.val), s.oper, zn);
				vt.dobl(s.per, zn);

			}
			// для выражений вида z = 10 + x
			else if ((s.oper != 0) && (s.val.size() > 0) && (zn != -0))
			{
				if (vt.find(s.val) == 0)
				{
					cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
					return Error();
				}
				zn = vt.operation(zn, s.oper, vt.getzn(s.val));
				vt.dobl(s.per, zn);

			}
			// для выражений вида z = 9 / 3 ;
			else if ((s.oper != 0) && (s.zn != -1) && (zn != 0))
			{

				zn = vt.operation(zn, s.oper, s.zn);
				vt.dobl(s.per, zn);
			}
		

			// для выражений вида x = 10 ;
			if ((s.zn != -1) && (s.per.size() > 0) && (s.oper == 0))
			{
				vt.zamen(s.per, s.zn);
			}
			// для выражений вида x = y ;
			else if ((s.val.size() > 0) && (s.per.size() > 0) && (s.zn == -1))
			{
				if (vt.find(s.val) == 0)
				{
					cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
					return Error();
				}
				vt.zamen(s.per, vt.getzn(s.val));

			}

			
			
		s.oper = 0;
		zn = 0;
		s.val.clear();
		s.zn = -1;
		

		return A17;
	}

	int a18()
	{
		return A18;
	}
	// увеличиваем уровень доступа - выдимости
	int am18()
	{
		l=l+1;
		
		return A18;
	}
	int a19()
	{
	
		return A19;
	}

	// очищаем память переменных находящихся вне области видимости
	int am19()
	{
		for(auto it = vt.var.begin();it!=vt.var.end();it++)
			vt.deldobl(l);
		l=l-1;
		
		return A19;
	}

	// увеличиваем уровень доступа - выдимости
	int loc1()
	{
		
		l=l+1;
		
		return A18;
	}

	// очищаем память переменных находящихся вне области видимости
	int loc2()
	{
		
		for (auto it = vt.var.begin(); it != vt.var.end(); it++)
			vt.deldobl(l);

		l=l-1;
		
		return A19;
	}

	// считываем переменую
	int avar()
	{
		s.val += s.ch;
		return A2;
	}
	// считываем переменую
	int avarin()
	{

		s.val += s.ch;
		
		return A4;
	}

	// создание числа
	int initdig()
	{
		
		s.zn = s.zn*10 + (s.ch - '0');
		
		return A10;
	}
	// арифм операция
	int oper()
	{
		if (vt.find(s.val)==1)
		{
			if (s.oper != 0)
			{
				a17();
				zn = vt.getzn(s.per);
				s.oper = s.ch;

			}
			else if (s.oper == 0)
			{
				// ситуация x + 	
				if (s.val.size() > 0 && s.zn == -1)
				{

					zn = vt.getzn(s.val);
					s.val.clear();
					s.oper = s.ch;
				}
				// ситуация 10 + x
				else if ((s.zn != -1) && (s.val.size() > 0))
				{
					zn = s.zn;
					s.zn = -1;
					s.val.clear();
					s.oper = s.ch;
				}
				else if (s.val.size() > 0)
				{
					zn = vt.getzn(s.val);
					s.val.clear();
					s.oper = s.ch;
				}
			}
		}
		else
		{
			cout << "Переменная " << s.val << " недоступна в этой области!" << endl;
			return Error();
		}

		return A3;
	}


	//завершающие состояния  выхода
	int Exit1()
	{
		vt.var.clear();
		return Stop;
	}
	int Exit2()
	{
		vt.var.clear();
		return Stop;
	}
	int Exit3()
	{
		vt.var.clear();
		return Stop;
	}


	// автомат - парсер с управляющей таблицей
Parser()
{

	for (int i = 0; i < state_number; i++)
	{
		for (int j = 0; j < class_number; j++)
		{
			table[i][j] = &Parser::Error;
		}
	}

	table[A1][Alpha] = &Parser::a2;			table[A1][Digit] = &Parser::Error;		table[A1][AR_OP] = &Parser::Error;		table[A1][Equal] = &Parser::Error;		table[A1][LT_sk] = &Parser::am18;		table[A1][RT_sk] = &Parser::am19;		table[A1][TochZap] = &Parser::Error;		table[A1][Space] = &Parser::a1;			table[A1][EndStr] = &Parser::a1;		table[A1][EndOF] = &Parser::Exit1;
	table[A2][Alpha] = &Parser::avar;		table[A2][Digit] = &Parser::avar;		table[A2][AR_OP] = &Parser::Error;		table[A2][Equal] = &Parser::a3;			table[A2][LT_sk] = &Parser::Error;		table[A2][RT_sk] = &Parser::Error;		table[A2][TochZap] = &Parser::a5;		table[A2][Space] = &Parser::a5;			table[A2][EndStr] = &Parser::Error;		table[A2][EndOF] = &Parser::Error;
	table[A3][Alpha] = &Parser::a4;			table[A3][Digit] = &Parser::a10;		table[A3][AR_OP] = &Parser::Error;		table[A3][Equal] = &Parser::Error;		table[A3][LT_sk] = &Parser::Error;		table[A3][RT_sk] = &Parser::Error;		table[A3][TochZap] = &Parser::Error;		table[A3][Space] = &Parser::a3;			table[A3][EndStr] = &Parser::Error;		table[A3][EndOF] = &Parser::Error;
	table[A4][Alpha] = &Parser::avarin;		table[A4][Digit] = &Parser::avarin;		table[A4][AR_OP] = &Parser::oper;		table[A4][Equal] = &Parser::Error;		table[A4][LT_sk] = &Parser::Error;		table[A4][RT_sk] = &Parser::Error;		table[A4][TochZap] = &Parser::a17;			table[A4][Space] = &Parser::Error;		table[A4][EndStr] = &Parser::Error;		table[A4][EndOF] = &Parser::Error;
	table[A5][Alpha] = &Parser::pr;			table[A5][Digit] = &Parser::pr;			table[A5][AR_OP] = &Parser::Error;		table[A5][Equal] = &Parser::Error;		table[A5][LT_sk] = &Parser::Error;		table[A5][RT_sk] = &Parser::Error;		table[A5][TochZap] = &Parser::c;			table[A5][Space] = &Parser::Error;		table[A5][EndStr] = &Parser::Error;		table[A5][EndOF] = &Parser::Error;
	table[A10][Alpha] = &Parser::a4;		table[A10][Digit] = &Parser::initdig;	table[A10][AR_OP] = &Parser::oper;		table[A10][Equal] = &Parser::Error;		table[A10][LT_sk] = &Parser::Error;		table[A10][RT_sk] = &Parser::Error;		table[A10][TochZap] = &Parser::a17;			table[A10][Space] = &Parser::Error;		table[A10][EndStr] = &Parser::Error;	table[A10][EndOF] = &Parser::Error;
	table[A17][Alpha] = &Parser::Error;		table[A17][Digit] = &Parser::Error;		table[A17][AR_OP] = &Parser::Error;		table[A17][Equal] = &Parser::Error;		table[A17][LT_sk] = &Parser::Error;		table[A17][RT_sk] = &Parser::Error;		table[A17][TochZap] = &Parser::Error;		table[A17][Space] = &Parser::a17;		table[A17][EndStr] = &Parser::a1;		table[A17][EndOF] = &Parser::Exit2;
	table[A18][Alpha] = &Parser::a2;		table[A18][Digit] = &Parser::Error;		table[A18][AR_OP] = &Parser::Error;		table[A18][Equal] = &Parser::Error;		table[A18][LT_sk] = &Parser::loc1;		table[A18][RT_sk] = &Parser::am19;		table[A18][TochZap] = &Parser::Error;		table[A18][Space] = &Parser::a18;		table[A18][EndStr] = &Parser::a18;		table[A18][EndOF] = &Parser::Error;
	table[A19][Alpha] = &Parser::a2;		table[A19][Digit] = &Parser::Error;		table[A19][AR_OP] = &Parser::Error;		table[A19][Equal] = &Parser::Error;		table[A19][LT_sk] = &Parser::am18;		table[A19][RT_sk] = &Parser::loc2;		table[A19][TochZap] = &Parser::Error;		table[A19][Space] = &Parser::a19;		table[A19][EndStr] = &Parser::a19;		table[A19][EndOF] = &Parser::Exit3;



}

void parse(const char* filename)
{
	ifstream in(filename);
	if (!in)
	{
		cout << "Не удалось открыть файл " << filename << endl;
		return;
	}

	int state = A1;   // Текущее состояние автомата.
	while (state != Stop && state != EndOF)
	{
		// Stop - техническое состояние. Когда автомат попадает в Stop, цикл останавливается.
		s.ch = in.get();
		s.token_class_ = transliterator(s.ch);
		state = (this->*table[state][s.token_class_])();
	}
	in.close();
}

};

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	Parser p;
	p.parse(argv[1]);

	return 0;
}
	
	