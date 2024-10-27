#include "Zone.h"

#pragma once

class RZone : public Zone<RZone>
{
public:
	RZone(int y, int x, bool adjtoPL)
		:Zone(y, x, adjtoPL) {
		newPopulation = 0;
	}

	vector<Laborer*> getLaborersAvailable()
	{
		vector<Laborer*> w;
		vector<Laborer*>::iterator it;

		for (it = workers.begin(); it != workers.end(); it++)
		{
			if ((*it)->isAvailable())
				w.push_back(*(it));
		}

		return w;
	}
	void PROGRESS_STEP()
	{
		if (inhabitants == 0 && adjacentToPowerGrid) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
		else if (inhabitants == 0 && isadjwithPopulation(1, 1)) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
		else if (inhabitants == 1 && isadjwithPopulation(2, 1)) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
		else if (inhabitants == 2 && isadjwithPopulation(4, 2)) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
		else if (inhabitants == 3 && isadjwithPopulation(6, 3)) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
		else if (inhabitants == 4 && isadjwithPopulation(8, 4)) {
			newPopulation = inhabitants + 1;
			t_workers.push_back(new Laborer());
		}
	}

	void implement() { 
		inhabitants = newPopulation; 
		workers.insert(workers.end(), t_workers.begin(), t_workers.end());
		t_workers.clear();
	}

private:
	int newPopulation;
	vector<Laborer*> workers, t_workers;
};