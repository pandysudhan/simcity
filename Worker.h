#pragma once

class Laborer {
private:
	bool isAvail;
public:
	Laborer() { isAvail = true; }

	bool assign() {
		if (!isAvail)return false;

		isAvail = false;
		return true;
	}

	bool isAvailable() { return isAvail; }
};
