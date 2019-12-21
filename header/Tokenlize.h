#pragma once
#pragma warning (disable:4996)
#define AnsNum 100010
#define AnsLen 110
#define TableNum 110
#define DeliNum 12
#define KeyNum 7
#include <bits/stdc++.h>
using namespace std;
namespace Tokenlize
{
	int consttop = 0;
	int identtop = 0;
	int cconsttop = 0;
	int sconsttop = 0;

	const int DelimeterNum = 22;
	const int Keynum = 9;

	//�����?
	char Delimeters[TableNum][TableNum] = { "(",")","<<","{","}",";","=","+","-","*","_","[","]",".","==","<=","<",">",">=",",","\'","\"","!=" }; //i+4
																																			 //�ؼ��ֱ�
    char Key[TableNum][TableNum] = { "int", "void", "if", "else", "char", "cout","double","float","for","while","return","putc" };   //i+26
																												//�ַ�������
	char Char_Constant[TableNum];
	//�ַ���������
	char String_Constant[TableNum][TableNum];
	//������
	char Constant[TableNum][TableNum];
	//��ʶ����
	char Identifier[TableNum][TableNum];



	using namespace std;

	typedef struct token
	{
		char target[AnsLen];
		int type;    //Token���? Identifier����0 Char_Constant����1 String_Constant����2 Constant����3 Key����4...25 Delimeters����26...34
	}Token;

	typedef struct Constant : Token     //����
	{
		int type = 0;
	}Const;

	typedef struct Delimeter : Token    //���?
	{
		int type = 1;
	}Deli;

	typedef struct Keyword : Token      //�ؼ���
	{
		int type = 2;
	}Keyw;

	typedef struct Identifier : Token   //��ʶ��
	{
		int type = 3;
	}Id;

	typedef struct Character : Token    
	{
		int type = 4;
	}Char;

	typedef struct String : Token      
	{
		int type = 5;
	}String;

	typedef struct output : Token      
	{
		int num;
	}Output;

	vector <Const> ConstList;
	vector <Deli> DeliList;
	vector <Keyw> KeyList;
	vector <Id> IdList;
	vector <Char> CharList;
	vector <String> StrList;

	vector <Output> OutputList;

