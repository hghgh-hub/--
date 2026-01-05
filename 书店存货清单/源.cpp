#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>


typedef struct {
	char bookname[100], writer[100], pub[100];
	float price;
	long int pubtime;
	int location, remain_num;
}BOK;
//定义链表结构
//节点
typedef struct tagnode {
	BOK book;//书籍信息
	struct tagnode* pNext;//指向下一个节点
}Node;

//创造头节点
Node* ghead = NULL;





//重建链表，读取文件
void loadbook() {
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, "books.dat", "rb");//vs2022限制，windows以外可能适配不了
	if (err != 0 || fp == NULL) {
		return;
	}
	BOK temp;//创建临时结构体变量，方便一个一个将文件中的内容转移到重建链表中
	while (fread(&temp, sizeof(BOK), 1, fp)) {
		Node* node = (Node*)malloc(sizeof(Node));
		if (!node) {
			//内存分配失败，清理并退出读取
			fclose(fp);
			return;
		}
		node->book = temp;
		node->pNext = NULL;
		if (ghead == NULL) {
			ghead = node;
		}
		else {
			Node* p = ghead;
			while (p->pNext != NULL) p = p->pNext;
			p->pNext = node;
		}
	}
	fclose(fp);
}
//打印书籍函数
void printbook() {
	system("cls");
	Node* p = ghead;
	if (p == NULL) {
		printf("系统中暂无书籍信息，请先录入书籍");
	}
	else {
		printf("==========================================================================================================================\n");
		printf("| %-30s | %-15s | %-15s | %-10s | %-8s | %-5s | %-8s |\n",
			"书名", "作者", "出版社", "出版时间", "价格", "库存", "位置");
		printf("==========================================================================================================================\n");

		while (p != NULL) {
			printf("| %-30s | %-15s | %-15s | %10ld | %8.2f | %5d | %8d |\n",
				p->book.bookname,
				p->book.writer,
				p->book.pub,
				p->book.pubtime,
				p->book.price,
				p->book.remain_num,
				p->book.location
			);
			printf("-------------------------------------------------------------------------------------------------------===============\n");
			p = p->pNext;
		}
	}
	system("pause");
}


//书籍保存函数
void savebook() {
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, "books.dat", "wb");
	//同样是vs2022限制
	if (err != 0 || fp == NULL) {
		printf("open file error!");
		system("pause");
		return;
	}
	Node* p = ghead;
	while (p != NULL) {
		fwrite(&p->book, sizeof(BOK), 1, fp);
		p = p->pNext;
	}
	fclose(fp);
	printf("书籍信息保存成功");
	system("pause");
	system("cls");

}

//书籍进货函数
void bookinput() {
	char ch;

	do {
		system("cls");
		Node* dp = ghead;
		char temp[100] = { 0 };
		int found = 0;
		//首先判断该书是否已存在
		printf("请你输入进货的书籍信息：\n");
		printf("书名：");
		scanf_s("%99s", temp, (unsigned)(sizeof(temp) / sizeof(temp[0])));

		// 修复：遍历所有节点来检测重复（原来用 dp->pNext 会漏掉最后一个节点）
		dp = ghead;
		while (dp != NULL) {
			if (strcmp(temp, dp->book.bookname) == 0) {
				printf("该书籍已存在，请前往书籍修改系统内修改信息\n");
				found = 1;
				system("pause");
				break;
			}
			dp = dp->pNext;
		}
		if (found) {
			return; // 保持你原来的行为：发现已存在就返回
		}

		// 创建一本书，在堆中分配内存
		Node* pnewbook = (Node*)malloc(sizeof(Node));
		if (!pnewbook) {
			printf("内存分配失败");
			system("pause");
			return;
		}
		// 先填写节点内的数据，然后再插入到链表（修复：避免插入未初始化节点）
		// 使用目标缓冲区大小作为 strcpy_s 的第二参数（更安全）
		strcpy_s(pnewbook->book.bookname, sizeof(pnewbook->book.bookname), temp);

		printf("\n请输入作者：");
		scanf_s("%99s", pnewbook->book.writer, (unsigned)sizeof(pnewbook->book.writer));

		printf("\n请输入出版社：");
		scanf_s("%99s", pnewbook->book.pub, (unsigned)sizeof(pnewbook->book.pub));

		printf("\n请输入出版时间：");
		scanf_s("%ld", &pnewbook->book.pubtime);

		printf("\n请输入价格：");
		scanf_s("%f", &pnewbook->book.price);

		printf("\n请输入进货数量：");
		scanf_s("%d", &pnewbook->book.remain_num);

		printf("\n请输入存放位置：");
		scanf_s("%d", &pnewbook->book.location);

		// 插入新的节点到链表尾部（在填写完字段后插入）
		pnewbook->pNext = NULL;
		if (ghead == NULL) {
			ghead = pnewbook;
		}
		else {
			Node* p = ghead;
			while (p->pNext != NULL) p = p->pNext;
			p->pNext = pnewbook;
		}

		printf("是否继续进货（y/n）：");
		scanf_s(" %c", &ch, (unsigned)sizeof(ch));

	} while (ch == 'y' || ch == 'Y');

	savebook();
	printbook();
	system("pause");
}

