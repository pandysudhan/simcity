#include "Zone.h"
#pragma once

class CZone : public Zone<CZone>
{
public:
	CZone(int y, int x, bool adjtoPL, SharedResources* c)
		:Zone(y, x, adjtoPL) {
		this->common = c;
		readyForExecution = false;
		good = NULL;
		worker = NULL;
	}

	void PROGRESS_STEP()
	{
		readyForExecution = false;
		vector<Product*> g = common->getSoldableProducts();
		vector<Laborer*> w = common->getAvailLaborers();
		if (inhabitants == 0 && adjacentToPowerGrid && w.size() >= 1 && g.size() >= 1) {
			readyForExecution = true;
			good = g.at(0);
			worker = w.at(0);
		}
		else if (inhabitants == 0 && isadjwithPopulation(1, 1) && w.size() >= 1 && g.size() >= 1) {
			readyForExecution = true;
			good = g.at(0);
			worker = w.at(0);
		}
		else if (inhabitants == 1 && isadjwithPopulation(2, 1) && w.size() >= 1 && g.size() >= 1) {
			readyForExecution = true;
			good = g.at(0);
			worker = w.at(0);
		}
	}

	void Activate() {
		if (readyForExecution) {
			PROGRESS_STEP();
			if (readyForExecution) {
				readyForExecution = false;
				++inhabitants;
				worker->assign();
				good->sell();
			}
		}
	}

private:
	bool readyForExecution;
	SharedResources* common;
	Product* good;
	Laborer* worker;
};
