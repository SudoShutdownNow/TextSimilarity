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
		cout << "文字相似度比较程序" << endl;
		cout << "======================" << endl << endl;

		cout << "1.导入文件" << endl;
		cout << "2.比较两文件相似度" << endl;
		cout << "3.查找相似度最高的文件" << endl;
		cout << "4.退出程序" << endl;
		cout << endl << "======================" << endl;
		cout << "当前已导入的文件：" << endl;
		cout << mydatabase;
		cout << "输入想要选择的功能: ";
		input:
		cin >> temp;
		if (temp >= 1 && temp <= 4)
		{
			return temp;
			break;
		}
		else
		{
			cout << "输入有误，请重试！";
			goto input;
			system("pause");
		}
	}
}

void importFiles()
{
	system("cls");
	cout << "请拖入要导入的文件或输入路径：";
	char filename[100];
	if(!(cin >> filename)) goto end;
	try {
		mydatabase.Import(filename, mydict);
	}
	catch (string)
	{
		cout << "文件打开失败，请重试！" << endl;
		system("pause");
	}
end:;
}

void compareFile()
{
	system("cls");
	cout << mydatabase;
	cout << endl << "请输入想要比较的文件A编号:";
	int tempA, tempB;
	cin >> tempA;
	cout << "请输入想要比较的文件B编号：";
	cin >> tempB;
	if (tempA > mydatabase.fileexist() || tempB > mydatabase.fileexist())
	{
		cout << "不存在您输入的编号，请重试！" << endl;
		system("pause");
	}
	cout << "AB两文件的相似度是：" << mydatabase.calcDifference(tempA, tempB) << "%" << endl;
	system("pause");
}

void findSimilar()
{
	int res[2];//用于存储结果的数组，【0】为目前最接近的文件编号，【1】为相似度
	system("cls");
	cout << mydatabase;
	cout << endl << "请输入想要查找的文件编号:";
	int temp;
	cin >> temp;
	res[0] = temp;
	res[1] = 0;
	int sim;//相似度
	if (temp > mydatabase.fileexist())
	{
		cout << "不存在您输入的编号，请重试！" << endl;
		system("pause");
	}
	for (int i = 0; i < mydatabase.fileexist(); i++)
	{
		if (i != temp - 1)
		{
			sim=mydatabase.calcDifference(temp, i+1);//后端会-1，从0开始，所以要给本来就从0开始的+1
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
	cout << "与" << temp << "最相近的文件是：" << res[0]+1 << " 相似度为：" << res[1] << "%" << endl;
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