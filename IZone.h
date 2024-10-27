#include "Zone.h"
using namespace std;

#pragma once

class IZone : public Zone<IZone>
{
public:
	IZone(int y, int x, bool adjtoPL, SharedResources* c)
		:Zone(y, x, adjtoPL) {
		this->common = c;
		readyForExecution = false;
		w1 = NULL;
		w2 = NULL;
	}

	void PROGRESS_STEP()
	{
		readyForExecution = false;
		vector<Laborer*> w = common->getAvailLaborers();
		if (inhabitants == 0 && adjacentToPowerGrid && w.size() >= 2) {
			w1 = w.at(0);
			w2 = w.at(1);
			readyForExecution = true;
		}
		else if (inhabitants == 0 && isadjwithPopulation(1, 1) && w.size() >= 2) {
			w1 = w.at(0);
			w2 = w.at(1);
			readyForExecution = true;
		}
		else if (inhabitants == 1 && isadjwithPopulation(2, 1) && w.size() >= 2) {
			w1 = w.at(0);
			w2 = w.at(1);
			readyForExecution = true;
		}
		else if (inhabitants == 2 && isadjwithPopulation(4, 2) && w.size() >= 2) {
			w1 = w.at(0);
			w2 = w.at(1);
			readyForExecution = true;
		}
	}

	vector<Product*> getProductsSoldable() {
		vector<Product*> g;
		vector<Product*>::iterator it;

		for (it = goods.begin(); it != goods.end(); it++)
		{
			if ((*it)->isSoldable())
				g.push_back(*(it));
		}

		return g;
	}

	void Activate() {
		if (readyForExecution) {
			PROGRESS_STEP();
			if (readyForExecution) {
				readyForExecution = false;
				++inhabitants;
				w1->assign();
				w2->assign();
				goods.push_back(new Product());
			}
		}
	}

private:
	bool readyForExecution;
	SharedResources* common;
	vector<Product*> goods;
	Laborer *w1, *w2;
};

