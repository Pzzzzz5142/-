#pragma once
#include "LL1a.h"
using namespace std;

struct DAGNode
{
    int id;             //标号
    string op;          //运算符号
    SemNode zhu;        //主标记
    vector<SemNode> fu; //副标记数组
    int houji0;
    int houji1;
};

class Optimizer
{
	struct DAGNode
	{
		int id;             //标号
		string op;          //运算符号
		SemNode zhu;        //主标记
		vector<SemNode> fu; //附加标记数组
		int houji0;
		int houji1;
	};
	vector<DAGNode> DAG; //DAG图
	vector<QtNode> QT;  //优化前
	vector<QtNode> QT2; //优化后
	vector<int> tab;    //标记，是否为基本快开始
	int numnum = 0;         //优化后当前四元式的位置

	string num2str(double i)//常数转化字符串
	{
		stringstream ss;
		ss << i;
		return ss.str();
	}

	int str2num(string str)//字符串转化常数
	{
		int num;
		stringstream ss(str);
		ss >> num;
		return num;
	}

	bool equal(SemNode a, SemNode b)//两个四元式对象是否相等
	{
		if ((a.x == b.x) && (a.y == b.y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool equall(DAGNode a, DAGNode b)//DAG节点根据主标记判等
	{
		if (equal(a.zhu, b.zhu))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int zhufu(SemNode a)//在附加标记中查找
	{
		for (int i = 0; i < DAG.size(); i++)
		{
			for (int j = 0; j < DAG[i].fu.size(); j++)
			{
				if (equal(a, DAG[i].fu[j]))
				{
					return i;
				}
			}
		}
		return -1;
	}

	void deal()//最后的删除处理，原则上是不需要的
	{
		for (int i = 0; i < DAG.size(); i++)
		{
			if ((DAG[i].zhu.y[0] == '#') && (zhufu(DAG[i].zhu) == 1))
			{
				for (int k = 0; k < DAG.size(); k++)
				{
					if (DAG[k].houji0 > i)
					{
						DAG[k].houji0--;
					}
					if (DAG[k].houji1 > i)
					{
						DAG[k].houji1--;
					}
				}
				for (auto it = DAG.begin(); it != DAG.end(); ++it)
				{
					if (equall(DAG[i], *it))
					{
						it = DAG.erase(it);
					}
					else
					{
						++it;
					}
				}
				i--;
			}
		}
	}



	bool dingyi(SemNode x) //是否作为主标记被定义过,初始化的时候用
	{
		for (int i = DAG.size() - 1; i >= 0; i--)
		{
			if (equal(x, DAG[i].zhu))
			{
				return 1;
			}
		}
		return 0;
	}

	int findzhu(SemNode key) //查找DAG图中的主标记,返回它的DAG节点的标号
	{
		for (int i = DAG.size() - 1; i >= 0; i--)
		{
			if (equal(key, DAG[i].zhu))
			{
				return i;
			}
		}
		return -1;
	}

	void chongfubiaoji(SemNode x) //重复的附加标记处理,主标记免删,其实主标记是中间变量的话可以删除
	{
		for (int i = DAG.size() - 1; i >= 0; i--)
		{
			for (int j = 0; j < DAG[i].fu.size(); j++)
			{
				if (equal(x, DAG[i].fu[j]))
				{
					for (auto it = DAG[i].fu.begin(); it != DAG[i].fu.end();)
					{
						if (equal(DAG[i].fu[j], *it))
						{
							it = DAG[i].fu.erase(it);
						}
						else
						{
							++it;
						}
					}
				}
			}
		}
	}

	void change(DAGNode& a)
	{
		int tempx;
		string tempy;
		if (a.zhu.y[0] == '#')
		{
			for (int i = 0; i < a.fu.size(); i++)
			{
				if (a.fu[i].y[0] != '#')
				{
					tempx = a.zhu.x;
					tempy = a.zhu.y;

					a.zhu.x = a.fu[i].x;
					a.zhu.y = a.fu[i].y;

					a.fu[i].x = tempx;
					a.fu[i].y = tempy;
				}
			}
		}
	}

	int findfu(SemNode key, DAGNode a) //查找判断某一个DAG节点的附加标记,返回它在附加标记的位置
	{
		for (int i = 0; i < a.fu.size(); i++)
		{
			if (equal(key, a.fu[i]))
			{
				return i;
			}
		}
		return -1;
	}

	int yunsuan(DAGNode a, SemNode b, SemNode c) //判断是否有前驱后继的关系，给定一个DAG节点，两个运算对象，根据的是节点
	{
		if ((a.houji0 >= 0) && (a.houji1 >= 0))//所在的DAG节点是有后继节点的
		{
			DAGNode k1 = DAG[a.houji0];//左运算对象所在的节点
			DAGNode k2 = DAG[a.houji1];//右运算对象所在的节点
			if ((equal(k1.zhu, b) || (findfu(b, k1) >= 0)) && ((equal(k2.zhu, c)) || (findfu(c, k2) >= 0)))
			{
				return 1;//两个界节点的主标记就是运算对象，或者运算对象在两个节点的附加标记中
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	void InitDAG(int m, int n) //根据当前的四元式初始化DAG图
	{
		for (int i = m; i <= n; i++)
		{
			if ((QT[i].a.y == ">") || (QT[i].a.y == "<") || (QT[i].a.y == "+") || (QT[i].a.y == "-") || (QT[i].a.y == "*") || (QT[i].a.y == "/") || (QT[i].a.y == ">=") || (QT[i].a.y == "<=") || (QT[i].a.y == "==")) //运算符，不是等于，包含关系运算和算术运算，需要改
			{
				if ((!dingyi(QT[i].b)) && (!((QT[i].b.y[0] == '@') && (QT[i].c.y[0] == '@'))) && QT[i].b.y[0] != '#') //没有定义过就定义,不是常数和中间变量
				{
					DAG.push_back(DAGNode());
					DAG[DAG.size() - 1].zhu = QT[i].b;
					DAG[DAG.size() - 1].id = DAG.size() - 1;
					DAG[DAG.size() - 1].houji0 = -1;
					DAG[DAG.size() - 1].houji1 = -1;
				}
				if ((!dingyi(QT[i].c)) && (!((QT[i].b.y[0] == '@') && (QT[i].c.y[0] == '@'))) && QT[i].c.y[0] != '#')
				{
					DAG.push_back(DAGNode());
					DAG[DAG.size() - 1].zhu = QT[i].c;
					DAG[DAG.size() - 1].id = DAG.size() - 1;
					DAG[DAG.size() - 1].houji0 = -1;
					DAG[DAG.size() - 1].houji1 = -1;
				}
			}/*
			if ((QT[i].a.y == "=") && QT[i].b.y[0] == '@' && QT[i].b.y[0] != '#')//直接赋值的常数
			{
				if (!dingyi(QT[i].b))
				{
					DAG.push_back(DAGNode());
					DAG[DAG.size() - 1].zhu = QT[i].b;
					DAG[DAG.size() - 1].id = DAG.size() - 1;
					DAG[DAG.size() - 1].houji0 = -1;
					DAG[DAG.size() - 1].houji1 = -1;
				}
			}*/
		}
	}

	void createDAG(int m, int n) //根据四元式创建DAG图,最复杂的来了
	{
		for (int i = m; i <= n; i++)
		{
			if ((QT[i].a.y == "=") && (QT[i].b.y[0] != '@')) //赋值四元式,不要是常数的,A=B
			{
				if (findzhu(QT[i].b) >= 0)//B是主标记
				{
					chongfubiaoji(QT[i].d);                      //A已经是定义过的附加标记的处理，主标记不删除
					DAG[findzhu(QT[i].b)].fu.push_back(QT[i].d); //B的副标记加A
					change(DAG[findzhu(QT[i].b)]);
				}
				else//B不是主标记
				{
					if (zhufu(QT[i].b) >= 0)//B是某个附加标记
					{
						chongfubiaoji(QT[i].d);
						DAG[zhufu(QT[i].b)].fu.push_back(QT[i].d);//找到作为附加标记所在的节点
						change(DAG[zhufu(QT[i].b)]);
					}
				}

				/*for(int j=DAG.size()-1;j>=0;j--)
				{
				  if(findfu( QT[i].b,DAG[j])>=0)
				  {
					chongfubiaoji(QT[i].d);
					DAG[findfu( QT[i].b,DAG[j])].fu.push_back(QT[i].d);
					change(DAG[findfu( QT[i].b,DAG[j])]);
				  }
				}*/
			}
			if (((QT[i].b.y[0] == '@') && (QT[i].c.y[0] == '@')) || ((QT[i].b.y[0] == '@') && (QT[i].c.x == -1) && (QT[i].a.y == "=")))//常数表达式,直接给出结果或者需要计算
			{

				if ((QT[i].b.y[0] == '@') && (QT[i].c.y[0] == '@'))//需要计算结果
				{
					int a1, a2, a3;
					string s1, s2, s3;

					for (int q = 1; QT[i].b.y[q]; q++)
					{
						s1 = s1 + QT[i].b.y[q];
					}
					for (int q = 1; QT[i].c.y[q]; q++)
					{
						s2 = s2 + QT[i].c.y[q];
					}
					a1 = QT[i].b.x;
					a2 = QT[i].c.x;//转化成数值
					if (QT[i].a.y == "+")
					{
						a3 = a1 + a2;
					}
					if (QT[i].a.y == "-")
					{
						a3 = a1 - a2;
					}
					if (QT[i].a.y == "*")
					{
						a3 = a1 * a2;
					}
					if (QT[i].a.y == "/")
					{
						a3 = a1 / a2;
					}
					s3 = num2str(a3);//转化成字符串
					   //cout<<s3;
					SemNode r;//计算得来的常数
					r.x = a3;
					r.y = '@';
					//cout<<r.y;
					if (findzhu(r) >= 0)//该常数已经定义过，作为主标记
					{
						chongfubiaoji(QT[i].d);//A已经是定义过的处理，主标记不删除
						DAG[findzhu(r)].fu.push_back(QT[i].d);
						change(DAG[findzhu(r)]);
					}
					else//该常数没有定义过
					{
						DAG.push_back(DAGNode());
						int o = DAG.size() - 1;
						DAG[o].zhu = r;
						DAG[o].id = o;
						DAG[o].houji0 = -1;
						DAG[o].houji1 = -1;
						chongfubiaoji(QT[i].d);//A已经是定义过的处理，主标记不删除
						DAG[o].fu.push_back(QT[i].d);
						change(DAG[o]);
					}
				}
				else//不需要计算了
				{
					SemNode r;//计算得来的常数
					r = QT[i].b;
					if (findzhu(r) >= 0)//该常数已经定义过
					{
						chongfubiaoji(QT[i].d);
						DAG[findzhu(r)].fu.push_back(QT[i].d);
						change(DAG[findzhu(r)]);
					}
					else//该常数没有定义过
					{

						DAG.push_back(DAGNode());
						int o = DAG.size() - 1;
						DAG[o].zhu = r;
						DAG[o].id = o;
						DAG[o].houji0 = -1;
						DAG[o].houji1 = -1;
						chongfubiaoji(QT[i].d);//A已经是定义过的处理，主标记不删除
						DAG[o].fu.push_back(QT[i].d);
						change(DAG[o]);
					}
				}
			}

			if (((QT[i].a.y == ">") || (QT[i].a.y == "<") || (QT[i].a.y == "+") || (QT[i].a.y == "-") || (QT[i].a.y == "*") || (QT[i].a.y == "/") || (QT[i].a.y == ">=") || (QT[i].a.y == "<=") || (QT[i].a.y == "==")) && (QT[i].b.y[0] != '@') && (QT[i].c.y[0] != '@'))//不是常数的运算表达式
			{
				int shifou = 0;
				for (int j = DAG.size() - 1; j >= 0; j--)//难点
				{
					if ((yunsuan(DAG[j], QT[i].b, QT[i].c) == 1) && DAG[j].op == QT[i].a.y)//这个DAG节点是运算表达式
					{
						chongfubiaoji(QT[i].d); //A已经是定义过的处理，主标记不删除
						DAG[j].fu.push_back(QT[i].d); //结果A作为附加标记了
						change(DAG[j]);
						shifou = 1;
						break;
					}
				}
				if (shifou == 0)//不存在这样的一个关于运算结果的节点,首先运算结果需要新建的
				{
					/*if(findzhu(QT[i].d)>=0)
					{
					   DAG[findzhu(QT[i].d)].op=QT[i].a.y;//给运算符号
						for (int k = DAG.size() - 1; k >= 0; k--)
					  {
						if(equal(DAG[k].zhu,QT[i].b))
						{
						   DAG[findzhu(QT[i].d)].houji0= k;
						}
						if(equal(DAG[k].zhu,QT[i].c))
						{
						   DAG[findzhu(QT[i].d)].houji1= k;
						}
					   if (findfu(QT[i].b, DAG[k]) >= 0)
					   {
						 DAG[findzhu(QT[i].d)].houji0= k;
					   }
					   if (findfu(QT[i].c, DAG[k]) >= 0)
					   {
					   DAG[findzhu(QT[i].d)].houji1= k;
					   }
					 }
				   }*/
				   //else 
				 //{ 
					chongfubiaoji(QT[i].d);//把以前的先删除掉再说
					DAG.push_back(DAGNode());//先申请一个DAG节点再说
					int o = DAG.size() - 1;
					DAG[o].op = QT[i].a.y;//运算符
					DAG[o].id = o;
					DAG[o].zhu = QT[i].d;//作为主标记
					for (int k = DAG.size() - 1; k >= 0; k--)
					{
						if (equal(DAG[k].zhu, QT[i].b))//作为主标记找到
						{
							DAG[o].houji0 = k;
						}
						if (equal(DAG[k].zhu, QT[i].c))
						{
							DAG[o].houji1 = k;
						}
						if (findfu(QT[i].b, DAG[k]) >= 0)//作为附加标记被找到
						{
							DAG[o].houji0 = k;
						}
						if (findfu(QT[i].c, DAG[k]) >= 0)
						{
							DAG[o].houji1 = k;
						}
					}
					// }

				}

			}
			if (((QT[i].a.y == ">") || (QT[i].a.y == "<") || (QT[i].a.y == "+") || (QT[i].a.y == "-") || (QT[i].a.y == "*") || (QT[i].a.y == "/") || (QT[i].a.y == ">=") || (QT[i].a.y == "<=") || (QT[i].a.y == "==")) && (((QT[i].b.y[0] != '@') && (QT[i].c.y[0] == '@')) || ((QT[i].b.y[0] == '@') && (QT[i].c.y[0] != '@'))))
			{
				int shifou = 0;
				for (int j = DAG.size() - 1; j >= 0; j--)//难点
				{
					if ((yunsuan(DAG[j], QT[i].b, QT[i].c) == 1) && DAG[j].op == QT[i].a.y)//这个DAG节点是运算表达式
					{
						chongfubiaoji(QT[i].d); //A已经是定义过的处理，主标记不删除
						DAG[j].fu.push_back(QT[i].d); //结果A作为附加标记了
						change(DAG[j]);
						shifou = 1;
						break;
					}
				}
				if (shifou == 0)//不存在这样的一个关于运算结果的节点,首先运算结果需要新建的
				{
					/*if(findzhu(QT[i].d)>=0)
					{
					   DAG[findzhu(QT[i].d)].op=QT[i].a.y;//给运算符号
						for (int k = DAG.size() - 1; k >= 0; k--)
					  {
						if(equal(DAG[k].zhu,QT[i].b))
						{
						   DAG[findzhu(QT[i].d)].houji0= k;
						}
						if(equal(DAG[k].zhu,QT[i].c))
						{
						   DAG[findzhu(QT[i].d)].houji1= k;
						}
					   if (findfu(QT[i].b, DAG[k]) >= 0)
					   {
						 DAG[findzhu(QT[i].d)].houji0= k;
					   }
					   if (findfu(QT[i].c, DAG[k]) >= 0)
					   {
					   DAG[findzhu(QT[i].d)].houji1= k;
					   }
					 }
				   }*/
				   //else 
				 //{ 
					chongfubiaoji(QT[i].d);//把以前的先删除掉再说
					DAG.push_back(DAGNode());//先申请一个DAG节点再说
					int o = DAG.size() - 1;
					DAG[o].op = QT[i].a.y;//运算符
					DAG[o].id = o;
					DAG[o].zhu = QT[i].d;//作为主标记
					for (int k = DAG.size() - 1; k >= 0; k--)
					{
						if (equal(DAG[k].zhu, QT[i].b))//作为主标记找到
						{
							DAG[o].houji0 = k;
						}
						if (equal(DAG[k].zhu, QT[i].c))
						{
							DAG[o].houji1 = k;
						}
						if (findfu(QT[i].b, DAG[k]) >= 0)//作为附加标记被找到
						{
							DAG[o].houji0 = k;
						}
						if (findfu(QT[i].c, DAG[k]) >= 0)
						{
							DAG[o].houji1 = k;
						}
					}
					// }

				}
			}
		}

	}

	void recombineDAG() //DAG图重组四元式
	{
		for (int i = 0; i < DAG.size(); i++)
		{
			if ((DAG[i].houji0 == -1) && (DAG[i].houji1 == -1)) //带附加标记的叶子节点
			{
				for (int j = 0; j < DAG[i].fu.size(); j++) //对于附加标记
				{
					if (DAG[i].fu[j].y[0] != '#') //不是临时变量
					{
						QT2.push_back(QtNode()); //生成赋值的四元式
						QT2[QT2.size() - 1].a.x = 0;
						QT2[QT2.size() - 1].a.y = "=";
						QT2[QT2.size() - 1].b = DAG[i].zhu;
						QT2[QT2.size() - 1].d = DAG[i].fu[j];
						QT2[QT2.size() - 1].c.x = -1;
						numnum++;
					}
				}
			}

			if (!((DAG[i].houji0 == -1) && (DAG[i].houji1 == -1))) //带附加标记的非叶子节点
			{
				QT2.push_back(QtNode());
				QT2[QT2.size() - 1].a.x = 0;
				QT2[QT2.size() - 1].a.y = DAG[i].op;
				QT2[QT2.size() - 1].b = DAG[DAG[i].houji0].zhu;
				QT2[QT2.size() - 1].c = DAG[DAG[i].houji1].zhu;
				QT2[QT2.size() - 1].d = DAG[i].zhu;
				numnum++;

				for (int j = 0; j < DAG[i].fu.size(); j++) //对于附加标记
				{
					if (DAG[i].fu[j].y[0] != '#') //不是临时变量
					{
						QT2.push_back(QtNode()); //生成赋值的四元式
						QT2[QT2.size() - 1].a.x = 0;
						QT2[QT2.size() - 1].a.y = "=";
						QT2[QT2.size() - 1].b = DAG[i].zhu;
						QT2[QT2.size() - 1].d = DAG[i].fu[j];
						QT2[QT2.size() - 1].c.x = -1;
						numnum++;
					}
				}
			}
		}
	}

	void optimazte(int m, int n)
	{
		DAG.clear();     //DAG图的清空
		InitDAG(m, n);   //开始的时候初始化DAG图
		createDAG(m, n); //建立DAG图
	   // deal();
		recombineDAG();  //重组生成四元式
	}


	void blockbegin() //基本快入口的寻找
	{
		for (int i = 0; i < QT.size(); i++)
		{
			tab.push_back(0); //标志位数组
		}

		tab[0] = 1; //第一个基本块语句开始
		for (int i = 1; i < QT.size(); i++)
		{
			if (QT[i - 1].d.x > 0) //前一个语句是转向语句
			{
				tab[i] = 1;             //转向语句后面的这个语句就是基本块语句开始
				tab[QT[i - 1].d.x] = 1; //转向语句转移到的语句也是开始
			}
		}
	}


	void optimaztion()//优化
	{
		int m = 0;
		int n = 0;
		blockbegin();//基本块人口的寻找
		int i = 0;
		while (i < QT.size())
		{
			//if(tab[i]==1)//基本快入口
			//{
			m = i;
			for (int j = i + 1; !(tab[j] == 1 || (QT[j].d.x > 0)) && j < QT.size(); j++)
			{
				n = j;
			}
			//cout<<n;
			if ((QT[m].a.y == ">") || (QT[m].a.y == "<") || (QT[m].a.y == "+") || (QT[m].a.y == "-") || (QT[m].a.y == "*") || (QT[m].a.y == "/") || (QT[m].a.y == ">=") || (QT[m].a.y == "<=") || (QT[m].a.y == "==") || (QT[m].a.y == "="))
			{

				if ((QT[n].a.y == ">") || (QT[n].a.y == "<") || (QT[n].a.y == "+") || (QT[n].a.y == "-") || (QT[n].a.y == "*") || (QT[n].a.y == "/") || (QT[n].a.y == ">=") || (QT[n].a.y == "<=") || (QT[n].a.y == "==") || (QT[m].a.y == "="))
				{
					optimazte(m, n);
				}
				else
				{
					optimazte(m, n - 1);
					QT2.push_back(QtNode());
					numnum++;
					QT2[QT2.size() - 1] = QT[n];
				}
			}
			else
			{
				QT2.push_back(QtNode());
				numnum++;
				QT2[QT2.size() - 1] = QT[m];
				if ((QT[n].a.y == ">") || (QT[n].a.y == "<") || (QT[n].a.y == "+") || (QT[n].a.y == "-") || (QT[n].a.y == "*") || (QT[n].a.y == "/") || (QT[n].a.y == ">=") || (QT[n].a.y == "<=") || (QT[n].a.y == "=="))
				{
					optimazte(m + 1, n);
				}
				else
				{
					optimazte(m + 1, n - 1);
					QT2.push_back(QtNode());
					numnum++;
					QT2[QT2.size() - 1] = QT[n];
				}
			}
			i = n + 1;
			//}
		}
	}

	void pr(DAGNode a)
	{

		cout << a.zhu.y;
		cout << ' ';
		for (int k = 0; k < a.fu.size(); k++)
		{
			cout << a.fu[k].y;
			cout << ' ';
		}
		cout << ' ';
		cout << ' ';
		cout << a.houji0;
		cout << ' ';
		cout << a.houji1;
		cout << endl;
	}


	void opsolve()
	{
		int op1 = 0;
		int op2 = 0;
		int i = 0;
		while (i < QT.size())
		{
			if ((QT[i].a.y == ">") || (QT[i].a.y == "<") || (QT[i].a.y == "+") || (QT[i].a.y == "-") || (QT[i].a.y == "*") || (QT[i].a.y == "/") || (QT[i].a.y == ">=") || (QT[i].a.y == "<=") || (QT[i].a.y == "==") || (QT[i].a.y == "="))
			{
				op1 = i;
				for (int j = i; j < QT.size() && ((QT[j].a.y == ">") || (QT[j].a.y == "<") || (QT[j].a.y == "+") || (QT[j].a.y == "-") || (QT[j].a.y == "*") || (QT[j].a.y == "/") || (QT[j].a.y == ">=") || (QT[j].a.y == "<=") || (QT[j].a.y == "==") || (QT[j].a.y == "=")) && j < QT.size();)
				{
					j++;
					op2 = j - 1;
				}
				optimazte(op1, op2);
				i = op2 + 1;
			}
			else
			{
				QT2.push_back(QT[i]);
				numnum++;
				i++;
			}
		}
	}

public:
    vector<QtNode> solve()
    {
		opsolve();
        for (auto& x : QT2)
        {
            if (x.a.y == "=")
            {
                swap(x.b, x.d);
            }
        }
        return QT2;
    }
	void show()
	{
		int i = 0;
		for (auto x : QT2)
			cout << '(' << i++ << ") " << x;
	}
    Optimizer(const LL1& a)
    {
        QT = a.QT;
        for (auto& x : QT)
        {
            if (x.a.y == "=")
            {
                swap(x.b, x.d);
            }
        }
    }
};