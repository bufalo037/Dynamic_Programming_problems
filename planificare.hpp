#pragma once
#include <vector>
#include <fstream>

class planificare
{
	private:

	int nr_probe;
	int durata_concurs;
	int pauza_probe;
	std::vector<int> *probe;

	planificare()
	{
		int aux;
		std::ifstream input("planificare.in");
		input >> this->nr_probe;
		input >> this->durata_concurs;
		input >> this->pauza_probe;

		probe = new std::vector<int>;
		probe->reserve(nr_probe);

		for(int i = 0; i < nr_probe; i++)
		{
			input >> aux;
			probe->push_back(aux);
		}
		
		input.close();
	}


	public:

	static planificare* singleton;	


	int get_nr_probe()
	{
		return this->nr_probe;
	}


	int get_durata_concurs()
	{
		return this->durata_concurs;
	}


	int get_pauza_probe()
	{
		return this->pauza_probe;
	}


	std::vector<int>* get_probe()
	{
		return this->probe;
	}


	static planificare* get_planificare()
	{
		
		if(planificare::singleton == NULL)
		{
			planificare::singleton = new planificare();
		}
		
		return planificare::singleton;
	}


	~planificare()
	{
		singleton = NULL;
		delete probe;
	}

	void get_rez();

};

class configuratie
{
	public:

	unsigned long long delay;
	int indice_prim_concurs;
	unsigned long long ant_delay;
	short timp_disponibil;
	short concurs;	

	configuratie(){}

	configuratie(short concurs, unsigned long long delay, short timp_disponibil)
	{
		this->delay = delay;
		this->timp_disponibil = timp_disponibil; // cat timp mai poti aloca in
		// concursul curent
		this->concurs = concurs; 
	}

};