#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 
#include <stdlib.h> 
#include <locale.h> 
#include <ctype.h> 
#include <string.h> 
#include <time.h> 

int choice[2], *pole, *open, n, m, Time, Bombs, BombsN;

int check_win() // �������� ������� ������
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (((pole[i*m + j] != 9) && (pole[i*m + j] != 19)) && (!open[i*m + j])) return 0;
	return 1;
}

void print_pole() // ����� ����
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (open[i*m + j])
			{
				if ((i == choice[0]) && (j == choice[1]))
				{
					if (pole[i*m + j] == 9) printf("# ");
					else printf("+ ", pole[i*m + j]);
				}
				else
				{
					if ((pole[i*m + j] == 9) || (pole[i*m + j] == 19)) printf("# ");
					else {
						if (pole[i*m + j] >= 10) printf("%d ", (pole[i*m + j] - 10));
						else printf("%d ", pole[i*m + j] );
					}
				}
			}
			else
			{
				if ((i == choice[0]) && (j == choice[1]))
				{
					if (pole[i*m + j] >= 10) printf("+ ");
					else printf("+ ");
				}
				else
				{
					if (pole[i*m + j] >= 10) printf("! ");
					else printf("* ", pole[i*m + j]);
				}
			}
		}
		printf("\n");
	}

	printf("\n\n");
	printf("�����: %d", (time(NULL) - Time));
	printf("       ����: %d", Bombs);
}

int open_opened(int x, int y) //�������� �������� ������(�.�. ������ ������)
{
	int i, j, count = 0, a = 0;
	for (i = x - 1; i - x < 2; i++)
		for (j = y - 1; j - y < 2; j++)
			if ((pole[i*m + j] >= 10) && (i >= 0) && (i < n) && (j >= 0) && (j < m)) count++;
	if (pole[x*m + y] == count)
		for (i = x - 1; i - x < 2; i++)
			for (j = y - 1; j - y < 2; j++)
				if (!openning(i, j)) a++;
	return a;
}

int openning(int x, int y) // �������� ������ 
{
	int a = 1;
	if (!((x >= 0) && (y >= 0) && (x < n) && (y < m))) return 1; //�������� �� ����� �� ������� ����
	if (pole[x*m + y] >= 10) return 1; //�������� �� ����

	open[x*m + y] = 1;
	if (pole[x*m + y] == 9) return 0; //�������� �� �����

	if (!pole[x*m + y]) //�������� �������� �� 0
		for (int i = x - 1; i - x < 2; i++)
			for (int j = y - 1; j - y < 2; j++)
				if (!open[i*m + j]) openning(i, j);


	return 1;
}

void mark() //������/������� ����
{
	if (pole[choice[0] * m + choice[1]] >= 10)
	{
		pole[choice[0] * m + choice[1]] -= 10;
		Bombs++;
		return;
	}
	if (!open[choice[0] * m + choice[1]]) pole[choice[0] * m + choice[1]] += 10;
	Bombs--;
}

void boom()
{
	FILE *F;
	char a;
	system("cls");
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			open[i*m + j] = 1;
	print_pole();
	F = fopen("boom.txt", "r");
	printf("\n\n\n");
	for (; (a = getc(F)) != -1; printf("%c", a));
	printf("\n\n\n");
	fclose(F);
}

void win()
{
	FILE *F;
	char a, str[20];
	system("cls");
	print_pole();
	Time = time(NULL) - Time;
	F = fopen("win.txt", "r");
	printf("\n\n\n");
	for (; (a = getc(F)) != -1; printf("%c", a));
	printf("\n\n\n");
	fclose(F);
	printf("������� ��� ���: ");
	scanf("%s", &str);
	F = fopen("Highscore table.txt", "a+");
	fprintf(F, "%s - %d ������", str, Time);
	fprintf(F, " ���� %d x %d ���� %d\n", n, m, BombsN);

	fclose(F);
}

void main()
{
	int i, j, x, y, count = 0;
	char a, str[30];
	FILE* F;
	setlocale(LC_ALL, "Russian");
	if ((F = fopen("settings.txt", "r")) == NULL)
	{
		printf("���������� ������� ����\n");
		return;
	}

	fscanf(F, "%d %d %d", &n, &m, &Bombs);
	fclose(F);
	BombsN = Bombs;
	pole = (int*)malloc(n*m * sizeof(int));
	open = (int*)malloc(n*m * sizeof(int));
	system("mode con cols=800 lines=50");


	for (i = 0; i < n; i++) //������������ �������� ���� 
		for (j = 0; j < m; j++)
		{
			pole[i*m + j] = 0;
			open[i*m + j] = 0;
		}

	srand(time(NULL));
	for (i = 0; i < Bombs; ) //���������� ������� 
	{
		x = rand() % (n*m);
		if (pole[x] == 9) continue;
		pole[x] = 9;
		i++;
	}

	for (i = 0; i < n; i++) //������ ���������� ���� � ������������ ������ 
		for (j = 0; j < m; j++)
		{
			if (pole[i*m + j] != 9)
			{
				for (x = i - 1; x - i < 2; x++)
					for (y = j - 1; y - j < 2; y++)
						if ((pole[x*m + y] == 9) && (x >= 0) && (x < n) && (y >= 0) && (y < m)) count++;
				pole[i*m + j] = count;
				count = 0;
			}
		}

	Time = time(NULL);
	print_pole();

	for (;;)
	{
		switch (_getch())
		{
		default: break;
		case 'w':
			if (choice[0] > 0)
				choice[0]--;
			break;
		case 'a':
			if (choice[1] > 0)
				choice[1]--;
			break;
		case 's':
			if (choice[0] < n - 1)
				choice[0]++;
			break;
		case 'd':
			if (choice[1] < m - 1)
				choice[1]++;
			break;
		case 13:
			if (!openning(choice[0], choice[1]))
			{
				boom();
				return;
			}
			if (check_win())
			{
				win();
				return;
			}
			break;
		case 32: mark(choice[0], choice[1]);
			break;
		case 'f':
			if (open_opened(choice[0], choice[1]))
			{
				boom();
				return;
			}
			if (check_win())
			{
				win();
				return;
			}
		}

		system("cls");
		print_pole();
	}
}