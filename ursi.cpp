// Copyright 2018 Costea Mihai

#include <fstream>
#include <string>

#define BUFFMAX 2000
const int MOD = 1e9 + 7;

void afisare(unsigned int nr)
{
	std::ofstream output("ursi.out");
	output << nr;
	output.close();
}


int numara_catiulite(std::string& message)
{
	int nr = 0, n = message.size();
	for (int i = 0; i < n; i++)
	{
		if(message[i] == '^')
			nr++;
	}
	return nr;
}


std::string* citire()
{
	std::ifstream input("ursi.in");
	std::string *buffer = new std::string;
	buffer->reserve(BUFFMAX);
	input >> *buffer;
	input.close();
	return buffer;
}


void prog_dinamica(std::string& message,
	unsigned int** configuratii_sub_msg, int nr_caciulite)
{
	int i, j, msg_size = message.size();

	// inseamna ca se poate crea 1 mesaj din 0 caciulite care contine 0 mesaje
	// invalide. Prin mesaj invalid ma refer la o subsecventa din mesajul
	// amestecat actual care nu respecta formatul \^\_*\^ . Unde caracterul
	// * semnifica ca potate sa fie oricate sau nici un underscore. (regex)

	// e f important ca cand se considera o configuratie invalida afecteaza
	// numarul de configuratii invalide. Spre exemplu daca as avea un numar
	// par de caractere ^ in secventa nu pot sa formez nici un mesaj care sa
	// aiba un numar impar de subsiruri invalide deoarece daca incerc sa formez
	// un subsir impar stric automat un sir de secvente valide care fiecare
	// secventa valida stricata va oferi un mesaj final care va contine un numar
	// par de secvente invalide. Se poate aplica si pe cazul cand am un nr impar
	// de numar de caciulite in mesaj si vreau sa stric un subsir de mesaje
	// valide.
	configuratii_sub_msg[0][0] = 1;
	for(i = 1; i <= nr_caciulite; i++)
	{
		// cazul de baza
		// se pot forma 0 mesaje de 0 caciulite care sa aiba i > 0 subsecvente
		// invalide
		configuratii_sub_msg[i][0] = 0;
	}

	for(j = 1; j <= msg_size; j++)
	{
		for(i = nr_caciulite; i >= 0; i--)
		{
			if(message[j - 1] == '^')
			{
				if(i == 0)
				{
					// nu pot sa consider ca adaug o caciulita la un submesaj
					// invalid pentru ca sunt 0 submesaje invalide. In schimb
					// luand un mesaj care contine un submesaj invalid de
					// dimensiune 1 oarecare pot sa il fac complet adaugand o
					// caciulita la acesta.
					configuratii_sub_msg[i][j] = (1LL * (i + 1) *
					 configuratii_sub_msg[i + 1][j - 1]) % MOD;
				}
				else
				{
					if(i != nr_caciulite)
					{
						// luand un mesaj oarecare cu i + 1 submesaje invalide
						// putem sa creiem i + 1 mesaje invalide de dimensiune 1
						// prin adaugarea la fiecare submesaj invalid din mesajul
						// prezentat caciulita de unire.
						// Avand configuratii_sub[i + 1][j - 1] astfel de mesaje
						// rezultatul va fi (i + 1)* configuratii_sub[i + 1][j - 1]
						configuratii_sub_msg[i][j] =
						((1LL * configuratii_sub_msg[i - 1][j - 1]) +
						(1LL * (i + 1) * configuratii_sub_msg[i + 1][j - 1]
						% MOD)) % MOD;
					}
					else
					{
						// nu exista suficiente caciulite sa le unesc de la i + 1
						configuratii_sub_msg[i][j] =
						configuratii_sub_msg[i - 1][j - 1];
					}
				}
			}
			else
			{
				// pentru un mesaj specific daca mai adaugi un underscore il
				// poti adauga la fiecare fiecare submesaj invalid din mesajul
				// tau care contine i mesaje invalide. Rezultand +i meaje pentru
				// un singur mesaj. Avand x mesaje raspunsul este x * i.
				configuratii_sub_msg[i][j] =
				(1LL * i * configuratii_sub_msg[i][j - 1]) % MOD;
			}
		}
	}
	afisare(configuratii_sub_msg[0][msg_size]);
}


int main()
{
	int nr_caciulite, i, size_msg;

	std::string* message = citire();

	nr_caciulite = numara_catiulite(*message);
	size_msg = message->size();

	// matrice care tine minte pentru fiecare caracter cate configuratii de o
	// anumita forma exista in matrice. Mai exact cate configuratii de un numar
	// variabil de submesaje invalide
	unsigned int **configuratii_sub_msg =
	(unsigned int **)malloc(sizeof(unsigned int *) * (nr_caciulite + 1));

	for(i = 0; i <= nr_caciulite; i++)
	{
		configuratii_sub_msg[i] = (unsigned int *)malloc(sizeof(unsigned int)
		 * (size_msg + 1));
	}
	prog_dinamica(*message, configuratii_sub_msg, nr_caciulite);

	for(i = 0; i <= nr_caciulite; i++)
	{
		free(configuratii_sub_msg[i]);
	}

	free(configuratii_sub_msg);
	delete message;

	return 0;
}
