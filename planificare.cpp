// Copyright 2018 Costea Mihai

#include "planificare.hpp"


unsigned long long get_cube(unsigned long long x)
{
	return x * x * x;
}


void afisare(unsigned long long cost, int concursuri)
{
	std::ofstream output("planificare.out");
	output << cost << " " << concursuri;
	output.close();
}


void correct_previous_cofigurations(configuratie *configuratii,
 configuratie *crt, int pauza, std::vector<int> *probe)
{
	configuratie *anterioara = crt - 1;

	int new_timp_disponibil;  // = crt->timp_disponibil;
	int ant_timp_disponibil;  // = anterioara->timp_disponibil;
	unsigned long long new_delay;  // = crt->delay;
	unsigned long long new_ant_delay;  // = anterioara->delay;

	if(crt != configuratii)
	{
		while(true)
		{
				new_timp_disponibil = crt->timp_disponibil;
				ant_timp_disponibil = anterioara->timp_disponibil;
				new_delay = crt->delay;
				new_ant_delay = anterioara->delay;

				if( crt->indice_prim_concurs - 1 != anterioara->indice_prim_concurs &&
					new_timp_disponibil - pauza - (*probe)[crt->indice_prim_concurs -1] >= 0)
				{
					ant_timp_disponibil += pauza + (*probe)[crt->indice_prim_concurs -1];
					new_timp_disponibil -= (pauza + (*probe)[crt->indice_prim_concurs -1]);
					new_ant_delay = anterioara->ant_delay + get_cube(ant_timp_disponibil);
					new_delay = new_ant_delay + get_cube(new_timp_disponibil);
				}
				else
				{
					break;
				}
				if(new_delay >= crt->delay)
				{
					break;
				}
				else
				{
					anterioara->delay = new_ant_delay;
					anterioara->timp_disponibil = ant_timp_disponibil;

					crt->delay = new_delay;
					crt->ant_delay = anterioara->delay;
					crt->timp_disponibil = new_timp_disponibil;
					crt->indice_prim_concurs = crt->indice_prim_concurs - 1;

					// merg recursiv in spate pana gasesc cea mai buna configuratie dupa
					// care ma intorc
					correct_previous_cofigurations(configuratii, anterioara, pauza, probe);
					crt->ant_delay = anterioara->delay;
					crt->delay = anterioara->delay + get_cube(crt->timp_disponibil);
				}
		}
	}
}


void planificare::get_rez()
	{
		planificare *plan = planificare::singleton;

		int nr_probe = plan->get_nr_probe();
		int pauza = plan->get_pauza_probe();
		int durata = plan->get_durata_concurs();
		std::vector<int> *probe = plan->get_probe();
		
		// aloc spatiu pentru a stoca configuratii

		configuratie *configuratii = new configuratie[nr_probe];
		configuratie *crt = configuratii;
		crt->timp_disponibil = durata;
		configuratie *anterioara;
		int next, i;

		// am scris prima oara pentru primul concurs doar pentru cas a nu am
		// if-uri in al doilea for sa fac programul mai eficient
		for(i = 0 ; i < nr_probe; i++)
		{
			if(crt->timp_disponibil == durata)
			{
				crt->delay = 0;
				crt->concurs = 1;
				crt->ant_delay = 0;
				crt->indice_prim_concurs = 0;
				crt->timp_disponibil -= (*probe)[i];
			}
			else
			{
				if (crt->timp_disponibil - pauza - (*probe)[i] >= 0)
				{
					crt->timp_disponibil = crt->timp_disponibil - pauza - (*probe)[i];
				}
				else
				{
					next = 1;
					break;
				}
			}
		}

// ----------------------------------------------------------------------------

		for(; i < nr_probe; i++)
		{
			if(next == 1)
			{
				anterioara = crt;
				crt++;

				anterioara->delay = anterioara->ant_delay +
				get_cube(anterioara->timp_disponibil);
				crt->indice_prim_concurs = i;
				crt->ant_delay = anterioara->delay;
				crt->timp_disponibil = durata;
				crt->concurs = anterioara->concurs + 1;
				next = 0;
				// in caz de se termina for-ul sa returnez direct
				// rezultatul
				crt->delay = anterioara->delay;
			}

			if(crt->timp_disponibil == durata)
			{
				crt->timp_disponibil -= (*probe)[i];
			}
			else
			{
				if (crt->timp_disponibil - pauza - (*probe)[i] >= 0)
				{
					crt->timp_disponibil = crt->timp_disponibil - pauza - (*probe)[i];
				}
				else
				{
					i--;
					// Poate ar putea fi aranjat mai bine!
					crt->delay = crt->ant_delay + get_cube(crt->timp_disponibil);
					correct_previous_cofigurations(configuratii, crt, pauza, probe);
					next = 1;
				}
			}
		}
		afisare(crt->delay, crt->concurs);
	}


planificare* planificare::singleton = NULL;


int main()
{
	planificare* plan = planificare::get_planificare();
	plan->get_rez();

	return 0;
}
