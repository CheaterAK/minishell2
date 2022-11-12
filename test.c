


  typedef struct  s_point
  {
    int           x;
    int           y;
  }               t_point;




void  flood_fill(char **tab, t_point size, t_point begin)
{
    char a = tab[begin.y][begin.x];
    for(int i = 0; i < size.y ; i++)
    {
        for(int j = 0; j < size.x; j++)
        {
            if(tab[i][j] == a)
            {
                if(tab[i][j - 1] == a || tab[i][j - 1] == 'F')
                    tab[i][j] = 'F';
                else if(tab[i][j + 1] == a || tab[i][j + 1] == 'F')
                    tab[i][j] = 'F';
                else if(i != 0 && (tab[i - 1][j] == a || tab[i - 1][j] == 'F'))
                    tab[i][j] = 'F';
                else if(i != (size.y - 1) && (tab[i + 1][j] == a || tab[i + 1][j] == 'F'))
                    tab[i][j] = 'F';
            }
        }
    }
}

#include <stdlib.h>
#include <stdio.h>


char** make_area(char** zone, t_point size)
{
	char** new;

	new = malloc(sizeof(char*) * size.y);
	for (int i = 0; i < size.y; ++i)
	{
		new[i] = malloc(size.x + 1);
		for (int j = 0; j < size.x; ++j)
			new[i][j] = zone[i][j];
		new[i][size.x] = '\0';
	}

	return new;
}

int main(void)
{
	t_point size = {8, 5};
	char *zone[] = {
		"00110000",
		"11110000",
		"11100111",
		"00001111",
		"11110111",
	};

	char**  area = make_area(zone, size);
	for (int i = 0; i < size.y; ++i)
		printf("%s\n", area[i]);
	printf("\n");

	t_point begin = {0, 0};
	flood_fill(area, size, begin);
	for (int i = 0; i < size.y; ++i)
		printf("%s\n", area[i]);
	return (0);
}
