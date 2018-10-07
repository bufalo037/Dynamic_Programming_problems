// Copyright 2018 Costea Mihai

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void afisare(const char* sir)
{
	FILE *output;
	output = fopen("numaratoare.out", "wt");
	fprintf(output, "%s", sir);
	fclose(output);	
}


void citire(int* suma, int* nr_pos, int* indice)
{

	FILE *input;
	input = fopen("numaratoare.in", "rt");
	fscanf(input,"%d %d %d",suma, nr_pos, indice);
	fclose(input);

}

char *int_to_ascii(int nr)
{
	// transforma un integer in echivalentul sau scris ca un sir de caractere
	// ascii cu un terminator de sir '\0'
	int copie = nr;
	int nr_cifre = 0;
	char *caractere;
	while(copie != 0)
	{
		copie /= 10;
		nr_cifre++;
	}
	caractere = (char *)malloc(sizeof(char) * (nr_cifre + 1));
	for(int i = 0; i < nr_cifre; i++)
	{
		caractere[i] = ((nr / (int)pow((double)10,
			(double)( nr_cifre - i - 1))) % 10) + '0';
	}
	caractere[nr_cifre] = '\0';
	return caractere;
}


void DP(int suma, int nr_pos, int indice)
{
	short *pozitie = (short *)malloc(sizeof(short) * nr_pos);
	int extragere = 0, ok = 0, insert, inserted = 0;
	int i, j;
	int insert_limit;
	if(nr_pos > suma)
	{
		afisare("-");
	}
	int last_non_one_index = 0;
	pozitie[0] = suma - nr_pos + 1;
	for(i = 1; i < nr_pos; i++)
	{
		pozitie[i] = 1;
	}

	for(i = 1; i <= indice; i++)
	{
		// daca am modificat array-ul atunci cand se iese din loop se da break
		inserted = 0;
		insert_limit = nr_pos;
		for(extragere = last_non_one_index; extragere >= 0; extragere--)
		{
			if(pozitie[extragere] - 1 >= pozitie[extragere + 1])
			{
				// daca diferenta dintre cel mai mic element si elementul extras
				// este cu 2 mai mare inseamna ca exista o miscare urmatoare
				if(pozitie[nr_pos - 1] + 1 <= pozitie[extragere] - 1)
				{
					// s-a gasit pozitia de la care sa se extraga un 1
					// este garantat ca extragerea este ok.
					pozitie[extragere]--;
					if(pozitie[extragere] == 1 && last_non_one_index == extragere)
					{
						last_non_one_index--;
					}
					for(insert = extragere + 1;insert < insert_limit ; insert++)
					{
						if(pozitie[insert] + 1 <= pozitie[insert - 1])
						{
							// inseamna ca am pus 1 si dupa ce maximizez 
							// elementele inseamna inserted devine indicator
							// ca pot sa extrag un nou element
							if(inserted == 0)
							{
								if(last_non_one_index < insert)
									last_non_one_index = insert;
								pozitie[insert]++;
								inserted = 1;
							}
							insert_limit = last_non_one_index;
							int diff = pozitie[insert - 1] - pozitie[insert];
							for(j = last_non_one_index; j > insert && diff != 0; j--)
							{
								// mut cat pot valorile mai mari ca 1 de pe 
								// ultimele pozitii cat mai infata pentru a
								// ca numerele sa fie mereu in ordine
								if(pozitie[j] > 1)
								{
									if(diff - pozitie[j] + 1 >= 0)
									{
										pozitie[insert] += (pozitie[j] - 1);
										diff = diff - pozitie[j] + 1;
										pozitie[j] = 1;
									}
									else
									{
										pozitie[j] = pozitie[j] - diff;
										pozitie[insert] += diff;
										last_non_one_index = j;
										break;
									} 	
								}
							}
							if(diff == 0 || j == insert)
							{
								last_non_one_index = j;
							}
						}
					}
				}
				else
				{
					// inseamna ca nu se mai poate extrage nici un element
					// si ok = 1 e semnal sa se iasa din for
					if(extragere == 0)
						ok = 1;
				}
			}
			else
			{
				// inseamna ca nu se mai poate extrage nici un element
				// si ok = 1 e semnal sa se iasa din for
				if(extragere == 0)
					ok = 1;
			}
			if(inserted == 1)
			{
				break;
			}		
		}
		if(ok == 1)
		{
			break;
		}
	}

	if(i == indice + 1)
	{
		// afisare C style
		char *suma_finala = (char *)malloc(sizeof(char) * 
			((nr_pos - 1) + ((nr_pos + 1) * 2) + 1) );
		int overhead = 0;
		char *nr_ascii;
		int cif_curenta;
		
		nr_ascii = int_to_ascii(suma);
		cif_curenta = 0;
		while(nr_ascii[cif_curenta] != '\0')
		{
			suma_finala[cif_curenta] = nr_ascii[cif_curenta];
			cif_curenta++;
		}

		overhead += cif_curenta;
		suma_finala[overhead] = '=';
		overhead++;

		for(i = 0; i < nr_pos; i++)
		{
			nr_ascii = int_to_ascii(pozitie[i]);
			cif_curenta = 0;
			while(nr_ascii[cif_curenta] != '\0')
			{
				suma_finala[overhead + cif_curenta] = nr_ascii[cif_curenta];
				cif_curenta++;
			}
			free(nr_ascii);
			overhead += cif_curenta;
			if(i != nr_pos - 1)
			{
				suma_finala[overhead] = '+';
				overhead++;
			}
		}
		suma_finala[overhead] = '\0';
		afisare(suma_finala);
	}
	else
	{
		afisare("-");
	}
}


int main()
{
	int suma, nr_pos, indice;
	citire(&suma, &nr_pos, &indice);
	DP(suma, nr_pos, indice);
	return 0;
}
