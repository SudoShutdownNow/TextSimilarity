#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
#pragma warning (disable : 4996)

struct wordsInText {
	int number;//�����ڴʵ��еı��
	int appearance;//���ֵĴ���
};

bool comp(const wordsInText& a, const wordsInText& b)//���ڶ���sort�����е�<����a�ĳ��ִ���С��b
{
	if (a.appearance > b.appearance)
	{
		return true;
	}
	else
	{
		return false;
	}
}


class Word {
protected:

	int number;
public:
	char word[10];
	Word(char* myword, int mynumber):number(mynumber)
	{
		strcpy(word, myword);
	}
	char* GetWord()
	{
		return word;
	}
};

class Dict {
	//�ʵ�ı���Ǵӵ�0�ſ�ʼ��
public:
	vector<Word> words;
	vector<Word>::iterator iter;
	int AddWord(char* word)
	{
		//����Ŀ��ʻ��Ƿ��Ѿ�����
		bool found = 0;
		iter = words.begin();
		while (iter != words.end())
		{
			if (strcmp(iter->GetWord(), word) == 0)
			{
				found = 1;
				break;
			}
			iter++;
		}

		if (found == 0)
		{
			//��������ĵ���word�ڴʵ��в����ڣ�Ϊ������һ�����ҷ���һ�����
			Word newword (word,words.size()+1);
			words.push_back(newword);
			
			return (words.size()-1);
		}
		else
		{
			//�����ĵ����ڴʵ��д���
			return (iter - words.begin());//�������ڴʵ����ǵڼ���
		}
	}
	char* getWord(int i)//����һ���ʵ�ı�ţ��ش���Ӧ����
	{
		return words[i].GetWord();
	}

	Dict()
	{

	};
};

class File {
protected:

	vector<wordsInText>::iterator iter;
	FILE* fp;
	
	vector<wordsInText> wordAppearNumber;
public:
	int wordcontain;//���ڴ洢ʵ�ʺ��ж��ٸ�����
	int feature[3][20];//������������ʽΪ �ʵ���-���ִ���
	char filename[100];
	bool isDisabled(char* word)//�����жϸô��ǲ���ͣ�ô�
	{
		FILE* disablelist;
		if (!(disablelist = fopen("disablelist.txt", "r")))
			throw("Disable word list not found");

		char* tempword = new char[10];//��ʱ�洢��ȡ���ĵ���
		while (1)
		{
			if ((fscanf(disablelist, "%s", tempword)) != EOF)//ѭ����ȡ����
			{
				if (strcmp(tempword, word) == 0)
				{
					return true;
				}
			}
			else
			{
				break;
			}
		}
		return false;

	}

	

	void calcFeture()

	{
		sort(wordAppearNumber.begin(),wordAppearNumber.end(),comp);
		for (int i = 0; i <= wordcontain && i<20; i++)//���ﲻ�����ı�С��20���ʵ���� 
		//�����д��󣡣�������iӦ���Ǵʱ���ʵ�ʺ��еĴʻ㣡��������
		//�޺��ˡ�����
		{
			feature[1][i] = wordAppearNumber[i].number;
			feature[2][i] = wordAppearNumber[i].appearance;
		}
	}

	int getAppearance(int dictno)//���𷵻ض�Ӧ�ʵ��ŵ��ʳ��ֵĴ���
	{
		for (int i = 0; i < wordcontain; i++)
		{
			if (wordAppearNumber[i].number == dictno) //���ò��Խ����
			{
				return wordAppearNumber[i].appearance;
			}
		}
		return 0;
	}

	File(char* myFilename,Dict* dict)
	{
		char* temp = new char[10];//��ʱ�洢�����ĵ���
		int tempnumber;//���ڴ洢��ȡ���ĵ������ֵ�������
		fp = fopen(myFilename, "r");
		if (fp == NULL) throw("File open error");
		strcpy(filename, myFilename);
		wordcontain = 0;
		//��ȡ�ļ�����
		while (1)
		{
			if (fscanf(fp, "%s", temp) != EOF)
			{
				cout << "��ȡ�� " << temp << endl;
				if (isDisabled(temp) == true) goto nextword;
				tempnumber = dict->AddWord(temp);//������
				bool exist = false;//���ڼ�¼�ô����Ƿ���ֹ�
				for (iter = wordAppearNumber.begin(); iter != wordAppearNumber.end(); iter++)//���Ҹô����Ƿ��ڴʱ��г��ֹ�
				{
					if (iter->number == tempnumber)
					{
						iter->appearance++;
						exist = true;
					}
					else
					{

					}
				}
				if (exist == false)//�����������ڱ��ļ��Ĵʱ���û���ֹ�����ӵ��ʱ�
				{
					wordcontain++;
					wordsInText tempword;
					tempword.number = tempnumber;
					tempword.appearance = 1;
					wordAppearNumber.push_back(tempword);
				}

				//wordAppearNumber[dict->AddWord(temp)]++;//�Ѷ����ĵ���ȫ������ʵ䣬ͬʱ���ڴʵ��ж�Ӧ�ı�Ŵ浽������
			}
			else //�����ļ�β�˳�ѭ��
			{
				fclose(fp);
				break;
			}
		nextword:;

		}
		calcFeture();
	}

	File() {};



};

class Database {
protected:
	vector<File>imported;
	vector<File>::iterator iter1;
public:
	int fileexist()
	{
		return(imported.size());
	}
	friend ostream& operator<<(ostream& out, Database& comein)
	{
		comein.iter1 = comein.imported.begin();
		if (comein.iter1 == comein.imported.end())
		{
			out << "<��>" << endl;
		}
		else
		{
			out << "�ѵ�����ļ���" << endl;
			for (int i = 1; comein.iter1 != comein.imported.end(); i++)
			{

				out << i << "\t" << comein.iter1->filename << endl;
				comein.iter1++;
			}
		}
		return out;
	}
	void Import(char* filename,Dict* dict)
	{
		try {//try-catch���δ��������
			File newfile(filename, dict);
			imported.push_back(newfile);
		}
		catch (string)
		{
			throw("import file error");
		}

	}

	float calcDifference(int A,int B)//A,BΪĿ��Ƚϵ��ļ���Ӧ�ı��
	{
		int feature[2][20];//0-A 1-B ���Ե���������
		for (int i = 0; i < 20; i++)//ת¼A�Ĵ�Ƶ
		{
			feature[0][i] = imported[A-1].feature[2][i];
		}


		for (int i = 0; i < 20; i++)//��ȡB�Ĵ�Ƶ
		{
			feature[1][i] = imported[B-1].getAppearance(imported[A-1].feature[1][i]);//��ȡA�ļ���ͬλ�õĴ����Ӧ�ʵ�����B�ļ��ĳ��ִ���
		}

		int multiple = 0, lengthA = 0, lengthB = 0;
		for (int i = 0; i < 20; i++)
		{
			multiple += feature[0][i] * feature[1][i];
			lengthA += feature[0][i] * feature[0][i];
			lengthB += feature[1][i] * feature[1][i];
		}

		float res = (multiple / (sqrt(lengthA) * sqrt(lengthB)));
		return res*100;
	
	}

	Database() {};

};


