// Copyright 2018 Costea Mihai

#include <fstream>
#include <utility>
#include <list>
#include <map>


void afisare(int Jon, int Sam)
{
	std::ofstream output("frati.out");
	output << Jon << " " << Sam;
	output.close();
}


bool comparare_pereche(const std::pair<int, int>& x1,
	const std::pair<int, int>& x2)
{
	if(x1.first >= x2.first)
		return true;
	return false;
}


void greedy_sol(int n,
	std::map<int, std::list<std::pair<int, int> > >& rewards)
{
	int jocuri = 0, benzi_des = 0;
	int ok = 0, middle = 0;
	std::list<std::pair<int, int> >::reverse_iterator it_sam;

	for(auto it = rewards.rbegin(); it != rewards.rend(); it++)
	{
		it->second.sort(comparare_pereche);
		it_sam = it->second.rbegin();
		middle = 0;
		for(std::list<std::pair<int, int> >::iterator it_jon = it->second.begin();
		 middle == 0;)
		{
			if(&(*it_jon) == &(*it_sam))
			{
				// cei doi iteratori au ajuns in acelas loc
				middle = 1;
			}

			if(ok == 0)
			{
				// randul lui jon
				jocuri += it_jon->first;
				it_jon++;
				ok = 1;
			}
			else
			{
				// randul lui sam
				benzi_des += it_sam->second;
				it_sam++;
				ok = 0;
			}
		}
	}
	afisare(jocuri, benzi_des);
}


void get_input(int& n,
 std::map<int, std::list<std::pair<int, int> > >& rewards)
{
	// http://www.cplusplus.com/reference/new/operator%20new/

	int aux1, aux2;
	std::pair<int, int> aux_pair;

	std::ifstream input("frati.in");

	input >> n;

	for(int i = 0; i < n; i++)
	{
		input >> aux1 >> aux2;
		// new nu realoca spatiu pentru aux ci doar reconstruieste obiectul
		// economisind niste timp
		aux_pair = std::pair<int, int>(aux1, aux2);
		rewards[aux1 + aux2].push_back(aux_pair);
	}

	input.close();
}


int main()
{
	int n;  // 10^6 < 2^32
	std::map<int, std::list<std::pair<int, int> > > rewards;
	get_input(n, rewards);
	greedy_sol(n, rewards);
	return 0;
}