//书籍查找函数
void Findbook() {
	system("cls");
	printf("请输入你想要查找的书籍名字：");
	char name[100] = { 0 };
	scanf_s("%99s", name, (unsigned)(sizeof(name) / sizeof(name[0])));
	int found = 0;//判断是否查找到书籍
	Node* p = ghead;
	while (p != NULL) {
		if (strcmp(p->book.bookname, name) == 0) {
			printf("-------------------------------------------------------------------------------------------------------\n");
			printf("| %-30s | %-15s | %-15s | %-10s | %-8s | %-5s | %-8s |\n",
				"书名", "作者", "出版社", "出版时间", "价格", "库存", "位置");
			printf("-------------------------------------------------------------------------------------------------------\n");
			printf("| %-30s | %-15s | %-15s | %10ld | %8.2f | %5d | %8d |\n",
				p->book.bookname,
				p->book.writer,
				p->book.pub,
				p->book.pubtime,
				p->book.price,
				p->book.remain_num,
				p->book.location
			);
			printf("-------------------------------------------------------------------------------------------------------\n");
			system("pause");
			found = 1;
			break;
		}
		p = p->pNext;
	}
	if (found == 0) {
		printf("输入书名不存在，请换一本吧");
	}
}


//删除书籍信息
void delebook() {
	system("cls");
	printf("请输入要删除的书籍信息");
	char name[100] = { 0 };
	scanf_s("%99s", name, (unsigned)(sizeof(name) / sizeof(name[0])));;
	int found = 0;
	Node* p = ghead;
	//定义一个指针指向前一个节点
	Node* bp = ghead;
	while (p != NULL) {
		if (strcmp(p->book.bookname, name) == 0) {
			printf("*\t%s\t*\t%s\t*\t%s\t*\t%ld\t*\t%f\t*\t%d\t*\t%d\t*\n",
				p->book.bookname,
				p->book.writer,
				p->book.pub,
				p->book.pubtime,
				p->book.price,
				p->book.remain_num,
				p->book.location
			);
			if (p == ghead) {   //要删除的节点为首节点
				ghead = p->pNext;
			}
			else {  //最后一个节点同样适用
				bp->pNext = p->pNext;
			}
			free(p);
			found = 1;
			break;
		}
		bp = p;
		p = p->pNext;
	}
	if (found) {
		printf("删除成功");
		savebook();
	}
	else {
		printf("删除失败，请检查输入是否正确");
	}
	system("pause");
}

//书籍修改函数
void bookchange() {

	int c;
	system("cls");
	printf("请输入你想要修改的书籍名字：");
	char name[100] = { 0 };
	scanf_s("%99s", name, (unsigned)(sizeof(name) / sizeof(name[0])));
	int found = 0;//判断是否查找到书籍
	Node* p = ghead;
	while (p != NULL) {
		if (strcmp(p->book.bookname, name) == 0) {
			printf("-------------------------------------------------------------------------------------------------------\n");
			printf("| %-30s | %-15s | %-15s | %-10s | %-8s | %-5s | %-8s |\n",
				"书名", "作者", "出版社", "出版时间", "价格", "库存", "位置");
			printf("-------------------------------------------------------------------------------------------------------\n");
			printf("| %-30s | %-15s | %-15s | %10ld | %8.2f | %5d | %8d |\n",
				p->book.bookname,
				p->book.writer,
				p->book.pub,
				p->book.pubtime,
				p->book.price,
				p->book.remain_num,
				p->book.location
			);
			printf("-------------------------------------------------------------------------------------------------------\n");
			printf("\n\n\n\n\n");
			printf("                                       1.修改书名");
			printf("                                       2.修改作者");
			printf("                                       3.修改出版社");
			printf("                                       4.修改出版时间");
			printf("                                       5.修改价格");
			printf("                                       6.修改库存");
			printf("                                       7.修改位置");
			printf("                                       8.修改全部");
			printf("                                       0.退出修改");
			printf("请输入你的选择：");
			scanf_s("%d", &c);
			switch (c) {
			case 1:
				printf("请输入书名：\n");
				scanf_s("%99s", p->book.bookname, (unsigned)sizeof(p->book.bookname));
				break;
			case 2:
				printf("请输入作者：\n");
				scanf_s("%99s", p->book.writer, (unsigned)sizeof(p->book.writer));
				break;
			case 3:
				printf("请输入出版社：\n");
				scanf_s("%99s", p->book.pub, (unsigned)sizeof(p->book.pub));
				break;
			case 4:
				printf("请输入出版时间：\n");
				scanf_s("%ld", &p->book.pubtime);
				break;
			case 5:
				printf("请输入价格：\n");
				scanf_s("%f", &p->book.price);
				break;
			case 6:
				printf("请输入库存：\n");
				scanf_s("%d", &p->book.remain_num);
				break;
			case 7:
				printf("请输入位置：\n");
				scanf_s("%d", &p->book.location);
				break;
			case 8:
				printf("请输入书名：\n");
				scanf_s("%99s", p->book.bookname, (unsigned)sizeof(p->book.bookname));
				printf("请输入作者：\n");
				scanf_s("%99s", p->book.writer, (unsigned)sizeof(p->book.writer));
				printf("请输入出版社：\n");
				scanf_s("%99s", p->book.pub, (unsigned)sizeof(p->book.pub));
				printf("请输入出版时间：\n");
				scanf_s("%ld", &p->book.pubtime);
				printf("请输入价格：\n");
				scanf_s("%f", &p->book.price);
				printf("请输入库存：\n");
				scanf_s("%d", &p->book.remain_num);
				printf("请输入位置：\n");
				scanf_s("%d", &p->book.location);
				break;
			}

			found = 1;
			break;

		}
		p = p->pNext;
	}
	if (found == 0) {
		printf("输入书名不存在，请换一本吧\n");
		return;
	}



	savebook();

}


