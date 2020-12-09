#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
#pragma warning (disable : 4996)

struct wordsInText {
	int number;//词语在词典中的编号
	int appearance;//出现的次数
};

bool comp(const wordsInText& a, const wordsInText& b)//用于定义sort函数中的<，即a的出现次数小于b
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
	//词典的编号是从第0号开始的
public:
	vector<Word> words;
	vector<Word>::iterator iter;
	int AddWord(char* word)
	{
		//查找目标词汇是否已经存在
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
			//如果进来的单词word在词典中不存在，为它创建一个并且分配一个编号
			Word newword (word,words.size()+1);
			words.push_back(newword);
			
			return (words.size()-1);
		}
		else
		{
			//进来的单词在词典中存在
			return (iter - words.begin());//返回它在词典中是第几个
		}
	}
	char* getWord(int i)//传来一个词典的编号，回传对应词语
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
	int wordcontain;//用于存储实际含有多少个词语
	int feature[3][20];//特征向量，格式为 词典编号-出现次数
	char filename[100];
	bool isDisabled(char* word)//用于判断该词是不是停用词
	{
		FILE* disablelist;
		if (!(disablelist = fopen("disablelist.txt", "r")))
			throw("Disable word list not found");

		char* tempword = new char[10];//临时存储读取到的单词
		while (1)
		{
			if ((fscanf(disablelist, "%s", tempword)) != EOF)//循环读取单词
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
		for (int i = 0; i <= wordcontain && i<20; i++)//这里不考虑文本小于20个词的情况 
		//这里有错误！！！！！i应该是词表里实际含有的词汇！！！！！
		//修好了。。。
		{
			feature[1][i] = wordAppearNumber[i].number;
			feature[2][i] = wordAppearNumber[i].appearance;
		}
	}

	int getAppearance(int dictno)//负责返回对应词典编号单词出现的次数
	{
		for (int i = 0; i < wordcontain; i++)
		{
			if (wordAppearNumber[i].number == dictno) //这句貌似越界了
			{
				return wordAppearNumber[i].appearance;
			}
		}
		return 0;
	}

	File(char* myFilename,Dict* dict)
	{
		char* temp = new char[10];//临时存储读到的单词
		int tempnumber;//用于存储读取到的单词在字典里的序号
		fp = fopen(myFilename, "r");
		if (fp == NULL) throw("File open error");
		strcpy(filename, myFilename);
		wordcontain = 0;
		//读取文件部分
		while (1)
		{
			if (fscanf(fp, "%s", temp) != EOF)
			{
				cout << "读取到 " << temp << endl;
				if (isDisabled(temp) == true) goto nextword;
				tempnumber = dict->AddWord(temp);//这句出错
				bool exist = false;//用于记录该词语是否出现过
				for (iter = wordAppearNumber.begin(); iter != wordAppearNumber.end(); iter++)//查找该词语是否在词表中出现过
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
				if (exist == false)//如果这个词语在本文件的词表里没出现过，添加到词表
				{
					wordcontain++;
					wordsInText tempword;
					tempword.number = tempnumber;
					tempword.appearance = 1;
					wordAppearNumber.push_back(tempword);
				}

				//wordAppearNumber[dict->AddWord(temp)]++;//把读到的单词全部存入词典，同时把在词典中对应的编号存到向量里
			}
			else //读到文件尾退出循环
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
			out << "<空>" << endl;
		}
		else
		{
			out << "已导入的文件：" << endl;
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
		try {//try-catch语句未经过测试
			File newfile(filename, dict);
			imported.push_back(newfile);
		}
		catch (string)
		{
			throw("import file error");
		}

	}

	float calcDifference(int A,int B)//A,B为目标比较的文件对应的编号
	{
		int feature[2][20];//0-A 1-B 各自的特征向量
		for (int i = 0; i < 20; i++)//转录A的词频
		{
			feature[0][i] = imported[A-1].feature[2][i];
		}


		for (int i = 0; i < 20; i++)//提取B的词频
		{
			feature[1][i] = imported[B-1].getAppearance(imported[A-1].feature[1][i]);//提取A文件相同位置的词语对应词典编号在B文件的出现次数
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


