#pragma once
#include "Syntax.h"

class Generator : public SyntAuto
{
	int numstr; //����� ������� ������. ��� ����� �������� �� ���������
	int NTX=705;

public:
	void Start(char*, char*);
	void getCountNTXA();
	void getCountNTtemp();
};