//书籍购买函数
void bookbuy() {
	system("cls");
	float total_price = 0;
	int found = 0;
	int buynum = 0;
	char ch = 'n';
	printbook();
	do {
		printf("请输入您要买的书籍名称");
		char name[100] = { 0 };
		scanf_s("%99s", name, (unsigned)sizeof(name));
		Node* p = ghead;
		while (p != NULL) {
			if (strcmp(p->book.bookname, name) == 0) {
				printf("请输入购买数量：");
				scanf_s("%d", &buynum);
				// 修复：允许购买数量等于剩余库存
				if (buynum <= (p->book.remain_num)) {
					total_price += (p->book.price) * buynum;
					p->book.remain_num -= buynum;
					printf("该书需要%f元\n", buynum * (p->book.price));
					found = 1;
				}
				else {
					printf("您购买的书籍库存不够哦！\n");
				}

			}
			p = p->pNext;
		}
		if (found != 1) {
			printf("未查询到书籍或库存不足\n");
		}

		printf("是否还想购买其他书籍（y/n）");
		scanf_s(" %c", &ch, (unsigned)sizeof(ch));

	} while (ch == 'y' || ch == 'Y');
	if (found) {
		printf("总共%f元\n", total_price);
	}
	savebook();


}

void menu1() {
	system("cls");
	printf("\n\n\n\n\n\n\n");
	printf("==================================欢迎使用书店存货清单系统=====================================\n");
	printf("                                   1.顾客模式\n");
	printf("                                   2.管理员模式\n");
	printf("                                   3.退出系统\n");
	printf("请输入你的选项：");
}

void menu2() {
	system("cls");
	printf("                                   1.购买书籍\n");
	printf("                                   2.显示所有书籍\n");
	printf("                                   3.退出系统\n");
	printf("请输入你的选项：");

}

void menu3() {
	system("cls");
	printf("                                   1.显示所有书籍\n");
	printf("                                   2.查找书籍\n");
	printf("                                   3.书籍进货\n");
	printf("                                   4.删除书籍\n");
	printf("                                   5.修改书籍信息\n");
	printf("                                   0.退出系统\n");


	printf("请输入你的选项：");

}

int main() {
	const char password[100] = "12345678";
	//重新加载链表
	loadbook();
	while (1) {
		system("cls");
		menu1();
		char ch1 = _getch();
		if (ch1 == '1') {
			menu2();
			char ch2 = _getch();
			switch (ch2) {
			case'1':
				bookbuy();
				break;
			case'2':
				printbook();
				break;
			case'3':
				printf("成功退出系统");
				goto out;
			default:
				printf("没有该选项");
				break;
			}
		}
		if (ch1 == '2') {
			char temp[10] = { 0 };
			printf("\n");
			printf("请输入管理员密码：");
			scanf_s("%9s", temp, (unsigned)(sizeof(temp) / sizeof(temp[0])));
			printf("\n");
			if (strcmp(password, temp) == 0) {
				printf("登录成功");
				menu3();
				char ch3 = _getch();
				switch (ch3) {
				case'1':
					printbook();
					break;
				case'2':
					Findbook();
					break;
				case'3':
					bookinput();
					break;
				case'4':
					delebook();
					break;
				case'5':
					bookchange();
					break;
				case'0':
					goto out;
					break;
				default:
					printf("没有该选项");
					break;
				}
			}
			else {
				printf("密码错误，请重新输入\n");
				system("pause");
			}
		}
		if (ch1 == '3') {
			goto out;
		}
	}



out:
	return 0;
}