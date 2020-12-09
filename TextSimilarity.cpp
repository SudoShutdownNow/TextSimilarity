#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "class.h"
using namespace std;

Database mydatabase;
Dict *mydict = new Dict();

int menu()
{
	int temp;
	while (1)
	{
		system("cls");
		cout << "�������ƶȱȽϳ���" << endl;
		cout << "======================" << endl << endl;

		cout << "1.�����ļ�" << endl;
		cout << "2.�Ƚ����ļ����ƶ�" << endl;
		cout << "3.�������ƶ���ߵ��ļ�" << endl;
		cout << "4.�˳�����" << endl;
		cout << endl << "======================" << endl;
		cout << "��ǰ�ѵ�����ļ���" << endl;
		cout << mydatabase;
		cout << "������Ҫѡ��Ĺ���: ";
		input:
		cin >> temp;
		if (temp >= 1 && temp <= 4)
		{
			return temp;
			break;
		}
		else
		{
			cout << "�������������ԣ�";
			goto input;
			system("pause");
		}
	}
}

void importFiles()
{
	system("cls");
	cout << "������Ҫ������ļ�������·����";
	char filename[100];
	if(!(cin >> filename)) goto end;
	try {
		mydatabase.Import(filename, mydict);
	}
	catch (string)
	{
		cout << "�ļ���ʧ�ܣ������ԣ�" << endl;
		system("pause");
	}
end:;
}

void compareFile()
{
	system("cls");
	cout << mydatabase;
	cout << endl << "��������Ҫ�Ƚϵ��ļ�A���:";
	int tempA, tempB;
	cin >> tempA;
	cout << "��������Ҫ�Ƚϵ��ļ�B��ţ�";
	cin >> tempB;
	if (tempA > mydatabase.fileexist() || tempB > mydatabase.fileexist())
	{
		cout << "������������ı�ţ������ԣ�" << endl;
		system("pause");
	}
	cout << "AB���ļ������ƶ��ǣ�" << mydatabase.calcDifference(tempA, tempB) << "%" << endl;
	system("pause");
}

void findSimilar()
{
	int res[2];//���ڴ洢��������飬��0��ΪĿǰ��ӽ����ļ���ţ���1��Ϊ���ƶ�
	system("cls");
	cout << mydatabase;
	cout << endl << "��������Ҫ���ҵ��ļ����:";
	int temp;
	cin >> temp;
	res[0] = temp;
	res[1] = 0;
	int sim;//���ƶ�
	if (temp > mydatabase.fileexist())
	{
		cout << "������������ı�ţ������ԣ�" << endl;
		system("pause");
	}
	for (int i = 0; i < mydatabase.fileexist(); i++)
	{
		if (i != temp - 1)
		{
			sim=mydatabase.calcDifference(temp, i+1);//��˻�-1����0��ʼ������Ҫ�������ʹ�0��ʼ��+1
			if (sim >= res[1])
			{
				res[0] = i;
				res[1] = sim;
			}

		}
		else
		{
			continue;
		}
	}
	cout << "��" << temp << "��������ļ��ǣ�" << res[0]+1 << " ���ƶ�Ϊ��" << res[1] << "%" << endl;
	system("pause");
}


int main()
{
	while (1)
	{
		switch (menu())
		{
		case 1:importFiles(); break;
		case 2:compareFile(); break;
		case 3:findSimilar(); break;
		case 4:exit(0); break;
		default:exit(0); break;
		}
	}
	return 0;
}