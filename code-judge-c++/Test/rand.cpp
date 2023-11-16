#include <bits/stdc++.h>

using namespace std;

int main()
{
	std::mt19937 rd(std::random_device{}());
	int x = 2000, y = 2000;
	printf("%d %d\n", x, y);
	for (int i = 1 ; i <= y; i++)
	{
		for (int j = 1; j <= x; j++)
		{
			if (j < 10 && i < 10) printf("1 ");
			else printf("%d ", (rd() % 10) == 1);
		}
		printf("\n");
	}
	return 0;
}