	void solve()
	{
        for (int i = 0; Delimeters[i][0] != '\0'; i++)
        {
            Deli TempDeli;
            int cnt = 0;
            for (int j = 0; Delimeters[i][j] != '\0'; j++)
            {
                TempDeli.target[j] = Delimeters[i][j];
                cnt = j + 1;
            }
            TempDeli.target[cnt] = '\0';
            DeliList.push_back(TempDeli);
            memset(TempDeli.target, 0, sizeof(TempDeli.target));
        }

        for (int i = 0; Key[i][0] != '\0'; i++)
        {
            Keyw TempKey;
            int cnt = 0;
            for (int j = 0; Key[i][j] != '\0'; j++)
            {
                TempKey.target[j] = Key[i][j];
                cnt = j + 1;
            }
            TempKey.target[cnt] = '\0';
            KeyList.push_back(TempKey);
            memset(TempKey.target, 0, sizeof(TempKey.target));
        }

        //����������
        string Str, OutStr;
        int top = 0;
        char TempToken[AnsNum];
        int TempTop = 0;
        bool quotation = false;

        ifstream OpenFile("./res/source.cpp");

        char ch;
        while (!OpenFile.eof())
        {
            OpenFile.get(ch);
            Str.push_back(ch);
        }
        OpenFile.close();

        Str.erase(Str.size() - 1);
        cout << Str;

        for (int p = 0; Str[p] != '#';)
        {
            if (Str[p] == ' ')
            {
                p++;
                continue;
            }
            //�жϱ�ʶ��
            else if (Str[p] == '_' || isalpha(Str[p]))
            {
                Id TempId;
                Output TempOut;

                for (; Str[p] != 0 && Str[p] != ' '; p++)  //������־-1��Ϊ0����' '
                {
                    if (Str[p] == '_' || isalpha(Str[p]) || isdigit(Str[p]))
                    {
                        TempToken[TempTop++] = Str[p];
                        TempId.target[TempTop] = Str[p];
                    }
                    else
                    {
                        break;
                    }
                }
                TempToken[TempTop] = '\0';
                TempId.target[TempTop] = '\0';

                //��AnsToken.target��ֵ
                //strcpy(AnsToken[top].target,TempToken);
                //���ҹؼ���
                int flag = 1;
                for (int i = 0; i < KeyList.size(); i++)
                {
                    if (!strcmp(TempToken, KeyList[i].target))
                    {
                        flag = 0;
                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 2;
                        TempOut.num = i;
                        OutputList.push_back(TempOut);
                        memset(TempOut.target, 0, sizeof(TempOut.target));
                        //��AnsToken.num��ֵ
                        //AnsToken[top].num=i+26;
                        break;
                    }
                }
                //������ǹؼ���?,���ұ�ʶ����
                //AnsToken[top].num=26;//��ʶ��Ϊ0
                if (flag)
                {
                    int flag1 = 1;
                    for (int i = 0; i < IdList.size(); i++)
                    {
                        if (!strcmp(TempToken, IdList[i].target))
                        {
                            //��AnsToken.num��ֵ
                            strcpy(TempOut.target, TempToken);
                            TempOut.type = 1;
                            TempOut.num = i;
                            OutputList.push_back(TempOut);
                            memset(TempOut.target, 0, sizeof(TempOut.target));
                            flag1 = 0;

                            //AnsToken[top].num=0;
                            break;
                        }
                    }
                    if (flag1)
                    {
                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 3;
                        TempOut.num = IdList.size();
                        IdList.push_back(TempId);
                        OutputList.push_back(TempOut);
                        memset(TempId.target, 0, sizeof(TempId.target));
                        memset(TempOut.target, 0, sizeof(TempOut.target));
                    }
                }
                //���TempToken
                TempTop = 0;
                memset(TempToken, 0, sizeof(TempToken));
                top++;
            }
            else if (isdigit(Str[p]))    //ֻ��ʶ����
            {
                Const TempConst;
                Output TempOut;

                bool Point = false;
                for (; Str[p] != 0 && Str[p] != ' '; p++)  //������־-1��Ϊ0����' '
                {
                    if (isdigit(Str[p]))
                    {
                        TempToken[TempTop++] = Str[p];
                        TempConst.target[TempTop] = Str[p];
                    }
                    else if (Str[p] == '.')
                    {
                        if (!Point)
                        {
                            TempToken[TempTop++] = Str[p];
                            TempConst.target[TempTop] = Str[p];
                            Point = true;
                        }
                        else
                        {
                            putchar(7);
                            cout << "Compile Error!!!!!" << endl;
                            cout << "Wrong number." << endl;
                            exit(0xffffffff);
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                //���ҳ�����
                int flag = 1;
                for (int i = 0; i < ConstList.size(); i++)
                {
                    if (!strcmp(TempToken, ConstList[i].target))
                    {
                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 0;
                        TempOut.num = i;
                        OutputList.push_back(TempOut);
                        memset(TempOut.target, 0, sizeof(TempOut.target));
                        flag = 0;
                        //��AnsToken.num��ֵ
                            //AnsToken[top].num=3;
                        break;
                    }
                }
                if (flag)
                {
                    strcpy(TempOut.target, TempToken);
                    TempOut.type = 0;
                    TempOut.num = ConstList.size();
                    ConstList.push_back(TempConst);
                    OutputList.push_back(TempOut);
                    memset(TempConst.target, 0, sizeof(TempConst.target));
                    memset(TempOut.target, 0, sizeof(TempOut.target));

                }
                TempTop = 0;
                memset(TempToken, 0, sizeof(TempToken));
                top++;
            }
            else if (Str[p] == ' ')
            {
                p++;
            }
            else if (Str[p] == '\'')
            {
                Output TempOut;
                Char TempChar;

                quotation = !quotation;
                TempToken[TempTop] = Str[p];
                TempChar.target[TempTop] = Str[p];

                TempToken[TempTop] = Str[++p];
                TempChar.target[TempTop] = Str[p];

                int flag = 1;
                for (int i = 0; i < CharList.size(); i++)
                {
                    if (!strcmp(TempToken, CharList[i].target))
                    {
                        TempOut.num = i;
                        flag = 0;
                    }
                }


                if (Str[p + 1] != '\'')
                {
                    putchar(7);
                    cout << "Compile Error!!!!!" << endl;
                    cout << "Wrong Character." << endl;
                    exit(0xffffffff);
                }
                else
                {
                    p++;
                    quotation = !quotation;
                    TempToken[TempTop] = Str[p - 1];
                    TempChar.target[TempTop] = Str[p - 1];
                    strcpy(TempOut.target, TempToken);
                    TempOut.type = 4;
                    if (flag)
                        TempOut.num = CharList.size();
                    OutputList.push_back(TempOut);
                    memset(TempOut.target, 0, sizeof(TempOut.target));
                    CharList.push_back(TempChar);
                    memset(TempChar.target, 0, sizeof(TempChar.target));

                    TempToken[0] = 0;
                    p++;
                }
            }
            else if (Str[p] == '\"')
            {
                String TempStr;
                Output TempOut;

                quotation = !quotation;
                TempToken[TempTop] = Str[p];
                TempStr.target[TempTop] = Str[p];

                p++;

                //ʶ���ַ�������
                while (Str[p] != '\"')
                {
                    if (Str[p] == '#')
                    {
                        putchar(7);
                        cout << "Compile Error!!!!!" << endl;
                        cout << "Wrong Character." << endl;
                        exit(0xffffffff);
                    }
                    TempToken[TempTop++] = Str[p];
                    TempStr.target[TempTop] = Str[p];
                    p++;
                }

                //strcpy(AnsToken[top].target,TempToken);
                int flag = 1;
                for (int i = 0; i < StrList.size(); i++)
                {
                    if (strcmp(TempToken, StrList[i].target))
                    {
                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 5;
                        TempOut.num = i;
                        OutputList.push_back(TempOut);
                        memset(TempOut.target, 0, sizeof(TempOut.target));
                        flag = 0;
                    }
                }
                if (flag)
                {

                    strcpy(TempOut.target, TempToken);
                    TempOut.type = 5;
                    TempOut.num = StrList.size();
                    StrList.push_back(TempStr);
                    OutputList.push_back(TempOut);
                    memset(TempStr.target, 0, sizeof(TempStr.target));
                    memset(TempOut.target, 0, sizeof(TempOut.target));
                }

                quotation = !quotation;

                TempTop = 0;
                memset(TempToken, 0, sizeof(TempToken));
                p++;
            }
            else if ((Str[p] == '<' && (Str[p + 1] == '<' || Str[p + 1] == '=')) || ((Str[p] == '>' || Str[p] == '=') && Str[p + 1] == '='))
            {
                Output TempOut;
                TempToken[0] = Str[p++];
                TempToken[1] = Str[p++];
                TempToken[2] = 0;

                for (int i = 0; i < DeliList.size(); i++)
                {
                    if (!strcmp(TempToken, DeliList[i].target))
                    {

                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 1;
                        TempOut.num = i;
                        OutputList.push_back(TempOut);
                        memset(TempOut.target, 0, sizeof(TempOut.target));
                        break;
                    }
                }

                //���TempToken
                TempTop = 0;
                memset(TempToken, 0, sizeof(TempToken));
                top++;
            }
            else
            {
                //ʶ��Ŀ�����?
                Output TempOut;

                if (Str[p] != 0 && Str[p] != ' ' && !isalpha(Str[p]) && !isdigit(Str[p]) && Str[p] != '_' && Str[p] != '#')  //������־-1��Ϊ0����' '
                {
                    TempToken[TempTop++] = Str[p++];
                }
                else
                {
                    break;
                }
                TempToken[TempTop] = 0;

                for (int j = 0; j < DeliList.size(); j++)
                {
                    if (!strcmp(TempToken, DeliList[j].target))
                    {
                        strcpy(TempOut.target, TempToken);
                        TempOut.type = 1;
                        TempOut.num = j;
                        OutputList.push_back(TempOut);
                        memset(TempOut.target, 0, sizeof(TempOut.target));

                        break;
                    }
                }
                TempTop = 0;
            }
        }


        if (OutputList[0].type == 1 || OutputList[0].type == 2)
            OutStr.append(OutputList[0].target);
        else if (OutputList[0].type == 0)
            OutStr.append("#INT");
        else if (OutputList[0].type == 3)
            OutStr.append("@I");
        else if (OutputList[0].type == 4)
            OutStr.append("@char");

        for (int i = 1; i < OutputList.size(); i++)
        {
            OutStr.push_back(' ');
            if (OutputList[i].type == 1 || OutputList[i].type == 2)
                OutStr.append(OutputList[i].target);
            else if (OutputList[i].type == 0)
                OutStr.append("#INT");
            else if (OutputList[i].type == 3)
                OutStr.append("@I");
            else if (OutputList[i].type == 4)
                OutStr.append("@char");
        }

        cout << OutStr;

        ofstream SaveFile1("./res/demo.txt", ios::ate);
        SaveFile1 << OutStr;
        SaveFile1.close();

        OutStr.clear();

        for (int i = 0; i < OutputList.size(); i++)
            if (OutputList[i].type == 0 || OutputList[i].type == 3 || OutputList[i].type == 4)
            {
                OutStr.push_back(' ');
                OutStr.append(OutputList[i].target);
            }

        cout << OutStr;

        ofstream SaveFile2("./res/ids", ios::ate);
        SaveFile2 << OutStr;
        SaveFile2.close();
	}
}
